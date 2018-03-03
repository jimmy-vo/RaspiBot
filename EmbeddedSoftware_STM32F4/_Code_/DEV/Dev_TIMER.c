#include "Dev_TIMER.h"
#include "Timing_Manage.h"
#include "Sona.h"

#define CRR_default 		0

uint16_t Capture_1, Capture_2, Capture_3, Capture_4, Capture_5, Capture_6, Capture_7, Capture_8, Capture_9;

void Initialize_TIMER (TIM_TypeDef* TIMx, uint16_t PERIOD, uint16_t Prescaler)
{
	TIM_TimeBaseInitTypeDef 							TIM_TimeBaseStructure;
	
	if 			(TIMx == TIM1 )		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1 , ENABLE);
	else if (TIMx == TIM2 )		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
	else if (TIMx == TIM3 )		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 , ENABLE);
	else if (TIMx == TIM4 )		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4 , ENABLE);
	else if (TIMx == TIM5 )		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5 , ENABLE);
	else if (TIMx == TIM6 )		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6 , ENABLE);
	else if (TIMx == TIM7 )		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7 , ENABLE);
	else if (TIMx == TIM8 )		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 , ENABLE);
	else if (TIMx == TIM9 )		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9 , ENABLE);
	else if (TIMx == TIM10)		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM10, ENABLE);
	else if (TIMx == TIM11)		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM11, ENABLE);
	else if (TIMx == TIM12)		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM12, ENABLE);
	else if (TIMx == TIM13)		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	else if (TIMx == TIM14)		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM14, ENABLE);

	/* Time base configuration */		
	TIM_TimeBaseStructure.TIM_Period				 = PERIOD-1;
	TIM_TimeBaseStructure.TIM_Prescaler			 = Prescaler-1;
	TIM_TimeBaseStructure.TIM_ClockDivision	 = 0;
	TIM_TimeBaseStructure.TIM_CounterMode 	 = TIM_CounterMode_Up;

	TIM_TimeBaseInit(TIMx, &TIM_TimeBaseStructure);
}


void Initialize_TIMER_PWM( TIM_TypeDef* TIMx, uint16_t channel ,uint16_t CRR)
{
	
	TIM_OCInitTypeDef											TIM_OCInitStructure;	
	TIM_BDTRInitTypeDef 									TIM_BDTRInitStruct;
	
	TIM_OCStructInit(&TIM_OCInitStructure);
	TIM_OCInitStructure.TIM_OCMode 				=	 	TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState		= 	TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OutputNState 	= 	TIM_OutputNState_Disable;
	TIM_OCInitStructure.TIM_Pulse					= 	CRR;
	TIM_OCInitStructure.TIM_OCPolarity  	= 	TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OCNPolarity  	= 	TIM_OCNPolarity_Low;
	TIM_OCInitStructure.TIM_OCIdleState 	=		TIM_OCIdleState_Reset;
	TIM_OCInitStructure.TIM_OCNIdleState	=		TIM_OCNIdleState_Reset;
		
	if ((channel & TIM_Channel_1)==TIM_Channel_1)  	TIM_OC1Init(TIMx, &TIM_OCInitStructure);
	if ((channel & TIM_Channel_2)==TIM_Channel_2) 	TIM_OC2Init(TIMx, &TIM_OCInitStructure);
	if ((channel & TIM_Channel_3)==TIM_Channel_3) 	TIM_OC3Init(TIMx, &TIM_OCInitStructure);
	if ((channel & TIM_Channel_4)==TIM_Channel_4) 	TIM_OC4Init(TIMx, &TIM_OCInitStructure);
	
	
	if ((TIMx == TIM1 ) || (TIMx == TIM8 ))						//required for timers 1 or 8
		{
			TIM_BDTRStructInit(&TIM_BDTRInitStruct);
			TIM_BDTRInitStruct.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
			TIM_BDTRInitStruct.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;
			TIM_BDTRInitStruct.TIM_OSSIState = TIM_OSSIState_Enable;
			TIM_BDTRConfig(TIMx, &TIM_BDTRInitStruct);
		}
	
}

