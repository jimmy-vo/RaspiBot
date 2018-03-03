#include "stm32f4xx_conf.h"

extern uint8_t shutdown;

				////////////////////////////////////////////DMA 1 Interupt is used////////////////////////////////////////
			 //  Stream0  //  Stream1  //  Stream2  //  Stream3  //  Stream4  //  Stream5  //  Stream6  //  Stream7  //                                      
			//////////////////////////////////////////////////////////////////////////////////////////////////////////
		 // Channel1  //  Channel4 //  Channel7 //  Channel4 // 				  //  Channel1 //  Channel1 //  Channel7 //                                        
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
	 // I2C1-RX   //  UART3-RX //  I2C2-RX  //  UART3-TX // 				  //  UART2-RX //  I2C1-TX	//  I2C2-TX  //                                     
	//////////////////////////////////////////////////////////////////////////////////////////////////////////

			////////////////////////////////////////////DMA 1 Interupt is used////////////////////////////////////////
		 //  Stream0  //  Stream1  //  Stream2  //  Stream3  //  Stream4  //  Stream5  //  Stream6  //  Stream7  //                                      
		//////////////////////////////////////////////////////////////////////////////////////////////////////////
	 //           // 	 ADC3	 	 //           //           //           //           //           //           //                                      
	//////////////////////////////////////////////////////////////////////////////////////////////////////////
 //           // 	Channel4 //           //           //           //           //           //           //                                        
//////////////////////////////////////////////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //										ADC_Voltage_Feedback											//	
/////////////////////////////////////////////////////////////////
#define VoltFB_GPIO_PORT						GPIOC
#define VoltFB_GPIO_PIN 						GPIO_Pin_0
#define VoltFB_GPIO_CLK  						RCC_AHB1Periph_GPIOC

#define VoltFB_ADC  					  		ADC3
#define VoltFB_ADC_CLK  						RCC_APB2Periph_ADC3
#define VoltFB_ADC_CHANNEL  				ADC_Channel_10

#define VoltFB_DMA_CLK  						RCC_AHB1Periph_DMA2
#define VoltFB_DMA_CHANNEL					DMA_Channel_2
#define VoltFB_DMA_STREAM						DMA2_Stream1
#define VoltFB_DMA_STREAM_IRQn			DMA2_Stream1_IRQn
#define VoltFB_DMA_IRQHANDLER   		DMA2_Stream1_IRQHandler
/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //														USART2														//									
/////////////////////////////////////////////////////////////////
#define UART2_GPIO_PORT					GPIOD
#define UART2_GPIO_CLK					RCC_AHB1Periph_GPIOD

#define UART2_IRQn							USART2_IRQn
#define UART2_IT								USART2_IRQHandler

#define UART2_TX_PIN						GPIO_Pin_5
#define UART2_TX_SOURCE					GPIO_PinSource5
#define UART2_RX_PIN						GPIO_Pin_6
#define UART2_RX_SOURCE					GPIO_PinSource6

// #define UART2_TX_DMA_CH					DMA_Channel_4
// #define UART2_TX_DMA_ST					DMA1_Stream6
// #define UART2_TX_DMA_IRQn				DMA1_Stream6_IRQn
// #define UART2_TX_DMA_FLAG				DMA_IT_TCIF6
// #define UART2_TX_DMA_IT					DMA1_Stream6_IRQHandler

#define UART2_RX_DMA_CH					DMA_Channel_4
#define UART2_RX_DMA_ST					DMA1_Stream5
#define UART2_RX_DMA_IRQn				DMA1_Stream5_IRQn
#define UART2_RX_DMA_FLAG				DMA_IT_TCIF5
#define UART2_RX_DMA_IT					DMA1_Stream5_IRQHandler
/////////////////////////////////////////////////////////////////
									
  /////////////////////////////////////////////////////////////////
 //														USART3														//									
/////////////////////////////////////////////////////////////////
#define UART3_GPIO_PORT					GPIOD
#define UART3_GPIO_CLK					RCC_AHB1Periph_GPIOD

#define UART3_IRQn							USART3_IRQn
#define UART3_IT								USART3_IRQHandler

