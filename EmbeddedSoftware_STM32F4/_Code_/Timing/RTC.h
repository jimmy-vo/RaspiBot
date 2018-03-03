#include "BalaPi_BoardConf.h"

#define TimingManage_TIM														TIM8
#define TimingManage_CLK														RCC_APB2Periph_TIM8

#define TimingManage_Period													0xffff 				//28000
#define TimingManage_Prescaler												16800

////////////////////////////////////////////////////////////////////////////////

#define TimingManage_CCR1													10000						//1s	
#define TimingManage_CCR2													10							//1ms
				#define FREQ_100kHz 10
				#define FREQ_30kHz  33
				#define FREQ_20kHz  50
				#define FREQ_10kHz  100
				#define FREQ_1kHz   1000
				#define FREQ_500Hz  2000
				#define FREQ_200Hz  5000

				#define FREQ_100Hz  10000
				#define FREQ_75Hz   15000
				#define FREQ_50Hz   20000
				#define FREQ_20Hz   50000
				#define FREQ_10Hz   100000
				#define FREQ_1Hz		1000000
#define TimingManage_CCR3													500							//50ms
////////////////////////////////////////////////////////////////////////////////

typedef struct {
								uint8_t 	enable ;		//1:enable 	0:disable
								uint8_t 	edit ;			//0: off 	1: hour		2:minite	3: second		
								int8_t 	day ;
								int8_t 	month ;
								int8_t 	year ;
								int8_t 	hour;
								int8_t 	minute;
								int8_t 	second;
								int16_t	milisecond;
								}	RCT;	
								

extern uint32_t ButtonTick;
extern RCT LCD_Clock;

void Initialize_Timer(void);
void Timing_Channel_1 (void);
void Timing_Channel_2 (void);
void Timing_Channel_3 (void);

uint64_t I2C_GetTick(void);
uint32_t Get_ButtonTick (void);


