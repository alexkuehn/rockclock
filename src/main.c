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

#include "hal/hal_if.h"
#include "os/os_if.h"
#include "os/scheduler_if.h"
#include "app/app_if.h"


int main(void)
{
	hal_init();
	os_init();
	app_init();


	while (1) {
		scheduler_process();

#ifdef LOOP_CALLBACK
		LOOP_CALLBACK();
#endif
	}

	return 0;
}
