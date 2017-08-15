/** 
 * Autotriggered OSCOPE with Timer0
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       15.08.2017
 * @file        main.c
 * @tested      AVR Atmega16
 * @inspiration 
 * ----------------------------------------------------------------------------------
 */
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lib/st7735.h"
#include "lib/oscope.h"

/**
 * @description INT0 interrupt
 *
 * @param  INT0_vect
 * @return Void
 */
ISR(INT0_vect) 
{
  // frequencies menu
  char *frequencies[ITEMS_FREQUENCIES] = {
    "f =  40kHz [T =  25us]", 
    "f =  10kHz [T = 0.1ms]", 
    "f = 2.5kHz [T = 0.4ms]", 
    "f =   1kHz [T =   1ms]"
  };
  // declaration & definition
  char *items[ITEMS] = {
    "1 FREQENCIES", 
    "2 VOLTAGES", 
    "3 VALUES", 
    "4 AXIS"
  };
  // main menu
  uint8_t menu = (0x0f & _selector);
  // sub menu
  uint8_t submenu = (_selector >> 4);
  // submenu not selected
  if (submenu == 0) {
    // if no exceed
    if (menu < ITEMS) {
      // needed clear the screen for the first time
      if (menu == 0) {
        // clear screen
        ClearScreen(0x0000);
      }
      // increment
      _selector++;
      // show menu
      ShowItems(items, ITEMS, (_selector & 0x0f));
    } else {
      // null menu and submenu
      _selector = 0;
    }
  // item 1 of menu
  } else if (menu == 1) {
    // check if no exceed submenu items
    if (submenu < ITEMS_FREQUENCIES) {
      // needed clear the screen for the first time
      if (submenu == 0) {
        // clear screen
        ClearScreen(0x0000);
      }
      // increment low menu and move to the left 4 position
      _selector = (((submenu+1) << 4) | menu);
      // show submenu
      ShowItems(frequencies, ITEMS_FREQUENCIES, submenu+1);
    } else {
      // clear screen
      ClearScreen(0x0000);
      // null submenu 
      _selector = (0x0f & _selector);
      // show menu
      ShowItems(items, ITEMS, (_selector & 0x0f));
    }    
  }
}

/**
 * @description INT1 interrupt
 *
 * @param  INT1_vect
 * @return Void
 */
ISR(INT1_vect) 
{
  // frequencies menu
  char *frequencies[ITEMS_FREQUENCIES] = {
    "f =  40kHz [T =  25us]", 
    "f =  10kHz [T = 0.1ms]", 
    "f = 2.5kHz [T = 0.4ms]", 
    "f =   1kHz [T =   1ms]"
  };
  // main menu
  uint8_t menu = (0x0f & _selector);
  // sub menu
  uint8_t submenu = (_selector >> 4);
  // needed clear the screen for the first time
  if (menu == 1) {
    // needed clear the screen for the first time
    if (submenu == 0) {
      // clear screen
      ClearScreen(0x0000);
      // increment low menu and move to the left 4 position
      _selector = (((submenu+1) << 4) | menu);
      // show submenu
      ShowItems(frequencies, ITEMS_FREQUENCIES, submenu+1);
    } else {
      // sreg value
      char sreg;
      // save SREG values
      sreg = SREG;
      // disable interrupts
      cli();      
      // switch off timer0
      TIMER0_START(0);
      // switch off ADC
      ADC_PRESCALER(0);
      // set timer counter
      OCR0 = _frequency[submenu-1][0];
      // set prescaler timer 0
      TIMER0_START(_frequency[submenu-1][1]);
      // set prescaler ADC
      ADC_PRESCALER(_frequency[submenu-1][2]);
      // enable adc 
      TIFR  |= (1 << OCF0);
      // set stored values
      SREG = sreg;
      // enable interrputs
      sei();
    }
  }
}

/**
 * @description ADC end converting
 *
 * @param  ADC_vect
 * @return Void
 */
ISR(ADC_vect) 
{
  // set to zero after match registers TCNT0 a OCR1B
  TIFR |= (1 << OCF1B);
  // check if index is less than max values
  if (_index++ < WIDTH)
  {
    // store value
    _buffer[_index-1] = ADCH;
  }
}

/**
 * @description Main function
 *
 * @param Void
 * @return Void
 */
int main(void)
{
  // init lcd driver
  St7735Init();
  // scope function
  StartScope();
  // return & exit
  return 0;
}
