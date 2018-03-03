#include "Dev_DIGITAL.h"
#include "stdio.h"
#include "Timing_Manage.h"
#include "Dev_TIMER.h"
#include "Dev_USART.h"
#include "Systick.h"
#include "Serial_Link.h"

Button 		Button_A,	Button_C,	Button_D,	Button_B, 	Button_OK;
uint8_t 	RGB_LOOP,	RED, BLUE, GREEN;



void Setup_BUTTON(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	  
	/* Enable lock */
	  RCC_AHB1PeriphClockCmd(Button_A_CLK |	Button_D_CLK |	Button_D_CLK |	Button_B_CLK |	Button_OK_CLK, ENABLE);
	
	//Configure BUTTONs pin as input floating ////////////////////////////////////
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
			GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;

		//Button Up
			GPIO_InitStructure.GPIO_Pin = Button_A_PIN;
			GPIO_Init(Button_A_PORT, &GPIO_InitStructure);
		//Button Down
			GPIO_InitStructure.GPIO_Pin = Button_C_PIN;
			GPIO_Init(Button_C_PORT, &GPIO_InitStructure);
		//Button Left
			GPIO_InitStructure.GPIO_Pin = Button_D_PIN;
			GPIO_Init(Button_D_PORT, &GPIO_InitStructure);
		//Button Right
			GPIO_InitStructure.GPIO_Pin = Button_B_PIN;
			GPIO_Init(Button_B_PORT, &GPIO_InitStructure);
		//Button Center
			GPIO_InitStructure.GPIO_Pin = Button_OK_PIN;
			GPIO_Init(Button_OK_PORT, &GPIO_InitStructure);	 
	//////////////////////////////////////////////////////////////////////////////  

	/* Enable SYSCFG clock */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	  
	/* Connect EXTI0 Line to PA0 pin */
	  SYSCFG_EXTILineConfig(Button_A_Port_source, Button_A_Pin_source); 						//Button Up
	  SYSCFG_EXTILineConfig(Button_C_Port_source, Button_C_Pin_source);					//Button Down
	  SYSCFG_EXTILineConfig(Button_D_Port_source, Button_D_Pin_source);					//Button Left
	  SYSCFG_EXTILineConfig(Button_B_Port_source, Button_B_Pin_source);				//Button Right
	  SYSCFG_EXTILineConfig(Button_OK_Port_source, Button_OK_Pin_source);			//Button Center


	/* Configure EXTI line */
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  
		//Button Up
			EXTI_InitStructure.EXTI_Line = Button_A_EXTI_Line;
			EXTI_Init(&EXTI_InitStructure);
	  
		//Button Down
			EXTI_InitStructure.EXTI_Line = Button_C_EXTI_Line;
			EXTI_Init(&EXTI_InitStructure);
	  
		//Button Left
			EXTI_InitStructure.EXTI_Line = Button_D_EXTI_Line;
			EXTI_Init(&EXTI_InitStructure);
	  
		//Button Right
			EXTI_InitStructure.EXTI_Line = Button_B_EXTI_Line;
			EXTI_Init(&EXTI_InitStructure);
			
		//Button Center
			EXTI_InitStructure.EXTI_Line = Button_OK_EXTI_Line;
			EXTI_Init(&EXTI_InitStructure);
		
	  
	/* Enable and set EXTI Interrupt to the lowest priority */
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x0F;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x0F;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	   
		//Button Up  
			NVIC_InitStructure.NVIC_IRQChannel = Button_A_IRQChannel;
			NVIC_Init(&NVIC_InitStructure);
	   
		//Button Down
			NVIC_InitStructure.NVIC_IRQChannel = Button_C_IRQChannel;
			NVIC_Init(&NVIC_InitStructure);
	   
		//Button Left
			NVIC_InitStructure.NVIC_IRQChannel = Button_D_IRQChannel;
			NVIC_Init(&NVIC_InitStructure);
	   
		//Button Right  
			NVIC_InitStructure.NVIC_IRQChannel = Button_B_IRQChannel;
			NVIC_Init(&NVIC_InitStructure);
			
		//Button Center  
			NVIC_InitStructure.NVIC_IRQChannel = Button_OK_IRQChannel;
			NVIC_Init(&NVIC_InitStructure);
	   
	   
		Button_A.State	 	=	released;
		Button_C.State		=	released;
		Button_D.State		=	released;
		Button_B.State		=	released;	
		Button_OK.State		=	released;		
}

