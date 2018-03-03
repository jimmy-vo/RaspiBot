#include "BalaPi_BoardConf.h"

#define Servo_Prescaler								0			//SystemCoreClock/5000000			 // pers 33.6
#define Servo_Period									16800				//31600 									// 20ms for servo period	
#define Servo_CCR											Servo_Period/2																	//initial
		
#define Servo_MAX  										Servo_Period 				//1580
#define Servo_MIN 		 								0								//3160
// 2370 = 1.5ms - for Servo

void Initialize_Servo (void);
void ServoX_Update(uint16_t dutycycle);
void ServoY_Update(uint16_t dutycycle);
