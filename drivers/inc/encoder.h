/**
 * High level library for encoder
 *
 * The responsibility of this library are:
 *  *
 *  * Obtaining the absolute values of encoder position control based on the initialization parameters
 *
 *
 */
#ifndef ENCODER_H
#define ENCODER_H
#include "encoder_hal.h"

typedef struct
{
    const Uint32 scalingFactor;
    const Uint32 startingValue;
} EncoderConf;

typedef struct Encoder
{
    EncoderConf *enc_confg;
    // Function pointer table
    void (*configure)(struct Encoder *enc, EncoderConf *confg, POSSPEED *module);
    Uint32 (*getValue)(struct Encoder *enc);
    void (*setValue)(struct Encoder *enc, Uint32 value);
    // Dynamic variables
    Uint32 current_absolute_position;
    // QEP module
    POSSPEED_handle module;

} Encoder;

void encoder_init(Encoder *enc);

void _configure(Encoder *enc, EncoderConf *confg, POSSPEED *module);


// Obtain value in absolute values in nanometers
Uint32 _getValue(Encoder *enc);

void _setValue(Encoder *enc, Uint32 value);
#endif
