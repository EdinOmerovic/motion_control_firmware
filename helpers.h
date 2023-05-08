/*
 * helpers.h
 *
 *  Created on: 28 Apr 2023
 *      Author: Master
 */

#ifndef HELPERS_H_
#define HELPERS_H_
#include "F2837xD_device.h"

void saturated_add_3(signed long *result, signed long a, signed long b);

Uint32 map(signed long x, signed long in_min, signed long in_max, Uint32 out_min, Uint32 out_max);

signed long saturate(signed long value, signed long min, signed long max);

#endif /* HELPERS_H_ */
