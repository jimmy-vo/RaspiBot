#include "Motor.h"
#include "Dev_TIMER.h"

void Initialize_Motor (void)
	{
		GPIO_InitTypeDef										GPIO_InitStructure;
				
		/* clock enable */
		RCC_AHB1PeriphClockCmd	(MOTOR_GPIO_CLK, ENABLE);
				
		/* TIM configuration */
		GPIO_InitStructure.GPIO_Pin 		= MOTOR_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode 		= GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed 	= GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType 	= GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd 		= GPIO_PuPd_NOPULL ;
		GPIO_Init(MOTOR_GPIO_PORT, &GPIO_InitStructure);

		/* Connect TIM pins  */
		GPIO_PinAFConfig(MOTOR1_GPIO_PORT, MOTOR1_GPIO_source, MOTOR1_GPIO_AF);
		GPIO_PinAFConfig(MOTOR2_GPIO_PORT, MOTOR2_GPIO_source, MOTOR2_GPIO_AF);
		GPIO_PinAFConfig(MOTOR3_GPIO_PORT, MOTOR3_GPIO_source, MOTOR3_GPIO_AF);
		GPIO_PinAFConfig(MOTOR4_GPIO_PORT, MOTOR4_GPIO_source, MOTOR4_GPIO_AF);

				
		Initialize_TIMER			(MOTOR_TIM, MOTOR_Period, MOTOR_Prescaler);
		Initialize_TIMER_PWM	(MOTOR_TIM, TIM_Channel_1 | TIM_Channel_2 | TIM_Channel_3 | TIM_Channel_4 , MOTOR_CCR);
		
		TIM_Cmd(MOTOR_TIM , ENABLE);
	}
	
void L_Motor_Update(Motor_DIR DIR, uint16_t dutycycle)
	{		
#ifdef _USING_L298_
		if (DIR==Forward)
			{
				MOTOR1_TIM_update(MOTOR_TIM, dutycycle);  
				MOTOR2_TIM_update(MOTOR_TIM, 0);   
			}
			else 
				{
					MOTOR1_TIM_update(MOTOR_TIM, 0);  
					MOTOR2_TIM_update(MOTOR_TIM, dutycycle);   					
				}
#endif
	}
	
void R_Motor_Update(Motor_DIR DIR, uint16_t dutycycle)
	{		
#ifdef _USING_L298_
		if (DIR==Forward)
			{
				MOTOR3_TIM_update(MOTOR_TIM, dutycycle);  
				MOTOR4_TIM_update(MOTOR_TIM, 0);   
			}
			else 
				{
					MOTOR3_TIM_update(MOTOR_TIM, 0);  
					MOTOR4_TIM_update(MOTOR_TIM, dutycycle);   					
				}
#endif
	}
