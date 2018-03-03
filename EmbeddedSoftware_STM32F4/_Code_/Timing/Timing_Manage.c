#include "Timing_Manage.h"
#include "User_Interface.h"
#include "Dev_TIMER.h"
#include "Dev_DIGITAL.h"
#include "Voltage_FB.h"
#include "Systick.h"
#include "encoder.h"

RCT LCD_Clock;
uint32_t capture = 0;
uint32_t ms_Tick;			

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
void Delay_ms (uint32_t ms)
{
	uint64_t CurTick = Get_msTick();
	while(Check_msTick(CurTick,ms)==0);
}
	
	
uint64_t Get_msTick(void)
{
    return ms_Tick;
}

uint8_t Check_msTick(uint64_t TickBase, uint64_t Time)
{
    uint64_t CurTick;
    
    CurTick = Get_msTick();
    
    if (CurTick > TickBase)
    {
        if (CurTick >= (TickBase + Time))
        {
            return 1;
        }
    }
    else
    {
        if (CurTick >= ((uint64_t)(TickBase + Time)))
        {
            return 1;
        }
    }
    return 0;
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
			ms_Tick++;
			Encoders_Read();
	} 

void Timing_Channel_3 (void)
	{
		if (Battery_Check())			RGB_Process(20);
		else 											
			{
				RGB_LOOP_OFF(); 
				shutdown=1;
			}
	}
