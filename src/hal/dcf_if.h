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

#ifndef SRC_HAL_DCF_IF_H_
#define SRC_HAL_DCF_IF_H_

/* external standard includes */
#include <stdint.h>

/* external includes */

/* project includes */

/* component includes */


/** @brief initialize DCF receiver component
 */
extern void dcf_init( void );

/** @brief get new second marker
 *
 * gets a new second marker. When the marker was fetched, the mailbox gets empty until new
 * second marker arrives
 *
 * @return True:second marker found since last fetch; False: no new second
 */
extern uint8_t dcf_mailbox_second_flag_get( void );

/** @brief get new decoded DCF77 frame
 *
 * gets a new DCF77 frame within a 64bit value (bits0-59) from the decoder mailbox
 *
 * @param[out] rawdcf pointer to 64bit memory location where new frame shoud be stored
 *
 * @return True:new frame available; False: no new frame available
 */
extern uint8_t dcf_mailbox_data_get( uint64_t *rawdcf);


#endif /* SRC_HAL_DCF_IF_H_ */
