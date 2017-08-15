/** 
 * Auxillary functions
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       04.02.2017
 * @file        st7735.c
 * @tested      AVR Atmega16
 * @inspiration 
 *
 */
#ifndef F_CPU
  #define F_CPU 16000000
#endif

#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
#include <util/delay.h>
#include "st7735.h"
#include "oscope.h"

// number of sample
volatile uint8_t _index = 0;
// selector
volatile uint8_t _selector = 0;
// array buffer
volatile uint8_t _buffer[WIDTH];

// array buffer
//   40kHz ( 25us) -> OCR0 =  49; N =  8; (ADC PRESCALER 16)
//   10kHz (100us) -> OCR0 = 199; N =  8; (ADC PRESCALER 32)
//  2.5kHz (0.4ms) -> OCR0 =  99; N = 64; (ADC PRESCALER 32)
//    1kHz (  1ms) -> OCR0 = 249; N = 64; (ADC PRESCALER 32)
// OCR0 
// Timer0 prescaler
// Ad converter prescaler
volatile uint8_t _frequency[ITEMS_FREQUENCIES][3] = {
  { 49, PRESCALER_8,ADC_PRESCALER_16}, 
  {199, PRESCALER_8,ADC_PRESCALER_32}, 
  { 99,PRESCALER_64,ADC_PRESCALER_32}, 
  {249,PRESCALER_64,ADC_PRESCALER_32}
};

/**
 * @description Init settings
 *
 * @param  Void
 * @return Void
 */
void StartScope(void)
{
  // interrupt init
  Int01Init();
  // init adc
  AdcInit();
  // init timer 1A
  Timer1AInit();
  // init timer 0
  Timer0Init();
  // globa interrupts enabled
  sei();
  // loop
  while(1) {
    // show if only not menu selected
    if (_selector == 0) {
      // show after if buffer full
      if (_index > WIDTH) {
        // show buffer
        BufferShow();
        // zero index
        _index = 0;
      }
    }
  }
}

/***
 * @description Init Timer0
 *
 * @param  Void
 * @return Void
 */
void Timer0Init(void)
{
  // zero counter
  TCNT0  = 0;
  //    foc0 = 40kHz:
  // ---------------------------------------------
  //    OCR0 = [fclk/(N.focnX)] - 1
  //    fclk = 16 Mhz
  //       N = 8
  //    foc0 = 1/Toc0
  // Pozn. 
  // Pri požadovanej frekvencii foc0 je pocitadlo dvakrat spustane, 
  //  t. j. čas prevodu musí byť menší ako 1/foc0,
  // -------------------------------------------------------------
  // Príklad
  //  foc0 = 40kHz => Toc0 = 25us 
  //  Toc0 > 13,5 * Tadc, platí ak 25 us > 13,5*1us
  //  Tadc = 1/fadc = 1/1000 000 => ADC PRESCALER = 16
  //  v prípade ADC PRESCALER = 32 => Tadc = 2us, tým pádom 
  //  neplatí podmienka Toc0 > 13,5*2us, pretože 25us < 27us 
  // -------------------------------------------------------------
  //   foc0    Toc0           
  //   40kHz ( 25us) -> OCR0 =  49; N =  8; (ADC PRESCALER 16)
  //   20kHz ( 50us) -> OCR0 =  99; N =  8; (ADC PRESCALER 32)
  //   10kHz (100us) -> OCR0 = 199; N =  8; (ADC PRESCALER 32)
  //    5kHz (0.2ms) -> OCR0 =  49; N = 64; (ADC PRESCALER 32)
  //  2.5kHz (0.4ms) -> OCR0 =  99; N = 64; (ADC PRESCALER 32)
  //    1kHz (  1ms) -> OCR0 = 249; N = 64; (ADC PRESCALER 32)
  OCR0 = 49;
  // PIN PB3 - OC0 ako vystupny 
  // DDRB  |= (1 << PB3);
  // Waveform generation - toggle
  TCCR0 |= (1 << COM00);
  // Mod 2 - CTC -> TOP = OCRnX
  TCCR0 |= (1 << WGM01);
  // nulovanie priznaku zhody OCR0 a TCNT0
  // priznak sa nastavuje aj bez povolenia globalnych preruseni
  // a povoleni preruseni od zhody TCNT0 a OCR0
  TIFR  |= (1 << OCF0);
  // start timer 0
  TIMER0_START(PRESCALER_8);  
}

/***
 * Inicializacia casovaca Timer1A
 * nastavenie frekvencie snimaneho impulzu
 *
 * @param  Void
 * @return Void
 */
void Timer1AInit(void)
{
  // nuluje pocitadlo
  TCNT1  = 0;
  // foc1A = 1kHz:
  // ---------------------------------------------
  //   OCR1A = [fclk/(2.N.foc1A)] - 1
  //    fclk = 16 Mhz
  //       N = 1
  //   foc1A = 1/Toc1A
  OCR1A = 15999;
  // PIN PD5 - OC1A ako vystupny 
  DDRD  |= (1 << PD5);
  // Waveform generation - toggle
  TCCR1A |= (1 << COM1A0);
  // Mod CTC -> TOP = OCR1A
  TCCR1B |= (1 << WGM12);
  // start timer 1A
  TIMER1A_START(PRESCALER_1);  
}

/***
 * Analogovo digitalny prevodnik 
 * - vstup PC0 (ADC0)
 * - referencne napatie AVcc s externym kondenzatorom na AREF pine
 * - preddelicka 128 bity ADPS2:0 = 8 v ADCSRA, kvoli podmienke, ze 
 *   frekvencia prevodu ma byt v rozmedzi 50-200 kHz. Pri 16Mhz a preddelicke
 *   frekvancia prevodu je 125kHz
 *
 * @param  Void
 * @return Void
 */