void RGB_Setup (void)
{
	GPIO_InitTypeDef 											GPIO_InitStructure;
	
	/* clock enable */
	RCC_AHB1PeriphClockCmd(RGB_GPIO_CLK, ENABLE);
	
	/* GPIO Configuration */
	GPIO_InitStructure.GPIO_Pin 		= RGB_PIN;
	GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_NOPULL ;
	GPIO_Init(RGB_PORT, &GPIO_InitStructure); 

	/* Connect TIM pins to AF */  
	GPIO_PinAFConfig(GREEN_PORT	, GREEN_PinSource	, GREEN_GPIO_AF);
	GPIO_PinAFConfig(RED_PORT		, RED_PinSource		, RED_GPIO_AF); 
	GPIO_PinAFConfig(BLUE_PORT	, BLUE_PinSource	, BLUE_GPIO_AF);

	Initialize_TIMER			(RGB_TIM, RGB_PERIOD, RGB_PRESCALER);
	Initialize_TIMER_PWM	(RGB_TIM, TIM_Channel_1 | TIM_Channel_2 | TIM_Channel_3 , RGB_CCR);	
	
	TIM_Cmd(RGB_TIM, ENABLE);	
	
	RGB_LOOP_ON();
}

void Initialize_DIGITAL (void)
{
	RGB_Setup();
	Setup_BUTTON();
}	

void RGB_SetLoop(int sw)
{
	switch (sw)	
	{
		case 0:			RGB_LOOP= sw;
								RGB_Update ( 0,0,0,0 );	
								break;
		case 1: 		RGB_LOOP= sw;
								RGB_Update ( 100,100,100,100 );	
								break;
		default:		if (RGB_LOOP) RGB_SetLoop(0);
								else					RGB_SetLoop(1);
								break;
	}
}

#define RGB_3 			30
#define RGB_2  			15
#define RGB_1  			0
#define RGB_IND			21
int8_t RGB_MATRIX [RGB_IND][3] = {		
																{ RGB_1   ,  RGB_1   , RGB_1   },//3
																{ RGB_2   ,  RGB_1   , RGB_1   },//4
																{ RGB_3   ,  RGB_1   , RGB_1   },//5

																{ RGB_1   ,  RGB_1   , RGB_1   },//6
																{ RGB_1   ,  RGB_2   , RGB_1   },//7
																{ RGB_1   ,  RGB_3   , RGB_1   },//8

																{ RGB_1   ,  RGB_1   , RGB_1   },//0
																{ RGB_1   ,  RGB_1   , RGB_2   },//1										
																{ RGB_1   ,  RGB_1   , RGB_3   },//2

																{ RGB_1   ,  RGB_1   , RGB_1   },//9

																{ RGB_2   ,  RGB_1   , RGB_2   },//16
																{ RGB_3   ,  RGB_1   , RGB_3   },//17

																{ RGB_1   ,  RGB_1   , RGB_1   },//18

																{ RGB_2   ,  RGB_2   , RGB_1   },//10
																{ RGB_3   ,  RGB_3   , RGB_1   },//11

																{ RGB_1   ,  RGB_1   , RGB_1   },//12

																{ RGB_1   ,  RGB_2   , RGB_2   },//13
																{ RGB_1   ,  RGB_3   , RGB_3   },//14

																{ RGB_1   ,  RGB_1   , RGB_1   },//15
																
																{ RGB_3   ,  RGB_3   , RGB_3   },//14
																
																{ RGB_1   ,  RGB_1   , RGB_1   },//15
																
														 };
int8_t RGB_Inicate, RGB_check;

void RGB_Process(int8_t brightness)
{
	if (RGB_LOOP)
		{
			RGB_check=0;	
			if 	(RGB_Inicate>=RGB_IND)  					RGB_Inicate = 0;	
			
			if (RED == RGB_MATRIX[RGB_Inicate][0]) RGB_check++;
			else
					{
						if 			(RED	<	RGB_MATRIX[RGB_Inicate][0]) RED++;
						else if	(RED	>	RGB_MATRIX[RGB_Inicate][0]) RED--;
					}
			
			if (GREEN == RGB_MATRIX[RGB_Inicate][1]) RGB_check++;
			else
					{
						if 			(GREEN	<	RGB_MATRIX[RGB_Inicate][1]) GREEN++;
						else if	(GREEN	>	RGB_MATRIX[RGB_Inicate][1]) GREEN--;
					}
					
			if (BLUE == RGB_MATRIX[RGB_Inicate][2]) RGB_check++;
			else
					{
						if 			(BLUE	<	RGB_MATRIX[RGB_Inicate][2]) BLUE++;
						else if	(BLUE	>	RGB_MATRIX[RGB_Inicate][2]) BLUE--;
					}
					
			if (RGB_check>=3)  RGB_Inicate++;
			RGB_Update ( brightness,  RED,  GREEN,  BLUE );	
		}
}


