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

typedef struct {
	uint8_t h;
	uint8_t m;
} dcftime_t;

extern uint8_t dcf_decode_eval_frame_q( uint64_t frame );
extern uint8_t dcf_decode_eval_inter_frame_q( dcftime_t act, dcftime_t last);

extern uint8_t dcf_decode_bcd( uint8_t input);

#endif /* SRC_APP_DCF_DECODE_H_ */
