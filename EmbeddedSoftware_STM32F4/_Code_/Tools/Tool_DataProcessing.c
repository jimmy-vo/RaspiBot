#include "stdio.h"
#include "Tool_DataProcessing.h"

uint8_t hex_to_ascii(uint8_t d)
{
    uint8_t temp;
    temp = d&0x0F;
    if (temp>9) temp += 7;
    return temp + '0';
}

uint8_t ascii_to_hex(uint8_t c)
{
    uint8_t temp;
    temp = c&0x7F;
    if (temp>'9') temp +=9;
    return temp&0x0F;
}

uint8_t byte_to_str(uint8_t *str, uint8_t byte)
{
    str[0] = hex_to_ascii(byte>>4);
    str[1] = hex_to_ascii(byte);
    return 2;
}

uint8_t StrToByte(uint8_t *str)
{
    return (ascii_to_hex(str[0])<<4) | (ascii_to_hex(str[1]));
}


uint8_t Word_to_str(uint32_t i, uint8_t *str)
{
    str[0] = hex_to_ascii(i>>12);
    str[1] = hex_to_ascii(i>>8);
    str[2] = hex_to_ascii(i>>4);
    str[3] = hex_to_ascii(i);
    
    return 4;
}

uint16_t Str_to_Word(uint8_t *str)
{
    return 	(((uint16_t)ascii_to_hex(str[0]))<<12) 	|
						(((uint16_t)ascii_to_hex(str[1]))<<8) 	| 
						(((uint16_t)ascii_to_hex(str[2]))<<4) 	| 
						 ((uint16_t)ascii_to_hex(str[3]));
}


uint8_t Int_to_str(uint32_t i, uint8_t *str)
{
    str[0] = hex_to_ascii(i>>28);
    str[1] = hex_to_ascii(i>>24);
    str[2] = hex_to_ascii(i>>20);
    str[3] = hex_to_ascii(i>>16);
    str[4] = hex_to_ascii(i>>12);
    str[5] = hex_to_ascii(i>>8);
    str[6] = hex_to_ascii(i>>4);
    str[7] = hex_to_ascii(i);
    
    return 8;
}

uint32_t Str_to_Int(uint8_t *str)
{
    return (((uint32_t)Str_to_Word(str))<<16) | ((uint32_t)Str_to_Word(&str[4]));
}

void Array_Copy(uint8_t *src, uint8_t *des, uint8_t length)
{
	while(length)
    {
			length--;
			(des[length] = src[length]);
		}
}

uint16_t StrCmp(uint8_t *src, uint8_t *des, uint8_t length)
{
    while(length)
    {
        length--;
        if (src[length] != des[length])
        {
            return 0;
        }
    }
    return 1;
}
