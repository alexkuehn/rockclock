/*
 * app_if.h
 *
 *  Created on: 07.08.2015
 *      Author: alex
 */

#ifndef SRC_APP_APP_IF_H_
#define SRC_APP_APP_IF_H_

extern void app_init(void);
extern void blink_process(void);

extern void led_process(void);

extern void loop_process(void);
#define LOOP_CALLBACK loop_process

#endif /* SRC_APP_APP_IF_H_ */
