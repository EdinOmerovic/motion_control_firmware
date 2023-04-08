#include "encoder.h"


void encoder_init(Encoder *enc)
{
    enc->configure = _configure;
    enc->getValue = _getValue;
    enc->setValue = _setValue;
}

void _configure(Encoder *enc, EncoderConf *confg)
{
    enc->enc_confg = confg;    
    enc->previous_encoder_value = 0;
    enc->current_absolute_position = confg->startingValue;
}



int _getValue(Encoder *enc)
{
    // Očitati vrijednost iz GLOBAL_POS_REGISTRA. Vjerovatno trebaš negdje imati pohranjenu prethodnu vrijednost
    // Esimiraš novu vrijednost na osnovu ove 

    // read ENC_POS
    int new_encoder_value; // ret raw ecoder value 
    
    enc->current_absolute_position = new_encoder_value / enc->enc_confg->scalingFactor;
    
    // TODO: check if the estimated position is within ranges. Did we hit overflow?
    
    enc->previous_encoder_value = new_encoder_value;

    return enc->current_absolute_position;
}

void _setValue(Encoder *enc, int value)
{
    enc->current_absolute_position = value;
}