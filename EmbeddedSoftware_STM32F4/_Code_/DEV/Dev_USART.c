#include "Dev_USART.h"
#include <stdio.h>
#include <stdlib.h>
#include "Timing_Manage.h"
#include "Serial_Link.h"

USART USART_2;
USART USART_3;

DMA_InitTypeDef  DMA_USART_InitStructure;	

uint8_t test [3];

uint8_t USART_WaitDMADisable (USART_Config* Line)
{
	uint32_t zero_time = Get_msTick();
//	//Set mode to single to automatically disable current dma stream	
//	Line->Mode			 														= USART_MODE_DMASingle;									
	
	while (1)
	{
		if(DMA_GetCmdStatus (Line->DMA_Stream) == DISABLE) 	 		
			return 1;
		if(Check_msTick(zero_time,100)) 
		{
			DMA_Cmd(Line->DMA_Stream,DISABLE);			
			return 0;
		}
	}
}
void USART_OpenPort (USART * serial, uint32_t BaudRate)
	{
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		
		if 			(serial->USART == USART1)	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
		else if (serial->USART == USART2)	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
		else if (serial->USART == USART3)	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
		else if (serial->USART == UART4)	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);
		else if (serial->USART == UART5)	  RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);
		else if (serial->USART == USART6)	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART6, ENABLE);
		
		/* Open Serial Port */
		USART_InitStructure.USART_BaudRate = BaudRate;
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		USART_InitStructure.USART_StopBits = USART_StopBits_1;
		USART_InitStructure.USART_Parity = USART_Parity_No;
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
		USART_Init(serial->USART, &USART_InitStructure);	
		
		
		/* If request, Configure DMA for TX */
		
		switch  (serial->TX.USART_DMA)	
			{
				case USART_DMA_Enable:
																if 	(serial->TX.DMA_Stream < DMA2_Stream0)   	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
																		else																			RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
																	
																DMA_DeInit(serial->TX.DMA_Stream);			 
																	DMA_USART_InitStructure.DMA_Channel 											= serial->TX.DMA_Channel;
																	DMA_USART_InitStructure.DMA_DIR 													= DMA_DIR_MemoryToPeripheral; // Transmit
																	DMA_USART_InitStructure.DMA_PeripheralBaseAddr						= (uint32_t)&serial->USART->DR;
																	DMA_USART_InitStructure.DMA_PeripheralInc 								= DMA_PeripheralInc_Disable;
																	DMA_USART_InitStructure.DMA_MemoryInc 										= DMA_MemoryInc_Enable;
																	DMA_USART_InitStructure.DMA_PeripheralDataSize 						= DMA_PeripheralDataSize_Byte;
																	DMA_USART_InitStructure.DMA_MemoryDataSize 								= DMA_MemoryDataSize_Byte;
																	DMA_USART_InitStructure.DMA_Mode 													= DMA_Mode_Circular;
																	DMA_USART_InitStructure.DMA_Priority 											= DMA_Priority_High;
																	DMA_USART_InitStructure.DMA_FIFOMode 											= DMA_FIFOMode_Enable;
																	DMA_USART_InitStructure.DMA_FIFOThreshold 								= DMA_FIFOThreshold_Full;
																	DMA_USART_InitStructure.DMA_MemoryBurst										= DMA_MemoryBurst_Single;
																	DMA_USART_InitStructure.DMA_PeripheralBurst								= DMA_PeripheralBurst_Single;
																DMA_Init(serial->TX.DMA_Stream, &DMA_USART_InitStructure); 
																
																/* Enable the DMA Interrupt */
																	NVIC_InitStructure.NVIC_IRQChannel 												= serial->TX.DMA_NVIC_IRQ;
																	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 			= 0;
																	NVIC_InitStructure.NVIC_IRQChannelSubPriority 						= 0;
																	NVIC_InitStructure.NVIC_IRQChannelCmd 										= ENABLE;
																NVIC_Init(&NVIC_InitStructure);

																/* Enable interrupt */
																DMA_ITConfig(serial->TX.DMA_Stream, DMA_IT_TC, ENABLE);
																																
																/* Enable the USART Tx DMA request */
																USART_DMACmd(serial->USART, USART_DMAReq_Tx, ENABLE);  
																
																/* Enable the DMA TX Stream */
																DMA_Cmd(serial->TX.DMA_Stream, DISABLE);																
																break;
					case USART_DMA_Disable:					
																/* Enable the USART Interrupt */
																	NVIC_InitStructure.NVIC_IRQChannel 												= serial->IT_NVIC_IRQ;
																	NVIC_InitStructure.NVIC_IRQChannelSubPriority 						= 0;
																	NVIC_InitStructure.NVIC_IRQChannelCmd 										= ENABLE;
																NVIC_Init(&NVIC_InitStructure);					
					
																/* Enable USART Receive interrupts */
																USART_ITConfig(serial->USART, USART_IT_TXE, DISABLE);		
																break;
			}				
		
		/* If request, Configure DMA for RX */
		switch  (serial->RX.USART_DMA)	
			{
				case USART_DMA_Enable:
																if 	(serial->RX.DMA_Stream < DMA2_Stream0)  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
																		else																		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE);
																
																	DMA_DeInit(serial->RX.DMA_Stream);			 
																	DMA_USART_InitStructure.DMA_Channel 											= serial->RX.DMA_Channel;
																	DMA_USART_InitStructure.DMA_DIR 													= DMA_DIR_PeripheralToMemory; // Receive
																	DMA_USART_InitStructure.DMA_PeripheralBaseAddr						= (uint32_t)&serial->USART->DR;
																	DMA_USART_InitStructure.DMA_PeripheralInc 								= DMA_PeripheralInc_Disable;
																	DMA_USART_InitStructure.DMA_MemoryInc 										= DMA_MemoryInc_Enable;
																	DMA_USART_InitStructure.DMA_PeripheralDataSize 						= DMA_PeripheralDataSize_Byte;
																	DMA_USART_InitStructure.DMA_MemoryDataSize 								= DMA_MemoryDataSize_Byte;
																	DMA_USART_InitStructure.DMA_Mode 													= DMA_Mode_Circular;
																	DMA_USART_InitStructure.DMA_Priority 											= DMA_Priority_High;
																	DMA_USART_InitStructure.DMA_FIFOMode 											= DMA_FIFOMode_Enable;
																	DMA_USART_InitStructure.DMA_FIFOThreshold 								= DMA_FIFOThreshold_Full;
																	DMA_USART_InitStructure.DMA_MemoryBurst										= DMA_MemoryBurst_Single;
																	DMA_USART_InitStructure.DMA_PeripheralBurst								= DMA_PeripheralBurst_Single;																																						
																											DMA_USART_InitStructure.DMA_Channel							= serial->RX.DMA_Channel;			//rx channel
																											DMA_USART_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)test;
																											DMA_USART_InitStructure.DMA_BufferSize 					= 12;									//rx dma length
																DMA_Init(serial->RX.DMA_Stream, &DMA_USART_InitStructure); 
																
																/* Enable the DMA Interrupt */
																	NVIC_InitStructure.NVIC_IRQChannel 												= serial->RX.DMA_NVIC_IRQ;
																	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority 			= 0;
																	NVIC_InitStructure.NVIC_IRQChannelSubPriority							= 0;
																	NVIC_InitStructure.NVIC_IRQChannelCmd 										= ENABLE;
																NVIC_Init(&NVIC_InitStructure);
																
																/* Enable interrupt */
																DMA_ITConfig(serial->RX.DMA_Stream, DMA_IT_TC, ENABLE);																																			
																
																/* Enable the USART Rx DMA request */
																USART_DMACmd(serial->USART, USART_DMAReq_Rx, ENABLE); 
																
																/* Enable the DMA RX Stream */
																DMA_Cmd(serial->RX.DMA_Stream, ENABLE);	
																break;
					case USART_DMA_Disable:					
																/* Enable the USART Interrupt */
																	NVIC_InitStructure.NVIC_IRQChannel 												= serial->IT_NVIC_IRQ;
																	NVIC_InitStructure.NVIC_IRQChannelSubPriority 						= 0;
																	NVIC_InitStructure.NVIC_IRQChannelCmd 										= ENABLE;
																NVIC_Init(&NVIC_InitStructure);					
					
																/* Enable USART Receive interrupts */
																USART_ITConfig(serial->USART, USART_IT_RXNE, DISABLE);		
																break;
				}
						
		USART_Cmd(serial->USART, ENABLE);
	}
