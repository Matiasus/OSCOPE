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
  char *v_frequencies[ITEMS_FREQUENCIES] = {
    "f =  40kHz [T =  25us]", 
    "f =  10kHz [T = 0.1ms]", 
    "f = 2.5kHz [T = 0.4ms]", 
    "f =   1kHz [T =   1ms]"
  };
  // declaration & definition
  char *v_items[ITEMS] = {
    "1 FREQENCIES", 
    "2 VOLTAGES", 
    "3 VALUES", 
    "4 AXIS"
  };
  // main menu
  char v_menu = (0x0f & _selector);
  // sub menu
  char v_sub_menu = (_selector >> 4);
  // submenu not selected
  if (v_menu == 0) {
    // if no exceed
    if (v_submenu == 0) {
      // clear screen
      ClearScreen(0x0000);
    }
    // check if no exceed max items
    if (v_menu < ITEMS) {
      // increment
      _selector++;
      // show menu
      f_show_menu(v_items, ITEMS, (_selector & 0x0f));
    } else {
      // null menu and submenu
      _selector = 0;
    }
  // item 1 of menu
  } else if (v_menu == 1) {
    // check if no exceed submenu items
    if (v_submenu < ITEMS_FREQUENCIES) {
      // needed clear the screen for the first time
      if (v_submenu == 0) {
        // clear screen
        ClearScreen(0x0000);
      }
      // increment low menu and move to the left 4 position
      _selector = (((v_submenu++) << 4) | v_menu);
      // show submenu
      f_show_menu(v_frequencies, ITEMS_FREQUENCIES, v_submenu);
    } else {
      // clear screen
      ClearScreen(0x0000);
      // null submenu 
      _selector = (0x0f & _selector);
      // show menu
      f_show_menu(items, ITEMS, (_selector & 0x0f));
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
  // sreg value
  char v_sreg;
  // frequencies menu
  char *v_frequencies[ITEMS_FREQUENCIES] = {
    "f =  40kHz [T =  25us]", 
    "f =  10kHz [T = 0.1ms]", 
    "f = 2.5kHz [T = 0.4ms]", 
    "f =   1kHz [T =   1ms]"
  };
  // array buffer
  //   40kHz ( 25us) -> OCR0 =  49; N =  8; (ADC PRESCALER 16)
  //   10kHz (100us) -> OCR0 = 199; N =  8; (ADC PRESCALER 32)
  //  2.5kHz (0.4ms) -> OCR0 =  99; N = 64; (ADC PRESCALER 32)
  //    1kHz (  1ms) -> OCR0 = 249; N = 64; (ADC PRESCALER 32)
  // OCR0, Timer0 prescaler, Ad converter prescaler
  char v_settings_freq[ITEMS_FREQUENCIES][3] = {
    { 49,  PRESCALER_8, ADC_PRESCALER_16}, 
    {199,  PRESCALER_8, ADC_PRESCALER_32}, 
    { 99, PRESCALER_64, ADC_PRESCALER_32}, 
    {249, PRESCALER_64, ADC_PRESCALER_32}
  };
  // main menu
  char v_menu = (0x0f & _selector);
  // sub menu
  char v_sub_menu = (_selector >> 4);
  // needed clear the screen for the first time
  if (v_menu == 1) {
    // needed clear the screen for the first time
    if (v_submenu == 0) {
      // clear screen
      ClearScreen(0x0000);
      // increment low menu and move to the left 4 position
      _selector = (((v_submenu++) << 4) | v_menu);
      // show submenu
      f_show_menu(v_frequencies, ITEMS_FREQUENCIES, v_submenu);
    } else {
      // save SREG values
      v_sreg = SREG;
      // disable interrupts
      cli();      
      // switch off timer0
      TIMER0_START(0);
      // switch off ADC
      ADC_PRESCALER(0);
      // set timer counter
      OCR0 = v_settings_freq[v_submenu-1][0];
      // set prescaler timer 0
      TIMER0_START(v_settings_freq[v_submenu-1][1]);
      // set prescaler ADC
      ADC_PRESCALER(settings_freq[v_submenu-1][2]);
      // enable adc 
      TIFR |= (1 << OCF0);
      // set stored values
      SREG = v_sreg;
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
