#include "include.h"


//////////////////////////////
int Front_Sona, Back_Sona;
//////////////////////////////

//////////////////////////////
int CAMST;
int Camera_X=0;
int Camera_Y=0;
int Servo_Speed;
int ROTATE;
//////////////////////////////

//////////////////////////////
int Motor_L1, Motor_L2, Motor_R1, Motor_R2;
int DIRECTION=0;
//////////////////////////////


int Left_Speed;
int Right_Speed;

#define distance_scale 		6.5*3.14159/20
int Left_Distance;
int Right_Distance;
int Left_Distance_TMP;
int Right_Distance_TMP;
int Left_Distance_CNT;
int Right_Distance_CNT;
int Left_Distance_IND;
int Right_Distance_IND;
int Left_Distance_STO;
int Right_Distance_STO;
//////////////////////////////
int T;
int error;

void* launch_Streaming()  
	{
		if (EN_Camera)
			{
				if(CAMST==0)
					{
						CAMST=1;
						system("sudo /home/pi/Raspi_Project/Raspibot/CAMERA start");
					}
			}		
			else
				{
					if(CAMST==1)
						{
							CAMST=0;
							system("sudo /home/pi/Raspi_Project/Raspibot/CAMERA stop");
						}
				}
				
		pthread_exit(NULL);
		printf ("----- %d \n",count++);	
	}

