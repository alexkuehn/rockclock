/*
 * dcf_decode.c
 *
 *  Created on: 19.10.2015
 *      Author: alex
 */

#include <stdint.h>

#include "../hal/dcf_if.h"
#include "dcf_decode_if.h"
#include "dcf_decode.h"
#include "dcf_decode_config.h"
#include "../services/bitops_if.h"
#include "../app/clock_if.h"

static uint64_t rawframe = 0;

void dcf_decode_process( void )
{
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
				clock_set( hours, minutes, 0);
			}
		}
	}


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
