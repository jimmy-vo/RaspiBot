
#include "Dev_LCD.h"
#include <stdio.h>


//////////////////////////////////////////////////////////////////
void lcd_data_line_write(u8 data);
u8 lcd_data_line_read(void);
void lcd_data_line_conf(GPIOConfig_Mode_TypeDef IO_mode);
void lcd_Init_HW(void);
void lcd_Busy_Wait(void);
void lcd_Control_Write(u8 data);
u8 lcd_Control_Read(void);
void lcd_Data_Write(u8 data);
u8 lcd_Data_Read(void);
static void delay(u32 nCount);
//////////////////////////////////////////////////////////////////



static void delay(u32 nCount)
{
	u32 index = 0;
	for(index = (168000 * nCount); index != 0; index--);
}

void lcd_data_line_write(u8 data)
{
	// Write only the highest 4 bits!
	SET_LCD_EN_Line() ; // Disable LCD
	//GPIO_Write(LCD_DATA_GPIO_PORT, ( GPIO_ReadOutputData(LCD_DATA_GPIO_PORT) & 0xFFF0 ) |(u16) ((data >> 4)&0x000F));
	
	GPIO_WriteBit(LCD_DATA_GPIO_PORT, LCD_D7, (BitAction)((data>>7) & 0x01));
	GPIO_WriteBit(LCD_DATA_GPIO_PORT, LCD_D6, (BitAction)((data>>6) & 0x01));
	GPIO_WriteBit(LCD_DATA_GPIO_PORT, LCD_D5, (BitAction)((data>>5) & 0x01));
	GPIO_WriteBit(LCD_DATA_GPIO_PORT, LCD_D4, (BitAction)((data>>4) & 0x01));
	
	CLR_LCD_EN_Line() ; // Enable LCD, Create a falling edge
	
	delay(5);

	SET_LCD_EN_Line() ; // Disable LCD
	//GPIO_Write(LCD_DATA_GPIO_PORT, (GPIO_ReadInputData(LCD_DATA_GPIO_PORT)&0xFFF0) |(u16)(data&0x000F));
	
	GPIO_WriteBit(LCD_DATA_GPIO_PORT, LCD_D7, (BitAction)((data>>3) & 0x01));
	GPIO_WriteBit(LCD_DATA_GPIO_PORT, LCD_D6, (BitAction)((data>>2) & 0x01));
	GPIO_WriteBit(LCD_DATA_GPIO_PORT, LCD_D5, (BitAction)((data>>1) & 0x01));
	GPIO_WriteBit(LCD_DATA_GPIO_PORT, LCD_D4, (BitAction)((data) & 0x01));
	
	CLR_LCD_EN_Line() ; // Enable LCD, create a falling edge

	delay(1);
}

u8 lcd_data_line_read(void)
{
	u8 HNib, LNib;															
	
	/* set data I/O lines to input */	
	lcd_data_line_conf(Input);
	/* set RS to "control" */
	CLR_LCD_RS_Line();
	/* set R/W to "read" */
	SET_LCD_RW_Line();

	delay(1);
  														
	/* Read only the lowest 8 bits!*/	
	SET_LCD_EN_Line(); 
	//HNib = (u8)(GPIO_ReadInputData(LCD_DATA_GPIO_PORT) & LCD_DATA_GPIO_PINS);
	
	
	HNib &= (GPIO_ReadInputDataBit(LCD_DATA_GPIO_PORT, LCD_D7)<<7);
	HNib &= (GPIO_ReadInputDataBit(LCD_DATA_GPIO_PORT, LCD_D6)<<6);
	HNib &= (GPIO_ReadInputDataBit(LCD_DATA_GPIO_PORT, LCD_D5)<<5);
	HNib &= (GPIO_ReadInputDataBit(LCD_DATA_GPIO_PORT, LCD_D4)<<4);
	
	CLR_LCD_EN_Line() ; // create a falling edge

	delay(1);

	SET_LCD_EN_Line(); 
	//LNib = (u8)(GPIO_ReadInputData(LCD_DATA_GPIO_PORT) & LCD_DATA_GPIO_PINS);
	
	LNib &= (GPIO_ReadInputDataBit(LCD_DATA_GPIO_PORT, LCD_D7)<<3);
	LNib &= (GPIO_ReadInputDataBit(LCD_DATA_GPIO_PORT, LCD_D6)<<2);
	LNib &= (GPIO_ReadInputDataBit(LCD_DATA_GPIO_PORT, LCD_D5)<<1);
	LNib &= (GPIO_ReadInputDataBit(LCD_DATA_GPIO_PORT, LCD_D4));
	
	CLR_LCD_EN_Line() ; // create a falling edge
																			
	return (HNib |(LNib >> 4));	
}

