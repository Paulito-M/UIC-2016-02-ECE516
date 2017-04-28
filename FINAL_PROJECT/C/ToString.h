#ifndef __TOSTRING_H
#define __TOSTRING_H

/*
 * DESCRIPTION:
 *   Convert the value to a 4-digit hex or decimal ASCII string representation
 *   starting at 
 */
 
void uint16_To4Str_Hex(
  uint8_t * _buf,         /* buffer to populate             */
  uint8_t _bufpos,        /* position in buffer to populate */
  uint16_t _value);       /* value to convert               */
  
void uint16_To4Str_Dec(
  uint8_t * _buf,         /* buffer to populate             */
  uint8_t _bufpos,        /* position in buffer to populate */
  uint16_t _value);       /* value to convert               */


#endif /* __TOSTRING_H */