void AdcInit(void)
{ 
  // reference voltage AVcc with external capacitor at AREF pin 
  ADMUX |= (1 << REFS0);
  // align to left -> ADCH
  ADMUX |= (1 << ADLAR);
  // setting adc
  ADCSRA |= (1 << ADIE)  | // adc interrupt enable
            (1 << ADEN)  | // adc enable
            (1 << ADATE);  // adc autotriggering enable
  //set prescaler
  // f = 16Mhz / 32 = 500 kHz  = 2 us
  // Tadc => 2us x 13.5 cykla = 27 us
  ADC_PRESCALER(ADC_PRESCALER_16); 
  // Timer/Counter0 Compare Match
  SFIOR |= (1 << ADTS1) | (1 << ADTS0);
  // select channel
  ADC_CHANNEL(1);
}

/***
 * @description Init Switch Interrupts INT0, INT1
 *
 * @param  Void
 * @return Void
 */
void Int01Init(void)
{
  // PD2 PD3 as input
  DDRD &= ~((1 << PD3) | (1 << PD2));
  // pull up activated
  PORTD |= (1 << PD3) | (1 << PD2);
  // INT0 - rising edge
  MCUCR |= (1 << ISC01) | (1 << ISC00);
  // INT1 - rising edge
  MCUCR |= (1 << ISC11) | (1 << ISC10);
  // enable interrupts INT0, INT1
  GICR |= (1 << INT1) | (1 << INT0);
}

/**
 * @description Axis show
 *
 * @param void
 * @return void
 */
void AxisShow()
{
  // init value for x
  uint8_t i = OFFSET_X;
  // color line
  uint16_t color = 0x5C4B;

  // draw axis x
  DrawLineHorizontal(OFFSET_X, OFFSET_X+WIDTH, OFFSET_Y+HEIGHT, color);
  //  draw axis x
  DrawLineHorizontal(OFFSET_X, OFFSET_X+WIDTH, OFFSET_Y, color);
  //  draw axis y
  DrawLineVertical(OFFSET_X, OFFSET_Y, OFFSET_Y+HEIGHT, color);
  //  draw axis y
  DrawLineVertical(OFFSET_X+WIDTH, OFFSET_Y, OFFSET_Y+HEIGHT, color);
  // draw auxillary axis x
  while (i <= WIDTH+OFFSET_X) {
    // draw auxillary signs up
    DrawLineVertical(i, OFFSET_Y, OFFSET_Y+HEIGHT, color);
    // move to right
    i += STEP_X;
  }
  // init value for y
  i = OFFSET_Y;
  // draw auxillary axis y
  while (i <= HEIGHT+OFFSET_Y) {
    // draw line
    DrawLineHorizontal(OFFSET_X, OFFSET_X+WIDTH, i, color);
    // move to right
    i += STEP_Y;
  }
}

/**
 * @description Show values on lcd
 *
 * @param void
 * @return void
 */
void BufferShow()
{
  // sreg value
  char sreg;
  // index
  uint8_t i = WIDTH;
  // save SREG values
  sreg = SREG;
  // disable interrupts
  cli();
  // clear screen
  ClearScreen(0x0000);
  // set text position
  SetPosition(0, OFFSET_Y+HEIGHT - 8);
  // vykreslenie osi
  AxisShow();
  // show buffer values
  while (i > 0) {
    // draw line
    //DrawLine(i-1+OFFSET_X, i+OFFSET_X, (_buffer[i-1]>>1), (_buffer[i]>>1), 0xffff);
    DrawLine(i-1+OFFSET_X, i+OFFSET_X, OFFSET_Y+(HEIGHT-(_buffer[i-1]>>1)), OFFSET_Y+(HEIGHT-(_buffer[i]>>1)), 0xffff);
    // decrement
    i--;
  }
  // show on screen
  UpdateScreen();
  // delay
  _delay_ms(1500);
  // set stored values
  SREG = sreg;
  // enable interrputs
  sei();
}

/**
 * @description Show items of menu and submenu
 *
 * @param  char*
 * @param  uint8_t
 * @param  uint8_t
 * @return Void
 */
void ShowItems(char **items, uint8_t count, uint8_t selector)
{
  uint8_t i = 0;
  // init position
  uint8_t height = 20;
  // init position
  uint8_t step_y = 16;
  // offset x 
  uint8_t offset_x = 10;
  // number of pixels
  uint16_t area = (SIZE_X+1)*(height+1);
 
  // set window
  SetWindow(0, SIZE_X, 5, 5 + height);
  // send color
  SendColor565(0x075f, area);
  // set text on position
  SetPosition(offset_x, 9);
  // draw string
  DrawString("MENU-SETTINGS ", 0x0000, X2);
  // increase value
  step_y += height;

  // check if reach the end
  while (i < count) {
    // fill background color
    if (i == (selector-1)) {
      // set window
      SetWindow(0, SIZE_X, step_y, step_y + height);
      // send color
      SendColor565(0xffff, area);
      // set text on position
      SetPosition(offset_x, step_y+3);
      // draw string
      DrawString(items[i], 0x0000, X2);
    } else {
      // set window
      SetWindow(0, SIZE_X, step_y, step_y + height);
      // send color
      SendColor565(0x0000, area);
      // set text on position
      SetPosition(offset_x, step_y+3);
      // draw string
      DrawString(items[i], 0xffff, X2);
    }
    // increase value
    step_y += height;
    // decrement
    i++;
  }    
  // update screen
  UpdateScreen();
}


