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

/* component includes */

uint8_t bitops64_count( uint64_t bits)
{
	uint8_t counter = 0;
	/* iterate over bits */
	while( bits )
	{
		counter += bits % 2;
		bits >>= 1;
	}
	return counter;
}

uint8_t bitops32_count( uint32_t bits)
{
	uint8_t counter = 0;
	/* iterate over bits */
	while( bits )
	{
		counter += bits % 2;
		bits >>= 1;
	}
	return counter;
}

uint8_t bcd2dec( uint8_t bcdval )
{
	return( ( (bcdval & 0xF0)>>4)*10 + (bcdval & 0x0F) );
}

uint8_t dec2bcd( uint8_t decval )
{
	return( (decval/10)*16 +(decval%10) );
}
