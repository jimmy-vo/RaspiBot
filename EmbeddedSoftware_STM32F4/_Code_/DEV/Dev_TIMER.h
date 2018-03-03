#include "BalaPi_BoardConf.h"

void Initialize_TIMER (TIM_TypeDef* TIMx, uint16_t PERIOD, uint16_t Prescaler);
void Initialize_TIMER_INTERRUPT( TIM_TypeDef* TIMx, uint16_t channel ,uint16_t CRR);
void Initialize_TIMER_PWM( TIM_TypeDef* TIMx, uint16_t channel ,uint16_t CRR);
