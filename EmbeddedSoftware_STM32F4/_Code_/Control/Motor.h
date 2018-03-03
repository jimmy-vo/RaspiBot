#include "BalaPi_BoardConf.h"

#define _USING_L298_




////5KHz
#define MOTOR_Prescaler								168					
#define MOTOR_Period									100/2								
#define MOTOR_CCR											0											//initial
		
#define MOTOR_MAX  										MOTOR_Period 			
#define MOTOR_MIN 		 								0					


typedef enum 
{
		Forward										= 1,
    Backward 			 						= 0
}Motor_DIR ;



void Initialize_Motor (void);
void L_Motor_Update(Motor_DIR DIR, uint16_t dutycycle);
void R_Motor_Update(Motor_DIR DIR, uint16_t dutycycle);
