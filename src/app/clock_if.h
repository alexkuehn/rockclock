/*
 * clock_if.h
 *
 *  Created on: 19.10.2015
 *      Author: alex
 */

#ifndef SRC_APP_CLOCK_IF_H_
#define SRC_APP_CLOCK_IF_H_

typedef struct
{
	uint8_t h;
	uint8_t m;
	uint8_t s;
} clock_t;



extern void clock_update( void );

extern void clock_set( uint8_t h, uint8_t m, uint8_t s);



#endif /* SRC_APP_CLOCK_IF_H_ */
