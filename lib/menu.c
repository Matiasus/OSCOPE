/** 
 * Auxillary functions
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       01.11.2017
 * @file        menu.c
 * @tested      AVR Atmega16
 * @inspiration 
 *
 */
#include <avr/interrupt.h>
#include <stdlib.h>
#include <util/delay.h>
#include "st7735.h"
#include "oscope.h"
#include "menu.h"

volatile uint8_t _flag = 0x00;

// TEXT for display
// --------------------------------------------------------
// menu text
const volatile char *_menu_items[MENU_ITEMS] = {
  "1 FREQENCIES", 
  "2 VOLTAGES", 
  "3 VALUES", 
  "4 AXIS"
};
// submenu text - frequencies
const volatile char *_submenu_1_items[SUBMENU_1_ITEMS] = {
  "f =  40kHz [T= 25us]", 
  "f =  10kHz [T=0.1ms]", 
  "f = 2.5kHz [T=0.4ms]", 
  "f =   1kHz [T=  1ms]"
};
// submenu text - voltages
const volatile char *_submenu_2_items[SUBMENU_2_ITEMS] = {
  "U str", 
  "U max",
  "U min",
  "U rms" 
};
// submenu text - values
const volatile char *_submenu_3_items[SUBMENU_3_ITEMS] = {
  "Values - yes", 
  "Values - no"
};
// submenu text - axis show
const volatile char *_submenu_4_items[SUBMENU_4_ITEMS] = {
  "Main, auxillary - on", 
  "Main, auxillary - off",
  "Auxillary - off", 
  "Main - off"
};

// SETTINGS for microprocesor
// --------------------------------------------------------
// array buffer
//   40kHz ( 25us) -> OCR0 =  49; N =  8; (ADC PRESCALER 16)
//   10kHz (100us) -> OCR0 = 199; N =  8; (ADC PRESCALER 32)
//  2.5kHz (0.4ms) -> OCR0 =  99; N = 64; (ADC PRESCALER 32)
//    1kHz (  1ms) -> OCR0 = 249; N = 64; (ADC PRESCALER 32)
// OCR0 
// Timer0 prescaler
// Ad converter prescaler
const volatile uint8_t _submenu_1_set[SUBMENU_1_ITEMS][3] = {
  { 49, PRESCALER_8,ADC_PRESCALER_16}, 
  {199, PRESCALER_8,ADC_PRESCALER_32}, 
  { 99,PRESCALER_64,ADC_PRESCALER_32}, 
  {249,PRESCALER_64,ADC_PRESCALER_32}
};

/**
 * @description Change frequency settings
 *
 * @param  uint8_t
 * @return Void
 */
void SetFreq(uint8_t sel_2nd_level)
{
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
  OCR0 = _submenu_1_set[sel_2nd_level-1][0];
  // set prescaler timer 0
  TIMER0_START(_submenu_1_set[sel_2nd_level-1][1]);
  // set prescaler ADC
  ADC_PRESCALER(_submenu_1_set[sel_2nd_level-1][2]);
  // enable adc 
  TIFR |= (1 << OCF0);
  // set stored values
  SREG = sreg;
  // enable interrputs
  sei();
}

/**
 * @description 
 *
 * @param  Void
 * @return Void
 */
void ShowValues()
{
  if ((_flag & 0x30) == 0) {
    // set position for values
    SetPosition(132, 6);
    // check which value frequency
    if ((_flag & 0x03) == 0) {
      // draw values
      DrawString(" 40k", 0xffff, X1);
    } else if ((_flag & 0x03) == 1) {
      // draw values
      DrawString(" 10k", 0xffff, X1);
    } else if ((_flag & 0x03) == 2) {
      // draw values
      DrawString("2.5k", 0xffff, X1);
    } else if ((_flag & 0x03) == 3) {
      // draw values
      DrawString("  1k", 0xffff, X1);
    }
  }
}

/**
 * @description 
 *
 * @param  Void
 * @return Void
 */
void ShowAxis()
{

  // init value for x
  uint8_t i = OFFSET_X;
  // color line
  uint16_t color = 0x5C4B;

  // --------------------------------------------------
  // Main axes show
  if ((_flag & 0xC0) == 0) {
    // Main axis x
    // draw axis x
    DrawLineHorizontal(OFFSET_X, OFFSET_X+WIDTH, OFFSET_Y+HEIGHT, color);
    //  draw axis x
    DrawLineHorizontal(OFFSET_X, OFFSET_X+WIDTH, OFFSET_Y, color);
    // Main axis y
    //  draw axis y
    DrawLineVertical(OFFSET_X, OFFSET_Y, OFFSET_Y+HEIGHT, color);
    //  draw axis y
    DrawLineVertical(OFFSET_X+WIDTH, OFFSET_Y, OFFSET_Y+HEIGHT, color);
    // move to right
    i = STEP_X;
    // draw auxillary axis x
    while (i < WIDTH) {
      // draw auxillary signs up
      DrawLineVertical(i, OFFSET_Y, OFFSET_Y+HEIGHT, color);
      // move to right
      i += STEP_X;
    }
    // init value for y
    i = OFFSET_Y;
    // draw auxillary axis y
    while (i < HEIGHT+OFFSET_Y) {
      // move to right
      i += STEP_Y;
      // draw line
      DrawLineHorizontal(OFFSET_X, OFFSET_X+WIDTH, i, color);
    }
  // --------------------------------------------------
  // Auxillary axis show
  } else if ((_flag & 0xC0) == 192) {
    // move to right
    i = STEP_X;
    // draw auxillary axis x
    while (i < WIDTH) {
      // draw auxillary signs up
      DrawLineVertical(i, OFFSET_Y, OFFSET_Y+HEIGHT, color);
      // move to right
      i += STEP_X;
    }
    // init value for y
    i = OFFSET_Y;
    // draw auxillary axis y
    while (i < HEIGHT+OFFSET_Y) {
      // move to right
      i += STEP_Y;
      // draw line
      DrawLineHorizontal(OFFSET_X, OFFSET_X+WIDTH, i, color);
    }
  // --------------------------------------------------
  // Main axis show
  } else if ((_flag & 0xC0) == 128) {
    // Main axis x
    // draw axis x
    DrawLineHorizontal(OFFSET_X, OFFSET_X+WIDTH, OFFSET_Y+HEIGHT, color);
    //  draw axis x
    DrawLineHorizontal(OFFSET_X, OFFSET_X+WIDTH, OFFSET_Y, color);
    // Main axis y
    //  draw axis y
    DrawLineVertical(OFFSET_X, OFFSET_Y, OFFSET_Y+HEIGHT, color);
    //  draw axis y
    DrawLineVertical(OFFSET_X+WIDTH, OFFSET_Y, OFFSET_Y+HEIGHT, color);
  }
}
