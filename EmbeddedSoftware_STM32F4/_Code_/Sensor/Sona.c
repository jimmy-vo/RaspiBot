#include "Sona.h"
#include "Timing_Manage.h"
#include "Dev_TIMER.h"
#include "Dev_DIGITAL.h"
#include "Serial_Link.h"

SONA Sona_A, Sona_B;
uint64_t Sona_Count;

uint64_t SONA_Get_tick (void)
	{
		return Sona_Count*10;
	}	

static void SONA_delay(u32 nCount)
{
	uint64_t zero = SONA_Get_tick();
	while(SONA_Get_tick()- zero >= nCount);
}	

void Setup_SONA_IO(void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	GPIO_InitTypeDef   GPIO_InitStructure;
	NVIC_InitTypeDef   NVIC_InitStructure;
	  
	/* Enable lock */
	  RCC_AHB1PeriphClockCmd(TRIG_A_CLK |	ECHO_A_CLK |	TRIG_B_CLK | ECHO_B_CLK, ENABLE);

	//Configure TRIG pinS as output ///////////////////////////////////
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
			GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

		//TRIG A
			GPIO_InitStructure.GPIO_Pin = TRIG_A_PIN;
			GPIO_Init(TRIG_A_PORT, &GPIO_InitStructure);
	
		//TRIG B
			GPIO_InitStructure.GPIO_Pin = TRIG_B_PIN;
			GPIO_Init(TRIG_B_PORT, &GPIO_InitStructure);
	
	//Configure ECHO pinS as input ////////////////////////////////////
			GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
			GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN;
			GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;
			GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;

		//ECHO A
			GPIO_InitStructure.GPIO_Pin = ECHO_A_PIN;
			GPIO_Init(ECHO_A_PORT, &GPIO_InitStructure);
	
		//ECHO B
			GPIO_InitStructure.GPIO_Pin = ECHO_B_PIN;
			GPIO_Init(ECHO_B_PORT, &GPIO_InitStructure);
	
	/* Enable SYSCFG clock */
	  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	  
	/* Connect EXTI Line to PA pin */
	  SYSCFG_EXTILineConfig(ECHO_A_Port_source, ECHO_A_Pin_source); 				
	  SYSCFG_EXTILineConfig(ECHO_B_Port_source, ECHO_B_Pin_source);				


	/* Configure EXTI line */
	  EXTI_InitStructure.EXTI_Mode 		= EXTI_Mode_Interrupt;
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  
		//ECHO A
			EXTI_InitStructure.EXTI_Line = ECHO_A_EXTI_Line;
			EXTI_Init(&EXTI_InitStructure);
			
		//ECHO B
			EXTI_InitStructure.EXTI_Line = ECHO_B_EXTI_Line;
			EXTI_Init(&EXTI_InitStructure);
		
	  
	/* Enable and set EXTI Interrupt to the lowest priority */
	  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x10;
	  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x10;
	  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	   
		//ECHO A  
			NVIC_InitStructure.NVIC_IRQChannel = ECHO_A_IRQChannel;
			NVIC_Init(&NVIC_InitStructure);
 	   
		//ECHO B
			NVIC_InitStructure.NVIC_IRQChannel = ECHO_B_IRQChannel;
			NVIC_Init(&NVIC_InitStructure);
}

void Setup_SONA_TIMER(void)
	{		
		Initialize_TIMER 						(SONA_TIM, SONA_TIM_Period,	SONA_TIM_Prescaler);
		Initialize_TIMER_INTERRUPT	(SONA_TIM, TIM_Channel_1 , SONA_TIM_CCR1);
		Initialize_TIMER_INTERRUPT	(SONA_TIM, TIM_Channel_2 , SONA_TIM_CCR1);
		TIM_Cmd(SONA_TIM, ENABLE);	
	}	
	
void Initialize_SONA (void)
{
	Setup_SONA_IO();
	Setup_SONA_TIMER();
// 	Sona_TurnOff();
}	


void SONA_TIMING_Channel_1 (void)			///1us for counting 
	{
		Sona_Count++;
	}

void SONA_TIMING_Channel_2 (void)			//
	{
 			GPIO_SetBits(TRIG_A_PORT, TRIG_A_PIN);
			GPIO_SetBits(TRIG_B_PORT, TRIG_B_PIN);
 			SONA_delay(10);		
 			GPIO_ResetBits(TRIG_A_PORT, TRIG_A_PIN);
			GPIO_ResetBits(TRIG_B_PORT, TRIG_B_PIN);
			Sona_Count =0;
	}

	
void ECHO_A_Interupt (void)
{
  if(EXTI_GetITStatus(ECHO_A_EXTI_Line) != RESET)
  {
		if (GPIO_ReadInputDataBit(ECHO_A_PORT, ECHO_A_PIN))
			{
				Sona_A.cnt = SONA_Get_tick();
			}
			else 
				{
					Sona_A.pulse_width 	= SONA_Get_tick() - Sona_A.cnt;					
					if (Sona_A.pulse_width)	Sona_A.cm = (float)Sona_A.pulse_width/58;
						DATA_OUT.Ultrasonic_A = (uint16_t) (Sona_A.cm *10);
				}
	EXTI_ClearITPendingBit(ECHO_A_EXTI_Line);
  }
}


void ECHO_B_Interupt (void)
{
  if(EXTI_GetITStatus(ECHO_B_EXTI_Line) != RESET)
  {
		if (GPIO_ReadInputDataBit(ECHO_B_PORT, ECHO_B_PIN))
			{
				Sona_B.cnt = SONA_Get_tick();
			}
			else 
				{
					Sona_B.pulse_width 	= SONA_Get_tick() - Sona_B.cnt;
					if (Sona_B.pulse_width)				Sona_B.cm = (float)Sona_B.pulse_width/58;	
						DATA_OUT.Ultrasonic_B = (uint16_t) (Sona_B.cm *10);
				}
	EXTI_ClearITPendingBit(ECHO_B_EXTI_Line);
  }
}

void Sona_TurnOff (void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	 
	  EXTI_InitStructure.EXTI_LineCmd = DISABLE;
	  
		//ECHO A
			EXTI_InitStructure.EXTI_Line = ECHO_A_EXTI_Line;
			EXTI_Init(&EXTI_InitStructure);
			
		//ECHO B
			EXTI_InitStructure.EXTI_Line = ECHO_B_EXTI_Line;
			EXTI_Init(&EXTI_InitStructure);
	
		TIM_Cmd(SONA_TIM, DISABLE);	
}


void Sona_TurnOn (void)
{
	EXTI_InitTypeDef   EXTI_InitStructure;
	
		EXTI_InitStructure.EXTI_Mode 		= EXTI_Mode_Interrupt;
	  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;  
	  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	  
		//ECHO A
			EXTI_InitStructure.EXTI_Line = ECHO_A_EXTI_Line;
			EXTI_Init(&EXTI_InitStructure);
			
		//ECHO B
			EXTI_InitStructure.EXTI_Line = ECHO_B_EXTI_Line;
			EXTI_Init(&EXTI_InitStructure);
	
		TIM_Cmd(SONA_TIM, ENABLE);	
}