#define UART3_RX_PIN						GPIO_Pin_9
#define UART3_RX_SOURCE					GPIO_PinSource9
#define UART3_TX_PIN						GPIO_Pin_8
#define UART3_TX_SOURCE					GPIO_PinSource8

#define UART3_RX_DMA_CH					DMA_Channel_4
#define UART3_RX_DMA_ST					DMA1_Stream1
#define UART3_RX_DMA_IRQn				DMA1_Stream1_IRQn
#define UART3_RX_DMA_FLAG				DMA_IT_TCIF1
#define UART3_RX_DMA_IT					DMA1_Stream1_IRQHandler

#define UART3_TX_DMA_CH					DMA_Channel_4
#define UART3_TX_DMA_ST					DMA1_Stream3
#define UART3_TX_DMA_IRQn				DMA1_Stream3_IRQn
#define UART3_TX_DMA_FLAG				DMA_IT_TCIF3
#define UART3_TX_DMA_IT					DMA1_Stream3_IRQHandler
/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //														I2C1															//									
/////////////////////////////////////////////////////////////////
#define I2C1_GPIO_PORT					GPIOB
#define I2C1_GPIO_CLK						RCC_AHB1Periph_GPIOB
#define I2C1_AF									GPIO_AF_I2C1

#define I2C1_SCL_PIN						GPIO_Pin_8
#define I2C1_SCL_SOURCE					GPIO_PinSource8
#define I2C1_SDA_PIN						GPIO_Pin_9
#define I2C1_SDA_SOURCE					GPIO_PinSource9

#define I2C1_RX_DMA_CH					DMA_Channel_1
#define I2C1_RX_DMA_ST					DMA1_Stream0
#define I2C1_RX_DMA_IRQn				DMA1_Stream0_IRQn
#define I2C1_RX_DMA_FLAG				DMA_IT_TCIF0
#define I2C1_RX_DMA_IT					DMA1_Stream0_IRQHandler

#define I2C1_TX_DMA_CH					DMA_Channel_1
#define I2C1_TX_DMA_ST					DMA1_Stream6
#define I2C1_TX_DMA_IRQn				DMA1_Stream6_IRQn
#define I2C1_TX_DMA_FLAG				DMA_IT_TCIF6
#define I2C1_TX_DMA_IT					DMA1_Stream6_IRQHandler
/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //														I2C2															//									
/////////////////////////////////////////////////////////////////
#define I2C2_GPIO_PORT					GPIOB
#define I2C2_GPIO_CLK						RCC_AHB1Periph_GPIOB
#define I2C2_AF									GPIO_AF_I2C2

#define I2C2_SCL_PIN						GPIO_Pin_10
#define I2C2_SCL_SOURCE					GPIO_PinSource10
#define I2C2_SDA_PIN						GPIO_Pin_11
#define I2C2_SDA_SOURCE					GPIO_PinSource11

#define I2C2_RX_DMA_CH					DMA_Channel_7
#define I2C2_RX_DMA_ST					DMA1_Stream2
#define I2C2_RX_DMA_IRQn				DMA1_Stream2_IRQn
#define I2C2_RX_DMA_FLAG				DMA_IT_TCIF2
#define I2C2_RX_DMA_IT					DMA1_Stream2_IRQHandler

#define I2C2_TX_DMA_CH					DMA_Channel_7
#define I2C2_TX_DMA_ST					DMA1_Stream7
#define I2C2_TX_DMA_IRQn				DMA1_Stream7_IRQn
#define I2C2_TX_DMA_FLAG				DMA_IT_TCIF7
#define I2C2_TX_DMA_IT					DMA1_Stream7_IRQHandler
/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //											ULTRASONIC															//									
/////////////////////////////////////////////////////////////////
#define TRIG_A_PORT									GPIOE
#define TRIG_A_PIN 					  	  	GPIO_Pin_7
#define TRIG_A_CLK  					  	  RCC_AHB1Periph_GPIOE
												
#define ECHO_A_PORT									GPIOE
#define ECHO_A_PIN 						    	GPIO_Pin_8
#define ECHO_A_CLK  						    RCC_AHB1Periph_GPIOE
#define ECHO_A_Pin_source 					GPIO_PinSource8
#define ECHO_A_Port_source					EXTI_PortSourceGPIOE
#define ECHO_A_EXTI_Line						EXTI_Line8
#define ECHO_A_IRQChannel						EXTI9_5_IRQn
#define ECHO_A_Interupt  						EXTI9_5_IRQHandler

