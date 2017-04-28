/*******************************************************************************
 * PD.c
 *   Pitch Detection functionality
 *
 * &HISTORY.
 *   2016-11-14 PNM - Created
 * &EHISTORY.
 *******************************************************************************
 */

/*
 * INCLUDE FILES
 */
 
 /* boilerplate platform headers */
#include "r_pdl_adc_12.h"
#include "r_pdl_cmt.h"
#include "r_pdl_io_port.h"
#include "r_pdl_definitions.h"

#include "switch.h"
#include "lcd.h"

#include "math.h"

#include "ToString.h"
#include "dfe.h"
#include "anf.h"

/*
 * CONSTANTS
 */
#define PI  (3.1415927)
#define FREQ  (10000)
#define STEP_DIVISOR (10000.0)
typedef enum  
{
  MODE_FREQX,
  MODE_DFE,
  MODE_ANF
} FILTER_MODE;

/*
 * GLOBAL VARIABLES
 */
/* Global ADC result container variable */
uint16_t usADC_Result[8] ;

/* Global switch flag variable, indicates which switches have been pressed */
extern volatile uint8_t gSwitchFlag;


/* 
 * step size scaled by 10000 so we can deal with integers 
 */
int gStep = 100;
int gStepSize = 100;

int gMaxADCreading = 0;

FILTER_MODE gFilterMode = MODE_FREQX;


/* 
 * DESCRIPTION:
 *   Process switch release events:
 *     SW1 - switch between zero-crossing mode and DFE algorithm
 *     SW2, SW3 - if DFE, cycle the stepsize up/down through its available
 *     values
 */     
void i_SwitchReleaseCB()
{
   static uint8_t lcd_buffer[] = "mu=0.0000\0";
            
  /* SW1 released */
  if ( gSwitchFlag & SWITCHPRESS_1 )
  {
    if ( gFilterMode == MODE_ANF )
      gFilterMode = MODE_FREQX;
    else
      gFilterMode++;
    
    /* cycle through the modes */  
    if ( gFilterMode == MODE_FREQX )
    {  
      DisplayLCD( LCD_LINE5, "TYPE : freqX" );
      DisplayLCD( LCD_LINE6, "            " );
    }
    else
    if ( gFilterMode == MODE_DFE )
    {
      DisplayLCD( LCD_LINE5, "TYPE : DFE  " );        
      uint16_To4Str_Dec( lcd_buffer, 5, gStep );  
      DisplayLCD( LCD_LINE6, lcd_buffer );        
      gMaxADCreading = 0;
      dfe_reset();      
    }
    else  /* gFilterMode == MODE_ANF */
    {
      DisplayLCD( LCD_LINE5, "TYPE : ANF  " );        
      uint16_To4Str_Dec( lcd_buffer, 5, gStep );  
      DisplayLCD( LCD_LINE6, lcd_buffer );        
      gMaxADCreading = 0;
      anf_reset();      
    }
      
    gSwitchFlag &= ~SWITCHPRESS_1;

  }  

  /* 
   * SW2, SW3 cycle through the possible values of the step size:
   * .001, .002, ..., .010  
   * .020, .030, ..., .1
   */
   
  /* SW2 released */
  if ( gSwitchFlag & SWITCHPRESS_2 )      
  {    
    if ( gFilterMode != MODE_FREQX )
    {
      /* STEP UP */  
      if ( gStep == 90 )
      {
        gStep = gStepSize = 100;
      }
      else
      if ( gStep == 1000 )
      {
        gStep = gStepSize = 10;
      }
      else
        gStep += gStepSize;
        
      uint16_To4Str_Dec( lcd_buffer, 5, gStep );  
      DisplayLCD( LCD_LINE6, lcd_buffer );    
      
      if ( gFilterMode == MODE_DFE )
        dfe_update( (float)gStep / STEP_DIVISOR );                   
      else
        anf_update( (float)gStep / STEP_DIVISOR );                    
    }
    gSwitchFlag &= ~SWITCHPRESS_2; 
  }
  
  /* SW3 released */
  if ( gSwitchFlag & SWITCHPRESS_3 )
  {
    if ( gFilterMode != MODE_FREQX )
    {
      /* STEP DOWN */  
      if ( gStep == 10 )
      {
        gStep = 1000;
        gStepSize = 100;
      }
      else
      if ( gStep == 100 )
      {
        gStep = 90;
        gStepSize = 10;
      }
      else
        gStep -= gStepSize;
                
      uint16_To4Str_Dec( lcd_buffer, 5, gStep );  
      DisplayLCD( LCD_LINE6, lcd_buffer );          
      
      if ( gFilterMode == MODE_DFE )
        dfe_update( (float)gStep / STEP_DIVISOR );                   
      else
        anf_update( (float)gStep / STEP_DIVISOR );                    
    }
    gSwitchFlag &= ~SWITCHPRESS_3;  
  }
}


