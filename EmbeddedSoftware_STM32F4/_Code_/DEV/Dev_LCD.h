#include "BalaPi_BoardConf.h"
#include <stdio.h>

/* LCD memory map */
#define LCD_LINE0_ADDR 				0x00 		// Start of line 0 in the DD-Ram
#define LCD_LINE1_ADDR 				0x40 		// Start of line 1 in the DD-Ram
#define LCD_LINE2_ADDR 				0x14 		// Only for LCD 20x4: start of line 2 in the DD-Ram
#define LCD_LINE3_ADDR 				0x54 		// Only for LCD 20x4: start of line 3 in the DD-Ram

/* LCD Commands */
#define LCD_DD_RAM_PTR 				0x80 		// Address Display Data RAM pointer
#define LCD_CG_RAM_PTR 				0x40 		// Address Character Generator RAM pointer
#define LCD_CLEAR_DISPLAY 		0x01 		// Clear entire display and set Display Data Address to 0
#define LCD_RETRN_HOME 				0x02 		// sets DDRAM address 0 and returns display from being shifted to original position.
#define LCD_DISP_INIT 				0x28 		// function set is 4 bit data length and 2 lines
#define LCD_INC_MODE 					0x06 		// Entry mode is display Data RAM pointer incremented after write
#define LCD_DISP_ON						0x0C		// Sets entire display on, cursor on and blinking of cursor position character
#define LCD_DISP_OFF					0x08   	// Sets entire display off, cursor off
#define LCD_CURSOR_ON					0x04		// turn on cursor
#define LCD_CURSOR_OFF				0x00    // turn off cursor
#define LCD_CUR_MOV_LEFT			0x10		// Cursor move and shift to left
#define LCD_CUR_MOV_RIGHT			0x14		// Cursor move and shift to right
#define LCD_BUSY            	0x80      		// LCD is busy

typedef enum {
   	Input = 0,
   	Output
} GPIOConfig_Mode_TypeDef;

#define SET_LCD_RS_Line()				GPIO_SetBits(LCD_RS_GPIO_PORT, LCD_RS_GPIO_PIN)
#define CLR_LCD_RS_Line()				GPIO_ResetBits(LCD_RS_GPIO_PORT, LCD_RS_GPIO_PIN)
#define SET_LCD_RW_Line()				GPIO_SetBits(LCD_RW_GPIO_PORT, LCD_RW_GPIO_PIN)
#define CLR_LCD_RW_Line()				GPIO_ResetBits(LCD_RW_GPIO_PORT, LCD_RW_GPIO_PIN)
#define SET_LCD_EN_Line()				GPIO_SetBits(LCD_EN_GPIO_PORT, LCD_EN_GPIO_PIN)
#define CLR_LCD_EN_Line()				GPIO_ResetBits(LCD_EN_GPIO_PORT, LCD_EN_GPIO_PIN)





void Initialize_LCD(void);
void LCD_Home(void);
void LCD_Clear(void);
void LCD_GotoXY(u8 row, u8 col);
void LCD_Print_Data(char* data, u8 nBytes);

