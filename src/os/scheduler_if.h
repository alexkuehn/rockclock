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

#ifndef SRC_OS_SCHEDULER_IF_H_
#define SRC_OS_SCHEDULER_IF_H_

/* external standard includes */
#include <stdint.h>

/* external includes */

/* project includes */

/* component includes */


/** @brief schedule table
 *
 * schedule table descriptor
 */
typedef struct
{
   uint32_t period;					/**< period of task in milliseconds */
   void (* periodic_cb)( void );	/**< pointer to function to execute */
} scheduletable_t;

/** @brief convert millisecond to system time unit
 *
 */
#define scheduler_period_ms(time) (time)

/** @brief task state types
 *
 */
typedef enum
{
  SCHED_STOP = 0,			/**< task is stopped */
  SCHED_RUN					/**< task is running */
} schedule_ctrl_state_t;


/** @brief scheduler process
 *
 * executes the scheduler process within the system loop
 *
 */
extern void scheduler_process();

/** @brief set process run state
 *
 * set the state of a scheduler arbitrated process
 *
 * @param[in] processid ID of the controlled process
 *
 * @param[in] controlcmd new state of the controlled process
 */
extern void scheduler_control( uint8_t processid, schedule_ctrl_state_t controlcmd );


#endif /* SRC_OS_SCHEDULER_IF_H_ */
