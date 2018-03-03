#include "MPU6050.h"
#include "Dev_I2C.h"
#include "Timing_Manage.h"
#include "Dev_DIGITAL.h"
#include "Serial_Link.h"
uint8_t MPU6050_RegDebug [MPU6050_Debug];

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////
////I2C	Driver
//
static I2C_Status I2C_RegWrite (MPU6050_Register Register, uint8_t *buffer)
{	
	uint8_t reg = Register;	
	
	I2C_Request(&IMU,	I2C_Phase_TX,  MPU6050_DEFAULT_ADDRESS, MPU6050_DEFAULT_ADDRESS, &reg, 1, buffer, 1);	
	return I2C_BUSY_Check (&IMU);
}

static I2C_Status I2C_Read (MPU6050_Register Register, uint8_t *buffer, uint32_t length)
{
	uint8_t reg =  Register;	
	
	I2C_Request(&IMU,	I2C_Phase_RX, MPU6050_DEFAULT_ADDRESS, MPU6050_DEFAULT_ADDRESS, &reg, 1, buffer, length);		
	return I2C_BUSY_Check (&IMU);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////ConnectionCheck
//
MPU6050_Status MPU6050_GetDeviceID(uint8_t *id) 
{
    if(I2C_Read(MPU6050_WHO_AM_I,id,1)!= I2C_STATUS_OK) 
				return MPU6050_STATUS_FAIL;
			else
				return MPU6050_STATUS_OK;
}

MPU6050_Status MPU6050_ConnectionCheck(void) 
{    
		uint8_t result;
    if((MPU6050_GetDeviceID(&result)==MPU6050_STATUS_OK)&&(result==104))
				return MPU6050_STATUS_OK;
			else						
					return MPU6050_STATUS_FAIL;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////REGISTER CONFIG	
//
MPU6050_Status MPU6050_Set_SMPLRT_DIV(uint8_t  div)
{	
	uint8_t check;	
	
	if(I2C_RegWrite	(MPU6050_SMPLRT_DIV,&div) != I2C_STATUS_OK)						return MPU6050_STATUS_FAIL;     
	if(I2C_Read			(MPU6050_SMPLRT_DIV,&check,1) != I2C_STATUS_OK)				return MPU6050_STATUS_FAIL;
	
	MPU6050_RegDebug[MPU6050_SMPLRT_DIV]   = check; 
	
	if (check!=div)																												return MPU6050_STATUS_FAIL;
	else 																																	return MPU6050_STATUS_OK; 		
}

MPU6050_Status MPU6050_Set_Configuration(MPU6050_DLPF_CFG  DLPF_CFG)
{	
	uint8_t check, newconfig = 0x00, mask = 0x00;	
	
	if(I2C_Read			(MPU6050_CONFIG,&newconfig,1) != I2C_STATUS_OK)				return MPU6050_STATUS_FAIL;	
	
	//DLPF
	if (DLPF_CFG != DLPF_CFG_DontCare) 						
	{
		newconfig &= MPU6050_DLPF_CFG_Clear;
		newconfig |= (DLPF_CFG<<MPU6050_DLPF_CFG_Position&0xFF);
		mask |= ~ MPU6050_DLPF_CFG_Clear;
	}
		
	if(I2C_RegWrite	(MPU6050_CONFIG,&newconfig) != I2C_STATUS_OK)					return MPU6050_STATUS_FAIL;     
	if(I2C_Read			(MPU6050_CONFIG,&check,1) != I2C_STATUS_OK)						return MPU6050_STATUS_FAIL; 
	   
	MPU6050_RegDebug[MPU6050_CONFIG]   = check; //26
	
	if ((check&mask)!=newconfig)																					return MPU6050_STATUS_FAIL;
	else 																																	return MPU6050_STATUS_OK; 		
}

MPU6050_Status MPU6050_Set_GYRO_Config (MPU6050_GYRO_FS  GYRO_FS)
{	
	uint8_t check, newconfig = 0x00, mask = 0x00;	
	
	if(I2C_Read			(MPU6050_GYRO_CONFIG,&newconfig,1) != I2C_STATUS_OK)	return MPU6050_STATUS_FAIL;	
	
	// prepare for config
	if (GYRO_FS != MPU6050_GYRO_FS_DontCare) 								
	{
		newconfig &= MPU6050_GYRO_FS_Clear;
		newconfig |= (GYRO_FS<<MPU6050_GYRO_FS_Position&0xFF);
		mask |= ~ MPU6050_GYRO_FS_Clear;
	}
		
	if(I2C_RegWrite	(MPU6050_GYRO_CONFIG,&newconfig) != I2C_STATUS_OK)		return MPU6050_STATUS_FAIL;     
	if(I2C_Read			(MPU6050_GYRO_CONFIG,&check,1) != I2C_STATUS_OK)			return MPU6050_STATUS_FAIL; 
	   
	MPU6050_RegDebug[MPU6050_GYRO_CONFIG]   = check; 
		   
	if ((check&mask)!=newconfig)																					return MPU6050_STATUS_FAIL;
	else 																																	return MPU6050_STATUS_OK; 		
}


MPU6050_Status MPU6050_Set_ACCEL_Config (MPU6050_ACCEL_FS  ACCEL_FS)
{	
	uint8_t check, newconfig = 0x00, mask = 0x00;	
	
	if(I2C_Read			(MPU6050_ACCEL_CONFIG,&newconfig,1) != I2C_STATUS_OK)	return MPU6050_STATUS_FAIL;	
	
	// prepare for config
	if (ACCEL_FS != MPU6050_GYRO_FS_DontCare) 									
	{
		newconfig &= MPU6050_ACCEL_FS_Clear;
		newconfig |= (ACCEL_FS<<MPU6050_ACCEL_FS_Position&0xFF);
		mask |= ~ MPU6050_ACCEL_FS_Clear;
	}
		
	
	if(I2C_RegWrite	(MPU6050_ACCEL_CONFIG,&newconfig) != I2C_STATUS_OK)		return MPU6050_STATUS_FAIL;     
	if(I2C_Read			(MPU6050_ACCEL_CONFIG,&check,1) != I2C_STATUS_OK)			return MPU6050_STATUS_FAIL; 
	   
	MPU6050_RegDebug[MPU6050_ACCEL_CONFIG]   = check; 
	   
	if ((check&mask)!=newconfig)																					return MPU6050_STATUS_FAIL;
	else 																																	return MPU6050_STATUS_OK; 		
}

uint8_t mask,newconfig;

MPU6050_Status MPU6050_Set_PWR_MGMT_1 ( MPU6050_SLEEP SLEEP, MPU6050_TEMPERATURE  TEMPERATURE, MPU6050_CLOCK CLOCK )
{	
	uint8_t check;//, newconfig = 0x00;// mask = 0x00;	
	
	if(I2C_Read			(MPU6050_PWR_MGMT_1,&newconfig,1) != I2C_STATUS_OK)		return MPU6050_STATUS_FAIL;	
	
	//SLEEP
	if (SLEEP != MPU6050_SLEEP_DontCare) 																
	{
		newconfig &= MPU6050_SLEEP_Clear;
		newconfig |= (SLEEP<<MPU6050_SLEEP_Position&0xFF);
		mask |= ~ MPU6050_SLEEP_Clear;
	}
	
	//TEMPERATURE
	if (TEMPERATURE != MPU6050_TEMPERATURE_DontCare) 														
	{
		newconfig &= MPU6050_TEMPERATURE_Clear;
		newconfig |= (TEMPERATURE<<MPU6050_TEMPERATURE_Position&0xFF);
		mask |= ~ MPU6050_TEMPERATURE_Clear;
	}
	
	//CLOCK
	if (CLOCK != MPU6050_CLOCK_DontCare) 											 														
	{
		newconfig &= MPU6050_CLOCK_Clear;
		newconfig |= (CLOCK<<MPU6050_CLOCK_Position&0xFF);
		mask |= ~ MPU6050_CLOCK_Clear;
	}
		
	if(I2C_RegWrite	(MPU6050_PWR_MGMT_1,&newconfig) != I2C_STATUS_OK)			return MPU6050_STATUS_FAIL;     
	if(I2C_Read			(MPU6050_PWR_MGMT_1,&check,1) != I2C_STATUS_OK)				return MPU6050_STATUS_FAIL; 
	   
	MPU6050_RegDebug[MPU6050_PWR_MGMT_1]   = check; 
	   
	if ((check&mask)!=newconfig)																					return MPU6050_STATUS_FAIL;
	else 																																	return MPU6050_STATUS_OK; 		
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////DATA READ
//
MPU6050_Status MPU6050_Get_rawDATA(Gyro *Gyro, Acce *Acce,float *Temp) 
{   
 uint8_t buffer[14];
	int16_t	rawtemp;
	
    if(I2C_Read (MPU6050_ACCEL_XOUT_H, buffer, 14) == I2C_STATUS_OK)
    {
        Acce->x =  DATA_OUT.Accex = (((uint16_t)buffer[0]) 	<< 8) | buffer[1];
        Acce->y =  DATA_OUT.Accey = (((uint16_t)buffer[2]) 	<< 8) | buffer[3];
        Acce->z =  DATA_OUT.Accez = (((uint16_t)buffer[4]) 	<< 8) | buffer[5];
			
				rawtemp = (((uint16_t)buffer[6]) 	<< 8) | buffer[7];
			
        Gyro->x =  DATA_OUT.Gyrox = (((uint16_t)buffer[8]) 	<< 8) | buffer[9];
        Gyro->y =  DATA_OUT.Gyroy = (((uint16_t)buffer[10]) << 8) | buffer[11];
        Gyro->z =  DATA_OUT.Gyroz = (((uint16_t)buffer[12]) << 8) | buffer[13];
			
				*Temp= ((float)rawtemp)/340.0f +36.53f;
				
				DATA_OUT.Temperature =  (int16_t) ((*Temp)*100);
    }
    return MPU6050_STATUS_OK;
}

MPU6050_Status MPU6050_Get_rawMotion(Gyro *Gyro, Acce *Acce) 
{   
 uint8_t buffer[14];
	int16_t	rawtemp;
	float	Temp;
	
    if(I2C_Read (MPU6050_ACCEL_XOUT_H, buffer, 14) == I2C_STATUS_OK)
    {
        Acce->x = DATA_OUT.Accex = (((uint16_t)buffer[0]) 	<< 8) | buffer[1];
        Acce->y = DATA_OUT.Accey =(((uint16_t)buffer[2]) 	<< 8) | buffer[3];
        Acce->z = DATA_OUT.Accez =(((uint16_t)buffer[4]) 	<< 8) | buffer[5];
				
				rawtemp = (((uint16_t)buffer[6]) 	<< 8) | buffer[7];
						
        Gyro->x = DATA_OUT.Gyrox = (((uint16_t)buffer[8]) 	<< 8) | buffer[9];
        Gyro->y = DATA_OUT.Gyroy = (((uint16_t)buffer[10]) << 8) | buffer[11];
        Gyro->z = DATA_OUT.Gyroz = (((uint16_t)buffer[12]) << 8) | buffer[13];
				
				Temp= ((float)rawtemp)/340.0f +36.53f;
				
				DATA_OUT.Temperature =  (int16_t) (Temp*100);
    }
    return MPU6050_STATUS_OK;
}
