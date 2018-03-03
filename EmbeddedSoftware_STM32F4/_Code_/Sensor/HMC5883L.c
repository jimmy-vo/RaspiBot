#include "HMC5883L.h"
#include "Dev_I2C.h"
#include "Serial_Link.h"

float x_scale,y_scale,z_scale,x_max,y_max,z_max;

uint8_t HMC5883L_RegDebug [HMC5883L_Debug];

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////I2C	Driver
//
static I2C_Status I2C_RegWrite (HMC58_Register Register, uint8_t *buffer)
{	
	uint8_t reg = Register;	
	
	I2C_Request(&IMU,	I2C_Phase_TX,  HMC5883L_DEFAULT_ADDRESS, HMC5883L_DEFAULT_ADDRESS, &reg, 1, buffer, 1);	
	return I2C_BUSY_Check (&IMU);
}

static I2C_Status I2C_Read (HMC58_Register Register, uint8_t *buffer, uint32_t length)
{
	uint8_t reg =  Register;	
	
	I2C_Request(&IMU,	I2C_Phase_RX, HMC5883L_DEFAULT_ADDRESS, HMC5883L_DEFAULT_ADDRESS, &reg, 1, buffer, length);		
	return I2C_BUSY_Check (&IMU);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////CONFIGURATION REGISTER																																																											 ////
//
HMC5883L_Status HMC5883L_Configuration_A(HMC5883L_SAMPLEAVERAGE  SAMPLEAVERAGE, HMC5883L_RATE RATE, HMC5883L_BIAS BIAS )
{	
	uint8_t check, newconfig = 0x00, mask = 0x00;	
	
	if(I2C_Read(HMC58_R_CONFA,&newconfig,1) != I2C_STATUS_OK)				return HMC5883L_STATUS_FAIL;	
	
	// SAMPLEAVERAGE
	if (SAMPLEAVERAGE != HMC5883L_SAMPLEAVERAGE_DontCare) 		
	{
		newconfig &= HMC5883L_SAMPLEAVERAGE_clear;
		newconfig |= (SAMPLEAVERAGE<<HMC5883L_SAMPLEAVERAGE_Position&0xFF);
		mask |= ~ HMC5883L_SAMPLEAVERAGE_clear;
	}
	
	//RATE
	if (RATE != HMC5883L_RATE_DontCare) 											
	{
		newconfig &= HMC5883L_RATE_clear;
		newconfig |= (RATE<<HMC5883L_RATE_Position&0xFF);
		mask |= ~ HMC5883L_RATE_clear;
	}
	
	//BIAS
	if (BIAS != HMC5883L_BIAS_DontCare) 					
	{
		newconfig &= HMC5883L_BIAS_clear;
		newconfig |= (BIAS<<HMC5883L_BIAS_Position&0xFF);
		mask |= ~ HMC5883L_BIAS_clear;
	}
		
	if(I2C_RegWrite	(HMC58_R_CONFA,&newconfig) != I2C_STATUS_OK)			return HMC5883L_STATUS_FAIL;     
	
	if(I2C_Read			(HMC58_R_CONFA,&check,1) != I2C_STATUS_OK)				return HMC5883L_STATUS_FAIL; 
	
	HMC5883L_RegDebug[HMC58_R_CONFA] = check;
	
	if ((check&mask)!=newconfig)																			return HMC5883L_STATUS_FAIL;
	else 																															return HMC5883L_STATUS_OK; 		
}

HMC5883L_Status HMC5883L_Configuration_B(HMC5883L_GAIN  GAIN )
{	
	uint8_t check, newconfig = 0x00, mask = 0x00;	
	
	if(I2C_Read(HMC58_R_CONFB,&newconfig,1) != I2C_STATUS_OK)					return HMC5883L_STATUS_FAIL;	
	
	// GAIN
	if (GAIN != HMC5883L_GAIN_DontCare) 									
	{
		newconfig &= HMC5883L_GAIN_clear;
		newconfig |= (GAIN<<HMC5883L_GAIN_Position&0xFF);
		mask |= ~ HMC5883L_GAIN_clear;
	}
		
	if(I2C_RegWrite	(HMC58_R_CONFB,&newconfig) != I2C_STATUS_OK)			return HMC5883L_STATUS_FAIL;     
	if(I2C_Read			(HMC58_R_CONFB,&check,1) != I2C_STATUS_OK)				return HMC5883L_STATUS_FAIL; 
	
	HMC5883L_RegDebug[HMC58_R_CONFB] = check;
	   
	if ((check&mask)!=newconfig)																			return HMC5883L_STATUS_FAIL;
	else 																															return HMC5883L_STATUS_OK; 		
}

HMC5883L_Status HMC5883L_Configuration_MODE(HMC5883L_HIGHSPEED  HIGHSPEED, HMC5883L_MODE MODE )
{	
	uint8_t check, newconfig = 0x00, mask = 0x00;	
	
	if(I2C_Read(HMC58_R_MODE,&newconfig,1) != I2C_STATUS_OK)					return HMC5883L_STATUS_FAIL;	
	
	// prepare for config
	if (HIGHSPEED != HMC5883L_HIGHSPEED_DontCare) 					
	{
		newconfig &= HMC5883L_HIGHSPEED_clear;
		newconfig |= (HIGHSPEED<<HMC5883L_HIGHSPEED_Position&0xFF);
		mask |= ~ HMC5883L_HIGHSPEED_clear;
	}
	if (MODE != HMC5883L_MODE_DontCare) 						
	{
		newconfig &= HMC5883L_MODE_clear;
		newconfig |= (MODE<<HMC5883L_MODE_Position&0xFF);
		mask |= ~ HMC5883L_MODE_clear;
	}
		
	if(I2C_RegWrite	(HMC58_R_MODE,&newconfig) != I2C_STATUS_OK)				return HMC5883L_STATUS_FAIL;     
	if(I2C_Read			(HMC58_R_MODE,&check,1) != I2C_STATUS_OK)					return HMC5883L_STATUS_FAIL; 
	
	HMC5883L_RegDebug[HMC58_R_MODE] = check;
	   
	if ((check&mask)!=newconfig)																			return HMC5883L_STATUS_FAIL;
	else 																															return HMC5883L_STATUS_OK; 		
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 																																																												 ////
//// 																							INDIVIDUAL CONFIGURATION 																									 ////
//// 																																																												 ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HMC5883L_Status HMC5883L_Set_OutputRate(HMC5883L_RATE RATE) 
{
	return HMC5883L_Configuration_A(HMC5883L_SAMPLEAVERAGE_DontCare, RATE, HMC5883L_BIAS_DontCare);	    			
}

HMC5883L_Status HMC5883L_Set_SAMPLEAVERAGE(HMC5883L_SAMPLEAVERAGE SAMPLEAVERAGE) 
{
	return HMC5883L_Configuration_A(SAMPLEAVERAGE, HMC5883L_RATE_DontCare, HMC5883L_BIAS_DontCare);	    			
}

HMC5883L_Status HMC5883L_Set_BIAS(HMC5883L_BIAS BIAS) 
{
	return HMC5883L_Configuration_A(HMC5883L_SAMPLEAVERAGE_DontCare, HMC5883L_RATE_DontCare, BIAS);	    			
}

HMC5883L_Status HMC5883L_Set_Gain(HMC5883L_GAIN gain) //if (gain > 7)
{
	return 	HMC5883L_Configuration_B(gain);
}

HMC5883L_Status HMC5883L_Set_Mode(HMC5883L_MODE MODE)
{
    return HMC5883L_Configuration_MODE(HMC5883L_HIGHSPEED_DontCare, MODE);
}
HMC5883L_Status HMC5883L_Set_HighSpeed(HMC5883L_HIGHSPEED HIGHSPEED)
{
    return HMC5883L_Configuration_MODE(HIGHSPEED , HMC5883L_MODE_DontCare);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// 																																																												 ////
//// 																										USER READ			 																											 ////
//// 																																																												 ////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////Individual    Register//////////////////////////////////////////////////////////////////////////////
HMC5883L_Status HMC5883L_GetDeviceID(uint8_t *id) 
{
    if(I2C_Read(	HMC58_R_IDA,id,3)!= I2C_STATUS_OK) 
				return HMC5883L_STATUS_FAIL;
			else
				return HMC5883L_STATUS_OK;
}

HMC5883L_Status HMC5883L_ConnectionCheck(void) 
{  
      uint8_t data[3];
     if(HMC5883L_GetDeviceID (data) == HMC5883L_STATUS_OK)
			{
				if((data[0] == 'H' && data[1] == '4' && data[2] == '3'))
						return HMC5883L_STATUS_OK;
					else
							return HMC5883L_STATUS_FAIL;
			}	
			else 	return HMC5883L_STATUS_FAIL;
}

HMC5883L_Status  HMC5883L_Get_RawCompact(Compass *Compact)
{	
    uint8_t data[6];
        
    if(I2C_Read(HMC58_R_X_MSB,data,6) == I2C_STATUS_OK) 
			{			
        Compact->x = DATA_OUT.Compx = (((uint16_t)data[0]) 	<< 8) | data[1];
        Compact->y = DATA_OUT.Compy =(((uint16_t)data[2]) 	<< 8) | data[3];
        Compact->z = DATA_OUT.Compz =(((uint16_t)data[4]) 	<< 8) | data[5];
				
				return HMC5883L_STATUS_OK;
			}
			else 
				return HMC5883L_STATUS_FAIL; 
}

/*
    Calibrate which has a few weaknesses.
    1. Uses wrong gain for first reading.
    2. Uses max instead of max of average when normalizing the axis to one another.
    3. Doesn't use neg bias. (possible improvement in measurement).
*/
// HMC5883L_Status HMC5883L_Calibrate_weaknesses(uint8_t gain)
// {
//     I2C_Status status_Com;
//     float x, y, z, mx=0, my=0, mz=0;
//     float max=0;
//     int i;
// 	
//     // get actual values
//     x_scale = 1; 
//     y_scale = 1;
//     z_scale = 1;
//     
// 	// Reg A DOR=0x010 + MS1,MS0 set to pos bias
// 	
//     if(HMC5883L_SetDOR(0x010 + HMC_POS_BIAS) != HMC5883L_STATUS_OK) return (HMC5883L_Status) status_Com;
//     if(HMC5883L_SetGain(gain) != I2C_STATUS_SUCCESS) return (HMC5883L_Status) status_Com;
//     
//     for (i=0; i<10; i++)
//     { 
//         HMC5883L_Setmode(1);
//         status_Com = (I2C_Status) HMC5883L_GetValues(&x,&y,&z);
//         if(status_Com != I2C_STATUS_SUCCESS) return (HMC5883L_Status) status_Com;
//         
//         if (x>mx) mx=x;
//         if (y>my) my=y;
//         if (z>mz) mz=z;
//     }

//     if (mx>max) max=mx;
//     if (my>max) max=my;
//     if (mz>max) max=mz;
//     x_max=mx;
//     y_max=my;
//     z_max=mz;
//     x_scale=max/mx; // calc scales
//     y_scale=max/my;
//     z_scale=max/mz;

//     status_Com = I2C_writeReg(&Sensor,HMC58X3_ADDR,HMC58X3_R_CONFA,0x010 );// set RegA/DOR back to default
//     return (HMC5883L_Status) status_Com;
// }

// HMC5883L_Status HMC5883L_GetValues(float *x,float *y,float *z)
// {
//     I2C_Status status_Com;
//     int xr,yr,zr;

//     status_Com = (I2C_Status)  HMC5883L_GetRaw(&xr, &yr, &zr);
//     *x= ((float) xr) / x_scale;
//     *y = ((float) yr) / y_scale;
//     *z = ((float) zr) / z_scale;
//     return (HMC5883L_Status) status_Com;
// }
