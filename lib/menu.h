/** 
 * Display manu operations
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       01.11.2017
 * @file        menu.h
 * @tested      AVR Atmega16
 * @inspiration 
 * ------------------------------------------------
 */

/** @definition */
#ifndef __MENU_H__
#define __MENU_H__

  // menu items
  #define MENU_ITEMS 4
  // submenu items
  #define SUBMENU_1_ITEMS 4
  // submenu items
  #define SUBMENU_2_ITEMS 4
  // submenu items
  #define SUBMENU_3_ITEMS 2
  // submenu items
  #define SUBMENU_4_ITEMS 4

  // flag show which item is selected
  extern volatile uint8_t _flag;

  // TEXT for display
  // --------------------------------------------------------
  // menu text
  extern const volatile char *_menu_items[MENU_ITEMS];
  
  // submenu text - frequencies
  extern const volatile char *_submenu_1_items[SUBMENU_1_ITEMS];
  // submenu text - voltages
  extern const volatile char *_submenu_2_items[SUBMENU_2_ITEMS];
  // submenu text - values
  extern const volatile char *_submenu_3_items[SUBMENU_3_ITEMS];
  // submenu text - axis
  extern const volatile char *_submenu_4_items[SUBMENU_4_ITEMS];

  // SETTINGS for microprocesor
  // --------------------------------------------------------  
  // array of process values
  extern const volatile uint8_t _submenu_1_proc[SUBMENU_1_ITEMS][3];
 
  /**
   * @description Frequency settings
   *
   * @param  uint8_t
   * @return Void
   */
  void SetFreq(uint8_t);

  /**
   * @description 
   *
   * @param  Void
   * @return Void
   */
  void ShowValues(void);

  /**
   * @description 
   *
   * @param  Void
   * @return Void
   */
  void ShowAxis(void);

#endif

