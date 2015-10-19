/*
 * dcf_if.h
 *
 *  Created on: 16.10.2015
 *      Author: alex
 */

#ifndef SRC_HAL_DCF_IF_H_
#define SRC_HAL_DCF_IF_H_

#include <stdint.h>

extern void dcf_init( void );
extern uint8_t dcf_mailbox_second_flag_get( void );
extern uint8_t dcf_mailbox_data_get( uint64_t *rawdcf);


#endif /* SRC_HAL_DCF_IF_H_ */