void Manual_Process (void)
	{		
		Command_Process();
		{///SPEED Process///////////////////////////////////////////////////////////////////
			if (EN_Speed==0)	
				{
					Left_Speed=0;
					Right_Speed=0;
				}	
				else if ((STATUS&bit2)==bit2 )		//SPEED READ
						{
							Left_Speed   	= 	(Left_ENCODER[0]<<8)   | Left_ENCODER[1];
							Right_Speed 	= 	(Right_ENCODER[0]<<8)  | Right_ENCODER[1];
						}
		}///////////////////////////////////////////////////////////////////////////////////				
			
		{///DISTANCE Process////////////////////////////////////////////////////////////////
			if (EN_Distance==0)	
					{
						Left_Distance_IND=1;
						Right_Distance_IND=1;
						Left_Distance_STO=0;
						Right_Distance_STO=0;
						Left_Distance=0;
						Right_Distance=0;
					}
					else if ((STATUS&bit3)==bit3)			//DISTANCE READ
						{
							//catch data
							Left_Distance_TMP 	=	Left_ENCODER[1];
							Right_Distance_TMP 	= 	Right_ENCODER[1];
							
							//Process
							if (Left_Distance_IND == Left_ENCODER[0]) 	
								Left_Distance = Left_Distance_TMP + Left_Distance_STO;
								else if (Left_Distance_IND < Left_ENCODER[0])
									{
										Left_Distance_STO	=	Left_Distance_STO	+	0xFF;
										Left_Distance_IND 	=  	Left_ENCODER[0];
										Left_Distance = Left_Distance_TMP + Left_Distance_STO;
									}
									else 
										{
											Left_Distance=-111;
										}
					
							
							if (Right_ENCODER[0]== Right_Distance_IND) 		
								Right_Distance = Right_Distance_TMP + Right_Distance_STO;
								else if ( Right_Distance_IND < Right_ENCODER[0])
									{
										Right_Distance_STO	=	Right_Distance_STO	+	0xFF;
										Right_Distance_IND 	=  	Right_ENCODER[0];
										Right_Distance = Right_Distance_TMP + Right_Distance_STO;
									}
									else 
										{
											Right_Distance=-111;
										}
							
						}
		}///////////////////////////////////////////////////////////////////////////////////
			
		{///MOVE Process////////////////////////////////////////////////////////////////////
				if (DIRECTION == UP)			{Motor_L1=100;	Motor_L2=100;		Motor_R1=100;			Motor_R2=100;}
				if (DIRECTION == DOWN)			{Motor_L1=-100;	Motor_L2=-100;		Motor_R1=-100;			Motor_R2=-100;}
				if (DIRECTION == LEFT)			{Motor_L1=-100;	Motor_L2=-100;		Motor_R1=100;			Motor_R2=100;}
				if (DIRECTION == RIGHT)			{Motor_L1=100;	Motor_L2=100;		Motor_R1=-100;			Motor_R2=-100;}
				if (DIRECTION == UP_LEFT)		{Motor_L1=0;	Motor_L2=0;			Motor_R1=100;			Motor_R2=100;}
				if (DIRECTION == UP_RIGHT)		{Motor_L1=100;	Motor_L2=100;		Motor_R1=0;				Motor_R2=0;}
				if (DIRECTION == DOWN_LEFT)		{Motor_L1=0;	Motor_L2=0;			Motor_R1=-100;			Motor_R2=-100;}
				if (DIRECTION == DOWN_RIGHT)	{Motor_L1=-100;	Motor_L2=-100;		Motor_R1=0;				Motor_R2=0;}
				if (DIRECTION == HALT)			{Motor_L1=0;	Motor_L2=0;			Motor_R1=0;				Motor_R2=0;}
		}///////////////////////////////////////////////////////////////////////////////////
		
		{///Camera Process////////////////////////////////////////////////////////////////////
			if (EN_Camera!=CAMST)
				{
					pthread_t STREAMING_THREAD; 
					error = pthread_create( &STREAMING_THREAD, NULL, &launch_Streaming, (void*) NULL);	
				}

		}///////////////////////////////////////////////////////////////////////////////////


			////////////Prepare DATA OUT for UART////////////
			UART_OUT [0] = 0xFF;
			UART_OUT [1] = 0xFF;
			UART_OUT [2] = Motor_L1;
			UART_OUT [3] = Motor_L2;
			UART_OUT [4] = Motor_R1;
			UART_OUT [5] = Motor_R2;
			UART_OUT [6] = Camera_X;
			UART_OUT [7] = Camera_Y;
			
			UART_OUT[TX_SIZE-1]= bit4|bit6;
			
			if (EN_Sona)		UART_OUT[TX_SIZE-1] = UART_OUT[TX_SIZE-1] | bit1;
				else 			UART_OUT[TX_SIZE-1] = UART_OUT[TX_SIZE-1] & ((~bit1)&0xFF);
			if (EN_Speed)		UART_OUT[TX_SIZE-1] = UART_OUT[TX_SIZE-1] | bit2;
				else 			UART_OUT[TX_SIZE-1] = UART_OUT[TX_SIZE-1] & ((~bit2)&0xFF);
			if (EN_Distance)	UART_OUT[TX_SIZE-1] = UART_OUT[TX_SIZE-1] | bit3;
				else 			UART_OUT[TX_SIZE-1] = UART_OUT[TX_SIZE-1] & ((~bit3)&0xFF);
				
			
			if(1)
				{//////////////////////////////////
				if (T < millis())
					{
						printf ("SP:%4d SE:%1d     ",
														RequestedSpeed,
														Servo_Speed			);
						printf ("%3d %3d - %05d %05d - %05d %05d - <|%d%d%d%d%d%d%d%d|>  ",
																					Front_Sona,
																					Back_Sona,
																					Left_Speed,
																					Right_Speed,
																					Left_Distance,
																					Right_Distance,
																					(STATUS&bit0)==bit0 ,
																					(STATUS&bit1)==bit1 ,
																					(STATUS&bit2)==bit2 ,
																					(STATUS&bit3)==bit3 ,
																					(STATUS&bit4)==bit4 ,
																					(STATUS&bit5)==bit5 ,
																					(STATUS&bit6)==bit6 ,
																					(STATUS&bit7)==bit7);
						printf ("TX:%3d RX:%3d Se:%3d Ma:%3d CV:%3d",
														TX_Rate,
														RX_Rate,
														SensorPost_Rate,
														Manual_Rate, 
														CV_Rate				);
						printf("\n");
						T=millis()+100;
					}
				}//////////////////////////////////				
			/////////////////////////////////////////////		
			digitalWrite (LEDW, LOW);
			digitalWrite (LEDB, LOW);
			digitalWrite (LEDG, LOW);	
				//////////////////////////////////	
				Manual_Rate=(millis()-Manual_Timing);
				Manual_Timing=millis();
			/////////////////////////////////////////////	
	}
void Manual_Clean(void)
	{	
			////////////Prepare DATA OUT for UART////////////
		UART_OUT [0] = 0xFF;
		UART_OUT [1] = 0xFF;
		UART_OUT [2] = 0;
		UART_OUT [3] = 0;
		UART_OUT [4] = 0;
		UART_OUT [5] = 0;
		UART_OUT [6] = Camera_X;
		UART_OUT [7] = Camera_Y;
		UART_OUT[TX_SIZE-1]= bit4|bit6;
			
		if (CAMST=1) // clean streaming
			{
				CAMST=0;
				system("sudo /home/pi/Raspi_Project/Raspibot/CAMERA stop");
			}
	}			
				
			
