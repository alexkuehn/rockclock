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

#include "../com/bt_if.h"

void app_init(void)
{
	bt_init();
}


void blink_process( void )
{
#if(0)
	io_toggle( LED_BLUE_PORT, LED_BLUE_PIN);
#endif
}

void led_process( void )
{
	static uint8_t col = 0;
	static uint8_t clock = 0;

	for( col=0; col < 60; col++)
	{
		if( col == clock )
		{
			ws2812_set_pixel(0, col, 0, 75, 75);
		}
		else
		{
			if( (col % 15) == 0)
			{
				ws2812_set_pixel(0, col, 30, 0x00, 10);
			}
			else
			{
				if( (col % 5) == 0)
				{
					ws2812_set_pixel(0, col, 15, 0x00, 15);

				}
				else
				{
					ws2812_set_pixel(0, col, 1, 1, 0);
				}
			}

			if( col == 30)
			{
				ws2812_set_pixel( 0, col , 15,15,15);
			}
		}
	}


	ws2812_update();

	clock++;
	if( clock == 60)
	{
		clock = 0;
	}
}

void loop_process(void)
{
#if(0)
	const uint8_t edata = 'T';
	uint8_t inputdata[8];
	int16_t readlen;

	readlen = usart_receive( inputdata, 8, 0);

	if( readlen > 0)
	{
		usart_transmit( inputdata, readlen);
	}
#endif
}

