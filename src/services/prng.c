/*
 * prng.c
 *
 *  Created on: 06.08.2015
 *      Author: alex
 */

#include <stdint.h>

uint8_t prng_get( void )
{
  static uint8_t seed = 0xAA;
  static uint8_t a=0;

  seed ^=seed << 3;
  seed ^=seed >> 5;
  seed ^= a++ >> 2;
  return seed;
}
