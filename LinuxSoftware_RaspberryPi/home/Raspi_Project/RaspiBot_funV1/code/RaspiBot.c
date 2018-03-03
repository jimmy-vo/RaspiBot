#include "include.h"

// Mutexes////////////////////////////////////////////////////////////
pthread_mutex_t userCommandMutex = PTHREAD_MUTEX_INITIALIZER;

//////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////
char Command_Link[COMMAND_SIZE];
char UART_OUT[TX_SIZE];
char Command[COMMAND_SIZE];
//////////////////////////////////////////////////////////////////////
int count=1;

//////////////////////////////
int RX_Rate;
int RX_Timing;
int TX_Rate;
int TX_Timing;
int HTML_Rate;
int HTML_Timing;
int SensorPost_Rate;
int SensorPost_Timing;
int Manual_Rate;
int Manual_Timing;
int CV_Rate;
int CV_Timing;
//////////////////////////////
int LEDTiming;
int STATUS;
//////////////////////////////

//////////////////////////////
int EN_Camera, EN_Speed, EN_Sona, EN_Distance, EN_LED, Function=1, SYSTEM;
//////////////////////////////

int RequestedSpeed 	= 	100;
int RequestedCAMx_Link;
int RequestedCAMy_Link;
int RequestedSpeed_Link;
//////////////////////////////

//////////////////////////////
int EN_TX, 			 ST_TX;
int EN_sensors,		 ST_sensors;
int EN_RX,			 ST_RX;
//////////////////////////////


//////////////////////////////
int Left_ENCODER[2];
int Right_ENCODER[2];
//////////////////////////////

int error;
int fd;
					

int Command_Process (void)
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
			

							
			if (Command[4] == '1')								EN_Sona=1;				//ULTRASONIC						
				else 											EN_Sona=0;											
			if (Command[5] == '1')								EN_Speed=1;				//SPEED
				else 											EN_Speed=0;	
			if (Command[6] == '1')								EN_Distance=1;			//DISTANCE
				else 											EN_Distance=0;	
			if (Command[7] == '1')								EN_Camera=1;			//CAMERA
				else 											EN_Camera=0;
			if (Command[8] == '1')			{EN_LED=1; digitalWrite (LEDC, HIGH);	}	//LED
				else 						{EN_LED=0; digitalWrite (LEDC, LOW);	}	
		//POWER//////////////////////////////////////////////		
			if 		(Command[9] == '1')				{SYSTEM=1;	system("sudo poweroff"); return 1;} 
			else if (Command[9] == '2')				{SYSTEM=2;	system("sudo reboot");	return 2;}
			else 									SYSTEM=0;	
		//Function///////////////////////////////////////////				
			//FaceDetect	
			if (Command[10] == '1')								{Function=1; }									
			//Manual								
			else 												Function=0; 	
			return 0;
	}
void* launch_TX()
	{
		while (EN_TX==1)
			{	
				int cnt=0 ;
				ST_TX=1;
				while (cnt < TX_SIZE)
					{
						 /// (cnt>1) printf ("%3d ",UART_OUT[cnt]);  
						serialPutchar (fd, UART_OUT[cnt++]);
						delay (10);
					}
				TX_Rate = millis() - TX_Timing;
				TX_Timing=millis();
				//printf ("\n");
			}
		ST_TX=0;	pthread_exit(NULL);
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
		while(EN_sensors==1)
			{
				ST_sensors=1;
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
		ST_sensors=0;		pthread_exit(NULL);
	}
void* launch_RX()  
	{
		int byte0, byte1, byte2;
		int	cnts=0;
		char tempUART[RX_SIZE];
		while(EN_RX==1)
			{		
				ST_RX=1;			
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
		ST_RX=0;		pthread_exit(NULL);
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
				pinMode (LEDW, OUTPUT) ;
				pinMode (LEDC, OUTPUT) ;
				
				
				pthread_t HTTP_THREAD; 
				error = pthread_create( &HTTP_THREAD, NULL, &launch_server, (void*) NULL);
				if (error) 		printf("fail to create launch_server Thread\n");

				
				printf("\n\n\n####################################################\n");
				printf("#                                                  #\n");
				printf("#                  ...RUNNING...                   #\n");		
				printf("#                                                  #\n");
				printf("####################################################\n\n\n");
		}///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			while(1) 
				{	
					if (Command_Process()) return 0;
					if (Function==0) 
						{
							printf ("\n launch_TX: Terminate exist thread, waiting it terminated...\n");
							while ((ST_TX!=0)||(EN_TX!=0)) EN_TX=0;		EN_TX=1;	
							pthread_t TX_THREAD; 			// create new thread
							error = pthread_create( &TX_THREAD, NULL, &launch_TX, (void*) NULL);			
							if (error) 		printf("fail to create launch_UART Thread\n");		

							printf ("\n launch_RX: Terminate exist thread, waiting it terminated...\n");
							while ((ST_RX!=0)||(EN_RX!=0)) EN_RX=0;		EN_RX=1;
							pthread_t RX_THREAD; 			// create new thread
							error = pthread_create( &RX_THREAD, NULL, &launch_RX, (void*) NULL);			
							if (error) 		printf("fail to create launch_UART Thread\n");
								
							printf ("\n launch_sensors: Terminate exist thread, waiting it terminated...\n");		
							while ((ST_sensors!=0)||(EN_sensors!=0)) EN_sensors=0;		EN_sensors=1; 	
							pthread_t SensorReport_THREAD; 			// create new thread
							error = pthread_create( &SensorReport_THREAD, NULL, &launch_sensors, (void*) NULL);		
							if (error) 		printf("fail to create launch_sensors Thread\n");
							
							printf("\nIn Manual MODE\n");		
							while (Function==0) Manual_Process();
							Manual_Clean();
						} 
						
				
					else if (Function==1)		//Facedetect
						{	
								EN_RX=0; EN_sensors=0;
								
								printf ("\n launch_Facedetect: Opening...\n");
								while (EN_Facedetect!=3);
								pthread_t FACEDETECT_THREAD; 
								error = pthread_create( &FACEDETECT_THREAD, NULL, &launch_Facedetect, (void*) NULL);		
								if (error) 		printf("fail to create Facedetect Thread\n");
								
								
								printf ("\n launch_TX: Terminate exist thread, waiting it terminated...\n");
								while ((ST_TX!=0)||(EN_TX!=0)) EN_TX=0;		EN_TX=1;	
								pthread_t TX_THREAD; 			// create new thread
								error = pthread_create( &TX_THREAD, NULL, &launch_TX, (void*) NULL);			
								if (error) 		printf("fail to create launch_UART Thread\n");			
							
							printf("\nIn Face Detection MODE\n");		
							while (Function==1) Facedetect_Process();
						}
					
				}
				return 0;
	}