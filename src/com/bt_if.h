/*
 * This file is part of ROCKCLOCK firmware
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

#ifndef SRC_COM_BT_IF_H_
#define SRC_COM_BT_IF_H_

/* external standard includes */
#include <stdint.h>

/* external includes */

/* project includes */

/* component includes */


/** @brief initialize the bluetooth component
 *
 */
extern void bt_init( void );

/** @brief get bluetooth initialization state
 *
 * gets the state of the initialization of bluetooth component
 *
 * @return boolean state, True if initialized
 */
extern uint8_t bt_is_ready( void );

#endif /* SRC_COM_BT_IF_H_ */
