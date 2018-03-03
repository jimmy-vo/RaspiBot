#include "Serial_Link.h"
#include "Dev_USART.h"
#include "Timing_Manage.h"
#include <math.h>

DATA DATA_OUT ;


#define  Serial_Link_Size 			43 
uint8_t Serial_Link_DATA[Serial_Link_Size] = "Balapi_";
uint8_t Serial_Link_Debug[Serial_Link_Size] = "Balapi*";

uint8_t* DebugString;
void SerialLink_Debug (char* message  )
{
	int i;
	for (i = 7; i<Serial_Link_Size; i++)		
		Serial_Link_Debug[i] = *(message++);			
	USART_Set (&USART_3,USART_TX, Serial_Link_Debug, Serial_Link_Size, USART_MODE_DMAInterrupt_Request);
}

void SerialLink_DebugProcess (void)
{	
	USART_Set (&USART_3,USART_TX, Serial_Link_Debug, Serial_Link_Size, USART_MODE_DMAInterrupt_Done);
}

void SerialLink_StreamProcess (void)
{		
	///////////////////////////////////////////////////////////////////////////																																																			 ////
	////Indicator	
	//	
	Serial_Link_DATA [6] = '_';
	
	///////////////////////////////////////////////////////////////////////////																																																			 ////
	////SPEED	
	//	
	//Left Speed - 1 byte
	Serial_Link_DATA [7]	= (uint8_t) DATA_OUT.Left_Speed;
	
	//Right Speed - 1 byte
	Serial_Link_DATA [8]	= (uint8_t) DATA_OUT.Right_Speed;
	
	///////////////////////////////////////////////////////////////////////////																																																			 ////
	////ENCODER	
	//	
	//Left Encoder - 2 bytes
	Serial_Link_DATA [9]	= (uint8_t) ((DATA_OUT.Left_Encoder &  0xFF00) >>8);
	Serial_Link_DATA [10]	= (uint8_t) (DATA_OUT.Left_Encoder & 0x00FF); 
	
	//Right Encoder - 2 bytes
	Serial_Link_DATA [11]	= (uint8_t) ((DATA_OUT.Right_Encoder & 0xFF00) >>8);
	Serial_Link_DATA [12]	= (uint8_t) (DATA_OUT.Right_Encoder & 0x00FF); 	 	
	
	///////////////////////////////////////////////////////////////////////////																																																			 ////
	////ULTRASONIC	
	//	
	//SONA A - 2 bytes
	Serial_Link_DATA [13]	= (uint8_t) ((DATA_OUT.Ultrasonic_A & 0xFF00) >>8);
	Serial_Link_DATA [14]	= (uint8_t) (DATA_OUT.Ultrasonic_A & 0x00FF); 	 	
	
	//SONA B - 2 bytes
	Serial_Link_DATA [15]	= (uint8_t) ((DATA_OUT.Ultrasonic_B & 0xFF00) >>8);
	Serial_Link_DATA [16]	= (uint8_t) (DATA_OUT.Ultrasonic_B & 0x00FF); 	 		
	
	///////////////////////////////////////////////////////////////////////////																																																			 ////
	////TEMPERATURE	
	//	
	//- 2 bytes
	Serial_Link_DATA [17]	= (uint8_t) ((DATA_OUT.Temperature & 0xFF00) >>8);
	Serial_Link_DATA [18]	= (uint8_t) (DATA_OUT.Temperature & 0x00FF); 	
	
	///////////////////////////////////////////////////////////////////////////																																																			 ////
	////ACCELERATION	
	//	
	//ACCELERATION X - 2 bytes
	Serial_Link_DATA [19]	= (uint8_t) ((DATA_OUT.Accex & 0xFF00) >>8);
	Serial_Link_DATA [20]	= (uint8_t) (DATA_OUT.Accex & 0x00FF); 	 
	
	//ACCELERATION y - 2 bytes
	Serial_Link_DATA [21]	= (uint8_t) ((DATA_OUT.Accey & 0xFF00) >>8);
	Serial_Link_DATA [22]	= (uint8_t) (DATA_OUT.Accey & 0x00FF); 	 	
	
	//ACCELERATION z - 2 bytes
	Serial_Link_DATA [23]	= (uint8_t) ((DATA_OUT.Accez & 0xFF00) >>8);
	Serial_Link_DATA [24]	= (uint8_t) (DATA_OUT.Accez & 0x00FF); 	 	
	
	///////////////////////////////////////////////////////////////////////////																																																			 ////
	////GYRO	
	//	
	//GYRO X - 2 bytes
	Serial_Link_DATA [25]	= (uint8_t) ((DATA_OUT.Gyrox & 0xFF00) >>8);
	Serial_Link_DATA [26]	= (uint8_t) (DATA_OUT.Gyrox & 0x00FF); 	 
	
	//GYRO y - 2 bytes
	Serial_Link_DATA [27]	= (uint8_t) ((DATA_OUT.Gyroy & 0xFF00) >>8);
	Serial_Link_DATA [28]	= (uint8_t) (DATA_OUT.Gyroy & 0x00FF); 	 	
	
	//GYRO z - 2 bytes
	Serial_Link_DATA [29]	= (uint8_t) ((DATA_OUT.Gyroz & 0xFF00) >>8);
	Serial_Link_DATA [30]	= (uint8_t) (DATA_OUT.Gyroz & 0x00FF); 	 	

	///////////////////////////////////////////////////////////////////////////																																																			 ////
	////COMPACT	
	//	
	
	//COMPACT X - 2 bytes
	Serial_Link_DATA [31]	= (uint8_t) ((DATA_OUT.Compx & 0xFF00) >>8);
	Serial_Link_DATA [32]	= (uint8_t) (DATA_OUT.Compx & 0x00FF); 	 
	
	//COMPACT y - 2 bytes
	Serial_Link_DATA [33]	= (uint8_t) ((DATA_OUT.Compy & 0xFF00) >>8);
	Serial_Link_DATA [34]	= (uint8_t) (DATA_OUT.Compy & 0x00FF); 	 	
	
	//COMPACT z - 2 bytes
	Serial_Link_DATA [35]	= (uint8_t) ((DATA_OUT.Compz & 0xFF00) >>8);
	Serial_Link_DATA [36]	= (uint8_t) (DATA_OUT.Compz & 0x00FF); 	 		

	///////////////////////////////////////////////////////////////////////////																																																			 ////
	////
	//	
		
	//Roll - 2 bytes
	Serial_Link_DATA [37]	= (uint8_t) ((DATA_OUT.Roll & 0xFF00) >>8);
	Serial_Link_DATA [38]	= (uint8_t) (DATA_OUT.Roll & 0x00FF); 	 	
	
	//Pitch - 2 bytes
	Serial_Link_DATA [39]	= (uint8_t) ((DATA_OUT.Pitch & 0xFF00) >>8);
	Serial_Link_DATA [40]	= (uint8_t) (DATA_OUT.Pitch & 0x00FF); 	 		
	
	//Yaw  - 2 bytes
	Serial_Link_DATA [41]	= (uint8_t) ((DATA_OUT.Yaw & 0xFF00) >>8);
	Serial_Link_DATA [42]	= (uint8_t) (DATA_OUT.Yaw & 0x00FF); 	 	
	
	if (DMA_GetCmdStatus (USART_3.TX.DMA_Stream)== DISABLE) 	
	USART_Set (&USART_3,USART_TX, Serial_Link_DATA, Serial_Link_Size,USART_MODE_DMAContinuos);
}

