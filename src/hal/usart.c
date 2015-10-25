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

#include "usart_if.h"
#include "usart_config.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

#include "../services/ringbuffer_if.h"

#include "../hal/io_if.h"
#include "../hal/timer_if.h"

static uint8_t rxbuffer_store[USART_BUFFERSIZE];
static uint8_t txbuffer_store[USART_BUFFERSIZE];

static ringbuffer_t rxbuffer,txbuffer;

void usart_init( void )
{
	/* Enable GPIOC clock for LED & USARTs. */
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable clocks for USART2. */
	rcc_periph_clock_enable(RCC_USART2);

	/* Setup GPIO pins for USART2 transmit. */
	gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3 );

	/* Setup USART1 TX pin as alternate function. */
	gpio_set_af(GPIOA, GPIO_AF1, GPIO2 | GPIO3 );

	/* Setup USART2 parameters. */
	usart_set_baudrate(USART2, BAUDRATE);
	usart_set_databits(USART2, 8);
	usart_set_parity(USART2, USART_PARITY_NONE);
	usart_set_stopbits(USART2, USART_CR2_STOP_1_0BIT);

	usart_set_mode(USART2, USART_MODE_TX_RX);
	usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

	/* init comm buffers */
	ringbuffer_init( &rxbuffer, rxbuffer_store, USART_BUFFERSIZE);
	ringbuffer_init( &txbuffer, txbuffer_store, USART_BUFFERSIZE);


	nvic_enable_irq( NVIC_USART2_IRQ );
	usart_enable_rx_interrupt( USART2);
	usart_disable_tx_interrupt( USART2);
	/* Finally enable the USART. */
	usart_enable(USART2);
}

void usart2_isr(void)
{
	uint8_t data;

	if( (USART2_ISR & USART_ISR_TXE) != 0)
	{

		if( ringbuffer_get_used(&txbuffer) > 0)
		{
			ringbuffer_get_data( &txbuffer, &data, 1);
			usart_send( USART2, data);
		}
		else
		{
			usart_disable_tx_interrupt( USART2);
		}
	}

	if( (USART2_ISR & USART_ISR_RXNE) != 0)
	{
		data = usart_recv( USART2);
		if( ringbuffer_get_free(&rxbuffer) > 0)
		{
			ringbuffer_put_data( &rxbuffer, &data, 1);
		}
	}

}

void usart_config_baudrate( uint32_t baudrate )
{
	usart_set_baudrate( USART2, baudrate);
}

void usart_transmit_blocking( uint16_t data )
{
	usart_send_blocking(USART2, data);
}

usart_status_t usart_transmit( uint8_t* data, uint16_t length)
{
	ringbuffer_put_data( &txbuffer, data, length);
	usart_enable_tx_interrupt(USART2);
	return E_OK;
}

int16_t usart_receive( uint8_t *buffer, uint16_t length, uint32_t timeout)
{
	uint16_t reallength;
	uint32_t timeouttrigger;
	int16_t retval;

	timeouttrigger = timer_get() + timeout;

	do
	{
		reallength = ringbuffer_get_used( &rxbuffer);

	}while( (timeout > 0) &&
			(timeouttrigger > timer_get()) &&
			(reallength < length) );

	if( reallength > length )
	{
		reallength = length;
	}

	if( reallength > 0)
	{
		ringbuffer_get_data(&rxbuffer, buffer, reallength);
		retval = reallength;
	}
	else
	{
		retval = -1;
	}
	return retval;
}

void usart_flush( void )
{
	ringbuffer_flush( &rxbuffer);
	ringbuffer_flush(&txbuffer);
}
