#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <fcntl.h>
#include <assert.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#include "mongoose.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <wiringPi.h>
#include <wiringSerial.h>


#define LEDG    0
#define LEDB    2
#define LEDW    3






///////////////////////////////////////////////////////////////////////////////
///////////UART Protocol///////////////////////////////////////////////////////
#define bit0        128
#define bit1        64
#define bit2        32
#define bit3        16
#define bit4        8
#define bit5        4
#define bit6        2
#define bit7        1													 
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
//     UART_OUT          Read more in USART4_IRQHandler()                    //
///////////////////////////////////////////////////////////////////////////////
//      0 |   START    |   |M1|M2|M3|M4|    |   STATUS    |                  //
//                                                                           //
//START:                                                        2 byte       // 0-1
//                                                                           //
//MOTOR:   |M L1 :  (-100,100)                                  1 byte       // 2    
//         |M L2 :  (-100,100)                                  1 byte       // 3
//         |M R1 :  (-100,100)                                  1 byte       // 4
//         |M R2 :  (-100,100)                                  1 byte       // 5
//																			 //
//																			 //
//SERVO		X :	   (-45;45) !88					     	       	1 byte       // 6
//          Y :    (-45;45) !88	                                1 byte       // 7
//                                                                           //
//                                                                           //
//STATUS:                                                       1 byte       // 8
//          0:                                                               //
//          1:  Enable SONA                             		             //
//          2:                         					                     //   
//          3:  Enable Speed                                                 //
//          4:  Enable Distance                                              //
//          5:                   				                             //
//          6:  Enable Camera                                            	 //
//          7:                                                               //
///////////////////////////////////////////////////////////////////////////////
#define     TX_SIZE  	  														9

///////////////////////////////////////////////////////////////////////////////
//INPUT                                                                      //    
///////////////////////////////////////////////////////////////////////////////
//      0 |  START |    |UF|UB| |E1|E2||E3|E4| |X|Y|Z|  |    STATUS    |     //
//                                                                           //
//START:                                                        3 byte       // 0-2
//                                                                           //
//                                                                           //
//SONA:    |UF  :   (0,400)                                     2 byte       // 3-4   
//         |UB  :   (0,400)                                     2 byte       // 5-6
//                                                                           //
//ENCODER: |E1  :   (0:65000)                                   2 byte       // 7-8
//         |E2  :   (0:65000)                                   2 byte       // 9-10
//                                                                           //
//STATUS:                                                       1 byte       // 11
//          0:                                                               //
//          1:  Enable SONA                             		             //
//          2:                         					                     //   
//          3:  Enable Speed                                                 //
//          4:  Enable Distance                                              //
//          5:                   				                             //
//          6:  Enable Camera                                            	 //
//          7:                                                               //
///////////////////////////////////////////////////////////////////////////////
#define     RX_SIZE  	                                						12


#define 	COMMAND_SIZE 													12


// Function declarations
void UART_send(void);
void setup_io(void);
static int http_callback(struct mg_connection *conn);
void* launch_server();
void* launch_sensors();
void* launch_UART();
void* launch_STREAMING	();
void Command_Process (void);
void Init_System (void);
int DATA_Process (void);


// Mutexes////////////////////////////////////////////////////////////
pthread_mutex_t userCommandMutex = PTHREAD_MUTEX_INITIALIZER;

//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
char Command_Link[COMMAND_SIZE];
char UART_OUT[TX_SIZE];
char Command[COMMAND_SIZE];
//////////////////////////////////////////////////////////////////////


//////////////////////////////
int RX_Rate;
int RX_Timing;
int TX_Rate;
int TX_Timing;
int HTML_Rate;
int HTML_Timing;
int SensorPost_Rate;
int SensorPost_Timing;
//////////////////////////////
int LEDTiming;
//////////////////////////////
int Front_Sona, Back_Sona, STATUS;
//////////////////////////////

//////////////////////////////
int EN_Camera, EN_Speed, EN_Sona, EN_Distance, EN_LED, SYSTEM_St;
//////////////////////////////

