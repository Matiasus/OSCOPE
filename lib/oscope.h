/** 
 * Basic functions for oscilloscop
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       04.11.2017
 * @file        oscope.h
 * @tested      AVR Atmega16
 * @inspiration 
 * ------------------------------------------------
 */

/** @definition */
#ifndef __OSCOPE_H__
#define __OSCOPE_H__

  /** @const Position loading image x start */
  #define POSITION_XS  20
  /** @const Position loading image x end */
  #define POSITION_XE 140
  /** @const Position loading image y start */
  #define POSITION_YS  40
  /** @const Position loading image y end */
  #define POSITION_YE  55

  // width (number of recorder data)
  #define WIDTH 127
  // height (number ADC resolution)
  #define HEIGHT 127
  // move x
  #define OFFSET_X 0
  // move y
  #define OFFSET_Y 0
  // step for auxillary axis x
  #define STEP_X 32
  // step for auxillary axis y
  #define STEP_Y 32
  // @var Timer/Counter prescalers
  #define PRESCALER_STOP    0
  #define PRESCALER_1       1
  #define PRESCALER_8       2
  #define PRESCALER_64      3
  #define PRESCALER_256     4
  #define PRESCALER_1024    5
  #define PRESCALER_T1_F    6
  #define PRESCALER_T1_R    7
  // @var ADC prescalers
  #define ADC_PRESCALER_16  4
  #define ADC_PRESCALER_32  5
  #define ADC_PRESCALER_64  6
  #define ADC_PRESCALER_128 7
  // Set Timer/Counter 0 prescaler
  #define TIMER0_START(PRESCALER) { TCCR0 &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); TCCR0 |= PRESCALER & 0x07; }
  // Set Timer/Counter 1 prescaler
  #define TIMER1A_START(PRESCALER) { TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); TCCR1B |= PRESCALER & 0x07; }
  // ADC channel selector
  #define ADC_CHANNEL(CHANNEL) { ADMUX &= 0xF8; ADMUX |= CHANNEL & 0x07; }
  // Set ADC prescaler
  #define ADC_PRESCALER(PRESCALER) { ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)); ADCSRA |= PRESCALER & 0x07; }
  
  // axis
  extern volatile uint8_t _selector;
  // counter
  extern volatile uint8_t _index;
  // array buffer
  extern volatile uint8_t _buffer[WIDTH];
 
  /**
   * @description Init settings of scope
   *
   * @param  Void
   * @return Void
   */
  void StartScope(void);

  /**
   * @description Init Timer0
   *
   * @param  Void
   * @return Void
   */
  void Timer0Init(void);

  /**
   * @descritpion Init Timer1A
   *
   * @param  Void
   * @return Void
   */
  void Timer1AInit(void);

  /**
   * @description Adc init
   *
   * @param  Void
   * @return Void
   */
  void AdcInit(void);

  /**
   * @description Interrupts init
   *
   * @param  Void
   * @return Void
   */
  void Int01Init(void);

  /**
   * @description Ports init
   *
   * @param  Void
   * @return Void
   */
  void PortsInit(void);
  
  /**
   * @description Axis show
   *
   * @param  Void
   * @return Void
   */
  void AxisShow(void);

  /**
   * @description Show values on lcd
   *
   * @param  Void
   * @return Void
   */
  void BufferShow(void);

  /**
   * @description Show menu
   *
   * @param  char*
   * @param  uint8_t
   * @return Void
   */
  void ShowItems(const volatile char**, uint8_t, uint8_t);

#endif

