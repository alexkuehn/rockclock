/*
 * usart_if.h
 *
 *  Created on: 07.08.2015
 *      Author: alex
 */

#ifndef SRC_HAL_USART_IF_H_
#define SRC_HAL_USART_IF_H_

#include <stdint.h>

typedef enum {
	E_OK = 0,
	E_FULL = -1
} usart_status_t;

extern void usart_transmit_blocking( uint16_t data );
extern usart_status_t usart_transmit( uint8_t * data, uint16_t length);
extern uint16_t usart_receive( uint8_t *buffer, uint16_t length);
extern void usart_config_baudrate( uint32_t baudrate );

#endif /* SRC_HAL_USART_IF_H_ */
