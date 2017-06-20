/** 
 * Auxillary functions
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       08.01.2016
 * @file        st7735.h
 * @tested      AVR Atmega16
 * @inspiration 
 * ------------------------------------------------
 */

/** @definition */
#ifndef __AUXILLARY_H__
#define __AUXILLARY_H__

  /** @const Position loading image x start */
  #define POSITION_XS  20
  /** @const Position loading image x end */
  #define POSITION_XE 140
  /** @const Position loading image y start */
  #define POSITION_YS  40
  /** @const Position loading image y end */
  #define POSITION_YE  55

  // sirka (pocet zaznamenanych dat)
  #define WIDTH 128
  // vyska
  #define HEIGHT 64
  // posuv x
  #define OFFSET_X 15
  // posuv y
  #define OFFSET_Y 60
  // krok na x ovej osi
  #define STEP_X 32
  // krok na y ovej osi
  #define STEP_Y 16
  // @var Mozne kombinacie preddelicky hodin/taktu
  #define PRESCALER_STOP   0
  #define PRESCALER_1      1
  #define PRESCALER_8      2
  #define PRESCALER_64     3
  #define PRESCALER_256    4
  #define PRESCALER_1024   5
  #define PRESCALER_T1_F   6
  #define PRESCALER_T1_R   7
  #define ADC_PRESCALER_16 4
  #define ADC_PRESCALER_32 5
  #define ADC_PRESCALER_64 6
  // Spustenie casovaca 0
  // - nulovanie bitov preddelicky
  // - nastavenie 
  #define TIMER0_START(PRESCALER) { TCCR0 &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); TCCR0 |= PRESCALER & 0x07; }
  // Spustenie casovaca 1A
  // - nulovanie bitov preddelicky
  // - nastavenie
  #define TIMER1A_START(PRESCALER) { TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); TCCR1B |= PRESCALER & 0x07; }
  // Vyber kanala
  // - nulovanie poslednych troch bitov
  // - nastavenie poslednych troch bitov podla kanala
  #define ADC_CHANNEL(CHANNEL) { ADMUX &= 0xF8; ADMUX |= CHANNEL & 0x07; }
  // Spustenie casovaca 1A
  // - nulovanie bitov preddelicky
  // - nastavenie
  #define ADC_PRESCALER(PRESCALER) { ADCSRA &= ~((1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0)); ADCSRA |= PRESCALER & 0x07; }

  // axis
  extern volatile uint8_t _axis;
  // pocitadlo
  extern volatile uint8_t _index;
  // pole hodnot buffra
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
   * @param  Void
   * @return Void
   */
  void ShowMenu(void);

#endif

