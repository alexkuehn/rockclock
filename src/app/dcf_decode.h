/*
 * dcf_decode.h
 *
 *  Created on: 19.10.2015
 *      Author: alex
 */

#ifndef SRC_APP_DCF_DECODE_H_
#define SRC_APP_DCF_DECODE_H_

typedef struct {
	uint8_t h;
	uint8_t m;
} dcftime_t;

extern uint8_t dcf_decode_eval_frame_q( uint64_t frame );
extern uint8_t dcf_decode_eval_inter_frame_q( dcftime_t act, dcftime_t last);

extern uint8_t dcf_decode_bcd( uint8_t input);

#endif /* SRC_APP_DCF_DECODE_H_ */
