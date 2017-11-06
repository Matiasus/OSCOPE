/** 
 * Menu display functions 
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
  "f = 40kHz [T= 25us]", 
  "f = 10kHz [T=100us]", 
  "f =  2kHz [T=0.5ms]", 
  "f = 500Hz [T=  2ms]"
};
// submenu text - voltages
const volatile char *_submenu_2_items[SUBMENU_2_ITEMS] = {
  "GAIN 1x", 
  "GAIN 2x",
  "GAIN 4x"
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
//   40kHz ( 25us) -> OCR0 =  49; N =   8; (ADC PRESCALER  16)
//   10kHz (100us) -> OCR0 = 199; N =   8; (ADC PRESCALER  32)
//    2kHz (0.5ms) -> OCR0 = 124; N =  64; (ADC PRESCALER  32)
//   500Hz (  2ms) -> OCR0 = 124; N = 256; (ADC PRESCALER 128)
// OCR0 
// Timer0 prescaler
// Ad converter prescaler
const volatile uint8_t _submenu_1_set[SUBMENU_1_ITEMS][3] = {
  { 49, PRESCALER_8,   ADC_PRESCALER_16}, 
  {199, PRESCALER_8,   ADC_PRESCALER_32}, 
  {124, PRESCALER_64,  ADC_PRESCALER_32}, 
  {124, PRESCALER_256, ADC_PRESCALER_128}
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
void SetVolt()
{
  // set position for values
  SetPosition(132, 8);
  // ---------------------------------------------
  // GAIN 1
  if ((_flag & 0x0C) == 0x00) {
    // set input voltage
    VOLT_CONTROL_PORT |= (1 << VOLT_CONTROL_1);
    // set input voltage
    VOLT_CONTROL_PORT &= ~((1 << VOLT_CONTROL_2)|
                           (1 << VOLT_CONTROL_3));
  // ---------------------------------------------
  // GAIN 2
  } else if ((_flag & 0x0C) == 4) {
    // set input voltage
    VOLT_CONTROL_PORT |= (1 << VOLT_CONTROL_2);
    // set input voltage
    VOLT_CONTROL_PORT &= ~((1 << VOLT_CONTROL_1)|
                           (1 << VOLT_CONTROL_3));
  // ---------------------------------------------
  // GAIN 4
  } else if ((_flag & 0x0C) == 8) {
    // set input voltage
    VOLT_CONTROL_PORT |= (1 << VOLT_CONTROL_3);
    // set input voltage
    VOLT_CONTROL_PORT &= ~((1 << VOLT_CONTROL_1)|
                           (1 << VOLT_CONTROL_2)); 
  }
}

/**
 * @description 
 *
 * @param  Void
 * @return Void
 */
void ShowValues()
{
  // condition for show values
  if ((_flag & 0x30) == 0) {
    // FREQUENCY
    // -------------------------------------
    // set position for values
    SetPosition(132, 6);
    // draw values
    DrawString("FREQ=", 0xffff, X1);
    // set position for values
    SetPosition(132, 16);
    // check which value frequency
    if ((_flag & 0x03) == 0) {
      // draw values
      DrawString("40kHz", 0xffff, X1);
    } else if ((_flag & 0x03) == 1) {
      // draw values
      DrawString("10kHz", 0xffff, X1);
    } else if ((_flag & 0x03) == 2) {
      // draw values
      DrawString(" 2kHz", 0xffff, X1);
    } else if ((_flag & 0x03) == 3) {
      // draw values
      DrawString("500Hz", 0xffff, X1);
    }
    // VOLTAGES
    // -------------------------------------
    // set position for voltages
    SetPosition(132, 38);
    // draw values
    DrawString("GAIN=", 0xffff, X1);
    // set position for values
    SetPosition(132, 48);
    // check which value frequency
    if ((_flag & 0x0C) == 0x00) {
      // draw values
      DrawString(" 1x", 0xffff, X1);
    } else if ((_flag & 0x0C) == 0x04) {
      // draw values
      DrawString(" 2x", 0xffff, X1);
    } else if ((_flag & 0x0C) == 0x08) {
      // draw values
      DrawString(" 4x", 0xffff, X1);
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
  // Main axis show
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
