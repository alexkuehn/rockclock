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

/** @defgroup ProjectMain Project Main Entry Point
 * @{
 */

/* external standard includes */

/* external includes */

/* project includes */
#include "hal/hal_if.h"
#include "os/os_if.h"
#include "os/scheduler_if.h"
#include "app/app_if.h"

/* component includes */

/** @brief main project entry point.
 *
 * the main entry point for the project with initialization and loop scheduling
 */
int main(void)
{
	/* initialize the subsystems */
	hal_init();
	os_init();
	/* last component to initialize is the application itself */
	app_init();

	/* main system loop */
	while (1) {
		/* runs the application task scheduler */
		scheduler_process();

		/* if the application needs a main loop process,
		 * execute it here
		 */
#ifdef LOOP_CALLBACK
		LOOP_CALLBACK();
#endif
	}

	return 0;	/* should never be reached in embedded system */
}

/**@}*/
