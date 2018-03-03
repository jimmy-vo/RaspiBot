#include "IMU.h"
#include "Serial_Link.h"
#include "Timing_Manage.h"
#include "Tool_DataProcessing.h"
#include "Dev_I2C.h"
#include <math.h>

Motion_Angle Acce_Angle;

static void IMU_delay(u32 nCount)
{
	u32 index = 0;
	for(index = (168000 * nCount); index != 0; index--);
}

Motion_Angle IMU_Get_Accel_Angles(Acce* acceleration)
{
	Acce_Angle.x = 57.295*atan((float)acceleration->y/ sqrt(pow((float)acceleration->z,2)+pow((float)acceleration->x,2)));
	Acce_Angle.y = 57.295*atan((float)-acceleration->x/ sqrt(pow((float)acceleration->z,2)+pow((float)acceleration->y,2)));	
	return Acce_Angle;
}	

IMU_Status Initialize_MPU6050(void)		
{    
    if(MPU6050_ConnectionCheck()==MPU6050_STATUS_OK)
			{	 				
				if	(MPU6050_Set_PWR_MGMT_1(MPU6050_SLEEP_DISABLE, MPU6050_TEMPERATURE_ENABLE, MPU6050_CLOCK_PLL_XGYRO)!= MPU6050_STATUS_OK)
 					return IMU_Status_FAIL;				
				if	(	MPU6050_Set_GYRO_Config(MPU6050_GYRO_FS_2000)!= MPU6050_STATUS_OK) 
 					return IMU_Status_FAIL;
				if	(MPU6050_Set_ACCEL_Config(MPU6050_ACCEL_FS_16)!= MPU6050_STATUS_OK) 
 					return IMU_Status_FAIL;		
				if	(MPU6050_Set_Configuration(DLPF_CFG_1)!= MPU6050_STATUS_OK)	
 					return IMU_Status_FAIL;	
				if	(	MPU6050_Set_SMPLRT_DIV(0)!= MPU6050_STATUS_OK)	
 					return IMU_Status_FAIL;
			}
    return IMU_Status_OK;
}
IMU_Status  Initialize_HMC5883L(void)
{
	if(HMC5883L_ConnectionCheck() != HMC5883L_STATUS_OK)								return IMU_Status_FAIL;	
	else
		{
			if(HMC5883L_Configuration_A(HMC5883L_SAMPLEAVERAGE_8, HMC5883L_RATE_15, HMC5883L_BIAS_NORMAL)!= I2C_STATUS_OK)		
				return 	IMU_Status_FAIL;					
			if(HMC5883L_Configuration_B(HMC5883L_GAIN_230)!= I2C_STATUS_OK)																									
				return  IMU_Status_FAIL;
			if(HMC5883L_Configuration_MODE(HMC5883L_HIGHSPEED_DontCare, HMC5883L_MODE_CONTINUOUS)!= I2C_STATUS_OK)				
				return  IMU_Status_FAIL;
		}
		return IMU_Status_OK;
}


IMU_Status Initialize_IMU (void)
{
	  GPIO_InitTypeDef  GPIO_InitStructure;
    /* GPIO Periph clock enable */
    RCC_AHB1PeriphClockCmd(I2C1_GPIO_CLK, ENABLE);
	
    /* Configure I2C2 pins: SCL and SDA */
    GPIO_InitStructure.GPIO_Pin 		=  	I2C1_SCL_PIN | I2C1_SDA_PIN;
		GPIO_InitStructure.GPIO_Mode 		= 	GPIO_Mode_AF;
		GPIO_InitStructure.GPIO_Speed 	= 	GPIO_Speed_100MHz;
		GPIO_InitStructure.GPIO_OType 	= 	GPIO_OType_OD;
		GPIO_InitStructure.GPIO_PuPd  	= 	GPIO_PuPd_NOPULL;
    GPIO_Init(I2C1_GPIO_PORT, &GPIO_InitStructure);
	
	  /* Connect GPIO pins to peripheral */
		GPIO_PinAFConfig(I2C1_GPIO_PORT, I2C1_SCL_SOURCE, I2C1_AF); 
		GPIO_PinAFConfig(I2C1_GPIO_PORT, I2C1_SDA_SOURCE, I2C1_AF);
	
 		IMU.TX.DMA									= I2C_DMA_Enable;
		IMU.TX.DMA_Channel					=	I2C1_TX_DMA_CH;
		IMU.TX.DMA_Stream						=	I2C1_TX_DMA_ST;
		IMU.TX.DMA_Flag							=	I2C1_TX_DMA_FLAG;
		IMU.TX.DMA_NVIC_IRQ					=	I2C1_TX_DMA_IRQn;
	
		IMU.RX.DMA									= I2C_DMA_Enable;
		IMU.RX.DMA_Channel					=	I2C1_RX_DMA_CH;
		IMU.RX.DMA_Stream						=	I2C1_RX_DMA_ST;
		IMU.RX.DMA_Flag							=	I2C1_RX_DMA_FLAG;
		IMU.RX.DMA_NVIC_IRQ					=	I2C1_RX_DMA_IRQn;
		
		IMU.TimeOut 									= 100;		//10ms
		IMU.I2C											= I2C1;		
		
		Initialize_I2C(&IMU,400000);			//400kHz
		
		
		IMU_delay(100);
 		if (Initialize_HMC5883L() !=	IMU_Status_OK)	return IMU_Status_FAIL;		
 		if (Initialize_MPU6050() 	!= 	IMU_Status_OK) 	return  IMU_Status_FAIL;		
		return IMU_Status_OK;
}


Motion_Angle ACC_ToAngle(Acce *acce)
{
	Motion_Angle Acc_motion;
	float Ax,Ay,Az;
	
	Ax =  ((float)acce->x/ACC_SCALE_FACTOR)*ACC_SENSITIVITY;
	Ay =  ((float)acce->y/ACC_SCALE_FACTOR)*ACC_SENSITIVITY;
	Az =  ((float)acce->z/ACC_SCALE_FACTOR)*ACC_SENSITIVITY;
	
	
	Acc_motion.x = (float) atan2(Ax, sqrtf( Ay*Ay + Az*Az) )* 57.2957795;
	Acc_motion.y = (float) atan2(Ay, sqrtf( Ay*Ax + Az*Az) )* 57.2957795;	
	Acc_motion.z = 0;	
	
	return Acc_motion;
}


void IMU_Process (void)
	{		
		Gyro gyro;
		Acce acce;
		Motion_Angle motion;
		Compass Compass;
		
		MPU6050_Get_rawMotion (&gyro, &acce);  
		HMC5883L_Get_RawCompact(&Compass);		
		motion = ACC_ToAngle (&acce);
		
		DATA_OUT.Roll = (int16_t) (motion.x *100);				
		DATA_OUT.Pitch = (int16_t) (motion.y *100);			
		DATA_OUT.Yaw = (int16_t) (motion.z *100);
	}
