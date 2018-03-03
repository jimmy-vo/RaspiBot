#include "BalaPi_BoardConf.h"
#include "INCLUDE.h"

uint8_t shutdown=0;
IMU_Status 		IMUst;
EEPROM_Status EEPROMst;
int main(void)
{
	Initialize_Timer();	
	Initialize_SerialLink();	
	Initialize_Volt_FB();
	Initialize_DIGITAL();
	Initialize_LCD();		
	Initialize_Motor();
	Initialize_Encoder();
	Initialize_Servo();
	Initialize_SONA();	
	Initialize_USART_2();
	IMUst = Initialize_IMU();
	EEPROMst = Initialize_EEPROM();
	
	if (IMUst == IMU_Status_OK) 					printf("IMU OK  "); 		
		else 																printf("IMU Fail"); 	
		
	
// 	while(Get_Button_State(&Button_OK) != pressed);
// 	LCD_Clear();
	
	while (shutdown==0)
		{	
//  		Serial_Link_OutProcess();
// 			Display_EEPROMtest();
			IMU_Process();
// 			Display_SONAtest();
// 			Display_RTC();
// 			Display_Motortest();
// 			Display_VoltFB();			
		}
	
	LCD_Clear();
		LCD_GotoXY(0,0);	printf("      BalaPi "); 	
		LCD_GotoXY(2,0);	printf("Shutting Down... "); 	
		Delay_ms(500);
	LCD_Clear();
}