#define TRIG_B_PORT									GPIOE
#define TRIG_B_PIN 						    	GPIO_Pin_14
#define TRIG_B_CLK  						    RCC_AHB1Periph_GPIOE

#define ECHO_B_PORT									GPIOE
#define ECHO_B_PIN									GPIO_Pin_15
#define ECHO_B_CLK									RCC_AHB1Periph_GPIOE
#define ECHO_B_Pin_source 					GPIO_PinSource15
#define ECHO_B_Port_source					EXTI_PortSourceGPIOE
#define ECHO_B_EXTI_Line						EXTI_Line15
#define ECHO_B_IRQChannel						EXTI15_10_IRQn
#define ECHO_B_Interupt  						EXTI15_10_IRQHandler
/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //											Servo																		//									
/////////////////////////////////////////////////////////////////
#define SERVOX_GPIO_PORT			GPIOA
#define SERVOX_GPIO_PIN				GPIO_Pin_0
#define SERVOX_GPIO_CLK				RCC_AHB1Periph_GPIOA
#define SERVOX_GPIO_AF				GPIO_AF_TIM5
#define SERVOX_GPIO_source		GPIO_PinSource0
#define SERVOX_TIM						TIM5
#define SERVOX_TIM_CLK				RCC_APB1Periph_TIM5
#define SERVOX_Channel_Init		TIM_OC1Init
#define SERVOX_TIM_update			TIM_SetCompare1
	
#define SERVOY_GPIO_PORT			GPIOA
#define SERVOY_GPIO_PIN				GPIO_Pin_1
#define SERVOY_GPIO_CLK				RCC_AHB1Periph_GPIOA
#define SERVOY_GPIO_AF				GPIO_AF_TIM5
#define SERVOY_GPIO_source		GPIO_PinSource1
#define SERVOY_TIM						TIM5
#define SERVOY_TIM_CLK				RCC_APB1Periph_TIM5
#define SERVOY_Channel_Init		TIM_OC2Init
#define SERVOY_TIM_update			TIM_SetCompare2


#define SERVO_GPIO_PORT				GPIOA
#define SERVO_GPIO_PIN				SERVOX_GPIO_PIN | SERVOY_GPIO_PIN
#define SERVO_TIM							TIM5
#define SERVO_TIM_CLK					RCC_APB1Periph_TIM5
/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //												Motor																	//									
/////////////////////////////////////////////////////////////////
#define MOTOR1_GPIO_PORT				GPIOC
#define MOTOR1_GPIO_PIN					GPIO_Pin_6
#define MOTOR1_GPIO_CLK					RCC_AHB1Periph_GPIOC
#define MOTOR1_GPIO_AF					GPIO_AF_TIM3
#define MOTOR1_GPIO_source			GPIO_PinSource6
#define MOTOR1_TIM							TIM3
#define MOTOR1_TIM_CLK					RCC_APB1Periph_TIM3
#define MOTOR1_TIM_update				TIM_SetCompare1
	
#define MOTOR2_GPIO_PORT				GPIOC
#define MOTOR2_GPIO_PIN					GPIO_Pin_7
#define MOTOR2_GPIO_CLK					RCC_AHB1Periph_GPIOC
#define MOTOR2_GPIO_AF					GPIO_AF_TIM3
#define MOTOR2_GPIO_source			GPIO_PinSource7
#define MOTOR2_TIM							TIM3
#define MOTOR2_TIM_CLK					RCC_APB1Periph_TIM3
#define MOTOR2_TIM_update				TIM_SetCompare2

#define MOTOR3_GPIO_PORT				GPIOC
#define MOTOR3_GPIO_PIN					GPIO_Pin_8
#define MOTOR3_GPIO_CLK					RCC_AHB1Periph_GPIOC
#define MOTOR3_GPIO_AF					GPIO_AF_TIM3
#define MOTOR3_GPIO_source			GPIO_PinSource8
#define MOTOR3_TIM							TIM3
#define MOTOR3_TIM_CLK					RCC_APB1Periph_TIM3
#define MOTOR3_TIM_update				TIM_SetCompare3

