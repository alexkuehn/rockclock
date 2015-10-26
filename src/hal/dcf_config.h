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

#ifndef SRC_HAL_DCF_CONFIG_H_
#define SRC_HAL_DCF_CONFIG_H_

/* external standard includes */

/* external includes */

/* project includes */

/* component includes */


#define CFG_DCF_DEBUG_NONE 0			/**< DCF component debug mode, deactivated */
#define CFG_DCF_DEBUG_PHY 1				/**< DCF component debug mode, physical layer over UART */
#define CFG_DCF_DEBUG_SIG 2				/**< DCF component debug mode, signal over UART */
#define CFG_DCF_DEBUG_SIG_LED 4			/**< DCF component debug mode, signal over LED */
#define CFG_DCF_DEBUG_SYNC_LED 8		/**< DCF component debug mode, sync pulse over LED */
#define CFG_DCF_DEBUG_SYNC_MACH 16		/**< DCF component debug mode, protocol state over UART */

#define CFG_DEBUG_DCF_SAMPLING ( CFG_DCF_DEBUG_SYNC_LED | CFG_DCF_DEBUG_SYNC_MACH)	/**< DCF component configured debug flags */


/* --> DCF receiver parameters */
#define DCF_DEBOUNCE 3			/**< number of consecutive samplepoints until bit value is determined */

#define DCF_STATE_SYNC 1200 	/**< duration of low phase in [ms] for DCF syncpulse detection */
#define DCF_STATE_MIN 500 		/**< minimum low pulse time between transitions in [ms] for valid signal */

#define DCF_STATE_LO_MIN 70		/**< minimum low bit time in [ms] for 0-bit detection */
#define DCF_STATE_LO_MAX 130	/**< maximum low bit time in [ms] for 0-bit detection */
#define DCF_STATE_HI_MIN 160	/**< minimum high bit time in [ms] for 1-bit detection */
#define DCF_STATE_HI_MAX 230	/**< maximum high bit time in [ms] for 1-bit detection */
/* <-- DCF receiver parameters */

#endif /* SRC_HAL_DCF_CONFIG_H_ */