/*******************************************************************************
* Local Function Prototypes
*******************************************************************************/
/* CMT interrupt callback function prototype declaration */
void CB_CMTADC(void);
/* 16 bit integer to character string function prototype declaration */

/*
 * DESCRIPTION:
 *   Initialize and start pitch detection functionality
 */
void PD_Init(void)
{
  /* 
   * Initialize LCD and display info
   */
  InitialiseLCD();
  DisplayLCD(LCD_LINE1, "PITCH DETECT");
  DisplayLCD(LCD_LINE2, "version 2.0");
  
  DisplayLCD(LCD_LINE4, "INPUT: mic");
  DisplayLCD(LCD_LINE5, "TYPE : freqX" );
 
  /*
   * Configure the ADC unit
   * The microphone is on channel 5
   */
  R_ADC_12_Create(
    0,
    PDL_ADC_12_SCAN_CONTINUOUS|PDL_ADC_12_CHANNEL_5 | PDL_ADC_12_DATA_ALIGNMENT_RIGHT | PDL_ADC_12_DIV_1, 
    PDL_ADC_12_TRIGGER_SOFTWARE,
    PDL_NO_DATA,
    PDL_NO_FUNC,
    0 );
  
  /* 
   * Configure CMT timer to execute callback function at FREQ rate 
   */
  R_CMT_Create(
    2,
    PDL_CMT_PERIOD,
    (float)( 1.0 / FREQ ), 
    CB_CMTADC,
    3
  );
  
  /* 
   * Start the ADC continuously performing conversions
   */
  R_ADC_12_Control( PDL_ADC_12_0_ON );
  
  /* 
   * Enable microphone pre-amplifier 
   * AMP_SHDN is pin 5 port 5 
   */
  R_IO_PORT_Modify( PDL_IO_PORT_5, PDL_IO_PORT_OR, 0x20 ); 

  /*
   * Initialize both DFE and ANF filters
   */
  
  dfe_init( (float)gStep / STEP_DIVISOR, ( 440.0 / FREQ * ( 2.0 * PI ) ) );
  anf_init( (float)gStep / STEP_DIVISOR, ( 440.0 / FREQ * ( 2.0 * PI ) ) );

  SetSwitchReleaseCallback( i_SwitchReleaseCB );
  
}



/*******************************************************************************
* End of function Init_ADCRepeat
*******************************************************************************/

/*******************************************************************************
* Outline      : CB_CMTADC
* Description  : CMT interrupt callback function. This callback function is
*         executed after every period of the CMT timer. The function 
*         fetches the ADC result, and stores it in the global variable
*         usADC_Result. The function then converts the ADC result into
*         a character string, and displays it on the debug LCD.
* Argument     : none
* Return value : none
*******************************************************************************/
static int gCount = 0;
static int gCurCount = 0;
static int gLastVal = 0;
void CB_CMTADC(void)
{
  /* Declare error flag */
  bool err = true;
  static int freq = 0;
  volatile float f;
  uint16_t ADCreading;
  
  /* Poll ADC unit until conversion is complete and store result in the
     global variable usADC_Result */  
  err &=  R_ADC_12_Read
      (
        0,
        usADC_Result
      );

  
  gCurCount += 1;
  
  if ( gFilterMode == MODE_FREQX )
  {
    /* estimate freq based on zero crossings */
    if ( ( gLastVal > 0x07FF ) && ( usADC_Result[5] < 0x07FF ) )
    {
      freq = FREQ / gCurCount;
      gCurCount = 0;
    }  
    gLastVal = usADC_Result[5];
  }
  else
  {
    /* DFE algorithm */
    /* remove DC bias and normalize */ 
    ADCreading = usADC_Result[5];
    if ( ADCreading > gMaxADCreading )
      gMaxADCreading = ADCreading;
  
    float sample = (float)usADC_Result[5] - 0x07FF;
    sample = sample / gMaxADCreading;
    
    if ( gFilterMode == MODE_DFE )
      f = dfe_step( sample );
    else /* MODE_ANF */
      f = anf_step( sample );
  }


static int slowDisplay = 0;
slowDisplay++;

  if  ( slowDisplay % 2500 == 0 ) 
  {
  if ( gFilterMode != MODE_FREQX )
    freq = f / ( 2.0 * PI ) * FREQ;
  
  uint8_t freq_buffer[]="F=     Hz\0";
  uint16_To4Str_Dec(freq_buffer,2,freq);  
  DisplayLCD(LCD_LINE7, freq_buffer); 
  
    /* Declare temporary character string */
  uint8_t lcd_buffer1[] = "ADC=0xWXYZ\0";
  
  uint16_To4Str_Hex(lcd_buffer1,6, usADC_Result[5]);  
  DisplayLCD(LCD_LINE8, lcd_buffer1);     

  }

  /*ToggleLED(); */
  /* Halt in while loop when RPDL errors detected */  
  while(!err);
}