#define MOTOR4_GPIO_PORT				GPIOC
#define MOTOR4_GPIO_PIN					GPIO_Pin_9
#define MOTOR4_GPIO_CLK					RCC_AHB1Periph_GPIOC
#define MOTOR4_GPIO_AF					GPIO_AF_TIM3
#define MOTOR4_GPIO_source			GPIO_PinSource9
#define MOTOR4_TIM							TIM3
#define MOTOR4_TIM_CLK					RCC_APB1Periph_TIM3
#define MOTOR4_TIM_update				TIM_SetCompare4

#define MOTOR_GPIO_PORT					GPIOC
#define MOTOR_GPIO_PIN					MOTOR1_GPIO_PIN | MOTOR2_GPIO_PIN | MOTOR3_GPIO_PIN | MOTOR4_GPIO_PIN
#define MOTOR_GPIO_CLK					RCC_AHB1Periph_GPIOC
#define MOTOR_TIM								TIM3
#define MOTOR_TIM_CLK						RCC_APB1Periph_TIM3
/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //														ENCODER														//									
/////////////////////////////////////////////////////////////////
// Right Motor Channels
#define LA_ENC_GPIO_PIN          GPIO_Pin_12
#define LA_ENC_GPIO_PORT         GPIOD
#define LA_ENC_GPIO_CLK          RCC_AHB1Periph_GPIOD
#define LA_ENC_SOURCE            GPIO_PinSource12
#define LA_ENC_AF                GPIO_AF_TIM5

#define LB_ENC_GPIO_PIN          GPIO_Pin_13
#define LB_ENC_GPIO_PORT         GPIOD
#define LB_ENC_GPIO_CLK          RCC_AHB1Periph_GPIOD
#define LB_ENC_SOURCE            GPIO_PinSource13
#define LB_ENC_AF                GPIO_AF_TIM5

#define A_ENC_TIMER              TIM5
#define A_ENC_TIMER_CLK          RCC_APB1Periph_TIM5	

// Left Motor Channels
#define RA_ENC_GPIO_PIN          GPIO_Pin_0
#define RA_ENC_GPIO_PORT         GPIOA
#define RA_ENC_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define RA_ENC_SOURCE            GPIO_PinSource0
#define RA_ENC_AF                GPIO_AF_TIM4

#define RB_ENC_GPIO_PIN          GPIO_Pin_1
#define RB_ENC_GPIO_PORT         GPIOA
#define RB_ENC_GPIO_CLK          RCC_AHB1Periph_GPIOA
#define RB_ENC_SOURCE            GPIO_PinSource1
#define RB_ENC_AF                GPIO_AF_TIM4

#define B_ENC_TIMER              TIM4
#define B_ENC_TIMER_CLK          RCC_APB1Periph_TIM4	
/////////////////////////////////////////////////////////////////
  /////////////////////////////////////////////////////////////////
 //														BUTTONs	  												//									
/////////////////////////////////////////////////////////////////
#define Button_A_PORT									GPIOD
#define Button_A_PIN 					  	  	GPIO_Pin_4
#define Button_A_CLK  					  	  RCC_AHB1Periph_GPIOD
#define Button_A_Pin_source 					GPIO_PinSource4
#define Button_A_Port_source					EXTI_PortSourceGPIOD
#define Button_A_EXTI_Line						EXTI_Line4
#define Button_A_IRQChannel						EXTI4_IRQn	
#define Button_A_Interupt  						EXTI4_IRQHandler
												
#define Button_B_PORT								GPIOD
#define Button_B_PIN								GPIO_Pin_3
#define Button_B_CLK								RCC_AHB1Periph_GPIOD
#define Button_B_Pin_source 				GPIO_PinSource3
#define Button_B_Port_source				EXTI_PortSourceGPIOD
#define Button_B_EXTI_Line					EXTI_Line3
#define Button_B_IRQChannel					EXTI3_IRQn
#define Button_B_Interupt  					EXTI3_IRQHandler

