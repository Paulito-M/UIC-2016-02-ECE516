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
*******************************************************************************/
/*******************************************************************************
* Copyright (C) 2010 Renesas Electronics Corporation. All rights reserved.    */ 
/*******************************************************************************
* File Name    : lcd.h
* Version      : 1.00
* Device       : R5F562N8
* Tool Chain   : RX Family C Compiler
* H/W Platform : RDK+RX62N
* Description  : Provides variable and function delarations for LCD.c file.
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY Version Description
*         : 01.09.2010 1.00    First Release
*******************************************************************************/

/* Multiple inclusion prevention macro */
#ifndef LCD_H
#define LCD_H

/*******************************************************************************
* Project Includes
*******************************************************************************/
/* Defines standard integer variable types used in this file */
#include <stdint.h>

/*******************************************************************************
* Macro Definitions
*******************************************************************************/
/* Number of lines on the LCD display */
#define MAXIMUM_LINES        2 

#define LCD_LINE1       0
#define LCD_LINE2       8
#define LCD_LINE3       16
#define LCD_LINE4       24
#define LCD_LINE5       32
#define LCD_LINE6       40
#define LCD_LINE7       48
#define LCD_LINE8       56

/******************************************************************************
* Global Function Prototypes
******************************************************************************/
/* LCD initialisation function declaration */
void InitialiseLCD(void);
/* Update display function declaration */
void DisplayLCD(uint8_t, const uint8_t *);
/* LCD write function delcaration */
void LCD_write(uint8_t, uint8_t);
/* Clear LCD function delcaration */
void ClearLCD(void);

/* End of multiple inclusion prevention macro */
#endif
