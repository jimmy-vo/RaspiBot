#include "BalaPi_BoardConf.h"


#define HMC5883L_ADDRESS            0x1E // this device only has one address
#define HMC5883L_DEFAULT_ADDRESS    (HMC5883L_ADDRESS<<1)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 																																																												 ////
//// 																						 REGISTER  MAPPING																													 ////
//// 																																																												 ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef enum
{
	HMC58_R_CONFA		= 0,
	HMC58_R_CONFB		= 1,
	HMC58_R_MODE		= 2,

	HMC58_R_X_MSB		= 3,
	HMC58_R_X_LSB		= 4,
	HMC58_R_Y_MSB		= 7, 
	HMC58_R_Y_LSB		= 8,
	HMC58_R_Z_MSB		= 5,
	HMC58_R_Z_LSB		= 6, 

	HMC58_R_STATUS	= 9,
	HMC58_R_IDA			= 10,
	HMC58_R_IDB			= 11,
	HMC58_R_IDC			= 12,
	HMC5883L_Debug	= 13
}HMC58_Register;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 																																																												 ////
//// 																						 HMC58_R_CONFA																															 ////
//// 																																																												 ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	HMC5883L_SAMPLEAVERAGE_Position		5	
#define	HMC5883L_SAMPLEAVERAGE_clear			0x9F	// 2 bits
typedef enum 
{
	HMC5883L_SAMPLEAVERAGE_1        		= 0x00,
	HMC5883L_SAMPLEAVERAGE_2        		= 0x01,
	HMC5883L_SAMPLEAVERAGE_4        		= 0x02,
	HMC5883L_SAMPLEAVERAGE_8        		= 0x03,
	HMC5883L_SAMPLEAVERAGE_DontCare   	= 0x1FF//keep old value
} HMC5883L_SAMPLEAVERAGE;

#define HMC5883L_RATE_Position			2	
#define HMC5883L_RATE_clear					0xE3 // 3 bits
typedef enum 
{
	HMC5883L_RATE_0p75         = 0x00,
	HMC5883L_RATE_1p5          = 0x01,
	HMC5883L_RATE_3            = 0x02,
	HMC5883L_RATE_7p5          = 0x03,
	HMC5883L_RATE_15           = 0x04,
	HMC5883L_RATE_30           = 0x05,
	HMC5883L_RATE_75           = 0x06,
	HMC5883L_RATE_DontCare     = 0x1FF //keep old value
} HMC5883L_RATE;
	
#define HMC5883L_BIAS_Position			0	
#define HMC5883L_BIAS_clear					0xFC // 2 bits
typedef enum 
{
	HMC5883L_BIAS_NORMAL        = 0x00,
	HMC5883L_BIAS_POSITIVE      = 0x01,
	HMC5883L_BIAS_NEGATIVE      = 0x02,
	HMC5883L_BIAS_DontCare      = 0x1FF,//keep old value
} HMC5883L_BIAS;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 																																																												 ////
//// 																						 HMC58_R_CONFb																															 ////
//// 																																																												 ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define	HMC5883L_GAIN_Position		 5	
#define	HMC5883L_GAIN_clear				 0x1F // 3 bits	
typedef enum 
{																			// sensor field range // Gain LSb/Gause //
	HMC5883L_GAIN_1370          = 0x00, //	 		± 0.88 Ga			//			1370			//
	HMC5883L_GAIN_1090          = 0x01, //		 	± 1.3 Ga			// 1090 (default)	//
	HMC5883L_GAIN_820           = 0x02, //			± 1.9 Ga			//			820				//
	HMC5883L_GAIN_660           = 0x03, //			± 2.5 Ga			//			660				//
	HMC5883L_GAIN_440           = 0x04, //			± 4.0 Ga			//			440				//
	HMC5883L_GAIN_390           = 0x05, //			± 4.7 Ga			//			390				//
	HMC5883L_GAIN_330           = 0x06, //			± 5.6 Ga			//			330				//
	HMC5883L_GAIN_230           = 0x07, //			± 8.1 Ga		 	//			230				//
	HMC5883L_GAIN_DontCare     	= 0x1FF,//keep old value
} HMC5883L_GAIN;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																																																			 ////
////HMC58_R_MODE	
//
#define	HMC5883L_HIGHSPEED_Position		7	
#define	HMC5883L_HIGHSPEED_clear			0x7F	
typedef enum 
{
	HMC5883L_HIGHSPEED_ENABLE    = 0x01,
	HMC5883L_HIGHSPEED_DISABLE   = 0x00,
	HMC5883L_HIGHSPEED_DontCare  = 0x1FF,//keep old value
}HMC5883L_HIGHSPEED;

#define	HMC5883L_MODE_Position		 0	
#define	HMC5883L_MODE_clear				 0xFC	
typedef enum 
{
	HMC5883L_MODE_CONTINUOUS    = 0x00,
	HMC5883L_MODE_SINGLE        = 0x01,
	HMC5883L_MODE_IDLE          = 0x02,
	HMC5883L_MODE_DontCare     = 0x1FF,//keep old value
}HMC5883L_MODE;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																																																			 ////
////HMC58_R_STATUS	
//
#define	HMC5883L_STATUS_Position 	0 
#define	HMC5883L_STATUS_Position 	0 
typedef enum  // read only, bit 0 indicates Ready, bit 1 indicate Lock
{
	HMC5883L_STATUS_LOCK_BIT     = 0x2,
	HMC5883L_STATUS_READY_BIT    = 0x1,
}HMC5883L_STATUS;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////																																																			 ////
////USER	
//
typedef struct 
{
    int16_t x;
    int16_t y;
    int16_t z;
}Compass;


typedef enum 
{
    HMC5883L_STATUS_FAIL		 						= 0,
    HMC5883L_STATUS_OK 									= 1,
}HMC5883L_Status;

HMC5883L_Status  HMC5883L_Get_RawCompact(Compass *Compact);

HMC5883L_Status HMC5883L_Configuration_A(HMC5883L_SAMPLEAVERAGE  SAMPLEAVERAGE, HMC5883L_RATE RATE, HMC5883L_BIAS BIAS );
HMC5883L_Status HMC5883L_Configuration_B(HMC5883L_GAIN  GAIN );
HMC5883L_Status HMC5883L_Configuration_MODE(HMC5883L_HIGHSPEED  HIGHSPEED, HMC5883L_MODE MODE );

HMC5883L_Status HMC5883L_Set_SAMPLEAVERAGE(HMC5883L_SAMPLEAVERAGE SAMPLEAVERAGE) ;
HMC5883L_Status HMC5883L_Set_OutputRate(HMC5883L_RATE RATE) ;
HMC5883L_Status HMC5883L_Set_BIAS(HMC5883L_BIAS BIAS) ;
HMC5883L_Status HMC5883L_Set_Gain(HMC5883L_GAIN gain);
HMC5883L_Status HMC5883L_Set_Mode(HMC5883L_MODE MODE);
HMC5883L_Status HMC5883L_Set_HighSpeed(HMC5883L_HIGHSPEED HIGHSPEED);

HMC5883L_Status HMC5883L_GetDeviceID(uint8_t *id) ;
HMC5883L_Status HMC5883L_ConnectionCheck(void) ;
