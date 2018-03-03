#include "BalaPi_BoardConf.h"




typedef enum 
{
	pressed=0,
	released=1,
}Button_State;


typedef struct 
{
	Button_State 		State;	
	uint64_t				PressTick;
	uint64_t				LastPressTime;
}Button;


void Initialize_DIGITAL(void);
extern Button 		Button_A,	Button_C,	Button_D,	Button_B, 	Button_OK;

void Request_LongPress	(Button *button, uint32_t presstime);
Button_State 	Get_Button_State	(Button *button);



void RGB_Process(int8_t brightness);
void RGB_Update (int8_t brightness, int8_t RED, int8_t GREEN, int8_t BLUE );
void RGB_SetLoop(int sw);

#define RGB_LOOP_ON() 								RGB_SetLoop(1)
#define RGB_LOOP_OFF() 								RGB_SetLoop(0) 
#define RGB_TOGGLE() 									RGB_SetLoop(3) 


