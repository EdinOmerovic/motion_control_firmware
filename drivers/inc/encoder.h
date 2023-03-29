/**
 * High level library for encoder 
 * 
 * The responsibility of this library are:
 *  *
 *  * Obtaining the absolute values of encoder position control based on the initialization parameters
 * 
 *  
*/


/**
 * Kako bi izgledalo korištenje:
 * Encoder* encoder = null;
 * encoder_init(&encoder)
 * 
 * encoder->configure(struct EncoderConfig) // postavi sve parametre neophodne za 
 * encoder->_read // Očitaj sve potebne vrijednosti sa sistema kako bi omogućio da se odredi povratna vrijednosti
 * encoder->getValue(void) // daj apsolutnu vrijednost enkodera u centimetrima od početne pozicije. 
 * encoder->setValue(int) // ovo će se koristiti za nuliranje enkodera ukoliko dođe do zakucavanja.
*/

typedef struct
{
    // dummy 
    int scalingParameteres;
    int absoluteDimentsions;
    int startingValue; 
}EncoderConf;

typedef struct
{
    void (*configure)(EncoderConf*);
}Encoder;



void _configure(Encoder* , EncoderConf*);

void _configure(Encoder* enc, EncoderConf* confg){

}