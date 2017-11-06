/** 
 * Oscilloscope
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       01.11.2017
 * @file        main.c
 * @tested      AVR Atmega16
 * @inspiration http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 * ----------------------------------------------------------------------------------
 */
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "lib/st7735.h"
#include "lib/oscope.h"
#include "lib/menu.h"

/**
 * @description INT0 interrupt
 *
 * @param  INT0_vect
 * @return Void
 */
ISR(INT0_vect) 
{
  // menu selector / 1st level
  uint8_t sel_1st_lev = (0x0f & _selector);
  // sub menu selector / 2nd level
  uint8_t sel_2nd_lev = (_selector >> 4);

  // clear screen if 1st attempt to menu
  if (sel_1st_lev == 0) {
    // clear screen
    ClearScreen(0x0000);
  }  
  // MENU 
  // ----------------------------------------------------------
  // pass through menu items
  if (sel_2nd_lev == 0) {
    // check if menu not exceed
    if (sel_1st_lev < MENU_ITEMS) {
      // increment menu item
      _selector++;
      // show menu
      ShowItems(_menu_items, MENU_ITEMS, _selector);
    // selector exceed max value
    } else {
      // null menu and submenu
      _selector = 0;
    }
  // SUBMENU - 1 
  // ----------------------------------------------------------
  // pass through submenu 1 items
  } else if (sel_1st_lev == 1) {
    // check if submenu 1 not exceed
    if (sel_2nd_lev < SUBMENU_1_ITEMS) {
      // submenu selector 
      // increment sub menu and move to the left 4 position
      _selector = (((sel_2nd_lev+1) << 4) | sel_1st_lev);
      // show submenu level 1
      ShowItems(_submenu_1_items, SUBMENU_1_ITEMS, sel_2nd_lev+1);
    // selector exceed max value
    } else {
      // null menu and submenu
      _selector = 0x01;
      // show menu
      ShowItems(_menu_items, MENU_ITEMS, _selector);   
    }
  // SUBMENU - 2 
  // ----------------------------------------------------------
  // pass through submenu 2 items
  } else if (sel_1st_lev == 2) {
    // check if submenu 1 not exceed
    if (sel_2nd_lev < SUBMENU_2_ITEMS) {
      // submenu selector 
      // increment sub menu and move to the left 4 position
      _selector = (((sel_2nd_lev+1) << 4) | sel_1st_lev);
      // show submenu level 1
      ShowItems(_submenu_2_items, SUBMENU_2_ITEMS, sel_2nd_lev+1);
    // selector exceed max value
    } else {
      // null menu and submenu
      _selector = 0x02;
      // show menu
      ShowItems(_menu_items, MENU_ITEMS, _selector);   
    }
  // SUBMENU - 3 
  // ----------------------------------------------------------
  // pass through submenu 3 items
  } else if (sel_1st_lev == 3) {
    // check if submenu 1 not exceed
    if (sel_2nd_lev < SUBMENU_3_ITEMS) {
      // submenu selector 
      // increment sub menu and move to the left 4 position
      _selector = (((sel_2nd_lev+1) << 4) | sel_1st_lev);
      // show submenu level 1
      ShowItems(_submenu_3_items, SUBMENU_3_ITEMS, sel_2nd_lev+1);
    // selector exceed max value
    } else {
      // null menu and submenu
      _selector = 0x03;
      // show menu
      ShowItems(_menu_items, MENU_ITEMS, _selector);   
    }
  // SUBMENU - 4 
  // ----------------------------------------------------------
  // pass through submenu 4 items
  } else if (sel_1st_lev == 4) {
    // check if submenu 1 not exceed
    if (sel_2nd_lev < SUBMENU_4_ITEMS) {
      // submenu selector 
      // increment sub menu and move to the left 4 position
      _selector = (((sel_2nd_lev+1) << 4) | sel_1st_lev);
      // show submenu level 1
      ShowItems(_submenu_4_items, SUBMENU_4_ITEMS, sel_2nd_lev+1);
    // selector exceed max value
    } else {
      // null menu and submenu
      _selector = 0x04;
      // show menu
      ShowItems(_menu_items, MENU_ITEMS, _selector);   
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
  // menu selector / 1st level
  uint8_t sel_1st_lev = (0x0f & _selector);
  // sub menu selector / 2nd level
  uint8_t sel_2nd_lev = (_selector >> 4);

  // show submenu items for corresponding menu items
  // - clear screen and load items given submenu
  if (sel_2nd_lev == 0) {
    // clear screen
    ClearScreen(0x0000);
    // -----------------------------------------------------------
    // SUBMENU 1
    // switch according to menu items
    if (sel_1st_lev == 1) {
      // submenu selector 
      // increment sub menu and move to the left 4 position
      _selector = (((sel_2nd_lev+1) << 4) | sel_1st_lev);
      // show submenu level 1
      ShowItems(_submenu_1_items, SUBMENU_1_ITEMS, sel_2nd_lev+1);
    // -----------------------------------------------------------
    // SUBMENU 2
    } else if (sel_1st_lev == 2) {
      // submenu selector 
      // increment sub menu and move to the left 4 position
      _selector = (((sel_2nd_lev+1) << 4) | sel_1st_lev);
      // show submenu level 1
      ShowItems(_submenu_2_items, SUBMENU_2_ITEMS, sel_2nd_lev+1);
    // -----------------------------------------------------------
    // SUBMENU 3
    } else if (sel_1st_lev == 3) {
      // submenu selector 
      // increment sub menu and move to the left 4 position
      _selector = (((sel_2nd_lev+1) << 4) | sel_1st_lev);
      // show submenu level 1
      ShowItems(_submenu_3_items, SUBMENU_3_ITEMS, sel_2nd_lev+1);
    // -----------------------------------------------------------
    // SUBMENU 4
    } else if (sel_1st_lev == 4) {
      // submenu selector 
      // increment sub menu and move to the left 4 position
      _selector = (((sel_2nd_lev+1) << 4) | sel_1st_lev);
      // show submenu level 1
      ShowItems(_submenu_4_items, SUBMENU_4_ITEMS, sel_2nd_lev+1);
    }
  // 1st submenu processing
  // -------------------------------------------------------------
  // this is case when operation should be performed for
  // example change frequency settings
  } else if (sel_1st_lev == 1) {
    // null flag
    _flag &= ~(0x03);
    // update _flag
    _flag |= (sel_2nd_lev-1);
    // set frequency
    SetFreq(sel_2nd_lev);
    // null menu and submenu
    _selector = 0x01;
    // show menu
    ShowItems(_menu_items, MENU_ITEMS, _selector);
  // 2nd submenu processing
  // -------------------------------------------------------------
  // this is case when operation should be performed for
  // change voltage settings
  } else if (sel_1st_lev == 2) {
    // null flag
    _flag &= ~(0x0C);
    // update _flag
    _flag |= ((sel_2nd_lev-1) << 2);
    // set frequency
    SetVolt();
    // null menu and submenu
    _selector = 0x02;
    // show menu
    ShowItems(_menu_items, MENU_ITEMS, _selector);
  // 3rd submenu processing
  // -------------------------------------------------------------
  // this is case when operation should be performed for
  // change value settings
  } else if (sel_1st_lev == 3) {
    // null flag
    _flag &= ~(0x30);
    // update _flag
    _flag |= ((sel_2nd_lev-1) << 4);
    // null menu and submenu
    _selector = 0x03;
    // show menu
    ShowItems(_menu_items, MENU_ITEMS, _selector);
  // 4th submenu processing
  // -------------------------------------------------------------
  // this is case when operation should be performed for
  // change axis settings
  } else if (sel_1st_lev == 4) {
    // null flag
    _flag &= ~(0xC0);
    // update _flag
    _flag |= ((sel_2nd_lev-1) << 6);
    // null menu and submenu
    _selector = 0x04;
    // show menu
    ShowItems(_menu_items, MENU_ITEMS, _selector);
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
  if (_index++ < WIDTH) {
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