void lcd_data_line_conf(GPIOConfig_Mode_TypeDef IO_mode)
{
	GPIO_InitTypeDef  GPIO_InitStructure;										  	
																			
	/* Enable the LCD Data line Clock */											
  	RCC_APB1PeriphClockCmd(LCD_DATA_GPIO_CLK, ENABLE);					
																			
	/* Configure the LCD Data lines */											
  	GPIO_InitStructure.GPIO_Pin = LCD_DATA_GPIO_PINS;  	 					
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;						
	if( IO_mode == Input )
	{							
       	/* Configure LCD D0~D7 lines as Input */									
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  }																		
  else if ( IO_mode == Output)						
	{																		
            /* Configure LCD D0~D7 lines in Output Push-Pull mode */					
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  }																		
																			
  GPIO_Init(LCD_DATA_GPIO_PORT, &GPIO_InitStructure);	 					
}


void lcd_Init_HW(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	/* Enable GPIO clocks for LCD control pins */
	RCC_AHB1PeriphClockCmd(LCD_RS_GPIO_CLK | LCD_RW_GPIO_CLK | LCD_EN_GPIO_CLK, ENABLE);

	/* initialize LCD control lines to output */
	GPIO_InitStructure.GPIO_Pin 	= LCD_RS_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_RS_GPIO_PORT, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin 	= LCD_RW_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LCD_RW_GPIO_PORT, &GPIO_InitStructure); 

	GPIO_InitStructure.GPIO_Pin 	= LCD_EN_GPIO_PIN;
	GPIO_InitStructure.GPIO_Mode 	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_Init(LCD_EN_GPIO_PORT, &GPIO_InitStructure); 

	/* Enable GPIO clocks for LCD data pins */
	RCC_AHB1PeriphClockCmd(LCD_DATA_GPIO_CLK, ENABLE);

	/* initialize LCD data port to output */
	GPIO_InitStructure.GPIO_Pin 	= LCD_DATA_GPIO_PINS;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;			
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_50MHz;
	GPIO_Init(LCD_DATA_GPIO_PORT, &GPIO_InitStructure); 
	
}

void lcd_Busy_Wait(void)
{
	 u8 status;

  	do  {
    	status = lcd_data_line_read();
  	}  while (status & 0x80);             /* Wait for busy flag */
}

// LCD_DD_RAM_PTR		: Address Display Data RAM pointer
// LCD_CLEAR_DISPLAY		: Clear entire display and set Display Data Address to 0
// LCD_RETRN_HOME		: sets DDRAM address 0 and returns display from being shifted to original position.
// LCD_DISP_INIT			: 8 bit data length and 2 lines
// LCD_INC_MODE			: Entry mode is display Data RAM pointer incremented after write
// LCD_DISP_ON			: Sets entire display on, cursor on and blinking of cursor position character
// LCD_DISP_OFF			: Sets entire display off, cursor off
// LCD_CURSOR_ON		: turn on cursor
// LCD_CURSOR_OFF		: turn off cursor
// LCD_CUR_MOV_LEFT		: Cursor move and shift to left
// LCD_CUR_MOV_RIGHT	: Cursor move and shift to right

void lcd_Control_Write(u8 data)
{
	/* write the control byte to the display controller */
	/* wait until LCD not busy */
	lcd_Busy_Wait();		
	/* set RS to "control" */
	CLR_LCD_RS_Line();
	/* set R/W to "write" */
	CLR_LCD_RW_Line();
	
	/* set data I/O lines to output (8bit) */
	lcd_data_line_conf(Output);
	
	/* output data, 8bits */
	lcd_data_line_write(data);
		
	/* leave data lines in input mode to save power consumption */
	/* set data I/O lines to input (8bit) */	
	lcd_data_line_conf(Input);
}


