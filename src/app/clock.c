/*
 * clock.c
 *
 *  Created on: 19.10.2015
 *      Author: alex
 */

#include <stdint.h>

#include "clock_if.h"
#include "clock.h"
#include "clock_config.h"
#include "../hal/ws2812_if.h"

#include "../app/dcf_decode_if.h"

static clock_t running_time = {0,0,0};

void clock_set( uint8_t h, uint8_t m, uint8_t s)
{
	running_time.h = h;
	running_time.m = m;
	running_time.s = s;
}
void clock_update( void )
{
	dcf_decode_process();
#if(CFG_DBG_DCF_BINARY)
	uint64_t dcfframe = 0;
	uint8_t i;
#endif


#if(CFG_DBG_DCF_BINARY)
	dcfframe = dcf_decode_get_raw();
	ws2812_set_pixel(0,59, 0,0,5);


	for( i=0; i < 59; i++)
	{
		if( (dcfframe & ((uint64_t)1ULL << i)) != 0 )
		{
			ws2812_set_pixel(0,i,10,0,0);
			if( i == 20)
			{
				ws2812_set_pixel(0,i,5,5,0);
			}
		}
		else
		{
			ws2812_set_pixel(0,i,0,10,0);
			if( i == 20)
				{
					ws2812_set_pixel(0,i,0,5,5);
				}
		}

	}

	ws2812_update();
#endif
	clock_tick();
	clock_display();
}

void clock_display( void)
{
	uint8_t i;

	for( i = 0; i < 60; i++)
	{
		if( i == 0 )
		{
			ws2812_set_pixel(0, i, 12,12,12);
		}
		else
		{
			if( (i % 15) == 0)
			{
				ws2812_set_pixel(0,i, 0,0,20);

			}
			else
			{
				if( (i % 5) == 0)
				{
					ws2812_set_pixel(0,i, 12,0,12);

				}
				else
				{
					ws2812_set_pixel(0,i, 1,0,1);
				}
			}
		}
	}
	ws2812_set_pixel(0, (running_time.h % 12)*5, 12,12,0);
	ws2812_set_pixel(0, running_time.m, 0, 20,0);
	ws2812_set_pixel(0, running_time.s, 0, 12,12);
	ws2812_update();
}

void clock_tick( void )
{
	running_time.s++;

	if( running_time.s == 60)
	{
		running_time.s = 0;
		running_time.m++;
	}
	if( running_time.m == 60)
	{
		running_time.m = 0;
		running_time.h++;
	}
	if( running_time.h == 24)
	{
		running_time.h = 0;
	}

}