void Initialize_TIMER_INTERRUPT( TIM_TypeDef* TIMx, uint16_t TIM_Channel ,uint16_t CRR)
{
	
		NVIC_InitTypeDef 											NVIC_InitStructure;
		TIM_OCInitTypeDef											TIM_OCInitStructure;
	
		IRQn_Type ITHandler;		
			
		if 			(TIMx == TIM1 )		ITHandler = TIM1_CC_IRQn;
		else if (TIMx == TIM2 )		ITHandler = TIM2_IRQn;
		else if (TIMx == TIM3 )		ITHandler = TIM3_IRQn;
		else if (TIMx == TIM4 )		ITHandler = TIM4_IRQn;
		else if (TIMx == TIM5 )		ITHandler = TIM5_IRQn;
		else if (TIMx == TIM6 )		ITHandler = TIM6_DAC_IRQn;
		else if (TIMx == TIM7 )		ITHandler = TIM7_IRQn;
		else if (TIMx == TIM8 )		ITHandler = TIM8_CC_IRQn;
		else if (TIMx == TIM8 )		ITHandler = TIM8_CC_IRQn;
		else if (TIMx == TIM9 )		ITHandler = TIM1_BRK_TIM9_IRQn;
			/* Enable the TIM gloabal Interrupt */
		NVIC_InitStructure.NVIC_IRQChannel =  ITHandler	;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
				
		TIM_OCInitStructure.TIM_OCMode 				= TIM_OCMode_Timing;
		TIM_OCInitStructure.TIM_OutputState 	= TIM_OutputState_Enable;
		TIM_OCInitStructure.TIM_OCPolarity 		= TIM_OCPolarity_High;
		TIM_OCInitStructure.TIM_Pulse 				= CRR;
		
				
		if ((TIM_Channel & TIM_Channel_1)==TIM_Channel_1)  			/* Output Compare Timing Mode configuration: Channel1 */
			{
				TIM_OC1Init(TIMx, &TIM_OCInitStructure);
				TIM_ITConfig(TIMx, TIM_IT_CC1 , ENABLE);
			}		
		if ((TIM_Channel & TIM_Channel_2)==TIM_Channel_2) 			/* Output Compare Timing Mode configuration: Channel2 */
			{
				TIM_OC2Init(TIMx, &TIM_OCInitStructure);
				TIM_ITConfig(TIMx, TIM_IT_CC2 , ENABLE);
			}
		
		if ((TIM_Channel & TIM_Channel_3)==TIM_Channel_3) 			/* Output Compare Timing Mode configuration: Channel3 */
			{
				TIM_OC3Init(TIMx, &TIM_OCInitStructure);
				TIM_ITConfig(TIMx, TIM_IT_CC3 , ENABLE);
			}
		if ((TIM_Channel & TIM_Channel_4)==TIM_Channel_4) 			/* Output Compare Timing Mode configuration: Channel4 */
			{
				TIM_OC4Init(TIMx, &TIM_OCInitStructure);
				TIM_ITConfig(TIMx, TIM_IT_CC4 , ENABLE);
			}	
}




void TIM1_CC_IRQHandler(void)
{
	
}

void TIM2_IRQHandler(void)
{

}

void TIM3_IRQHandler(void)
{
	
}

void TIM4_IRQHandler(void)
{
	
}

void TIM5_IRQHandler(void)
{
	
}

void TIM6_DAC_IRQHandler(void)
{
	
}

void TIM7_IRQHandler(void)
{
	
}
void TIM8_CC_IRQHandler(void)
{
		if (TIM_GetITStatus(TIM8, TIM_IT_CC1) != RESET)			
			{
				TIM_ClearITPendingBit(TIM8, TIM_IT_CC1);
				////////////////////////////////////////////////	
					Timing_Channel_1();
				////////////////////////////////////////////////	
					Capture_8 = TIM_GetCapture1(TIM8);
				TIM_SetCompare1(TIM8, Capture_8 + TimingManage_CCR1);
			}

		if (TIM_GetITStatus(TIM8, TIM_IT_CC2) != RESET)
			{
				TIM_ClearITPendingBit(TIM8, TIM_IT_CC2);
				////////////////////////////////////////////////	
					Timing_Channel_2();
				////////////////////////////////////////////////	
				Capture_8 = TIM_GetCapture2(TIM8);
				TIM_SetCompare2(TIM8, Capture_8 + TimingManage_CCR2);
			}
			
		if (TIM_GetITStatus(TIM8, TIM_IT_CC3) != RESET)
			{
				TIM_ClearITPendingBit(TIM8, TIM_IT_CC3);
				////////////////////////////////////////////////	
					Timing_Channel_3();
				////////////////////////////////////////////////	
				Capture_8 = TIM_GetCapture3(TIM8);
				TIM_SetCompare3(TIM8, Capture_8 + TimingManage_CCR3);
			}
}

void TIM1_BRK_TIM9_IRQHandler(void)
{
	if (TIM_GetITStatus(SONA_TIM, TIM_IT_CC1) != RESET)			
		{
			TIM_ClearITPendingBit(SONA_TIM, TIM_IT_CC1);
			////////////////////////////////////////////////	
				SONA_TIMING_Channel_1();
			////////////////////////////////////////////////				
				Capture_9 = TIM_GetCapture1(SONA_TIM);
			TIM_SetCompare1(SONA_TIM, Capture_9 + SONA_TIM_CCR1);
		}

	if (TIM_GetITStatus(SONA_TIM, TIM_IT_CC2) != RESET)
		{
			TIM_ClearITPendingBit(SONA_TIM, TIM_IT_CC2);
			////////////////////////////////////////////////	
				SONA_TIMING_Channel_2();
			////////////////////////////////////////////////	
			Capture_9 = TIM_GetCapture2(SONA_TIM);
			TIM_SetCompare2(SONA_TIM, Capture_9 + SONA_TIM_CCR2);
		}
}