void Initialize_SerialLink (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
		
	RCC_AHB1PeriphClockCmd(UART3_GPIO_CLK, ENABLE);
	
  GPIO_InitStructure.GPIO_Pin = UART3_RX_PIN | UART3_TX_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
  GPIO_Init(UART3_GPIO_PORT, &GPIO_InitStructure);
 
  /* Connect USART pins to AF */
  GPIO_PinAFConfig(UART3_GPIO_PORT, UART3_TX_SOURCE, GPIO_AF_USART3);
  GPIO_PinAFConfig(UART3_GPIO_PORT, UART3_RX_SOURCE, GPIO_AF_USART3);
	
	USART_3.USART 							= USART3;
	USART_3.IT_NVIC_IRQ					= USART3_IRQn;
	
	USART_3.TX.USART_DMA				= USART_DMA_Enable;
	USART_3.TX.DMA_Channel 			= UART3_TX_DMA_CH;
	USART_3.TX.DMA_Stream	 			= UART3_TX_DMA_ST;
	USART_3.TX.DMA_NVIC_IRQ 		= UART3_TX_DMA_IRQn;

	USART_3.RX.USART_DMA				= USART_DMA_Enable;
	USART_3.RX.DMA_Channel 			= UART3_RX_DMA_CH;
	USART_3.RX.DMA_Stream	 			= UART3_RX_DMA_ST;
	USART_3.RX.DMA_NVIC_IRQ 		= UART3_RX_DMA_IRQn;
	
	USART_OpenPort(&USART_3,9600);	
	
	USART_Set (&USART_3,USART_TX, Serial_Link_DATA, Serial_Link_Size,USART_MODE_DMAContinuos);
}


