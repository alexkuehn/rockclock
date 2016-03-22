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

static clock_t running_time = {0,0,0};                 /**< actual time object */

static uint8_t muted = false;			               /**< actual state of display mute */
static hires_pixel_t clock_vfb_integrator[CLOCK_ELEMENTS];   /**< framebuffer for clock elements */
static pixel_t clock_vfb_next[CLOCK_ELEMENTS];         /**< framebuffer for next clock elements */
static pixeldiff_t transition_pattern[CLOCK_ELEMENTS]; /**< transition pattern between clock cycles */
static uint8_t firstcall = true;                       /**< flag that this is our first cycle */
                         
void clock_set( uint8_t h, uint8_t m, uint8_t s)
{
	running_time.h = h;
	running_time.m = m;
	running_time.s = s;
}

void clock_clear_fb( void )
{
  for( uint8_t i=0; i < CLOCK_ELEMENTS; i++)
    {
      clock_vfb_integrator[i].r  = 0;
      clock_vfb_integrator[i].g  = 0;
      clock_vfb_integrator[i].b  = 0;

      clock_vfb_next[i].r  = 0;
      clock_vfb_next[i].g  = 0;
      clock_vfb_next[i].b  = 0;

      transition_pattern[i].r  = 0;
      transition_pattern[i].g  = 0;
      transition_pattern[i].b  = 0;
    }
}

void clock_update( void )
{
	static uint8_t clockstep = 0;

    if( firstcall == true )
    {
        firstcall = false;
        clock_clear_fb();
    }
    
	/* show clock */
    clock_display_step(clockstep);
    
	if( clockstep == (MAX_CLOCKSTEP-1))
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

        clock_display();


       
	}

	/* run through the clockstep */
	clockstep++;
	clockstep %= MAX_CLOCKSTEP;

}

void clock_set_pixel( uint8_t pos, uint8_t red, uint8_t green, uint8_t blue )
{
    clock_vfb_next[pos].r = red;
    clock_vfb_next[pos].g = green;
    clock_vfb_next[pos].b = blue;
}

void clock_display_step( uint8_t step )
{
        for( uint8_t i=0; i < CLOCK_ELEMENTS; i++ )
          {
            if( step == (MAX_CLOCKSTEP-1) )
              {
                clock_vfb_integrator[i].r = clock_vfb_next[i].r*1024;
                clock_vfb_integrator[i].g = clock_vfb_next[i].g*1024;
                clock_vfb_integrator[i].b = clock_vfb_next[i].b*1024;
              }
            else
              {
                clock_vfb_integrator[i].r += transition_pattern[i].r;
                clock_vfb_integrator[i].g += transition_pattern[i].g;
                clock_vfb_integrator[i].b += transition_pattern[i].b;
              }
            if( muted == false )
              {
                ws2812_set_pixel(0, i, clock_vfb_integrator[i].r/1024,
                             clock_vfb_integrator[i].g/1024,
                             clock_vfb_integrator[i].b/1024 );
              }
          }

        if( muted == false )
          {
            ws2812_update();
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
			clock_set_pixel(DISP_ROT(i), 0,6,8);
		}
		else
		{
			if( (i % 15) == 0)
			{
				clock_set_pixel(DISP_ROT(i), 0,0,10);

			}
			else
			{
				if( (i % 5) == 0)
				{
					clock_set_pixel(DISP_ROT(i), 0,0,5);

				}
				else
				{
				    clock_set_pixel(DISP_ROT(i), 0,0,1);
				}
			}
		}
	}

	hourscale = (running_time.h % 12)*5;
	clock_set_pixel( DISP_ROT((running_time.h % 12)*5), 20,0,0);
	clock_set_pixel( DISP_ROT(running_time.m), 0, 20,0);
	clock_set_pixel( DISP_ROT(running_time.s), 15, 12,0);

	if( (hourscale == running_time.m) )
	{
		if( running_time.m == running_time.s)
		{
			clock_set_pixel( DISP_ROT(running_time.s), 35, 25,0);
		}
		else
		{
			clock_set_pixel( DISP_ROT(running_time.m), 20, 20,0);
		}
	}
	else
	{
		if( running_time.m == running_time.s)
		{
			clock_set_pixel( DISP_ROT(running_time.m), 0, 45,0);
		}

		if( hourscale == running_time.s)
		{
			clock_set_pixel( DISP_ROT(hourscale), 45, 0,0);
		}
	}
	clock_fb_update();
}

void clock_fb_update( void )
{
  for(uint8_t i=0; i < CLOCK_ELEMENTS; i++ )
    {
      transition_pattern[i].r = ((clock_vfb_next[i].r * 1024) - clock_vfb_integrator[i].r)/MAX_CLOCKSTEP;
      transition_pattern[i].g = ((clock_vfb_next[i].g * 1024) - clock_vfb_integrator[i].g)/MAX_CLOCKSTEP;
      transition_pattern[i].b = ((clock_vfb_next[i].b * 1024) - clock_vfb_integrator[i].b)/MAX_CLOCKSTEP;
    }
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
