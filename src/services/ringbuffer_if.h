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

#ifndef SRC_SERVICES_RINGBUFFER_IF_H
#define SRC_SERVICES_RINGBUFFER_IF_H

#include <stdint.h>

typedef struct {
	uint8_t *buffer;
	uint16_t length;
	volatile uint16_t readpos;
	volatile uint16_t writepos;
} ringbuffer_t;

extern void ringbuffer_init( ringbuffer_t *rb, void  *databuffer, uint16_t datalength );
extern uint16_t ringbuffer_get_free( ringbuffer_t *rb );
extern uint16_t ringbuffer_get_used( ringbuffer_t *rb );
extern uint16_t ringbuffer_get_length( ringbuffer_t *rb );

extern void ringbuffer_flush( ringbuffer_t *rb );

extern uint8_t ringbuffer_get_data( ringbuffer_t *rb, void *databuffer, uint16_t datalength);
extern uint8_t ringbuffer_put_data( ringbuffer_t *rb, void *databuffer, uint16_t datalength);

#endif
