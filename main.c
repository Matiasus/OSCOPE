/** 
 * Example of TFT controller st7735 wit ADC101S101
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       23.01.2016
 * @file        main.c
 * @tested      AVR Atmega16
 * @inspiration http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 * ----------------------------------------------------------------------------------
 */
#include <avr/interrupt.h>
#include "lib/st7735.h"
#include "lib/auxillary.h"

/**
 * @description ADC end converting
 *
 * @param ADC_vect
 * @return Void
 */
ISR(ADC_vect) 
{
  // set to zero after match registers TCNT0 a OCR1B
  TIFR |= (1 << OCF1B);
  // value
  _buffer[_index++] = ADCH;
}

/**
 * @description Main function
 *
 * @param Void
 * @return Void
 */
int main(void)
{
  // loading logo
  ShowLoading();  
  // scope function
  StartScope();
  // return & exit
  return 0;
}