//////////////////////////////
int CAMST=0;
int RequestedCAMx;
int RequestedCAMy;
int RequestedCAMx_Link;
int RequestedCAMy_Link;
int Camera_X=0;
int Camera_Y=0;
int Servo_Speed;
int ROTATE;
#define 		HALT		0
#define 		UP 			1
#define 		DOWN		2
#define 		LEFT 		3
#define 		RIGHT		4
#define 		UP_LEFT		5
#define 		UP_RIGHT	6
#define 		DOWN_LEFT	7
#define 		DOWN_RIGHT	8
//////////////////////////////

//////////////////////////////
int Motor_L1, Motor_L2, Motor_R1, Motor_R2;
int RequestedSpeed 	= 	100;
int RequestedSpeed_Link;
int DIRECTION=0;
// #define 		HALT		0
// #define 		UP 			1
// #define 		DOWN		2
// #define 		LEFT 		3
// #define 		RIGHT		4
// #define 		UP_LEFT		5
// #define 		UP_RIGHT	6
// #define 		DOWN_LEFT	7
// #define 		DOWN_RIGHT	8
//////////////////////////////

//////////////////////////////
int Left_ENCODER[2];
int Right_ENCODER[2];

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
int STATUS;
int fd;
					

void Command_Process (void)
	{
		pthread_mutex_lock( &userCommandMutex );
		strcpy(&Command[0], &Command_Link[0]);
		RequestedSpeed = RequestedSpeed_Link;
		Camera_X = RequestedCAMx_Link;
		Camera_Y = RequestedCAMy_Link;
		pthread_mutex_unlock( &userCommandMutex );
		
			if			((Command[0] == '1')&&(Command[1] == '1'))	DIRECTION = HALT;	
			else if		((Command[2] == '1')&&(Command[3] == '1'))	DIRECTION = HALT;
			else if		((Command[0] == '1')&&(Command[2] == '1'))	DIRECTION = UP_LEFT;
			else if 	((Command[0] == '1')&&(Command[3] == '1'))	DIRECTION = UP_RIGHT;
			else if 	((Command[1] == '1')&&(Command[2] == '1'))	DIRECTION = DOWN_LEFT;
			else if 	((Command[1] == '1')&&(Command[3] == '1'))	DIRECTION = DOWN_RIGHT;	
			else if		(Command[1] == '1')							DIRECTION = DOWN;	
			else if		(Command[0] == '1')							DIRECTION = UP;	
			else if 	(Command[2] == '1')							DIRECTION = LEFT;	
			else if 	(Command[3] == '1') 						DIRECTION = RIGHT;	
			else 													DIRECTION = HALT;
			

						
			if (Command[4] == '1')								EN_Sona=1;			//ULTRASONIC						
				else 											EN_Sona=0;											
			if (Command[5] == '1')								EN_Speed=1;			//SPEED
				else 											EN_Speed=0;	
			if (Command[6] == '1')								EN_Distance=1;		//DISTANCE
				else 											EN_Distance=0;	
			if (Command[7] == '1')								EN_Camera=1;		//CAMERA
				else 											EN_Camera=0;
			if (Command[8] == '1')								EN_LED=1;			//CAMERA
				else 											EN_LED=0;		
			if (Command[9] == '1')								SYSTEM_St=1;		//POWER
				else if (Command[9] == '2')						SYSTEM_St=2;		
				else 											SYSTEM_St=0;	
						
										
	}
