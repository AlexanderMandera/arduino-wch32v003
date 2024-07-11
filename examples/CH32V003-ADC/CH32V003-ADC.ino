#include <stdio.h>
#include <Arduino.h>



uint8_t adc_pins[] = {
    A2,         // A0
    A1,         // A1
    C4,         // A2
    D2,         // A3
    D3,         // A4
    D5,         // A5
    D6,         // A6
    D4,         // A7
    ADC_CH_REF, // A8
    ADC_CH_CAL, // A9
};


void setup()
{
  
}

void loop()
{
  printf("------------\n");
  for (int i = 0; i < sizeof(adc_pins); i++)
  {
    printf("ADC %d V=%d\n", i, analogRead(adc_pins[i]));
  }
  delay(3000);
}
