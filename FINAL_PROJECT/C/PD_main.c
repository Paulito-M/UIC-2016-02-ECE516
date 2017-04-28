/*******************************************************************************
 * PD_main.c
 *
 *
 * &HISTORY.
 *   2016-11-14 PNM - Created
 * &EHISTORY.
 *******************************************************************************
 */

/*
 * INCLUDE FILES
 */
#include<stdint.h>
#include "lcd.h"
#include "machine.h"
#include "intrinsics.h"
#include "hwsetup.h"

#include "PD.h"

/*
 * DESCRIPTION:
 *   Set up hardware, start up pitch detection, and loop forever.
 *
 */
void main(void)
{
  /* 
   * setup hardware 
   */
  HardwareSetup();

  /*
   * enable interrupts
   */
  __enable_interrupt();

  /* 
   * Initialize and start Pitch Detection functionality
   */
  PD_Init();

  /*
   * don't stop believin'
   */
  while ( 1 );
}