void UART_send()
	{
		int cnt=0 ;
		////////////Prepare DATA OUT for UART////////////
		UART_OUT [0] = 0xFF;
		UART_OUT [1] = 0xFF;
		UART_OUT [2] = Motor_L1;
		UART_OUT [3] = Motor_L2;
		UART_OUT [4] = Motor_R1;
		UART_OUT [5] = Motor_R2;
		UART_OUT [6] = Camera_X;
		UART_OUT [7] = Camera_Y;

		if (EN_Sona)		UART_OUT[TX_SIZE-1] = UART_OUT[TX_SIZE-1] | bit1;
			else 			UART_OUT[TX_SIZE-1] = UART_OUT[TX_SIZE-1] & ((~bit1)&0xFF);
		if (EN_Speed)		UART_OUT[TX_SIZE-1] = UART_OUT[TX_SIZE-1] | bit3;
			else 			UART_OUT[TX_SIZE-1] = UART_OUT[TX_SIZE-1] & ((~bit3)&0xFF);
		if (EN_Distance)	UART_OUT[TX_SIZE-1] = UART_OUT[TX_SIZE-1] | bit4;
			else 			UART_OUT[TX_SIZE-1] = UART_OUT[TX_SIZE-1] & ((~bit4)&0xFF);
		if (EN_Camera)		UART_OUT[TX_SIZE-1] = UART_OUT[TX_SIZE-1] | bit6;
			else 			UART_OUT[TX_SIZE-1] = UART_OUT[TX_SIZE-1] & ((~bit6)&0xFF);		
		////////////SEND DATA OUT ////////////////////////	
		while (cnt < TX_SIZE)
			{
				// if (cnt>1) printf ("%3d ",UART_OUT[cnt]);  
				serialPutchar (fd, UART_OUT[cnt++]);
				delay (10);
			}
		TX_Rate = millis() - TX_Timing;
		TX_Timing=millis();
	}
void* launch_server() 				
	{
		struct mg_context *ctx;
		struct mg_callbacks callbacks;

		const char *options[] = {"listening_ports", "3000", NULL};
		memset(&callbacks, 0, sizeof(callbacks));
		callbacks.begin_request = http_callback;

		ctx = mg_start(&callbacks, NULL, options);
	}
static int http_callback(struct mg_connection *conn) 
	{
		int m=3;
		int mark=0;
		int temptspeed=0;
		int temptCAMx=0;
		int temptCAMy=0;
		const struct mg_request_info *request_info = mg_get_request_info(conn);
		char tempCommand [COMMAND_SIZE+3];
		
		strncpy(&tempCommand[0], &request_info->query_string[0], COMMAND_SIZE+3);
		tempCommand[COMMAND_SIZE+3] = 0;
		if ((tempCommand[0] == 's') && (tempCommand[1] == 'e') && (tempCommand[2] == 't'))
				{
				//////////////////////////////////		Make sure network is connected
					digitalWrite (LEDG, HIGH);				
					HTML_Rate=millis()-HTML_Timing;
					HTML_Timing=millis();
				//////////////////////////////////
					
					pthread_mutex_lock( &userCommandMutex );
					strncpy(&Command_Link[0], &tempCommand[3], COMMAND_SIZE);
					pthread_mutex_unlock( &userCommandMutex );
					// Send an HTTP header back to the client
					//mg_printf(conn, "HTTP/1.1 200 OK\r\n"  "Content-Type: text/plain\r\n\r\n");
				}
		if ((tempCommand[0] == 's') && (tempCommand[1] == 'p') && (tempCommand[2] == 'e'))
				{
				m=3;
					while (m<COMMAND_SIZE)
					{
						if (tempCommand[m++] != '|') 
							{
								temptspeed = temptspeed*10;
								temptspeed = temptspeed + tempCommand[m-1]-48;
							}
							else 
								{
									//////////////////////////////////		Make sure network is connected	
										digitalWrite (LEDG, HIGH);	
										HTML_Rate=millis()-HTML_Timing;
										HTML_Timing=millis();
									//////////////////////////////////
									m = COMMAND_SIZE;
									pthread_mutex_lock( &userCommandMutex );
									RequestedSpeed_Link=temptspeed;
									pthread_mutex_unlock( &userCommandMutex );
								}
					}
					// Send an HTTP header back to the client
					//mg_printf(conn, "HTTP/1.1 200 OK\r\n"  "Content-Type: text/plain\r\n\r\n");
				}
				
		if ((tempCommand[0] == 'c') && (tempCommand[1] == 'a') && (tempCommand[2] == 'm'))
				{
				m=3;
					while (m<COMMAND_SIZE)
					{
						if (tempCommand[m] != 'x') 
							{
								if (m>3) 	temptCAMx = temptCAMx*10;
								temptCAMx = temptCAMx + tempCommand[m]-48;
								mark=++m+1;
							} 
							else while (m<COMMAND_SIZE)
								{
									if (tempCommand[++m] != 'y') 
										{
											if (m>mark)		temptCAMy = temptCAMy*10;
											temptCAMy = temptCAMy + tempCommand[m]-48;
										}
										else 
											{
												//////////////////////////////////		Make sure network is connected
													digitalWrite (LEDG, HIGH);		
													HTML_Rate=millis()-HTML_Timing;
													HTML_Timing=millis();
												//////////////////////////////////
												m = COMMAND_SIZE;        
												pthread_mutex_lock( &userCommandMutex );
												RequestedCAMx_Link=temptCAMx-45;
												RequestedCAMy_Link=temptCAMy-45;
												pthread_mutex_unlock( &userCommandMutex );
											}
								}
								
					}
					// Send an HTTP header back to the client
					//mg_printf(conn, "HTTP/1.1 200 OK\r\n"  "Content-Type: text/plain\r\n\r\n");
				}

		return 1;
	}
