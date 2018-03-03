#include "Timing_Manage.h"
#include "User_Interface.h"
#include "Dev_TIMER.h"
#include "Dev_DIGITAL.h"
#include "Voltage_FB.h"
#include "Systick.h"


RCT LCD_Clock;
uint32_t capture = 0;
uint32_t ButtonTick;	
uint64_t I2C_Tick;				

void Initialize_Timer(void)
	{		
		Initialize_TIMER 						(TimingManage_TIM, TimingManage_Period,	TimingManage_Prescaler);
		Initialize_TIMER_INTERRUPT	(TimingManage_TIM, TIM_Channel_1 , TimingManage_CCR1);
		Initialize_TIMER_INTERRUPT	(TimingManage_TIM, TIM_Channel_2 , TimingManage_CCR2);
		Initialize_TIMER_INTERRUPT	(TimingManage_TIM, TIM_Channel_3 , TimingManage_CCR3);
		
		TIM_Cmd(TimingManage_TIM, ENABLE);	
	}	
	
void Increase_RTC (void)
	{
		if (LCD_Clock.enable)
				{
						if (LCD_Clock.second  < 59) 
							{
								LCD_Clock.second ++;
							}
						else if (LCD_Clock.minute  < 59) 
							{
								LCD_Clock.second =0;
								LCD_Clock.minute ++;	
							}
						else if (LCD_Clock.hour  < 59) 
							{
								LCD_Clock.second =0;
								LCD_Clock.minute =0;	
								LCD_Clock.hour ++;
							} 
							else 
							{
								
								LCD_Clock.second =0;
								LCD_Clock.minute =0;	
								LCD_Clock.hour =0;
							}
				}
	}	

void Increase_ButtonTick (void)
	{
			if (ButtonTick++>=300000)	ButtonTick=0;
	}
	
void 	Increase_I2CTick(void)
	{	
			I2C_Tick++;
	}
	
	///////////////////////////////////////////////////////
 //											EXPORT										   	//
///////////////////////////////////////////////////////	
uint64_t I2C_GetTick(void)
{
    return I2C_Tick;
}

	///////////////////////////////////////////////////////
 //											Tim8											   	//
///////////////////////////////////////////////////////	
void Timing_Channel_1 (void)
	{
			Increase_RTC();
	}
	
void Timing_Channel_2 (void)
	{
			Increase_ButtonTick();
			Increase_I2CTick();
	} 

void Timing_Channel_3 (void)
	{
			RGB_Process(20);
			//Battery_Check();
	}