u8 lcd_Control_Read(void)
{
	/* read the control byte from the display controller */
	register u8 data;

	/* wait until LCD not busy */
	lcd_Busy_Wait();	

	/* set data I/O lines to input (8bit) */
	lcd_data_line_conf(Input);
	
	/* set RS to "control" */
	CLR_LCD_RS_Line();

	/* set R/W to "read" */
	SET_LCD_RW_Line();

	/*  input data, 8bits */
	data = lcd_data_line_read();
	
	
	/* leave data lines in input mode to save power consumption */
	/* set data I/O lines to input (8bit) */
	lcd_data_line_conf(Input);

	return data;
}

void lcd_Data_Write(u8 data)
{
	/* write a data byte to the display */
	/* wait until LCD not busy */
	lcd_Busy_Wait();		
	/* set RS to "data" */
	SET_LCD_RS_Line();
	/* set R/W to "write" */
	CLR_LCD_RW_Line();
	
	/* set data I/O lines to output (8bit) */
	lcd_data_line_conf(Output);
	/* output data, 8bits */
	lcd_data_line_write(data);
	
	/* leave data lines in input mode to save power consumption */
	/* set data I/O lines to input (8bit) */	
	lcd_data_line_conf(Input);
}

u8 lcd_Data_Read(void)
{
	/* read a data byte from the display */
	register u8 data;

	/* wait until LCD not busy */
	lcd_Busy_Wait();	

	/* set data I/O lines to input (8bit) */
	lcd_data_line_conf(Input);
	
	/* set RS to "data" */
	SET_LCD_RS_Line();

	/* set R/W to "read" */
	SET_LCD_RW_Line();
		
	/*  input data, 8bits */
	data = lcd_data_line_read();

	/* leave data lines in input mode to save power consumption */
	/* set data I/O lines to input (8bit) */			
	lcd_data_line_conf(Input);

	return data;
}

void Initialize_LCD(void)
{
	/* initialize hardware */
	lcd_Init_HW();

	/* Set 4-bits interface */
	lcd_Control_Write(0x33);		 
	delay(10);
	lcd_Control_Write(0x32);

	delay(10);

	/* Start to set LCD function */
	lcd_Control_Write(LCD_DISP_INIT);
	
	/* clear LCD */
	lcd_Control_Write(LCD_CLEAR_DISPLAY);
	/* wait 60ms */
	
	/* set entry mode */
	lcd_Control_Write(LCD_INC_MODE);
	
	/* set display to on */	
	lcd_Control_Write(LCD_DISP_ON);	
	
	/* move cursor to home and set data address to 0 */
	lcd_Control_Write(LCD_RETRN_HOME);
	
		
	LCD_Clear();
}



void LCD_Home(void)
{
	// move cursor to home
	lcd_Control_Write(LCD_RETRN_HOME);
	delay(5);
}



void LCD_Clear(void)
{	
	lcd_Control_Write(LCD_CLEAR_DISPLAY);
	delay(5);
	LCD_GotoXY(0,0);
	delay(5);
}



void LCD_GotoXY(u8 x, u8 y)
{
	register u8 DDRAMAddr;

	// remap lines into proper order
	switch(x)
	{
	case 0: DDRAMAddr = LCD_LINE0_ADDR+y; break;
	case 1: DDRAMAddr = LCD_LINE1_ADDR+y; break;
	case 2: DDRAMAddr = LCD_LINE2_ADDR+y; break;	// for LCD 16x4 or 20x4 only
	case 3: DDRAMAddr = LCD_LINE3_ADDR+y; break;
	default: DDRAMAddr = LCD_LINE0_ADDR+y;
	}

	// set data address
	lcd_Control_Write(LCD_DD_RAM_PTR | DDRAMAddr);
	delay(5);
}



void LCD_Print_Data(char* data, u8 nBytes)
{
	register u8 i;

	/* check to make sure we have a good pointer */
	if (!data) return;

	/* print data */
	for(i=0; i<nBytes; i++)
	{
		lcd_Data_Write(data[i]);
		delay(5);
	}
}


#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
	/* Place your implementation of fputc here */
  	/* e.g. write a character to the LCD */
	lcd_Data_Write((u8)ch);

  	return ch;
}

