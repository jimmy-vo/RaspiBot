#include "Systick.h"
#include "Dev_LCD.h"
#include <stdio.h>

__IO uint64_t TickCnt = 0;
uint64_t cnt;


void Initialize_SystemTick (void )
{
		SysTick_Config(168000000 / 1000); 
}

void SysTick_Handler()
{
    if (TickCnt) TickCnt--;
}



void delay_ms(uint32_t ms)
{
    TickCnt = ms;
    while(TickCnt);
}