void* launch_sensors() 
	{
			while(1)
				{
					if(EN_Sona)  //Sona write
						{
							FILE* f = fopen("/var/www/sona.txt", "w");
							fprintf(f, "Front Range: %03d&nbsp;cm <br /> Back Range: %03d&nbsp;cm", Front_Sona,Back_Sona );
							fclose(f);
						}
						else 
							{				
							FILE* f = fopen("/var/www/sona.txt", "w"); 
							fprintf(f, " ");
							fclose(f);
							}
							
					if(EN_Speed) //SPEED
						{
							FILE* f = fopen("/var/www/SPEED.txt", "w");
							fprintf(f, "%04d&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp%04d", Left_Speed,Right_Speed );
							fclose(f);
						}
						else 
							{				
							FILE* f = fopen("/var/www/SPEED.txt", "w");
							fprintf(f, " ");
							fclose(f);
							} 
					if(EN_Distance) //DISTANCE
						{
							FILE* f = fopen("/var/www/DISTANCE.txt", "w");
							fprintf(f, "%d&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp%d", Left_Distance,Right_Distance );
							fclose(f);
						}
						else 
							{	
							Left_Distance	=0;						
							FILE* f = fopen("/var/www/DISTANCE.txt", "w");
							fprintf(f, " ");
							fclose(f);
							} 
							SensorPost_Rate=millis()-SensorPost_Timing;
							SensorPost_Timing=millis();
				}
	}

void* launch_STREAMING()  
	{
		if (EN_Camera)
			{
				if(~CAMST)
					{
					CAMST=1;
					system("sudo /home/pi/Raspi_Project/Raspibot/CAMERA start");
					digitalWrite (LEDW, HIGH);
					 while (CAMST);
					}
			}		
		else
			{
				Camera_X = 0;
				Camera_Y = 0;
				if(CAMST)
					{
					CAMST=0;
					system("sudo /home/pi/Raspi_Project/Raspibot/CAMERA stop");
					}
			}
	}
void* launch_UART()  
	{
		int byte0, byte1, byte2;
		int	cnts=0;
		char tempUART[RX_SIZE];
		while(1)
			{			
				if (cnts>=RX_SIZE) 			
					{		
							Front_Sona 			= 		(tempUART[3]<<8) | tempUART[4];
							Back_Sona  			= 		(tempUART[5]<<8) | tempUART[6];
							Left_ENCODER [0]  	= 		tempUART[7]; 
							Left_ENCODER [1]  	= 		tempUART[8];
							Right_ENCODER [0]	= 		tempUART[9]; 
							Right_ENCODER [1]	=		tempUART[10];
							STATUS  			= 		tempUART[RX_SIZE-1];
							cnts=0;									
					}
					else if (cnts>2) 
						{
							tempUART[cnts++] = serialGetchar (fd);
							// printf ("%2x ",tempUART[cnts-1]);
						}
						else 
							{
								byte0 = byte1;
								byte1 = byte2;
								byte2 = serialGetchar (fd);
													
								if ((byte0 == 0xFF) &&(byte1 == 0xFF)&&(byte2 == 0x00))
									{
										cnts	=0;								
										tempUART[cnts++]=0xFF;
										tempUART[cnts++]=0xFF;
										tempUART[cnts++]=0x00;
										///////////////////////////
										digitalWrite (LEDB, HIGH);		
										RX_Rate=millis()-RX_Timing;
										RX_Timing=millis();
										
										// printf("\n");
									}
							}			
					
			}
	}
