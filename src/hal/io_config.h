/*
 * io_config.h
 *
 *  Created on: 06.08.2015
 *      Author: alex
 */

#ifndef SRC_HAL_IO_CONFIG_H_
#define SRC_HAL_IO_CONFIG_H_

#include <libopencm3/stm32/gpio.h>

#define LED_BLUE_PORT 	GPIOC
#define LED_BLUE_PIN  	GPIO8

#define HC05_CMD_PORT	GPIOA
#define HC05_CMD_PIN	GPIO1

#define DCF_IN_PORT		GPIOC
#define DCF_IN_PIN		GPIO0

#endif /* SRC_HAL_IO_CONFIG_H_ */
