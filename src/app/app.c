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


void app_init(void)
{

}


void blink_process( void )
{
	io_toggle( LED_BLUE_PORT, LED_BLUE_PIN);
}

void hello_process( void )
{
	uint8_t j;

	j = 0;
	while( str[j] != '\0')
	{
		usart_transmit(str[j]);
		j++;
	}
	usart_transmit('\r');
	usart_transmit('\n');
}

void led_process( void )
{
	static uint8_t col = 0;
	static uint8_t step = 0;

	if( step == 0)
	{

		for( col=0; col < 12; col++)
		{
			ws2812_set_pixel(0, col, 0x01, 0, 0);

		}
		step =1;
	}
	else
	{
		for( col=0; col < 12; col++)
				{
					ws2812_set_pixel(0, col, 0x0, 0x01, 0);

				}
		step=0;

	}


	ws2812_update();


}
