/*
 * ws2812_if.h
 *
 *  Created on: 09.08.2015
 *      Author: alex
 */

#ifndef SRC_HAL_WS2812_IF_H_
#define SRC_HAL_WS2812_IF_H_

#include <stdint.h>

#include "ws2812_config.h"
#include "ws2812.h"

extern uint8_t ws2812_get_updating( void );
extern void ws2812_update( void );

extern void ws2812_set_pixel( uint8_t row, uint8_t col, uint8_t red, uint8_t green, uint8_t blue );
extern void ws2812_clear( void );

#endif /* SRC_HAL_WS2812_IF_H_ */
