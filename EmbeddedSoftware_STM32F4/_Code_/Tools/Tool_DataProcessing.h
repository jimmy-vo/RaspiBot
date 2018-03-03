#include "stm32f4xx_conf.h"


typedef struct 
{
    int16_t x;
    int16_t y;
    int16_t z;
}VECTOR_3D_INT;

typedef struct 
{
    float x;
    float y;
    float z;
}VECTOR_3D_FLOAT;





uint8_t hex_to_ascii(uint8_t d);
uint8_t ascii_to_hex(uint8_t c);
uint8_t byte_to_str(uint8_t *str, uint8_t byte);
uint8_t Word_to_str(uint32_t i, uint8_t *str);
uint16_t Str_to_Word(uint8_t *str);
uint8_t Int_to_str(uint32_t i, uint8_t *str);
uint32_t Str_to_Int(uint8_t *str);
void Array_Copy(uint8_t *src, uint8_t *des, uint8_t length);
uint16_t StrCmp(uint8_t *src, uint8_t *des, uint8_t length);
