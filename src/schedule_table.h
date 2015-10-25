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

#ifndef SRC_SCHEDULE_TABLE_H_
#define SRC_SCHEDULE_TABLE_H_


#include "app/app_if.h"

/* definition of scheduled tasks */
/* structure is built according to following rule
 * { scheduler_period_ms( <<PERIODIC_TIME>> ), <<PTR TO TASK PROCESS>> }
 */
#define schedule_cb_table                          \
{                                                  \
   { scheduler_period_ms(1000),	&clock_process	}, 	\
}

#define PROCESS_CLOCK 0



#endif /* SRC_SCHEDULE_TABLE_H_ */