void RGB_Update (int8_t brightness, int8_t RED, int8_t GREEN, int8_t BLUE )
{
	float scale = (RGB_PERIOD * (float) brightness /100);
	
		RED_PERIOD_UPDATE		(RGB_TIM,	(uint16_t)	(scale	*	RED		/	100));
		GREEN_PERIOD_UPDATE	(RGB_TIM,	(uint16_t)	(scale	*	GREEN	/	100));
		BLUE_PERIOD_UPDATE	(RGB_TIM, (uint16_t)	(scale	*	BLUE	/	100));
}


void Button_A_Interupt (void)
{
  if(EXTI_GetITStatus(Button_A_EXTI_Line) != RESET)
  {
		if (GPIO_ReadInputDataBit(Button_A_PORT,Button_A_PIN))
			{
				Button_A.State 						= released;
				Button_A.LastPressTime 		= Get_msTick()- Button_A.PressTick;
				Button_A.PressTick 				= 0;
				RGB_LOOP_ON();
			}
			else 
				{
					Button_A.State 					= pressed;
					Button_A.PressTick			=	Get_msTick();
					Button_A.LastPressTime 	= 0;
					RGB_LOOP_OFF();
				}
		SerialLink_Debug("Button A");
		EXTI_ClearITPendingBit(Button_A_EXTI_Line);
  }
}

void Button_C_Interupt(void)
{
  if(EXTI_GetITStatus(Button_C_EXTI_Line) != RESET)
  {
		if (GPIO_ReadInputDataBit(Button_C_PORT,Button_C_PIN))
			{
				Button_C.State 						= released;
				Button_C.LastPressTime 		= Get_msTick()- Button_C.PressTick;
				Button_C.PressTick 				= 0;
				RGB_LOOP_ON();
			}
			else 
				{
					Button_C.State 					= pressed;
					Button_C.PressTick			=	Get_msTick();
					Button_C.LastPressTime 	= 0;
					RGB_LOOP_OFF();
				}
			SerialLink_Debug("Button C");
			EXTI_ClearITPendingBit(Button_C_EXTI_Line);
  }
}

void Button_D_Interupt(void)
{
  if(EXTI_GetITStatus(Button_D_EXTI_Line) != RESET)
  {
		
		if (GPIO_ReadInputDataBit(Button_D_PORT,Button_D_PIN))
			{
				Button_D.State 						= released;
				Button_D.LastPressTime 		= Get_msTick()- Button_D.PressTick;
				Button_D.PressTick 				= 0;
				RGB_LOOP_ON();
			}
			else 
				{
					Button_D.State 					= pressed;
					Button_D.PressTick			=	Get_msTick();
					Button_D.LastPressTime 	= 0;
					RGB_LOOP_OFF();
				}
		SerialLink_Debug("Button D");
    EXTI_ClearITPendingBit(Button_D_EXTI_Line);
  }
}

void Button_B_Interupt(void)
{
  if(EXTI_GetITStatus(Button_B_EXTI_Line) != RESET)
  {
		
		if (GPIO_ReadInputDataBit(Button_B_PORT,Button_B_PIN))
			{
				Button_B.State 					= released;
				Button_B.LastPressTime 	= Get_msTick()- Button_B.PressTick;
				Button_B.PressTick 			= 0;
				RGB_LOOP_ON();
			}
			else 
				{
					Button_B.State 					= pressed;
					Button_B.PressTick			=	Get_msTick();
					Button_B.LastPressTime 	= 0;
					RGB_LOOP_OFF();
				}
		SerialLink_Debug("Button B");
    EXTI_ClearITPendingBit(Button_B_EXTI_Line);
  }
}

void Button_OK_Interupt(void)
{
  if(EXTI_GetITStatus(Button_OK_EXTI_Line) != RESET)
  {
		
		if (GPIO_ReadInputDataBit(Button_OK_PORT,Button_OK_PIN))
			{
				Button_OK.State 					= released;
				Button_OK.LastPressTime 	= Get_msTick()- Button_OK.PressTick;
				Button_OK.PressTick 			= 0;
				RGB_LOOP_ON();
			}
			else 
				{
					Button_OK.State 					= pressed;
					Button_OK.PressTick				=	Get_msTick();
					Button_OK.LastPressTime 	= 0;
					RGB_LOOP_OFF();
				}
		SerialLink_Debug("Button OK");
    EXTI_ClearITPendingBit(Button_OK_EXTI_Line);
  }
}



void Request_LongPress	(Button *button, uint32_t presstime)
{
	uint64_t long_press = Get_msTick() - button->PressTick;
	if (long_press < button->LastPressTime) 
		long_press = button->LastPressTime;
	while (long_press<presstime);
}

Button_State Get_Button_State	(Button *button)
{
	return button->State;
}


