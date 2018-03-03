#include "BalaPi_BoardConf.h"

typedef struct 
							{
									int8_t									Left_Speed;								///	1byte speed
									int8_t								Right_Speed;							///	1byte speed
								
									int16_t	  					Left_Encoder;								/// 2byte Encoder
									int16_t	  					Right_Encoder;							/// 2byte Encoder
								
									uint16_t	  					Ultrasonic_A;								/// 2byte Encoder
									uint16_t	  					Ultrasonic_B;							/// 2byte Encoder
								
									int16_t	  					Temperature;							/// 2byte Encoder
									
									int16_t	  					Gyrox;							/// 2byte Encoder
									int16_t	  					Gyroy;							/// 2byte Encoder
									int16_t	  					Gyroz;							/// 2byte Encoder
									
									int16_t	  					Accex;							/// 2byte Encoder
									int16_t	  					Accey;							/// 2byte Encoder
									int16_t	  					Accez;							/// 2byte Encoder
									
									int16_t	  					Compx;							/// 2byte Encoder
									int16_t	  					Compy;							/// 2byte Encoder
									int16_t	  					Compz;							/// 2byte Encoder
									
									int16_t	  					Roll;							/// 2byte Encoder
									int16_t	  					Pitch;							/// 2byte Encoder
									int16_t	  					Yaw;							/// 2byte Encoder
							}DATA;
							
							
							
extern DATA DATA_OUT;


void Initialize_SerialLink (void);
void SerialLink_Send (uint8_t* str, uint32_t len);

void SerialLink_StreamProcess (void);

void SerialLink_Debug (char* message  );
void SerialLink_DebugProcess (void);

