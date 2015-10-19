/*
 * bitops_if.h
 *
 *  Created on: 19.10.2015
 *      Author: alex
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
