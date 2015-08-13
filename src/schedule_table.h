/*
 * schedule_table.h
 *
 *  Created on: 06.08.2015
 *      Author: alex
 */

#ifndef SRC_SCHEDULE_TABLE_H_
#define SRC_SCHEDULE_TABLE_H_


#include "app/app_if.h"

/* definition of scheduled tasks */
/* structure is built according to following rule
 * { scheduler_period_ms( <<PERIODIC_TIME>> ), <<PTR TO TASK PROCESS>> }
 */
#define schedule_cb_table                          \
{                                                  \
   { scheduler_period_ms(1000),  &blink_process  },       \
   { scheduler_period_ms(3000), &hello_process   },      \
   { scheduler_period_ms(50),	&led_process	}, 	\
}

#define PROCESS_BLINK 0
#define PROCESS_HELLO 1
#define PROCESS_LED 2


#endif /* SRC_SCHEDULE_TABLE_H_ */
