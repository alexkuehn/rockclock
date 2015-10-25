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

#include "ringbuffer_if.h"
#include <stdint.h>


void ringbuffer_init( ringbuffer_t *rb, void  *databuffer, uint16_t datalength )
{
	rb->buffer = (uint8_t *)databuffer;
	rb->length = datalength;
	rb->readpos = 0;
	rb->writepos = 0;
}

uint16_t ringbuffer_get_free( ringbuffer_t *rb )
{
	uint16_t length = rb->length;

	uint16_t usedlength = ringbuffer_get_used( rb );
	return((length-usedlength)-1);

}

uint16_t ringbuffer_get_used( ringbuffer_t *rb )
{
	uint16_t readpos = rb->readpos;
	uint16_t writepos = rb->writepos;
	uint16_t length = rb->length;

	uint16_t filled = writepos-readpos;
	if( writepos < readpos )
	{
		filled = (length-readpos)+writepos;
	}
	return filled;
}

uint16_t ringbuffer_get_length( ringbuffer_t *rb )
{
	return rb->length-1;
}

void ringbuffer_flush( ringbuffer_t *rb )
{
	rb->readpos = rb->writepos;
}

uint8_t ringbuffer_get_data( ringbuffer_t *rb, void *databuffer, uint16_t datalength)
{
	uint16_t readpos = rb->readpos;
	uint16_t length = rb->length;
	uint8_t *buffer = rb->buffer;

	// get number of bytes available
	uint16_t available = ringbuffer_get_used(rb);

	if (available > datalength)
	{
		available = datalength;
	}
	if (available < 1)
		return 0;		// return number of bytes copied

	uint8_t *p = (uint8_t *)databuffer;
	uint16_t i = 0;

	while (available > 0)
	{
		uint16_t j = length - readpos;
		if (j > available)
		{
			j = available;
		}
		for( uint16_t k =0; k < j; k++)
		{
			*(p+i+k)=*(buffer+readpos+k);
		}
		i += j;
		available -= j;
		readpos += j;
		if (readpos >= length)
			readpos = 0;
	}
	rb->readpos = readpos;
	return i;                   // return number of bytes copied
}

uint8_t ringbuffer_put_data( ringbuffer_t *rb, void *databuffer, uint16_t datalength)
{
	uint16_t writepos = rb->writepos;
	uint16_t length = rb->length;
	uint8_t *buffer = rb->buffer;

	uint16_t available = ringbuffer_get_free(rb);
	if (available > datalength)
	{
		available = datalength;
	}
	if (available < 1)
	{
		return 0;               // return number of bytes copied
	}

	uint8_t *p = (uint8_t *)databuffer;
	uint16_t i = 0;

	while (available > 0)
	{
		uint16_t j = length - writepos;
		if (j > available)
		{
			j = available;
		}

		for( uint16_t k=0; k < j; k++)
		{
			*(buffer+writepos+k) = *(p+i+k);
		}
		i += j;
		available -= j;
		writepos += j;
		if (writepos >= length)
		{
			writepos = 0;
		}
	}

	rb->writepos = writepos;

	return i;                   // return number of bytes copied
}
