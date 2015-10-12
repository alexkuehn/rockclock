/*
 * scheduler_if.h
 *
 *  Created on: 06.08.2015
 *      Author: alex
 */

#ifndef SRC_OS_SCHEDULER_IF_H_
#define SRC_OS_SCHEDULER_IF_H_

#include <stdint.h>

/* definition of schedule table */
typedef struct
{
   uint32_t period;
   void (* periodic_cb)( void );
} scheduletable_t;

#define scheduler_period_ms(time) (time)

/* definition of scheduler control states */
typedef enum
{
  SCHED_STOP = 0,
  SCHED_RUN
} schedule_ctrl_state_t;


/* interface: periodic process scheduler
 */
extern void scheduler_process();

/* interface: control process scheduling
 * processid: id of controlled process
 * controlcmd: cmd id to set process state
 */
extern void scheduler_control( uint8_t processid, schedule_ctrl_state_t controlcmd );


#endif /* SRC_OS_SCHEDULER_IF_H_ */
