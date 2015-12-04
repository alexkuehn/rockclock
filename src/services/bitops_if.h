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

#ifndef SRC_SERVICES_BITOPS_IF_H_
#define SRC_SERVICES_BITOPS_IF_H_

/* external standard includes */
#include <stdint.h>

/* external includes */

/* project includes */

/* component includes */

/** @brief generate 64bit bitmask at position
 *
 * generates a 64bit mask for bit at position
 *
 * @param[in] pos position of set bit in mask
 *
 * @return 64bit mask with bit at postion set
 */
#define BITOPS64_BIT(pos) (1ULL<<pos)

/** @brief generate a 64bit mask with a explicit length
 *
 * generates a 64bit mask with all bits set for a given length
 *
 * @param[in] len length of set mask
 *
 * @return 64bit mask with length masked in
 */
#define BITOPS64_MASK( len ) (BITOPS64_BIT(len)-1)

/** @brief extract a value with variable length out of an 64bit field
 *
 * with a given length and position, a value is extracted out of a 64bit filed
 *
 * @param[in] val 64bit input value
 * @param[in] pos startposition of extracted value
 * @param[in] length length of extracted value
 *
 * @return 64bit value extracted and aligned with position0
 */
#define BITOPS64_EXTRACT( val, pos, len) ( ((val)>>pos) & BITOPS64_MASK(len) )

/** @brief counts number of set bits in a 64bit value
 *
 * counts the number of bits set in a 64bit value and returns the value
 *
 * @param[in] bits 64bit input value
 *
 * @return number of set bits
 */
extern uint8_t bitops64_count( uint64_t bits);

/** @brief counts number of set bits in a 32bit value
 *
 * counts the number of bits set in a 32bit value and returns the value
 *
 * @param[in] bits 32bit input value
 *
 * @return number of set bits
 */
extern uint8_t bitops32_count( uint32_t bits);

/** @brief convert bcd coded value to decimal
 *
 * convert bcd coded digits to decimal value
 *
 * @param[in] bcdval bcd coded digits
 *
 * @return decimal value
 */
extern uint8_t bcd2dec( uint8_t bcdval );

/** @brief convert decimal value to bcd coded value
 *
 * convert decimal value to bcd coded value
 *
 * @param[in] decval bcd coded digits
 *
 * @return bcd coded representation
 */
extern uint8_t dec2bcd( uint8_t decval );

#endif /* SRC_SERVICES_BITOPS_IF_H_ */
