#include "INCLUDE.h"

static void UI_delay(u32 nCount)
	{
		u32 index = 0;

		/* default system clock is 168MHz */
		for(index = (168000 * nCount); index != 0; index--)
		{
		}
	}


void Display_Servotest(uint32_t step)
	{
		uint32_t 				x 	= (Servo_MAX+Servo_MIN)/2;
		uint32_t				y		=	(Servo_MAX+Servo_MIN)/2;
		uint32_t				add;
		LCD_Clear();
		LCD_GotoXY(0,0);		printf("=====Servo Test=====");	
		LCD_GotoXY(3,0);		printf("--------------------");
		ServoX_Update(x);
		ServoY_Update(y);	
		while (1)
		{
			add= (Servo_MAX-Servo_MIN)/step;

			if ( x>= Servo_MAX)		x = Servo_MIN;
			else								 	x=x+add;
			y=x;

			LCD_GotoXY(2,2);		printf("%06d    %06d",x,y);
			ServoX_Update(x);
			ServoY_Update(y);
		}
	}

void Display_Motortest(void)
	{
		Motor_DIR dirL, dirR;
		float speedL, speedR;
		int16_t	dutyL, DutyR;
		LCD_GotoXY(0,0);				printf("Motor Test");
		
		LCD_GotoXY(2,0);				printf("Left  :%0.1f",speedL); 
		LCD_GotoXY(3,0);				printf("Right :%0.1f",speedR);
		while (1)
			{
				if 	(Get_Button_State(&Button_A) == pressed) 	
					{
						speedL += 0.1f;
						LCD_GotoXY(2,7);				printf("%0.1f  ",speedL); 
					}
				else if 	(Get_Button_State(&Button_B) == pressed) 	
					{
						speedL -= 0.1f;
						LCD_GotoXY(2,7);				printf("%0.1f  ",speedL); 
					}
				else	if 	(Get_Button_State(&Button_C) == pressed) 
					{				
						speedR += 0.1f;
						LCD_GotoXY(3,7);				printf("%0.1f  ",speedR); 
					}
				else if 	(Get_Button_State(&Button_D) == pressed) 	
					{
						speedR -= 0.1f;
						LCD_GotoXY(3,7);				printf("%0.1f  ",speedR); 
					} 
					
					if 			((dirL==Backward)&(speedL>0.0f)) 	dirL = Forward;
					else if ((dirL==Forward)&(speedL<0.0f)) 	dirL = Backward;
					
					if 			((dirR==Backward)&(speedR>0.0f)) 	dirR = Forward;
					else if ((dirR==Forward)&(speedR<0.0f)) 	dirR = Backward;
					
				if 			(Left_Speed() > speedL)				L_Motor_Update(dirL,--dutyL);					
				else if (Left_Speed() < speedL)				L_Motor_Update(dirL,++dutyL);
					
				if 			(Right_Speed() > speedL)				R_Motor_Update(dirR,--DutyR);					
				else if (Right_Speed() < speedL)				R_Motor_Update(dirR,++DutyR);
					LCD_GotoXY(2,15);		printf ("%.2f   ",Left_Speed());
					LCD_GotoXY(3,15);		printf ("%.2f   ",Right_Speed());
			}
	}

#define serialtestmax 16
	uint8_t  size=16;
	USART * serial;
void Display_SERIALtest(void)
	{			
		uint8_t i, max = serialtestmax, TX_USART[serialtestmax], RX_USART[serialtestmax];
		serial = &USART_3;
			LCD_GotoXY(0,0);		printf("USART TEST..........");	
			if 			(Get_Button_State(&Button_B) != released) 
				{	if (size<max)				size++;}
			else if (Get_Button_State(&Button_A) != released) 
				{ if (size>0)					size--;}
			LCD_GotoXY(1,0);		printf("size: %2d/%d",size,max);	

		if (Get_Button_State(&Button_A) == released && Get_Button_State(&Button_B) == released)
		{		
				LCD_GotoXY(3,0);		printf("RX is initialized ");	
			USART_Set(serial, USART_RX, RX_USART, size,USART_MODE_DMASingle);	///////////////////////
			LCD_GotoXY(3,0);		printf("                  ");		
		}			
		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////		
		while (Get_Button_State(&Button_A) == released && Get_Button_State(&Button_B) == released)
		{
			LCD_GotoXY(0,0);		printf("USART TEST..........");	
			
			for (i=0; i<serialtestmax; i++)		TX_USART[i] = i + 'a'+ (uint8_t) (Get_msTick()&0xf);		
			for (i=0; i<serialtestmax; i++)		RX_USART[i] = 0;
			
			LCD_GotoXY(2,0);		printf("TX: ");	
			for (i=0; i<serialtestmax; i++)		printf("%c",TX_USART[i]);
			USART_Set(serial, USART_TX, TX_USART, size,USART_MODE_DMASingle);//////////////////////////
			
			LCD_GotoXY(0,0);		printf("USART TEST          ");	
			LCD_GotoXY(3,0);		printf("RX: ");	
			for (i=0; i<serialtestmax; i++)		printf("%c",RX_USART[i]);
		}
			LCD_Clear();
	}


