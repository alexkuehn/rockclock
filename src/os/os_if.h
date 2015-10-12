/*
 * os_if.h
 *
 *  Created on: 06.08.2015
 *      Author: alex
 */

#ifndef SRC_OS_OS_IF_H_
#define SRC_OS_OS_IF_H_

#include <stdint.h>

/* interface: initialize os components
 */
extern void os_init( void );

extern void delay( uint32_t millis);

#endif /* SRC_OS_OS_IF_H_ */
