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
#include "../hal/i2c_if.h"
#include "../services/bitops_if.h"

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

const uint8_t gammatable[256] = {
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,
    0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  1,
    1,  1,  1,  1,  1,  1,  1,  1,  1,  2,  2,  2,  2,  2,  2,  2,
    2,  3,  3,  3,  3,  3,  3,  3,  4,  4,  4,  4,  4,  5,  5,  5,
    5,  6,  6,  6,  6,  7,  7,  7,  7,  8,  8,  8,  9,  9,  9, 10,
   10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
   17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
   25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
   37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
   51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
   69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
   90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
  115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
  144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
  177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
  215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 };

enum
{
	CLOCK_MARKER_TOP = 0,
	CLOCK_MARKER_QUARTER,
	CLOCK_MARKER_FIVE,
	CLOCK_MARKER_ELEM,
	CLOCK_MARKER_HOUR,
	CLOCK_MARKER_MINUTE,
	CLOCK_MARKER_SECOND,
	CLOCK_MARKER_HMS,
	CLOCK_MARKER_HM,
	CLOCK_MARKER_HS,
	CLOCK_MARKER_MS,
	CLOCK_MARKER_SIZE
} clock_marker_e;

const pixel_t clock_markers[CLOCK_MARKER_SIZE] = {
		{   0,  66,  74 },		/* CLOCK_MARKER_TOP */
		{   0,   0,  80 },		/* CLOCK_MARKER_QUARTER */
		{   0,   0,  61 },		/* CLOCK_MARKER_FIVE */
		{   0,   0,  32 },		/* CLOCK_MARKER_ELEM */
		{ 102,   0,   0 },		/* CLOCK_MARKER_HOUR */
		{   0, 102,   0 },		/* CLOCK_MARKER_MINUTE */
		{  92,  85,   0 },		/* CLOCK_MARKER_SECOND */
		{ 126, 112,   0 },		/* CLOCK_MARKER_HMS */
		{ 102, 102,   0 },		/* CLOCK_MARKER_HM */
		{ 137,   0,   0 },		/* CLOCK_MARKER_HS */
		{   0, 137,   0 },		/* CLOCK_MARKER_MS */
};
#define GAMMA_TRANSFORM(p) (gammatable[(p)])

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

void clock_set_pixel( uint8_t pos, pixel_t pixelval )
{
    clock_vfb_next[pos].r = pixelval.r;
    clock_vfb_next[pos].g = pixelval.g;
    clock_vfb_next[pos].b = pixelval.b;
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
                ws2812_set_pixel(0, i, GAMMA_TRANSFORM(clock_vfb_integrator[i].r/1024),
                		GAMMA_TRANSFORM(clock_vfb_integrator[i].g/1024),
						GAMMA_TRANSFORM(clock_vfb_integrator[i].b/1024) );
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
			clock_set_pixel(DISP_ROT(i), clock_markers[CLOCK_MARKER_TOP]);
		}
		else
		{
			if( (i % 15) == 0)
			{
				clock_set_pixel(DISP_ROT(i), clock_markers[CLOCK_MARKER_QUARTER]);

			}
			else
			{
				if( (i % 5) == 0)
				{
					clock_set_pixel(DISP_ROT(i), clock_markers[CLOCK_MARKER_FIVE]);

				}
				else
				{
				    clock_set_pixel(DISP_ROT(i), clock_markers[CLOCK_MARKER_ELEM]);
				}
			}
		}
	}

	hourscale = (running_time.h % 12)*5;
	clock_set_pixel( DISP_ROT((running_time.h % 12)*5), clock_markers[CLOCK_MARKER_HOUR]);
	clock_set_pixel( DISP_ROT(running_time.m), clock_markers[CLOCK_MARKER_MINUTE]);
	clock_set_pixel( DISP_ROT(running_time.s), clock_markers[CLOCK_MARKER_SECOND]);

	if( (hourscale == running_time.m) )
	{
		if( running_time.m == running_time.s)
		{
			clock_set_pixel( DISP_ROT(running_time.s), clock_markers[CLOCK_MARKER_HMS]);
		}
		else
		{
			clock_set_pixel( DISP_ROT(running_time.m), clock_markers[CLOCK_MARKER_HM]);
		}
	}
	else
	{
		if( running_time.m == running_time.s)
		{
			clock_set_pixel( DISP_ROT(running_time.m), clock_markers[CLOCK_MARKER_MS]);
		}

		if( hourscale == running_time.s)
		{
			clock_set_pixel( DISP_ROT(hourscale), clock_markers[CLOCK_MARKER_HS]);
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

void clock_resync( void )
{
	uint8_t rtcbuffer[3];

	i2c_receive_blocking( 0x68, 0x00, &rtcbuffer[0], 3);
	clock_set( bcd2dec(rtcbuffer[2]), bcd2dec(rtcbuffer[1]), bcd2dec(rtcbuffer[0]));
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
			clock_resync();
		}
		muted = mutestate;
	}
}

void clock_tick( void )
{
	static uint16_t resync_time = 0;

	if( resync_time == MAX_RESYNC_TIME)
	{
		clock_resync();
	}
	else
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
}
