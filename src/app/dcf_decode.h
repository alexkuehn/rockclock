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

#ifndef SRC_APP_DCF_DECODE_H_
#define SRC_APP_DCF_DECODE_H_

/* external standard includes */


/* external includes */

/* project includes */

/* component includes */


/** @brief DCF decoded time
 *
 */
typedef struct {
	uint8_t h;		/**< hours */
	uint8_t m;		/**< minutes */
} dcftime_t;

/** @brief check frame validity
 *
 * checks the validity of a DCF frame by extracting hours and minutes
 * and checking their parity bits and the constant minute markers
 *
 * @param[in] frame DCF77 64bit frame
 *
 * @return Boolean True, if frame is valid
 */
extern uint8_t dcf_decode_eval_frame_q( uint64_t frame );

/** @brief check inter frame validity
 *
 * checks the validity of a DCF frame by comparing the hours and minutes
 * with a previous frame information.
 * It is expected that the time is increasing only by one minute
 *
 * @param[in] act Time information from actual decoded frame
 *
 * @param[in] last Time information from last decoded frame
 *
 * @return Boolean True, if Time is plausible
 */
extern uint8_t dcf_decode_eval_inter_frame_q( dcftime_t act, dcftime_t last);

/** @brief converts BCD value in decimal
 *
 * converts the incoming BCD value into a decimal value
 *
 * @param[in] input BCD coded value
 *
 * @return decimal coded representation of input
 */
extern uint8_t dcf_decode_bcd( uint8_t input);

#endif /* SRC_APP_DCF_DECODE_H_ */
