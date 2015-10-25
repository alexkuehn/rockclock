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

#ifndef SRC_SERVICES_BITOPS_IF_H_
#define SRC_SERVICES_BITOPS_IF_H_

#include <stdint.h>

#define BITOPS64_BIT(pos) (1ULL<<pos)
#define BITOPS64_MASK( len ) (BITOPS64_BIT(len)-1)
#define BITOPS64_EXTRACT( val, pos, len) ( ((val)>>pos) & BITOPS64_MASK(len) )

extern inline uint8_t bitops64_count( uint64_t bits)
{
	uint8_t counter = 0;
	while( bits )
	{
		counter += bits % 2;
		bits >>= 1;
	}
	return counter;
}

extern inline uint8_t bitops32_count( uint32_t bits)
{
	uint8_t counter = 0;
	while( bits )
	{
		counter += bits % 2;
		bits >>= 1;
	}
	return counter;
}

#endif /* SRC_SERVICES_BITOPS_IF_H_ */
