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
#ifndef SRC_HAL_DCF_H_
#define SRC_HAL_DCF_H_

/* external standard includes */
#include <stdint.h>

/* external includes */

/* project includes */

/* component includes */

/** @brief process DCF signal @sample time
 *
 * called with the 1ms sample time, the incoming DCf signal is processed
 */
extern void dcf_sample_process( void );

/** @brief DCF signal protocol decoder process
 *
 * decodes the physical layer DCF protocol with sample input
 */
extern void dcf_signal_process( uint8_t dcfsignal );

/** @brief DCF protocol decoder state
 *
 */
typedef enum
{
	DCF_SIG_INIT,		/**< protocol in initialization phase */
	DCF_SIG_SYNCED		/**< protocol frame synced */
} dcf_sig_state_t;

#endif /* SRC_HAL_DCF_H_ */
