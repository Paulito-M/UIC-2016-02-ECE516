 /* boilerplate platform headers */
#include "r_pdl_adc_12.h"
#include "r_pdl_cmt.h"
#include "r_pdl_io_port.h"
#include "r_pdl_definitions.h"

/*******************************************************************************
* Outline      : uint16_ToString
* Description  : Function converts a 16 bit integer into a character string, 
*         inserts it into the array via the pointer passed at execution.
* Argument     : * output_string : Pointer to uint8_t array that will hold 
*                   character string.
*               pos : uint8_t number, element number to begin 
*                   inserting the character string from (offset).
*          input_number : 16 bit integer to convert into a string.
* Return value : none
* Note       : No input validation is used, so output data can overflow the
*         array passed.
*******************************************************************************/
void uint16_To4Str_Hex(uint8_t * output_string, uint8_t pos, 
           uint16_t input_number)
{
  /* Declare temporary charcter storage variable, and bit_shift variable */  
  uint8_t a = 0x00, bit_shift = 12u;
  
  /* Declare 16bit mask variable */
  uint16_t mask = 0xF000;
  
  /* Loop through until each hex digit is converted to an ASCII character */
  while(bit_shift < 30u)
  {
    /* Mask and shift the hex digit, and store in temporary variable, a */ 
    a = (input_number & mask) >> bit_shift;
    
    /* Convert the hex digit into an ASCII character, and store in output
       string */
    output_string[pos] = (a < 0x0A) ? (a + 0x30) : (a + 0x37);
    
    /* Shift the bit mask 4 bits to the right, to convert the next digit */
    mask = mask >> 4u;
    
    /* Decrement the bit_shift counter by 4 (bits in a each digit) */
    bit_shift -= 4u;
    
    /* Increment the output string location */
    pos++;
  }
}
/*******************************************************************************
* End of function uint16_t_ToString
*******************************************************************************/

/*******************************************************************************
* Outline      : uint16_ToStringDec
* Description  : Function converts a 16 bit integer into a character string, 
*         inserts it into the array via the pointer passed at execution.
* Argument     : * output_string : Pointer to uint8_t array that will hold 
*                   character string.
*               pos : uint8_t number, element number to begin 
*                   inserting the character string from (offset).
*          input_number : 16 bit integer to convert into a string.
* Return value : none
* Note       : No input validation is used, so output data can overflow the
*         array passed.
*******************************************************************************/
void uint16_To4Str_Dec(uint8_t * output_string, uint8_t pos, 
           uint16_t input_number)
{  
  uint8_t digit = 3;
  uint8_t val;
  uint16_t decimal=1000;
  
  while ( decimal > 0 )
  {
    val = input_number / decimal;
    input_number = input_number % decimal;
    decimal = decimal / 10;
    output_string[pos++]=val+0x30;
    digit--;
  }
}