/*
 * io.c
 *
 *  Created on: 06.08.2015
 *      Author: alex
 */

#include "io_config.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void io_init( void )
{
	/* Enable GPIOC clock for LED & USARTs. */
	rcc_periph_clock_enable(RCC_GPIOC);

	/* Setup GPIO pin GPIO8/9 on GPIO port C for LEDs. */
	gpio_mode_setup(LED_BLUE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_BLUE_PIN);

	gpio_mode_setup(LED_BLUE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9);
	gpio_mode_setup(LED_BLUE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO10);

}

void io_toggle( uint32_t port, uint16_t pos)
{
	gpio_toggle( port, pos);
}

void io_on( uint32_t port, uint16_t pos)
{
	gpio_set( port, pos);
}

void io_off( uint32_t port, uint16_t pos)
{
	gpio_clear( port, pos);
}
