/*
 * io_if.h
 *
 *  Created on: 06.08.2015
 *      Author: alex
 */

#ifndef SRC_HAL_IO_IF_H_
#define SRC_HAL_IO_IF_H_

#include "io_config.h"

extern void io_toggle( uint32_t port, uint16_t pos);
extern void io_on( uint32_t port, uint16_t pos);
extern void io_off( uint32_t port, uint16_t pos);
extern uint8_t io_get( uint32_t port, uint16_t pos);

#endif /* SRC_HAL_IO_IF_H_ */
