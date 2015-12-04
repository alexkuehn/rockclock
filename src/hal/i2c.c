/*
 * This file is part of ROCKCLOCK firmware.
 *
 * Copyright (c) 2015 Alexander Kühn <prj@alexkuehn.de>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

/* external standard includes */

/* external includes */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/i2c.h>

/* project includes */

/* component includes */
#include "i2c_config.h"
#include "../hal/i2c_if.h"

#include "../hal/io_if.h"

void i2c_init( void )
{
	uint8_t buffer[3];
	/* Enable GPIOF clock for I2C2. */
	rcc_periph_clock_enable(RCC_GPIOF);

	/* Enable clocks for I2C2. */
	rcc_periph_clock_enable(RCC_I2C2);

	/* Setup GPIO pins for I2C2 mode */
	gpio_mode_setup(GPIOF, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO6 | GPIO7 );
	gpio_set_output_options(GPIOF, GPIO_OTYPE_OD, GPIO_OSPEED_50MHZ, GPIO6 | GPIO7);

	/* Setup I2C2 parameters for 48Mhz pclk. */
	I2C_CR1(I2C2) &= ~(I2C_CR1_PE);
	I2C_TIMINGR(I2C2) &= ~(I2C_TIMINGR_PRESC);
	I2C_TIMINGR(I2C2) |= I2C_TIMINGR_PRESC_VAL(0xB);
	I2C_TIMINGR(I2C2) &= ~(I2C_TIMINGR_SCLL);
	I2C_TIMINGR(I2C2) |= I2C_TIMINGR_SCLL_VAL(0x13);
	I2C_TIMINGR(I2C2) &= ~(I2C_TIMINGR_SCLH);
	I2C_TIMINGR(I2C2) |= I2C_TIMINGR_SCLH_VAL(0xF);
	I2C_TIMINGR(I2C2) &= ~(I2C_TIMINGR_SDADEL);
	I2C_TIMINGR(I2C2) |= I2C_TIMINGR_SDADEL_VAL(0x2);
	I2C_TIMINGR(I2C2) &= ~(I2C_TIMINGR_SCLDEL);
	I2C_TIMINGR(I2C2) |= I2C_TIMINGR_SCLDEL_VAL(0x4);

	I2C_CR1(I2C2) |= I2C_CR1_PE;

}

void i2c_receive_blocking( uint8_t busaddr, uint8_t reg, uint8_t *data, uint8_t length)
{
	uint8_t transfer_progress;
	uint8_t i;
	/* wait until i2c module is ready */
	while( (I2C_ISR(I2C2) & I2C_ISR_BUSY) != 0);
	while( (I2C_CR2(I2C2) & I2C_CR2_START) !=0);

	/* program i2c module for transfer size, adress and autoend mode
	 * and set write flag
	 */
	I2C_CR2(I2C2) &= ~ (I2C_CR2_NBYTES);
	I2C_CR2(I2C2) |= I2C_CR2_NBYTES_VAL(1);
	I2C_CR2(I2C2) &= ~ (I2C_CR2_SADD);
	I2C_CR2(I2C2) &= ~(I2C_CR2_RD_WRN);
	I2C_CR2(I2C2) |= I2C_CR2_SADD_VAL((busaddr<<1));
	I2C_CR2(I2C2) &= ~(I2C_CR2_AUTOEND);

	I2C_CR2(I2C2) |= I2C_CR2_START;

	transfer_progress = 1;
	while( transfer_progress == 1)
	{
		if( (I2C_ISR(I2C2) & I2C_ISR_TXIS) != 0 )
		{
			transfer_progress = 0;
		}
		while( (I2C_ISR(I2C2) & I2C_ISR_NACKF) != 0);
	}
	I2C_TXDR(I2C2) = reg;
	while( (I2C_CR2(I2C2) & I2C_CR2_START) !=0);
	I2C_CR2(I2C2) &= ~ (I2C_CR2_NBYTES);
	I2C_CR2(I2C2) |= I2C_CR2_NBYTES_VAL(length);
	I2C_CR2(I2C2) |= (I2C_CR2_RD_WRN);
	I2C_CR2(I2C2) |= (I2C_CR2_AUTOEND);

	I2C_CR2(I2C2) |= I2C_CR2_START;

	for( i =0; i < length; i++ )
	{
		while( (I2C_ISR(I2C2) & I2C_ISR_RXNE) == 0);
		data[i] = I2C_RXDR(I2C2);
	}
}
void i2c_transmit_blocking( uint8_t busaddr, uint8_t reg, uint8_t *data, uint8_t length)
{
	uint8_t transfer_progress;
	uint8_t i;
	/* wait until i2c module is ready */
	while( (I2C_ISR(I2C2) & I2C_ISR_BUSY) != 0);
	while( (I2C_CR2(I2C2) & I2C_CR2_START) !=0);

	/* program i2c module for transfer size, adress and autoend mode
	 * and set write flag
	 */
	I2C_CR2(I2C2) &= ~ (I2C_CR2_NBYTES);
	I2C_CR2(I2C2) |= I2C_CR2_NBYTES_VAL(length+1);
	I2C_CR2(I2C2) &= ~ (I2C_CR2_SADD);
	I2C_CR2(I2C2) &= ~(I2C_CR2_RD_WRN);
	I2C_CR2(I2C2) |= I2C_CR2_SADD_VAL((busaddr<<1));
	I2C_CR2(I2C2) |= I2C_CR2_AUTOEND;

	I2C_CR2(I2C2) |= I2C_CR2_START;

	transfer_progress = 1;
	while( transfer_progress == 1)
	{
		if( (I2C_ISR(I2C2) & I2C_ISR_TXIS) != 0 )
		{
			transfer_progress = 0;
		}
		while( (I2C_ISR(I2C2) & I2C_ISR_NACKF) != 0);
	}
	I2C_TXDR(I2C2) = reg;
	for( i =0; i < length; i++ )
	{
		transfer_progress = 1;
		while( transfer_progress == 1)
		{
			if( (I2C_ISR(I2C2) & I2C_ISR_TXIS) != 0 )
			{
				transfer_progress = 0;
			}
			while( (I2C_ISR(I2C2) & I2C_ISR_NACKF) != 0);
		}
		I2C_TXDR(I2C2) = data[i];
	}
}
