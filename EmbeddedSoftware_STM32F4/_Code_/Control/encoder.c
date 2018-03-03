#include "encoder.h"

//speeds
int32_t EnA_Speed;
int32_t EnB_Speed;


//distances
int32_t EnA_Distance;
int32_t EnB_Distance;


// counter
int32_t EnA_cnt_Old;
int32_t EnB_cnt_Old;
int32_t EnA_cnt_New;
int32_t EnB_cnt_New;


void Initialize_Encoder (void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	  // Timer peripheral clock enable
  RCC_APB1PeriphClockCmd (B_ENC_TIMER_CLK|A_ENC_TIMER_CLK, ENABLE);

  // turn on the clocks for each of the ports needed
  RCC_AHB1PeriphClockCmd (LA_ENC_GPIO_CLK | LB_ENC_GPIO_CLK | RA_ENC_GPIO_CLK | RB_ENC_GPIO_CLK, ENABLE);

  // GPIO Pin configure
		GPIO_StructInit (&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	
  GPIO_InitStructure.GPIO_Pin = LA_ENC_GPIO_PIN;
  GPIO_Init (LA_ENC_GPIO_PORT, &GPIO_InitStructure);
	
  GPIO_InitStructure.GPIO_Pin = LB_ENC_GPIO_PIN;
  GPIO_Init (LB_ENC_GPIO_PORT, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = RA_ENC_GPIO_PIN;
  GPIO_Init (RA_ENC_GPIO_PORT, &GPIO_InitStructure);
  
	GPIO_InitStructure.GPIO_Pin = RB_ENC_GPIO_PIN;
  GPIO_Init (RB_ENC_GPIO_PORT, &GPIO_InitStructure);

  // Connect the pins to Alternate Functions
  GPIO_PinAFConfig (LA_ENC_GPIO_PORT, LA_ENC_SOURCE, LA_ENC_AF);
  GPIO_PinAFConfig (LB_ENC_GPIO_PORT, LB_ENC_SOURCE, LB_ENC_AF);
  GPIO_PinAFConfig (RA_ENC_GPIO_PORT, RA_ENC_SOURCE, RA_ENC_AF);
  GPIO_PinAFConfig (RB_ENC_GPIO_PORT, RB_ENC_SOURCE, RB_ENC_AF);

  // rising polarity to let it use both edges
  TIM_EncoderInterfaceConfig (A_ENC_TIMER, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
  TIM_EncoderInterfaceConfig (B_ENC_TIMER, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);
	
  TIM_SetAutoreload (A_ENC_TIMER, 0xffff);
  TIM_SetAutoreload (B_ENC_TIMER, 0xffff);

  // turn on the timer/counters
  TIM_Cmd (A_ENC_TIMER, ENABLE);
  TIM_Cmd (B_ENC_TIMER, ENABLE);
  Encoders_Reset();
}


void Encoders_Reset (void)
{
  __disable_irq();
  EnA_cnt_Old 	= 0;
  EnB_cnt_Old 	= 0;
  EnA_Distance 	= 0;
  EnB_Distance 	= 0;
  TIM_SetCounter (A_ENC_TIMER, 0);
  TIM_SetCounter (B_ENC_TIMER, 0);
  Encoders_Read();
  __enable_irq();
}


void Encoders_Read (void)
{
  EnA_cnt_Old = EnA_cnt_New;																			
	EnB_cnt_Old = EnB_cnt_New;
		
  EnA_cnt_New = TIM_GetCounter (A_ENC_TIMER) ;								
	EnB_cnt_New = TIM_GetCounter (B_ENC_TIMER) ;
  
	EnA_Speed 	= EnA_cnt_New - EnA_cnt_Old;								
	EnB_Speed 	= EnB_cnt_New - EnB_cnt_Old;
   
	EnA_Distance += EnA_Speed;									
	EnB_Distance += EnB_Speed;
}	

float Left_Speed(void)
{
	float speed = (float)EnB_Speed*1000  / In_Revolution;
	return speed;
}

float Right_Speed(void)
{
	float speed = (float)EnA_Speed*1000 / In_Revolution;
	return speed;
}