int DATA_Process (void)
	{	
	{///SPEED Process///////////////////////////////////////////////////////////////////
			if (EN_Speed==0)	
				{
					Left_Speed=0;
					Right_Speed=0;
				}	
				else if ((STATUS&bit3)==bit3 )		//SPEED READ
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
				else if ((STATUS&bit4)==bit4)			//DISTANCE READ
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
			int STREAMING_THREAD_ExitCode = pthread_create( &STREAMING_THREAD, NULL, &launch_STREAMING, (void*) NULL);			
		}
	}///////////////////////////////////////////////////////////////////////////////////
	
	{///LED Process////////////////////////////////////////////////////////////////////
			if (EN_LED)					digitalWrite (7, HIGH);
				else 					digitalWrite (7, LOW);
	}///////////////////////////////////////////////////////////////////////////////////

	{///Power Process////////////////////////////////////////////////////////////////////
			if (SYSTEM_St==1)
				{
					 system("sudo poweroff");
					return 1;
				}
			else if (SYSTEM_St==2)
				{
					 system("sudo reboot");
					return 2;
				}
				else return 0;
	}///////////////////////////////////////////////////////////////////////////////////
	}
int  main(int argc, char **argv) 
{ 
	{////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			printf("\n####################################################");
			printf("\n##              ==  |-----------|  ==             ##");
			printf("\n##              ==-<| RASPI-BOT |>-==             ##");
			printf("\n##              ==  |-----------|  ==             ##"); 
			printf("\n####################################################\n\n\n\n");
		  	printf("\n  Vo Nguyen Duy \n");    
			printf("\n  vnd241092@gmail.com   \n\n\n");
			//Setup WiringPi	
			if (wiringPiSetup () == -1)	{
											printf ("Unable to start wiringPi\n") ;
											return 1;
										}
			//Setup UART GPIO
			else if 	((fd = serialOpen ("/dev/ttyAMA0", 19200)) < 0)	{
																			printf ("Unable to open serial device\n") ;
																			return 2;
																		}
			//Setup LED GPIO		
			pinMode (LEDG, OUTPUT) ;
			pinMode (LEDB, OUTPUT) ;
			pinMode (7, OUTPUT) ;
			digitalWrite (LEDG, LOW);
			digitalWrite (LEDB, LOW);
			digitalWrite (LEDW, LOW);
			digitalWrite (7, HIGH);	
			printf("\n\n\n####################################################\n");
			printf("#                                                  #\n");
			printf("#                  ...RUNNING...                   #\n");		
			printf("#                                                  #\n");
			printf("####################################################\n\n\n");
			
			
			pthread_t HTTP_THREAD; 
			int HTTP_THREAD_ExitCode = pthread_create( &HTTP_THREAD, NULL, &launch_server, (void*) NULL);
			pthread_t SensorReport_THREAD; 
			int SensorReport_THREAD_ExitCode = pthread_create( &SensorReport_THREAD, NULL, &launch_sensors, (void*) NULL);
			pthread_t UART_THREAD; 
			int UART_THREAD_ExitCode = pthread_create( &UART_THREAD, NULL, &launch_UART, (void*) NULL);	
	}///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		while(1) 
			{	
			
				Command_Process();
				STATUS=DATA_Process();	if (STATUS!=0)	return STATUS;
				UART_send();
				
				if(1)
					{//////////////////////////////////
						printf ("SP:%4d SE:%1d     ",
														RequestedSpeed,
														Servo_Speed);
						printf ("%3d %3d - %05d %05d - %05d %05d - <|%d%d%d%d%d%d%d%d|>       ",
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
						printf ("TX:%3d RX:%3d HTML:%3d Sensor:%3d",
																	TX_Rate,
																	RX_Rate,
																	HTML_Rate, 
																	SensorPost_Rate);
						printf("\n");
					}//////////////////////////////////
					digitalWrite (LEDW, LOW);
					digitalWrite (LEDB, LOW);
					digitalWrite (LEDG, LOW);
			}
			return 0;
}