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

typedef struct
{
    // dummy
    int scalingFactor;
    int absoluteDimentsions;
    int startingValue;
} EncoderConf;

typedef struct Encoder
{
    EncoderConf *enc_confg;
    // Function pointer table
    void (*configure)(struct Encoder *enc, EncoderConf *confg);
    int (*getValue)(struct Encoder *enc);
    void (*setValue)(struct Encoder *enc, int value);
    // Dynamic variables
    int previous_encoder_value;
    int current_absolute_position;

} Encoder;

void encoder_init(Encoder *enc);

void _configure(Encoder *enc, EncoderConf *confg);

int _getValue(Encoder *enc);

void _setValue(Encoder *enc, int value);
#endif
