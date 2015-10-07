/*
 * app.c
 *
 *  Created on: 07.08.2015
 *      Author: alex
 */

#include "../hal/usart_if.h"
#include "../hal/io_if.h"

#include "../hal/ws2812_if.h"

#include "../services/prng_if.h"

const uint8_t str[] ="Hallo Susanne, ich liebe Dich!";
const uint8_t delimiter[] = {'\r', '\n'};

void app_init(void)
{

}


void blink_process( void )
{
#if(0)
	io_toggle( LED_BLUE_PORT, LED_BLUE_PIN);
#endif
}

void hello_process( void )
{
	usart_transmit((uint8_t *)str, 30);
	usart_transmit((uint8_t *)delimiter,2);
}

void led_process( void )
{
	static uint8_t col = 0;
	static uint8_t clock = 0;

	for( col=0; col < 12; col++)
	{
		if( col == clock )
		{
			ws2812_set_pixel(0, col, 0x1F, 0, 0);
		}
		else
		{
			ws2812_set_pixel(0, col, 0x0, 0x03, 0);
		}
	}

	ws2812_update();

	clock++;
	if( clock == 12)
	{
		clock = 0;
	}
}