void  USART_Set (USART * serial,USART_DIR DIR, uint8_t *Buffer, uint32_t BufferSize, USART_MODE MODE)
{																																																																	
	switch (DIR)
		{		
			case USART_TX:																							
										switch (serial->TX.USART_DMA)
														{
															case USART_DMA_Enable:			
																											switch (MODE)
																											{
																												case USART_MODE_DMAInterrupt_Request: 																													
																													serial->TX.Mode			 														= MODE;																																		
																													break;
																												default: 
																													DMA_Cmd(serial->TX.DMA_Stream,DISABLE);		
																													serial->TX.Mode			 														= MODE;												
																													DMA_USART_InitStructure.DMA_Channel							= serial->TX.DMA_Channel;			//tx channel
																													DMA_USART_InitStructure.DMA_PeripheralBaseAddr	= (uint32_t)&serial->USART->DR;
																													DMA_USART_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)Buffer;								
																													DMA_USART_InitStructure.DMA_DIR 								= DMA_DIR_MemoryToPeripheral;	//tx dma type
																													DMA_USART_InitStructure.DMA_BufferSize 					= BufferSize;									//tx dma length															
																													DMA_Init		(serial->TX.DMA_Stream, &DMA_USART_InitStructure);																											
																													DMA_ITConfig(serial->TX.DMA_Stream, DMA_IT_TC, ENABLE);						
																													DMA_Cmd(serial->TX.DMA_Stream, ENABLE);				
																													break;
																											}
																											break;	
															case USART_DMA_Disable:																																													
																											serial->TX.Buffer 															= Buffer;																		
																											serial->TX.BuffPos 															= 0;																
																											serial->TX.BuffSize 														= BufferSize;
																											serial->TX.Mode																	= MODE;	
																											USART_ITConfig(serial->USART, USART_IT_TXE, ENABLE);		
																											break;
														}
										break;	
			case USART_RX:																											
										switch (serial->RX.USART_DMA)
														{
															case USART_DMA_Enable:																										
																											DMA_Cmd(serial->RX.DMA_Stream, DISABLE);																																	
																											DMA_USART_InitStructure.DMA_Channel							= serial->RX.DMA_Channel;			//rx channel
																											DMA_USART_InitStructure.DMA_PeripheralBaseAddr 	= (uint32_t)&serial->USART->DR;
																											DMA_USART_InitStructure.DMA_Memory0BaseAddr 		= (uint32_t)Buffer;
																											DMA_USART_InitStructure.DMA_BufferSize 					= BufferSize;									//rx dma length
																											DMA_USART_InitStructure.DMA_DIR 								= DMA_DIR_PeripheralToMemory;	//rx dma type
																											DMA_Init		(serial->RX.DMA_Stream, &DMA_USART_InitStructure);		
																											serial->RX.Mode 																= MODE;		
																											DMA_Cmd(serial->RX.DMA_Stream, ENABLE);						
// 																											DMA_SetCurrDataCounter (serial->RX.DMA_Stream,0);
															
															
																											break;	
															case USART_DMA_Disable:	
																											serial->RX.Buffer 															= Buffer;																		
																											serial->RX.BuffPos 															= 0;																
																											serial->RX.BuffSize 														= BufferSize;
																											serial->RX.Mode 																= MODE;		
																											USART_ITConfig(serial->USART, USART_IT_RXNE, ENABLE);																		
																											break;
														}
										break;
		}		
		
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																																						 ////
////DMA MODE
//
// void UART2_TX_DMA_IT(void) 
// {	
//   if (DMA_GetITStatus	(UART2_TX_DMA_ST, UART2_TX_DMA_FLAG))
// 		{
// 			DMA_ClearITPendingBit(UART2_TX_DMA_ST,  UART2_TX_DMA_FLAG);				
// 			DMA_Cmd(UART2_TX_DMA_ST, DISABLE);																																												
// 		}
// }

