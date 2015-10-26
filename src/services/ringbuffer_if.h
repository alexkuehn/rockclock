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

#ifndef SRC_SERVICES_RINGBUFFER_IF_H
#define SRC_SERVICES_RINGBUFFER_IF_H

/* external standard includes */
#include <stdint.h>

/* external includes */

/* project includes */

/* component includes */


/** @brief Ringbuffer class
 *
 * ringbuffer descriptor class
 */
typedef struct {
	uint8_t *buffer;			/**< pointer to payload memory */
	uint16_t length;			/**< length of buffer */
	volatile uint16_t readpos;	/**< actual read position */
	volatile uint16_t writepos; /**< actual write position */
} ringbuffer_t;

/** @brief initializes ringbuffer
 *
 * initializes the ringbuffer object with cleared read/write positions and sets memory
 * position
 *
 * @param[in] rb ringbuffer object
 *
 * @param[in] databuffer memory location for ringbuffer payload
 *
 * @param[in] datalength length of ringbuffer payload
 */
extern void ringbuffer_init( ringbuffer_t *rb, void  *databuffer, uint16_t datalength );

/** @brief get amount of free ringbuffer elements
 *
 * gets the number of free elements of a ringbuffer
 *
 * @param[in] rb ringbuffer object
 *
 * @return number of free elements
 */
extern uint16_t ringbuffer_get_free( ringbuffer_t *rb );

/** @brief get amount of used ringbuffer elements
 *
 * gets the number of used elements of a ringbuffer
 *
 * @param[in] rb ringbuffer object
 *
 * @return number of used elements
 */
extern uint16_t ringbuffer_get_used( ringbuffer_t *rb );

/** @brief get length of ringbuffer
 *
 * gets the length of ringbuffer payload
 *
 * @param[in] rb ringbuffer object
 *
 * @return length of ringbuffer
 */
extern uint16_t ringbuffer_get_length( ringbuffer_t *rb );

/** @brief flushs a ringbuffer
 *
 * flushs the ringbuffer by resetting read/write positions
 *
 * @param[in] rb ringbuffer object
 *
 */
extern void ringbuffer_flush( ringbuffer_t *rb );

/** @brief take data from ringbuffer
 *
 * take elements from ringbuffer
 *
 * @param[in] rb ringbuffer object
 *
 * @param[out] databuffer memory location where data should be placed
 *
 * @param[in] datalength how much data should be taken
 *
 * @return how many elements were really taken from the buffer
 */
extern uint8_t ringbuffer_get_data( ringbuffer_t *rb, void *databuffer, uint16_t datalength);

/** @brief put data into ringbuffer
 *
 * write data elements into the ringbuffer
 *
 * @param[in] rb ringbuffer object
 *
 * @param[in] databuffer memory location where data should be copied from
 *
 * @param[in] datalength how much data should be placed into ringbuffer
 *
 * @return number of real copied elements
 */
extern uint8_t ringbuffer_put_data( ringbuffer_t *rb, void *databuffer, uint16_t datalength);

#endif
