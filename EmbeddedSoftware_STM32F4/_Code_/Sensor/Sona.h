#include "BalaPi_BoardConf.h"

#define SONA_TIM								TIM9

#define SONA_TIM_Period					0xffff
#define SONA_TIM_Prescaler			168

#define SONA_TIM_CCR1						10							//10us
#define SONA_TIM_CCR2						10000*5					//50ms		sona cycle in ms

typedef struct 
{				
	uint32_t						cnt;	
	uint32_t						pulse_width;
	float								cm;
}SONA;



void Initialize_SONA(void);

void SONA_TIMING_Channel_1 (void)	;
void SONA_TIMING_Channel_2 (void)	;
void Sona_TurnOff (void);
void Sona_TurnOn (void);

extern SONA Sona_A, Sona_B;

