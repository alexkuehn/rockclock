/*
 * ws2812.h
 *
 *  Created on: 09.08.2015
 *      Author: alex
 */

#ifndef SRC_HAL_WS2812_H_
#define SRC_HAL_WS2812_H_

#include "ws2812_config.h"


#define NR_OF_ROWS (8)

#define BITS_PER_LED 24
#define WS2812_BUFFERSIZE (NR_OF_LEDS_PER_CH*BITS_PER_LED)


#define NR_OF_WAITCOUNTS (18)

/* interfaces */
extern void ws2812_init( void );

#endif /* SRC_HAL_WS2812_H_ */
