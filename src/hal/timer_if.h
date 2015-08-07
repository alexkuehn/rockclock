/*
 * timer_if.h
 *
 *  Created on: 06.08.2015
 *      Author: alex
 */

#ifndef SRC_HAL_TIMER_IF_H_
#define SRC_HAL_TIMER_IF_H_

#include <stdint.h>

/* interface: get timer value
 * returns: 32bit timer increased every ms
 */
extern uint32_t timer_get( void );

#endif /* SRC_HAL_TIMER_IF_H_ */
