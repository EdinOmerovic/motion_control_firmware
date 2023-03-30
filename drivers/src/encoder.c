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
}

int _getValue(Encoder *enc)
{
    return 0;
}

void _setValue(Encoder *enc, int value)
{
}