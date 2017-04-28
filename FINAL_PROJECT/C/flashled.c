/* Adapted for use with IAR Embedded Workbench */
/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : flashLED.c 
* Version      : 1.00
* Device       : R5F562N8
* Tool Chain   : IAR Embedded Workbench
* H/W Platform : RSK+RX62N
* Description  : Defines LED flashing functions used in this tutorial.
*******************************************************************************/
/*******************************************************************************
* History     : 23.08.2010 Ver. 1.00 First Release
*******************************************************************************/

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* CMT RPDL function defintions */
#include "r_pdl_cmt.h"
/* IO Port RPDL function definitions */
#include "r_pdl_io_port.h"
/* General RPDL function definitions */
#include "r_pdl_definitions.h"
/* Defines switch functions and global variables used in this file */
#include "switch.h"
/* Provides declarations of functions defined in this file */
#include "flashLED.h"

/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
void CB_CMTFlash(void);

/*******************************************************************************
* Global Variables
*******************************************************************************/
/* Flash count variable - the LEDs are toggled as many times as the value this
   variable is initialised to. */
uint16_t gFlashCount = 400u;
/* The switch press flag indicates when switches have been pressed, or are being
   held down (see switch.h for more details) */
extern volatile uint8_t gSwitchFlag;

/*******************************************************************************
* Outline      : FlashLED
* Description  : Function configures a CMT timer to periodically call the call-
*         back function CB_CMTFlash which toggles the LEDs. The function
*         waits in a while loop untill the user LEDs have been toggled
*         400 times and the gFlashCount variable reaches zero. If switch
*         SW1 is pressed, the while loop ends early.
* Argument     : none
* Return value : none
*******************************************************************************/
void FlashLED(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Configure periodic CMT timer to call CB_CMTFlash every 100ms */
  err &=  R_CMT_Create
      (
        3,
        PDL_CMT_PERIOD,
        100E-3,
        CB_CMTFlash,
        2
      );
  
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
  
  /* Wait in a while loop until the variable gFlashCount is zero */
  while(gFlashCount)
  {
    /* Check if switch SW1 has been pressed */
    if(gSwitchFlag & SWITCHPRESS_ALL)
    {
      /* Reset the switch flag and the flash count variable to zero */
      gSwitchFlag = gFlashCount = 0x0;
    }
  }
  
  /* Destroy the timer unit, to allow the CMT channel to be used by
     another function */
  err &=  R_CMT_Destroy
      (
        1
      );
}
/*******************************************************************************
* End of function FlashLED
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_CMTFlash
* Description  : This callback function is called ever 100ms by a CMT interrupt.
*         The function calls the ToggleLED function which toggles the 
*         user LEDs. The function decrements the count variable 
*         gFlashCount each time it is executed.
* Argument     : none
* Return value : none
*******************************************************************************/
void CB_CMTFlash(void)
{
  /* Decrement the flash count variable */
  gFlashCount--;
  
  /* Execute the toggle LEDs function */
  ToggleLED();
}
/*******************************************************************************
* End of function CB_CMTFlash
*******************************************************************************/

/*******************************************************************************
* Outline      : ToggleLED
* Description  : This function performs a logical XOR on the output status of
*         pins connected to the user LEDs, toggling their value every
*         time the function is executed.
* Argument     : none
* Return value : none
*******************************************************************************/
void ToggleLED(void)
{
  /* Declare error flag */
  bool err = true;
  
  /* Toggle the user LEDs by performing a logical XOR with 1 */
  err &=  R_IO_PORT_Modify
      (
        PDL_IO_PORT_D,
        PDL_IO_PORT_XOR,
        /*0xff*/ 0x80
      );
      
  /* Toggle the user LEDs by performing a logical XOR with 1 */    
  err &=  R_IO_PORT_Modify
      (
        PDL_IO_PORT_E,
        PDL_IO_PORT_XOR,
        /*0xff*/0x00
      );    

  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}
/*******************************************************************************
* End of function ToggleLED
*******************************************************************************/
