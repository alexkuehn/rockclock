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
#include <stdbool.h>

/* external includes */

/* project includes */
#include "../hal/ws2812_if.h"
#include "../app/dcf_decode_if.h"

/* component includes */
#include "clock_if.h"
#include "clock.h"
#include "clock_config.h"

static clock_t running_time = {0,0,0};	/**< actual time object */

static uint8_t muted = false;			/**< actual state of display mute */

void clock_set( uint8_t h, uint8_t m, uint8_t s)
{
	running_time.h = h;
	running_time.m = m;
	running_time.s = s;
}
void clock_update( void )
{
	/* try to decode the DCF value */
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
	/* increment the clock */
	clock_tick();
	/* show clock */
	if( muted == false)
	{
		clock_display();
	}
}

void clock_display( void)
{
	uint8_t i;
	uint8_t hourscale;

	for( i = 0; i < 60; i++)
	{
		if( i == 0 )
		{
			ws2812_set_pixel(0, DISP_ROT(i), 0,6,8);
		}
		else
		{
			if( (i % 15) == 0)
			{
				ws2812_set_pixel(0,DISP_ROT(i), 0,0,10);

			}
			else
			{
				if( (i % 5) == 0)
				{
					ws2812_set_pixel(0,DISP_ROT(i), 0,0,5);

				}
				else
				{
					ws2812_set_pixel(0,DISP_ROT(i), 0,0,1);
				}
			}
		}
	}

	hourscale = (running_time.h % 12)*5;
	ws2812_set_pixel(0, DISP_ROT((running_time.h % 12)*5), 20,0,0);
	ws2812_set_pixel(0, DISP_ROT(running_time.m), 0, 20,0);
	ws2812_set_pixel(0, DISP_ROT(running_time.s), 15, 12,0);

	if( (hourscale == running_time.m) )
	{
		if( running_time.m == running_time.s)
		{
			ws2812_set_pixel(0, DISP_ROT(running_time.s), 35, 25,0);
		}
		else
		{
			ws2812_set_pixel(0, DISP_ROT(running_time.m), 20, 20,0);
		}
	}
	else
	{
		if( running_time.m == running_time.s)
		{
			ws2812_set_pixel(0, DISP_ROT(running_time.m), 0, 45,0);
		}

		if( hourscale == running_time.s)
		{
			ws2812_set_pixel(0, DISP_ROT(hourscale), 45, 0,0);
		}
	}
	ws2812_update();
}

void clock_mute( uint8_t mutestate )
{
	if( mutestate != muted )
	{
		if( mutestate == true )
		{
			ws2812_clear();
			ws2812_update();
		}
		else
		{
			clock_display();
		}
		muted = mutestate;
	}
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
