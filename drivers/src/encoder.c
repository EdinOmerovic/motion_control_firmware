#include "encoder.h"

#define STARTING_VALUE 0

void encoder_init(Encoder *enc)
{
    enc->configure = _configure;
    enc->getValue = _getValue;
    enc->setValue = _setValue;
    // Quadratic encoder pulse module
    InitEQep1Gpio();
    //InitEPwm1Gpio();
}

void _configure(Encoder *enc, EncoderConf *confg, POSSPEED *module)
{
    enc->enc_confg = confg;    
    enc->previous_encoder_value = STARTING_VALUE;
    enc->current_absolute_position = confg->startingValue;
    enc->module = module;
    // QEP
    enc->module->init(enc->module);
}


// Get absolute position in nanometers
Uint32 _getValue(Encoder *enc)
{
    // read the counter value from the quadratic encode module
    Uint32 new_encoder_value = enc->module->read(enc->module);
    
    // Overflow and underflow protection
    if (new_encoder_value >= enc->previous_encoder_value){
        enc->current_absolute_position += (new_encoder_value - enc->previous_encoder_value) / enc->enc_confg->scalingFactor;
    }else{
        enc->current_absolute_position -= (enc->previous_encoder_value - new_encoder_value) / enc->enc_confg->scalingFactor;
    }
    
    // Napravi da se ne moze biti negativna vrijednost
    // TODO: check if the estimated position is within ranges. Did we hit overflow?
    
    enc->previous_encoder_value = new_encoder_value;

    return enc->current_absolute_position;
}

void _setValue(Encoder *enc, Uint32 value)
{
    enc->current_absolute_position = value;
}
