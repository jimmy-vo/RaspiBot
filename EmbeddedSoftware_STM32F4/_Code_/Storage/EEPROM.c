#include "EEPROM.h"

#include "Timing_Manage.h"
#include "Tool_DataProcessing.h"
#include "Dev_I2C.h"

#include "Dev_DIGITAL.h"
#include "Dev_LCD.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 																																																												 ////
//// 																								I2C	Driver																															 ////
//// 																																																												 ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
EEPROM_Status EEPROM_Write (uint16_t Register, uint8_t *buffer, uint8_t length)
	{	
		I2C_Status State;
		uint8_t reg[2];	
		reg[0] 		= (uint8_t) ((Register&0x7F00)>>8);
		reg[1] 		= (uint8_t)	(Register&0xFF);
		
		State = I2C_BUSY_Check (&EEPROM);
		if (State!=I2C_STATUS_OK) return EEPROM_Status_FAIL;
		
		State	= I2C_Request(&EEPROM, I2C_Phase_TX, LC24256_WRITE_ADDRESS, LC24256_WRITE_ADDRESS, reg, 2, buffer, length);	
		if (State!=I2C_STATUS_OK) return EEPROM_Status_FAIL;
		
		return EEPROM_Status_OK;
	}

EEPROM_Status EEPROM_Read (uint16_t Register, uint8_t *buffer, uint32_t length)
	{	
		I2C_Status State;
		uint8_t reg[2];	
		reg[0] 		= (uint8_t) ((Register&0x7F00)>>8);
		reg[1] 		= (uint8_t)	(Register&0xFF);	
		
		State = I2C_BUSY_Check (&EEPROM);
		if (State!=I2C_STATUS_OK) return EEPROM_Status_FAIL;
			
		State = I2C_Request(&EEPROM, I2C_Phase_RX, LC24256_WRITE_ADDRESS, LC24256_READ_ADDRESS, reg, 2, buffer, length);	
		if (State!=I2C_STATUS_OK) return EEPROM_Status_FAIL;
				
		return EEPROM_Status_OK;
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 																																																												 ////
//// 																								USERCODE																																 ////
//// 																																																												 ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// 24lC256 has 32000 x byte =32kbit * 8 = 256 kbit
// 64 byte/page
// ==>500 pages
EEPROM_Status Initialize_EEPROM (void)
{		
	  GPIO_InitTypeDef  GPIO_InitStructure;
	
    /* GPIO Periph clock enable */
    RCC_AHB1PeriphClockCmd(I2C2_GPIO_CLK, ENABLE);
	
    /* Configure I2C2 pins: SCL and SDA */
    GPIO_InitStructure.GPIO_Pin 		=  	I2C2_SCL_PIN | I2C2_SDA_PIN;
		GPIO_InitStructure.GPIO_Mode 		= 	GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd  	= 	GPIO_PuPd_NOPULL;
	
    GPIO_Init(I2C2_GPIO_PORT, &GPIO_InitStructure);
	
	  /* Connect GPIO pins to peripheral */
		GPIO_PinAFConfig(I2C2_GPIO_PORT, I2C2_SCL_SOURCE, I2C2_AF); 
		GPIO_PinAFConfig(I2C2_GPIO_PORT, I2C2_SDA_SOURCE, I2C2_AF);
	
		EEPROM.TimeOut 									= 100;		//100ms
		EEPROM.I2C											= I2C2;		
	
 		EEPROM.TX.DMA										= I2C_DMA_Enable;
		EEPROM.TX.DMA_Channel						=	I2C2_TX_DMA_CH;
		EEPROM.TX.DMA_Stream						=	I2C2_TX_DMA_ST;
		EEPROM.TX.DMA_Flag							=	I2C2_TX_DMA_FLAG;
		EEPROM.TX.DMA_NVIC_IRQ					=	I2C2_TX_DMA_IRQn;
			
 		EEPROM.RX.DMA										= I2C_DMA_Enable;
		EEPROM.RX.DMA_Channel						=	I2C2_RX_DMA_CH;
		EEPROM.RX.DMA_Stream						=	I2C2_RX_DMA_ST;
		EEPROM.RX.DMA_Flag							=	I2C2_RX_DMA_FLAG;
		EEPROM.RX.DMA_NVIC_IRQ					=	I2C2_RX_DMA_IRQn;
		
		Initialize_I2C(&EEPROM,400000);			//300kHz		
		
		return EEPROM_Status_OK;
}



EEPROM_Status EEPROM_WaitStandby(void)      
{
  __IO uint16_t tmpSR1 = 0;
  __IO uint32_t Standby_trial = 0;
	uint32_t zero_time;
	I2C_Status State;

	State = I2C_BUSY_Check(&EEPROM);
	if (State!=I2C_STATUS_OK) return EEPROM_Status_FAIL;
	
  while (Standby_trial < MAXStandby_trial)
		{
			I2C_GenerateSTART(EEPROM.I2C, ENABLE);
			
			State = I2C_EVENT_Check(&EEPROM, I2C_EVENT_MASTER_MODE_SELECT);		
			if (State!=I2C_STATUS_OK) return EEPROM_Status_FAIL;
			
			I2C_Send7bitAddress(EEPROM.I2C, LC24256_WRITE_ADDRESS, I2C_Direction_Transmitter);
			
			/* Wait for ADDR flag to be set (Slave acknowledged his address) */
			zero_time = Get_msTick();    
			while((tmpSR1 & (I2C_SR1_ADDR | I2C_SR1_AF)) == 0)
				{     
					tmpSR1 = EEPROM.I2C->SR1;				
					if(Check_msTick(zero_time,EEPROM.TimeOut)) return EEPROM_Status_FAIL;
				}
							 
			/* Check if the ADDR flag has been set */
			if (tmpSR1 & I2C_SR1_ADDR)
				{
					/* Clear ADDR Flag by reading SR1 then SR2 registers (SR1 have already 
						 been read) */
					(void)EEPROM.I2C->SR2;
					 
					I2C_GenerateSTOP(EEPROM.I2C, ENABLE);
						
					return EEPROM_Status_OK;
				}
				else
					{
						/*!< Clear AF flag */
						I2C_ClearFlag(EEPROM.I2C, I2C_FLAG_AF);                  
					}
					Standby_trial++;
		}
		return EEPROM_Status_FAIL;
}




EEPROM_Status EEPROM_ReadPage (uint8_t *buffer, uint16_t page)
{
// 		if (EEPROM_WaitStandby() == EEPROM_Status_FAIL) 
// 			return EEPROM_Status_FAIL;			
// 		else	
		return EEPROM_Read (page * Page_Size, buffer, Page_Size);
}

EEPROM_Status EEPROM_WritePage (uint8_t *buffer, uint16_t page)
{		
	return EEPROM_Write (page * Page_Size, buffer, Page_Size);	
}

