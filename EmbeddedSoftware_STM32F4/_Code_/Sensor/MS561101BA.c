#include "MS561101BA.h"
#include "Timing_Manage.h"
#include "Dev_I2C.h"

#define EXTRA_PRECISION 5 // trick to add more precision to the pressure and temp readings
#define CONVERSION_TIME 10000 // conversion time in 10 microseconds
uint64_t g_last_time_temperature = 0;
uint64_t g_last_time_pressure = 0;
uint64_t g_zero_time;
MS561101BA_Convert_Object g_MS561101BA_convert = MS561101BA_CONVERT_PRESSURE;
uint32_t data_pre,data_temp;
int32_t DeltaTemp;
int64_t Off,Sens;
int64_t temp_Off,temp_Sens;
int64_t pressure_temp,temperature_temp;
int32_t pressure;
int32_t temperature;

uint64_t g_last_time_temperature;
uint64_t g_last_time_pressure;
uint64_t g_zero_time;
uint16_t _C1,_C2,_C3,_C4,_C5,_C6; 

MS561101BA_Status  MS561101BA_ResetChip()
{
    I2C_Status  status_Com;
    status_Com = I2C_writeCommand(&Sensor, MS561101BA_ADDR_CSB_LOW, MS561101BA_RESET);
    return (MS561101BA_Status) status_Com;
}

MS561101BA_Status  MS561101BA_readPROM(uint8_t *buffer)
{
    uint8_t i;
    uint8_t command;
    I2C_Status status_Com;
    for(i=0;i<6;i++)
    {
        command = MS561101BA_PROM_BASE_ADDR + (2*i);
        status_Com = I2C_readBytes(&Sensor,MS561101BA_ADDR_CSB_LOW,command,2,(buffer + (i*2)));
        if(status_Com != I2C_STATUS_SUCCESS) return (MS561101BA_Status) status_Com;
    }
    return (MS561101BA_Status) status_Com;
}

MS561101BA_Status  MS561101BA_startConversion(uint8_t command)
{
    return (MS561101BA_Status) I2C_writeCommand(&Sensor, MS561101BA_ADDR_CSB_LOW,command);
}

//Get conversion called after startConversion
MS561101BA_Status  MS561101BA_getConversion(uint8_t *buffer)
{
    return (MS561101BA_Status) I2C_readBytes(&Sensor,MS561101BA_ADDR_CSB_LOW,0,MS561101BA_D1D2_SIZE,buffer);
}

MS561101BA_Status_Convert  MS561101BA_rawTemperature(uint8_t OSR,uint8_t *buffer)
{
    MS561101BA_Status_Convert status_Convert;
    MS561101BA_Status  status_Com;
    uint64_t time_now;
    time_now = SensorGetTick();
    if(g_last_time_temperature == 0)
    {
        status_Com = MS561101BA_startConversion(MS561101BA_D2 + OSR);
        if(status_Com != I2C_STATUS_SUCCESS) return MS561101BA_STATUS_CONVERT_FAIL;
        g_last_time_temperature = time_now;
        status_Convert = MS561101BA_STATUS_START_CONVERT;
    }
    else
    {
        if(SensorCheckTick(g_last_time_temperature,CONVERSION_TIME))
        {
            status_Com = MS561101BA_getConversion(buffer);
            if(status_Com != I2C_STATUS_SUCCESS) return MS561101BA_STATUS_CONVERT_FAIL;
            g_last_time_temperature = 0;
            status_Convert = MS561101BA_STATUS_CONVERT_FINISH;
        }
        else
        {
            status_Convert = MS561101BA_STATUS_CONVERTING;
        }
    }
    return status_Convert;
}

MS561101BA_Status_Convert  MS561101BA_rawPressure(uint8_t OSR,uint8_t *buffer)
{
    MS561101BA_Status_Convert status_Convert;
    MS561101BA_Status status_Com;
    uint64_t time_now;
    time_now = SensorGetTick();
    if(g_last_time_pressure == 0)
    {
        status_Com = MS561101BA_startConversion(MS561101BA_D1 + OSR);
        if(status_Com != I2C_STATUS_SUCCESS) return MS561101BA_STATUS_CONVERT_FAIL;
        g_last_time_pressure = time_now;
        status_Convert = MS561101BA_STATUS_START_CONVERT;
    }
    else
    {
        if(SensorCheckTick(g_last_time_pressure,CONVERSION_TIME))
        {
            status_Com = MS561101BA_getConversion(buffer);
            if(status_Com != I2C_STATUS_SUCCESS) return MS561101BA_STATUS_CONVERT_FAIL;
            g_last_time_pressure = 0;
            status_Convert = MS561101BA_STATUS_CONVERT_FINISH;
        }
        else
        {
            status_Convert = MS561101BA_STATUS_CONVERTING;
        }
    }
    return status_Convert;
}

MS561101BA_Status_Convert 	MS561101BA_getData()
{
    uint8_t buffer[3];

    MS561101BA_Status_Convert status_Convert;
   
    
    switch(g_MS561101BA_convert)
    {
        case MS561101BA_CONVERT_PRESSURE:
        {
            status_Convert = MS561101BA_rawPressure(MS561101BA_OSR_4096,buffer);
            if(status_Convert == MS561101BA_STATUS_CONVERT_FINISH)
            {
                data_pre = ((uint32_t)buffer[0]<<16) + ((uint32_t)buffer[1]<<8) + buffer[2];
                g_MS561101BA_convert = MS561101BA_CONVERT_TEMPERATURE;
            }
            else if(status_Convert == MS561101BA_STATUS_CONVERT_FAIL)
            {
            }
        }break;
        case MS561101BA_CONVERT_TEMPERATURE:
        {
            status_Convert = MS561101BA_rawTemperature(MS561101BA_OSR_4096,buffer);
            if(status_Convert == MS561101BA_STATUS_CONVERT_FINISH)
            {
                data_temp = ((uint32_t)buffer[0]<<16) + ((uint32_t)buffer[1]<<8) + buffer[2];
                g_MS561101BA_convert = MS561101BA_CONVERT_PRESSURE;
                //Caculation DT
                DeltaTemp = ((uint32_t)_C5)<<8;
                DeltaTemp = (int32_t)data_temp - DeltaTemp;
                // Caculation Off
                Off = (int64_t)_C2;
                Off  = Off<<16;
                temp_Off = _C4;
                temp_Off *= DeltaTemp;
                temp_Off = temp_Off>>7;
                Off += temp_Off;
                // Caculate Temp
                temperature_temp = (int64_t)DeltaTemp;
                temperature_temp *= _C6;
                temperature_temp = temperature_temp>>23;
                temperature = temperature_temp + 2000;
                
                //Caculate Sens
                
                // Sens = (((uint32_t)_C1) << 15) + (((int64_t)(_C3 * DeltaTemp)) >> 8);
                temp_Sens = (int64_t)_C3;
                temp_Sens *= DeltaTemp;
                temp_Sens = temp_Sens>>8;
                Sens = _C1;
                Sens = Sens<<15;
                Sens += temp_Sens;
                
                //Caculate pressure

                pressure_temp = data_pre;
                pressure_temp *= Sens;
                pressure_temp = pressure_temp>>21;
                pressure_temp -= Off;
                
                
                pressure = pressure_temp>>15;
                
            }
            else if(status_Convert == MS561101BA_STATUS_CONVERT_FAIL)
            {
            }
        }break;
        default: break;
    }
    return status_Convert;
}