#define Button_C_PORT								GPIOD
#define Button_C_PIN 						    GPIO_Pin_2
#define Button_C_CLK  						    RCC_AHB1Periph_GPIOD
#define Button_C_Pin_source 					GPIO_PinSource2
#define Button_C_Port_source					EXTI_PortSourceGPIOD
#define Button_C_EXTI_Line						EXTI_Line2
#define Button_C_IRQChannel					EXTI2_IRQn
#define Button_C_Interupt  					EXTI2_IRQHandler

#define Button_D_PORT								GPIOD
#define Button_D_PIN 						    GPIO_Pin_0
#define Button_D_CLK  						    RCC_AHB1Periph_GPIOD
#define Button_D_Pin_source 					GPIO_PinSource0
#define Button_D_Port_source					EXTI_PortSourceGPIOD
#define Button_D_EXTI_Line						EXTI_Line0
#define Button_D_IRQChannel					EXTI0_IRQn
#define Button_D_Interupt  					EXTI0_IRQHandler


#define Button_OK_PORT							GPIOD
#define Button_OK_PIN								GPIO_Pin_1
#define Button_OK_CLK								RCC_AHB1Periph_GPIOD
#define Button_OK_Pin_source 				GPIO_PinSource1
#define Button_OK_Port_source				EXTI_PortSourceGPIOD
#define Button_OK_EXTI_Line					EXTI_Line1
#define Button_OK_IRQChannel				EXTI1_IRQn
#define Button_OK_Interupt  				EXTI1_IRQHandler
/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //												RGB			 															//									
/////////////////////////////////////////////////////////////////
#define RGB_CCR										0
#define RGB_PRESCALER							168
#define RGB_PERIOD								1000

#define BLUE_PORT									GPIOE
#define BLUE_PIN									GPIO_Pin_9
#define BLUE_CLK									RCC_AHB1Periph_GPIOE
#define BLUE_PinSource						GPIO_PinSource9
#define BLUE_GPIO_AF							GPIO_AF_TIM1
#define BLUE_PERIOD_UPDATE				TIM_SetCompare1


#define GREEN_PORT								GPIOE
#define GREEN_PIN									GPIO_Pin_11
#define GREEN_CLK									RCC_AHB1Periph_GPIOE 
#define GREEN_PinSource						GPIO_PinSource11
#define GREEN_GPIO_AF							GPIO_AF_TIM1
#define GREEN_PERIOD_UPDATE				TIM_SetCompare2

#define RED_PORT									GPIOE
#define RED_PIN										GPIO_Pin_13
#define RED_CLK										RCC_AHB1Periph_GPIOE
#define RED_PinSource							GPIO_PinSource13
#define RED_GPIO_AF								GPIO_AF_TIM1
#define RED_PERIOD_UPDATE					TIM_SetCompare3

#define RGB_PORT									GPIOE
#define RGB_PIN 				    			BLUE_PIN | RED_PIN | GREEN_PIN
#define RGB_GPIO_CLK							RCC_AHB1Periph_GPIOE
#define RGB_TIM										TIM1
/////////////////////////////////////////////////////////////////

  /////////////////////////////////////////////////////////////////
 //												LCD 20x4  														//									
/////////////////////////////////////////////////////////////////
#define LCD_RS_GPIO_PORT					GPIOE
#define LCD_RS_GPIO_CLK						RCC_AHB1Periph_GPIOE 
#define LCD_RS_GPIO_PIN						GPIO_Pin_6

#define LCD_RW_GPIO_PORT					GPIOE
#define LCD_RW_GPIO_CLK						RCC_AHB1Periph_GPIOE
#define LCD_RW_GPIO_PIN						GPIO_Pin_5

#define LCD_EN_GPIO_PORT					GPIOE
#define LCD_EN_GPIO_CLK						RCC_AHB1Periph_GPIOE
#define LCD_EN_GPIO_PIN						GPIO_Pin_4

#define LCD_D4										GPIO_Pin_3
#define LCD_D5										GPIO_Pin_2
#define LCD_D6										GPIO_Pin_1
#define LCD_D7										GPIO_Pin_0
#define LCD_DATA_GPIO_PINS      	(LCD_D4|LCD_D5|LCD_D6|LCD_D7)
#define LCD_DATA_GPIO_PORT 				GPIOE
#define LCD_DATA_GPIO_CLK					RCC_AHB1Periph_GPIOE
/////////////////////////////////////////////////////////////////