void UART2_RX_DMA_IT(void)
{
  if (DMA_GetITStatus	(UART2_RX_DMA_ST, UART2_RX_DMA_FLAG))
		{
			DMA_ClearITPendingBit(UART2_RX_DMA_ST, UART2_RX_DMA_FLAG);																																	
		} 
}

void UART3_TX_DMA_IT(void) 
{	
  if (DMA_GetITStatus	(UART3_TX_DMA_ST, UART3_TX_DMA_FLAG))
		{					
			switch (USART_3.TX.Mode)
			{
				case USART_MODE_DMAContinuos:
																			SerialLink_StreamProcess();
																			DMA_ClearITPendingBit(UART3_TX_DMA_ST,  UART3_TX_DMA_FLAG);	
																			break;
				case USART_MODE_DMASingle:												
																			DMA_Cmd(UART3_TX_DMA_ST, DISABLE);			
																			DMA_ClearITPendingBit(UART3_TX_DMA_ST,  UART3_TX_DMA_FLAG);	
																			break;
				case USART_MODE_DMAInterrupt_Request:						
																			DMA_Cmd(UART3_TX_DMA_ST, DISABLE);	
																			DMA_ClearITPendingBit(UART3_TX_DMA_ST,  UART3_TX_DMA_FLAG);	
																			SerialLink_DebugProcess();
																			break;
				case USART_MODE_DMAInterrupt_Done:						
																			DMA_Cmd(UART3_TX_DMA_ST, DISABLE);	
																			DMA_ClearITPendingBit(UART3_TX_DMA_ST,  UART3_TX_DMA_FLAG);	
																			SerialLink_StreamProcess();		
																			break;			
			}
		}
}

