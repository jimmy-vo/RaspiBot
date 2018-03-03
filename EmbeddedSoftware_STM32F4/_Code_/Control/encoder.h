#include "BalaPi_BoardConf.h"

#define cnt_per_revolution		334
#define gear_ratio 						34
// #define In_Revolution					1336
#define In_Revolution					11356

void Initialize_Encoder (void);
void Encoders_Reset (void);
void Encoders_Read (void);

float Left_Speed(void);
float Right_Speed(void);
