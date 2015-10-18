/*
 * dcf_config.h
 *
 *  Created on: 18.10.2015
 *      Author: alex
 */

#ifndef SRC_HAL_DCF_CONFIG_H_
#define SRC_HAL_DCF_CONFIG_H_


#define CFG_DCF_DEBUG_NONE 0
#define CFG_DCF_DEBUG_PHY 1
#define CFG_DCF_DEBUG_SIG 2
#define CFG_DCF_DEBUG_SIG_LED 4

#define CFG_DEBUG_DCF_SAMPLING (CFG_DCF_DEBUG_PHY | CFG_DCF_DEBUG_SIG | CFG_DCF_DEBUG_SIG_LED)



#endif /* SRC_HAL_DCF_CONFIG_H_ */
