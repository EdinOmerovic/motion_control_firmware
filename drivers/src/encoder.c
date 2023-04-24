#include "encoder.h"

// Encoder object setup (function pointer mapping)
void encoder_init(Encoder *enc)
{
    enc->configure = _configure;
    enc->getValue = _getValue;
    enc->setValue = _setValue;
    // Quadratic encoder pulse hardware module init
    InitEQep1Gpio();
}

void _configure(Encoder *enc, EncoderConf *confg, POSSPEED *module)
{
    // Config mapping
    enc->enc_confg = confg;    
    // QEP module mapping
    enc->module = module;
    enc->module->init(enc->module);
    // Initial value
    enc->current_absolute_position = confg->startingValue;
}


void saturated_add(Uint32 *a, signed long *b)
{
    if (b >= 0)
    {
        // Overflow
        Uint32 res = *a + *b;
        if (res < *a) // This can only happen if overflow
        {
            *a = 0xffffffff;
            return;
        }

    }else{
        // Under-flow
        if( (*b)*(-1) >= *a )
        {
            *a = 0;
            return;
        }
    }

    *a += *b;
}

// Get absolute position in micrometers
Uint32 _getValue(Encoder *enc)
{
    // read the counter value from the quadratic encode module and calculate difference:
    signed long difference = enc->module->read(enc->module) - (enc->module->previous_value / enc->enc_confg->scalingFactor);
    
    // Overflow and underflow protection
    // Passing by reference.
    // Same as: enc->current_absolute_position += difference;
    saturated_add(&enc->current_absolute_position, &difference);
    
    return enc->current_absolute_position;
}

void _setValue(Encoder *enc, Uint32 value)
{
    enc->current_absolute_position = value;
}
