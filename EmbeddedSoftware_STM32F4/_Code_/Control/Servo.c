#include "Servo.h"
#include "Dev_TIMER.h"

void Initialize_Servo (void)
	{
		GPIO_InitTypeDef GPIO_InitStructure;
		
    GPIO_StructInit(&GPIO_InitStructure);
		
		
		/* clock enable */
		RCC_AHB1PeriphClockCmd(SERVOX_GPIO_CLK | SERVOY_GPIO_CLK, ENABLE);
		
		/* TIM configuration */
		GPIO_InitStructure.GPIO_Pin = SERVO_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
		GPIO_Init(SERVO_GPIO_PORT, &GPIO_InitStructure);

		/* Connect TIM pins  */
		GPIO_PinAFConfig(SERVOX_GPIO_PORT, SERVOX_GPIO_source, SERVOX_GPIO_AF);
		GPIO_PinAFConfig(SERVOY_GPIO_PORT, SERVOY_GPIO_source, SERVOY_GPIO_AF);


		Initialize_TIMER			(SERVO_TIM, Servo_Period, Servo_Prescaler);
		Initialize_TIMER_PWM	(SERVO_TIM, TIM_Channel_1 | TIM_Channel_2, Servo_CCR);
		
		
		TIM_Cmd(SERVO_TIM , ENABLE);
	}
	
void ServoX_Update(uint16_t dutycycle)
	{
		SERVOX_TIM_update(SERVOX_TIM, dutycycle);    // 2370 = 1.5ms - for Servo
	}
	
void ServoY_Update(uint16_t dutycycle)
	{
		SERVOY_TIM_update(SERVOY_TIM, dutycycle);    // 2370 = 1.5ms - for Servo
	}
