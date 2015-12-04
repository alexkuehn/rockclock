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
#include <stdint.h>

/* external includes */

/* project includes */
#include "../hal/dcf_if.h"
#include "../services/bitops_if.h"
#include "../app/clock_if.h"
#include "../hal/i2c_if.h"

/* component includes */
#include "dcf_decode_if.h"
#include "dcf_decode.h"
#include "dcf_decode_config.h"


static uint64_t rawframe = 0;					/**< actual raw DCF frame */
static dcftime_t last_decoded_time = {0,0};		/**< last decoded time from valid frame */
static uint8_t timeupdate_count = 0;


void dcf_decode_process( void )
{
	uint8_t rtcbuffer[3];
	uint64_t tmpframe;
	uint8_t inner_frame_q;
	uint8_t minutes_bcd;
	uint8_t hours_bcd;
	uint8_t minutes;
	uint8_t hours;
	if( dcf_mailbox_data_get(&tmpframe)== 1)
	{
		rawframe = tmpframe;
		if( dcf_decode_eval_frame_q( rawframe) == 1 )
		{
			/* received consistent frame */
			minutes_bcd = BITOPS64_EXTRACT( rawframe, 21, 7);
			hours_bcd = BITOPS64_EXTRACT( rawframe, 29, 6);
			minutes = dcf_decode_bcd( minutes_bcd);
			hours = dcf_decode_bcd( hours_bcd);
			if( (hours < 24) && (minutes < 60) )
			{
				/* if frame is plausible,
				 * check plausibility of two consecutive frames
				 * synchronize clock only when two frames are monotonic
				 */
				dcftime_t acttime;
				acttime.h = hours;
				acttime.m = minutes;
				if( dcf_decode_eval_inter_frame_q( acttime, last_decoded_time)== 1)
				{
					clock_set( hours, minutes, 0);

					/* update the RTC memory only every 20th valid frame */
					if( (timeupdate_count % 20)== 0 )
					{
						rtcbuffer[0] = dec2bcd( 0 );
						rtcbuffer[1] = dec2bcd( minutes );
						rtcbuffer[2] = dec2bcd( hours );
						i2c_transmit_blocking( 0x68, 0x00, &rtcbuffer[0], 3);

					}
				}
				last_decoded_time.h = hours;
				last_decoded_time.m = minutes;
			}
		}
	}

}

uint8_t dcf_decode_eval_inter_frame_q( dcftime_t act, dcftime_t last)
{
	uint8_t valid =1;
	uint8_t hour_ovfl = 0;
	if( last.m <59 )
	{
		if( act.m != (last.m+1))
		{
			valid = 0;
		}

	}
	else
	{
		hour_ovfl = 1;
		if( act.m != 0)
		{
			valid = 0;
		}

	}

	if( hour_ovfl )
	{
		if( act.h == last.h )
		{
			valid = 0;
		}
		if( last.h < 23 )
		{
			if( act.h != (last.h+1))
			{
				valid = 0;
			}
		}
		else
		{
			if( act.h != 0)
			{
				valid = 0;
			}
		}
	}
	return valid;
}

uint8_t dcf_decode_bcd( uint8_t input)
{
	uint8_t result;
	result = (input & 0x0F);
	result += ((input >> 4) & 0x0F)*10;

	return result;
}

uint8_t dcf_decode_eval_frame_q( uint64_t frame )
{
	uint8_t q_result = 1;
	uint32_t minutes;
	uint32_t hours;
	uint8_t parity;

	/* first bit needs to be zero */
	if( BITOPS64_EXTRACT(frame, 0, 1) != 0)
	{
		q_result = 0;
	}

	/* bit 20 begin of time always 1 */
	if( BITOPS64_EXTRACT(frame, 20,1) == 0)
	{
		q_result = 0;
	}

	minutes = BITOPS64_EXTRACT( frame, 21, 8);
	hours = BITOPS64_EXTRACT( frame, 29, 7);

	parity = bitops32_count( minutes);
	if( parity & 1)
	{
		q_result = 0;
	}

	parity = bitops32_count( hours);
	if( parity & 1)
	{
		q_result = 0;
	}
	return q_result;
}

uint64_t dcf_decode_get_raw( void )
{
	return rawframe;
}
