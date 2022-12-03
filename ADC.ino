#include <avr/io.h>
#include "ADC.h"

void ADC_Init() {
  ADCSRA = ADCSRA | (1 << ADEN);
  ADCSRA = ADCSRA | (1 << ADPS0);
  ADCSRA = ADCSRA | (1 << ADPS1);
  ADCSRA = ADCSRA | (1 << ADPS2);

  ADMUX = ADMUX | (1 << REFS0);
  ADMUX = ADMUX & ~(1 << REFS1);
}

int ADC_Read(int channel){ 
  
  ADMUX = ADMUX & ~(1 << MUX0);
  ADMUX = ADMUX & ~(1 << MUX1);
  ADMUX = ADMUX & ~(1 << MUX2);
  ADMUX = ADMUX & ~(1 << MUX3);
  ADMUX = ADMUX | channel;

  ADCSRA = ADCSRA | (1 << ADSC);

  while(ADCSRA & (1 << ADSC));

  return ADC;
}