Gyro acceleration;
Acce gyro;
Compass compact;
float Tempe;
MPU6050_Status State6050;
HMC5883L_Status HMC5883L;
void Display_IMU (void)
	{		
 					State6050 = MPU6050_Get_rawDATA (&acceleration,&gyro,&Tempe);  
 		
 					LCD_GotoXY(0,0);		printf("IMU Test ");
 					LCD_GotoXY(0,13);		printf("%.2f*C",Tempe);	 
 		
 					LCD_GotoXY(1,0);		printf("%6d",acceleration.x);
 					LCD_GotoXY(2,0);		printf("%6d",acceleration.y);
 					LCD_GotoXY(3,0);		printf("%6d",acceleration.z);

 					LCD_GotoXY(1,7);		printf("%6d",gyro.x);
 					LCD_GotoXY(2,7);		printf("%6d",gyro.y);
 					LCD_GotoXY(3,7);		printf("%6d",gyro.z);	 
 					if (State6050 == MPU6050_STATUS_FAIL ) LCD_Clear();
		
 					HMC5883L = HMC5883L_Get_RawCompact(&compact);
		
 					LCD_GotoXY(1,14);		printf("%6d",compact.x);
 					LCD_GotoXY(2,14);		printf("%6d",compact.y);
 					LCD_GotoXY(3,14);		printf("%6d",compact.z);	 
 					if (HMC5883L == HMC5883L_STATUS_FAIL ) LCD_Clear();
	}

uint16_t i, page, error ; 
uint8_t TX_EPP[Page_Size], RX_EPP[Page_Size];
void Display_EEPROMtest(void)
	{
		uint16_t Total = Total_Page;
		EEPROM_Status State;	
		LCD_GotoXY(0,0);				printf("EEPROM TEST....");
		LCD_GotoXY(1,0);				printf("Reset Memory");
// 		for (page = 0; page<Total_Page;  page++)
// 		{
// 			EEPROM_WritePage (TX_EPP, page);
// 			LCD_GotoXY(1,14);	 printf("%d %",page*100/Total);
// 		}
		
		page=0;
		
		while (page<Total_Page)
			{
				LCD_GotoXY(0,0);				printf("EEPROM TEST...."); 		
				
				for (i=0; i<Page_Size; i++)		TX_EPP[i] =(uint8_t) (Get_msTick()&0xff)+ (uint8_t)i;

				LCD_GotoXY(1,0);	printf("PAGE: %5d/%d",page,Total);  
				LCD_GotoXY(2,0);	printf("DATA: %3d",TX_EPP[0]&0xff);  
				State = EEPROM_WritePage (TX_EPP, page);
				
				if (State == EEPROM_Status_FAIL)	
					{				
						LCD_GotoXY(2,10);	printf("WRITE FAIL");   
					}
					else				
							{				
								LCD_GotoXY(2,10);	printf("WRITE OK  ");  
								State = EEPROM_ReadPage (RX_EPP, page);	
								if (State == EEPROM_Status_FAIL)	
									{				
										LCD_GotoXY(2,10);	printf("READ FAIL");   
									}
									else 
										{
											LCD_GotoXY(2,10);	printf("READ OK  ");  
											page = page+1;/////////////////////////////
											LCD_GotoXY(2,10);	printf("          ");
											for (i=0; i<Page_Size; i++)				if	(TX_EPP[i] != RX_EPP[i]) 	error++;
											LCD_GotoXY(3,0);	printf("Error: %5d",error);  
										}
						}			
				}
				
		while (page>=Total_Page)
			{				
				LCD_GotoXY(0,0);		printf("    EEPROM TEST     ");  
				LCD_GotoXY(1,0);		printf("     COMPLETE!      ");  
				LCD_GotoXY(2,0);		printf("   Press any key... ");  
				while (Button_OK.State != pressed);
				page=0;
			}						
	}

void Display_SONAtest(void)
	{
		LCD_GotoXY(0,0);				printf("ULTRASONIC TEST......");  	
		LCD_GotoXY(1,0);				printf("Unit:   cm   inch"); 
		LCD_GotoXY(2,0);				printf("   A:");
		LCD_GotoXY(3,0);				printf("   B:");
		while (Get_Button_State(&Button_OK) == released)
			{
				LCD_GotoXY(2,7);				printf("%3d" ,(int)Sona_A.cm); 
				LCD_GotoXY(3,7);				printf("%3d"	,(int)Sona_B.cm); 
				
				if			(Get_Button_State(&Button_A) == pressed)
					{
						Sona_TurnOn();
						LCD_GotoXY(0,0);				printf("ULTRASONIC TEST   ON");  	
					}
				else if	(Get_Button_State(&Button_B) == pressed)
					{
						Sona_TurnOff();
						LCD_GotoXY(0,0);				printf("ULTRASONIC TEST  OFF");  
					}
				
			}
	}


