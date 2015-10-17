/*
 * io.c
 *
 *  Created on: 06.08.2015
 *      Author: alex
 */

#include "io_config.h"
#include "../hal/io_if.h"
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void io_init( void )
{
	/* Enable GPIOC clock for LED & USARTs. */
	rcc_periph_clock_enable(RCC_GPIOC);
	rcc_periph_clock_enable(RCC_GPIOB);
	rcc_periph_clock_enable(RCC_GPIOA);

	/* Setup GPIO pin GPIO8/9 on GPIO port C for LEDs. */
	gpio_mode_setup(LED_BLUE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, LED_BLUE_PIN);
	gpio_mode_setup(HC05_CMD_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, HC05_CMD_PIN);


	gpio_mode_setup(LED_BLUE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO9);
	gpio_mode_setup(LED_BLUE_PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO10);

	gpio_mode_setup( DCF_IN_PORT, GPIO_MODE_INPUT, GPIO_PUPD_NONE, DCF_IN_PIN);

	io_off( LED_BLUE_PORT, LED_BLUE_PIN);
	io_off( HC05_CMD_PORT, HC05_CMD_PIN);
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

uint8_t io_get( uint32_t port, uint16_t pos)
{
	return gpio_get( port, pos);
}

