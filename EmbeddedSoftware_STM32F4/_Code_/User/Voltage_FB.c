#include "Voltage_FB.h"
#include "Timing_Manage.h"

uint16_t ADC_Volt[VoltFB_buffsize];
uint16_t Battery_Cell=2;


void Initialize_Volt_FB(void)
	{
		ADC_InitTypeDef      				 	ADC_InitStructure;
		ADC_CommonInitTypeDef 				ADC_CommonInitStructure;
		DMA_InitTypeDef       				DMA_InitStructure;
		GPIO_InitTypeDef      				GPIO_InitStructure;
		
		/* Enable ADC, DMA and GPIO clocks ****************************************/
		RCC_AHB1PeriphClockCmd(VoltFB_DMA_CLK | VoltFB_GPIO_CLK, ENABLE);
		RCC_APB2PeriphClockCmd(VoltFB_ADC_CLK, ENABLE);

		ADC_DeInit();	
		
		/* DMA2 Stream0 channel0 configuration **************************************/
		DMA_DeInit(VoltFB_DMA_STREAM);

		DMA_InitStructure.DMA_Channel 								= VoltFB_DMA_CHANNEL;  
		DMA_InitStructure.DMA_PeripheralBaseAddr 			= VoltFB_ADC_ADDRESS;
		DMA_InitStructure.DMA_Memory0BaseAddr 				= (uint32_t)&ADC_Volt;
		DMA_InitStructure.DMA_DIR 										= DMA_DIR_PeripheralToMemory;
		DMA_InitStructure.DMA_BufferSize 							= VoltFB_buffsize;
		DMA_InitStructure.DMA_PeripheralInc 					= DMA_PeripheralInc_Disable;
		DMA_InitStructure.DMA_MemoryInc 							= DMA_MemoryInc_Enable;
		DMA_InitStructure.DMA_PeripheralDataSize 			= DMA_PeripheralDataSize_HalfWord;
		DMA_InitStructure.DMA_MemoryDataSize 					= DMA_MemoryDataSize_HalfWord;
		DMA_InitStructure.DMA_Mode 										= DMA_Mode_Circular;
		DMA_InitStructure.DMA_Priority 								= DMA_Priority_High;
		DMA_InitStructure.DMA_FIFOMode 								= DMA_FIFOMode_Disable;         
		DMA_InitStructure.DMA_FIFOThreshold 					= DMA_FIFOThreshold_HalfFull;
		DMA_InitStructure.DMA_MemoryBurst 						= DMA_MemoryBurst_Single;
		DMA_InitStructure.DMA_PeripheralBurst 				= DMA_PeripheralBurst_Single;

		DMA_Init(VoltFB_DMA_STREAM, &DMA_InitStructure);
		DMA_Cmd(VoltFB_DMA_STREAM, ENABLE);

		/* Configure ADC pin as analog input ******************************/
		GPIO_InitStructure.GPIO_Pin = VoltFB_GPIO_PIN;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
		GPIO_Init(VoltFB_GPIO_PORT, &GPIO_InitStructure);

		/* ADC Common Init **********************************************************/
		ADC_CommonInitStructure.ADC_Mode 							=	 ADC_Mode_Independent;
		ADC_CommonInitStructure.ADC_Prescaler 				=	 ADC_Prescaler_Div2;
		ADC_CommonInitStructure.ADC_DMAAccessMode 		=	 ADC_DMAAccessMode_Disabled;
		ADC_CommonInitStructure.ADC_TwoSamplingDelay 	=	 ADC_TwoSamplingDelay_5Cycles;
		ADC_CommonInit(&ADC_CommonInitStructure);

		/* ADC Init ****************************************************************/
		ADC_InitStructure.ADC_Resolution 							=	 ADC_Resolution_12b;
		ADC_InitStructure.ADC_ScanConvMode					 	=	 DISABLE;
		ADC_InitStructure.ADC_ContinuousConvMode 			=	 ENABLE;
		ADC_InitStructure.ADC_ExternalTrigConvEdge 		=	 ADC_ExternalTrigConvEdge_None;
		ADC_InitStructure.ADC_DataAlign 							=	 ADC_DataAlign_Right;
		ADC_InitStructure.ADC_NbrOfConversion 				=	 VoltFB_buffsize;
		ADC_Init(VoltFB_ADC, &ADC_InitStructure);

		/* ADC3 regular channel2 configuration *************************************/
		ADC_RegularChannelConfig(VoltFB_ADC, VoltFB_ADC_CHANNEL, 1, ADC_SampleTime_3Cycles);

		/* Enable DMA request after last transfer (Single-ADC mode) */
		ADC_DMARequestAfterLastTransferCmd(VoltFB_ADC, ENABLE);

		/* Enable ADC DMA */
		ADC_DMACmd(VoltFB_ADC, ENABLE);

		/* Enable ADC */
		ADC_Cmd(VoltFB_ADC, ENABLE);
		
		ADC_SoftwareStartConv(VoltFB_ADC);
		Delay_ms(200);
		Cell_Detect();
	}



float Get_Voltage(void)
	{			
		uint16_t 	i=0;
		uint16_t	ADCVAl=0;
		float Volt;
		
		while (i<VoltFB_buffsize)
					{
							ADCVAl = ADCVAl + (ADC_Volt[i++] );
					}
		ADCVAl = ADCVAl/ VoltFB_buffsize ;
		Volt =  ((float)ADCVAl) * ADCtoVolt * VF_SlopeScale + VF_SlopeShift;
		
		return Volt;
	}
	
uint16_t CELL;
uint16_t Cell_Detect(void)
	{
		float New_VOLTAGE=0;
		int Loop=10;			// limit cell detect loop
		
		CELL=0;
		while (New_VOLTAGE==0) New_VOLTAGE = Get_Voltage();
		if (New_VOLTAGE<=5) 	
				return 0;
			else					
				while ((New_VOLTAGE>max_cell*CELL || New_VOLTAGE<min_cell*CELL) && Loop-- )
					{
						if 			(New_VOLTAGE>max_cell*(float)CELL)										CELL++;
						else if (New_VOLTAGE<min_cell*(float)CELL) 										CELL--;
						else																													Loop=0;
					}
		return CELL;
	}

uint8_t Battery_Check(void)
	{
		float New_VOLTAGE = Get_Voltage();
		if ((CELL!=0)&&(New_VOLTAGE<=3.3*(float)CELL)) 				return 0;
			else 																								return 1;
	}
