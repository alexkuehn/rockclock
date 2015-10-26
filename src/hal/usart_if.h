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

/* external standard includes */
#include <stdint.h>

/* external includes */

/* project includes */

/* component includes */


/** @brief usart buffer status
 *
 */
typedef enum {
	E_OK = 0,		/**< buffer fill status ok */
	E_FULL = -1		/**< buffer is full */
} usart_status_t;

/** @brief blocking transmit of data value over USART
 *
 * transmit the data over USART and wait until data was sent
 *
 * @param[in] data data to be sent
 */
extern void usart_transmit_blocking( uint16_t data );

/** @brief non-blocking transmit of data over USART
 *
 * buffer the data and transmit them over USART in background
 *
 * @param[in] data pointer to data to be sent
 *
 * @param[in] length of data to be sent
 *
 * @return transmit buffer status
 */
extern usart_status_t usart_transmit( uint8_t * data, uint16_t length);

/** @brief receive data from USART
 *
 * receive some amount of data and wait a specific time for the receiving
 *
 * @param[out] buffer memory location for storing received data
 *
 * @param[in] length length of data to be received
 *
 * @param[in] timeout timeout in [ms] until data receiving is finished
 *
 * @return number of received data
 */
extern int16_t usart_receive( uint8_t *buffer, uint16_t length, uint32_t timeout);

/** @brief configure the baudrate of USART channel
 *
 * set the USART baudrate
 *
 * @param[in] baudrate baudrate in baud
 *
 */
extern void usart_config_baudrate( uint32_t baudrate );

/** @brief flush the USART buffers
 *
 */
extern void usart_flush( void );

#endif /* SRC_HAL_USART_IF_H_ */
