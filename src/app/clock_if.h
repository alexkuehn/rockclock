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

#ifndef SRC_APP_CLOCK_IF_H_
#define SRC_APP_CLOCK_IF_H_

#include <stdint.h>

typedef struct
{
	uint8_t h;
	uint8_t m;
	uint8_t s;
} clock_t;



extern void clock_update( void );

extern void clock_set( uint8_t h, uint8_t m, uint8_t s);



#endif /* SRC_APP_CLOCK_IF_H_ */
