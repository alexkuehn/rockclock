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

#ifndef SRC_HAL_USART_IF_H_
#define SRC_HAL_USART_IF_H_

#include <stdint.h>

typedef enum {
	E_OK = 0,
	E_FULL = -1
} usart_status_t;

extern void usart_transmit_blocking( uint16_t data );
extern usart_status_t usart_transmit( uint8_t * data, uint16_t length);
extern int16_t usart_receive( uint8_t *buffer, uint16_t length, uint32_t timeout);
extern void usart_config_baudrate( uint32_t baudrate );
extern void usart_flush( void );

#endif /* SRC_HAL_USART_IF_H_ */