void UART3_RX_DMA_IT(void)
{
  if (DMA_GetITStatus	(UART3_RX_DMA_ST, UART3_RX_DMA_FLAG))
  {
    DMA_ClearITPendingBit(UART3_RX_DMA_ST, UART3_RX_DMA_FLAG);
  }
 
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////INTERRUPT MODE
//
void USART2_IRQHandler(void)
{
	uint8_t intput;
	if( USART_GetITStatus(USART2, USART_IT_RXNE) )
		{ 																																			  		
			intput = 	USART_ReceiveData(USART2);																																											
// 			if (intput=='*') USART_3.RX.BuffPos =0;	else																					//first uint8_t protocol
			*(USART_2.RX.Buffer + USART_2.RX.BuffPos++) 			= 				intput;	
			if (USART_2.RX.BuffPos >= USART_2.RX.BuffSize) 							USART_2.RX.BuffPos = 0;  	//for ring buffer
		}
	
	if(USART_GetITStatus(USART2, USART_IT_TXE) != RESET)
		{																																																															
			USART_SendData(USART2, *(USART_2.TX.Buffer + USART_2.TX.BuffPos++));		
			if (USART_2.TX.BuffPos >= USART_2.TX.BuffSize) 						//Disable the UART4 Transmit interrupt
				USART_ITConfig(USART2, USART_IT_TXE, DISABLE);
		}
}

void USART3_IRQHandler(void)
{
	uint8_t intput;
	if( USART_GetITStatus(USART3, USART_IT_RXNE) )
		{  		
			intput = 	USART_ReceiveData(USART3);																																																											
// 			if (intput=='*') USART_3.RX.BuffPos =0;	else																					//first uint8_t protocol
			*(USART_3.RX.Buffer + USART_3.RX.BuffPos++) 			= 				intput;	
			if (USART_3.RX.BuffPos >= USART_3.RX.BuffSize) 							USART_3.RX.BuffPos = 0;  	//for ring buffer
		}		
	if(USART_GetITStatus(USART3, USART_IT_TXE))
		{  																																																												
			USART_SendData(USART3, *(USART_3.TX.Buffer + USART_3.TX.BuffPos++));		
			if (USART_3.TX.BuffPos >= USART_3.TX.BuffSize) 						//Disable the UART4 Transmit interrupt
				USART_ITConfig(USART3, USART_IT_TXE, DISABLE);
		}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																																																	 ////
////USER CODE
//
void Initialize_USART_2 (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;		
	uint8_t Helo[23] = "USART 2 initialized! \n";
	
	RCC_AHB1PeriphClockCmd(UART2_GPIO_CLK, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = UART2_RX_PIN | UART2_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(UART2_GPIO_PORT, &GPIO_InitStructure);
 
  /* Connect USART pins to AF */
  GPIO_PinAFConfig(UART2_GPIO_PORT, UART2_RX_SOURCE, GPIO_AF_USART2);
  GPIO_PinAFConfig(UART2_GPIO_PORT, UART2_TX_SOURCE, GPIO_AF_USART2);
	
	USART_2.USART 							= USART2;
	USART_2.IT_NVIC_IRQ					= USART2_IRQn;
	
	
// 	USART_2.TX.USART_DMA				= USART_DMA_Enable;
// 	USART_2.TX.DMA_Channel 			= UART2_TX_DMA_CH;
// 	USART_2.TX.DMA_Stream	 			= UART2_TX_DMA_ST;
// 	USART_2.TX.DMA_NVIC_IRQ			= UART2_TX_DMA_IRQn;
	
// 	USART_2.RX.USART_DMA				= USART_DMA_Enable;
	USART_2.RX.DMA_Channel 			= UART2_RX_DMA_CH;
	USART_2.RX.DMA_Stream	 			= UART2_RX_DMA_ST;
	USART_2.RX.DMA_NVIC_IRQ 		= UART2_RX_DMA_IRQn;

	USART_OpenPort			(&USART_2,9600);	
	
	 USART_Set (&USART_2,USART_TX, Helo, 20, USART_MODE_DMASingle);
}