Display Display_VoltFB(void)
	{
		int Cell ;
		float Voltage;

		UI_delay(30);
		Cell = Cell_Detect();
		LCD_GotoXY(0,0);			printf ("%d-Cell Li-Po Battery",Cell);
		Voltage = Get_Voltage();
		LCD_GotoXY(2,0);			printf ("Voltage:    %0.1f V ",Voltage);

		UI_delay(200);
	}


Display Display_RTC(void)
	{
		int16_t Edit_Add;
		RCT EditRTC;
		LCD_Clock.enable=1;									//enable counter in timer
		LCD_Clear();
		LCD_GotoXY(0,0);		printf("===DIGITAL  CLOCK===");		
		LCD_GotoXY(3,0);		printf("====================");
		while (LCD_Clock.enable)					//4:stop to edit
			{			
				if (LCD_Clock.edit) 	//show editing value
					{
						if (Get_Button_State(&Button_OK)==pressed)					
							{		
								while (Button_OK.State == pressed);
								LCD_GotoXY(2,3);		printf("              ");										
								// save to current time
								LCD_Clock.hour 		= EditRTC.hour;
								LCD_Clock.minute 	= EditRTC.minute;
								LCD_Clock.second 	= EditRTC.second;
								LCD_Clock.edit = 0;
							}
								else
									{
										if 	(Get_Button_State(&Button_A) == pressed) 	
											{										
												while (Button_A.State == pressed);
												LCD_Clock.edit--;
												if 	(LCD_Clock.edit<=0)	
														LCD_Clock.edit = 3;
											}
										if (Get_Button_State(&Button_B) == pressed)  
											{										
												while (Button_B.State == pressed);
												LCD_Clock.edit++;
												if (LCD_Clock.edit>=4) 
														LCD_Clock.edit = 1;
											}									
										if (Get_Button_State(&Button_D) == pressed) 		
											{
												Edit_Add = -1 ;
											}
										if (Get_Button_State(&Button_C) == pressed) 	
											{
												Edit_Add = 1 ;
											}
							
										/// Time edit mark
										switch (LCD_Clock.edit)
											{
												case 1: 		
																		LCD_GotoXY(1,4); 																		printf("  ");
																		if 			((EditRTC.hour<=60)&&(EditRTC.hour>=0))			EditRTC.hour		+=	Edit_Add;
																		Edit_Add=0;
																		if 			(EditRTC.hour>=60)													EditRTC.hour=0;
																		else if (EditRTC.hour<0)														EditRTC.hour=59;
																		LCD_GotoXY(1,4);	printf("%02d",EditRTC.hour);
																		break;
												case 2: 		
																		LCD_GotoXY(1,9); 																		printf("  ");
																		if 			((EditRTC.minute<=60)&&(EditRTC.minute>=0))	EditRTC.minute		+=	Edit_Add;
																		Edit_Add=0;
																		if 			(EditRTC.minute>=60)												EditRTC.minute=0;
																		else if (EditRTC.minute<0)													EditRTC.minute=59;		
																		LCD_GotoXY(1,9);	printf("%02d",EditRTC.minute);
																		break;
												case 3: 		
																		LCD_GotoXY(1,14); 																		printf("  ");
																		if 			((EditRTC.second<=60)&&(EditRTC.second>=0))	EditRTC.second		+=	Edit_Add;
																		Edit_Add=0;
																		if 			(EditRTC.second>=60)												EditRTC.second=0;
																		else if (EditRTC.second<0)													EditRTC.second=59;				
																		LCD_GotoXY(1,14);	printf("%02d",EditRTC.second);
																		break;
											}
								}
					}
					else	//show counting value
						{
							LCD_GotoXY(1,4);	printf("%02d : %02d : %02d",LCD_Clock.hour, LCD_Clock.minute, LCD_Clock.second);
							if (Get_Button_State(&Button_OK)==pressed)					
								{		
									while (Button_OK.State == pressed);
									EditRTC.hour 		= LCD_Clock.hour;
									EditRTC.minute 	= LCD_Clock.minute;
									EditRTC.second 	= LCD_Clock.second;
									LCD_Clock.edit = 1;
								}
						}
			}	
	}


Display Display_StartUp(void)
	{
		LCD_Clear();
		LCD_GotoXY(0,0);		printf("=======BalaPi=======");	
		LCD_GotoXY(1,0);		printf("              Duy Vo");
		LCD_GotoXY(3,0);		printf("vnd241092 @gmail.com");
		UI_delay (300);
	}


