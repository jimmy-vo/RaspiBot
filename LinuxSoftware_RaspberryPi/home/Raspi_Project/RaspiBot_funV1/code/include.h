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


#include <opencv2/core/core_c.h>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "interface/vcos/vcos.h"
#include "interface/vcos/vcos.h"
#include "bcm_host.h"
#include "interface/mmal/mmal.h"
#include "interface/mmal/util/mmal_default_components.h"
#include "interface/mmal/util/mmal_connection.h"

#include "vgfont.h"



#define LEDG    0
#define LEDB    2
#define LEDW    3
#define LEDC    7





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
//          2:  Enable Speed        					                     //   
//          3:  Enable Distance                                              //
//          4:                                         					     //
//          5:                   				                             //
//          6:  				                                           	 //
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
//          0:        0                                                      //
//          1:  Enable SONA                             		             //
//          2:  Enable Speed        					                     //   
//          3:  Enable Distance                                              //
//          4:  	1			                                             //
//          5:       0            				                             //
//          6:  	1		                                            	 //
//          7:       0                                                        //
///////////////////////////////////////////////////////////////////////////////
#define     RX_SIZE  	                                					12


#define 	COMMAND_SIZE 													13

///////////////////////////////////////////////////////////////////////////////
void* launch_TX();
static int http_callback(struct mg_connection *conn);
void* launch_server();
void* launch_sensors();
void* launch_RX();
int Command_Process (void);
///////////////////////////////////////
extern char Command_Link[COMMAND_SIZE];
extern char UART_OUT[TX_SIZE];
extern char Command[COMMAND_SIZE];

extern int RequestedSpeed ;
extern int RequestedCAMx_Link;
extern int RequestedCAMy_Link;
extern int RequestedSpeed_Link;

extern int RX_Rate;
extern int RX_Timing;
extern int TX_Rate;
extern int TX_Timing;
extern int HTML_Rate;
extern int HTML_Timing;
extern int SensorPost_Rate;
extern int SensorPost_Timing;
extern int Manual_Rate;
extern int Manual_Timing;
extern int CV_Rate;
extern int CV_Timing;

extern int count;

extern int STATUS;

extern int EN_Camera, EN_Speed, EN_Sona, EN_Distance, EN_LED, Function, SYSTEM;
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//MANUAL                                                                     //    
///////////////////////////////////////////////////////////////////////////////
void Manual_Process (void);
void Manual_Clean(void);
void* launch_Streaming();  
///////////////////////////////////////
  
extern int Left_ENCODER[2];
extern int Right_ENCODER[2];
extern int Left_Speed;
extern int Right_Speed;
#define distance_scale 		6.5*3.14159/20
extern int Left_Distance;
extern int Right_Distance;
extern int Left_Distance_TMP;
extern int Right_Distance_TMP;
extern int Left_Distance_CNT;
extern int Right_Distance_CNT;
extern int Left_Distance_IND;
extern int Right_Distance_IND;
extern int Left_Distance_STO;
extern int Right_Distance_STO;

extern int CAMST;
extern int RequestedCAMx;
extern int RequestedCAMy;
extern int RequestedCAMx_Link;
extern int RequestedCAMy_Link;
extern int Camera_X;
extern int Camera_Y;
extern int Servo_Speed;
extern int ROTATE;
#define 		HALT		0
#define 		UP 			1
#define 		DOWN		2
#define 		LEFT 		3
#define 		RIGHT		4
#define 		UP_LEFT		5
#define 		UP_RIGHT	6
#define 		DOWN_LEFT	7
#define 		DOWN_RIGHT	8

extern int Front_Sona, Back_Sona;

extern int Motor_L1, Motor_L2, Motor_R1, Motor_R2;
extern int DIRECTION;
///////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////
//FACE DETECT                                                                //    
/////////////////////////////////////////////////////////////////////////////// 
static void video_buffer_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer);
int FacedetectPrepare(int rotation);
void* launch_Facedetect( ) ;
void Facedetect_Process();
int CloseFacedetect();
void FACESearch();;
///////////////////////////////////////
#define MMAL_CAMERA_PREVIEW_PORT 0
#define MMAL_CAMERA_VIDEO_PORT 1
#define MMAL_CAMERA_CAPTURE_PORT 2

#define 	thr_1 		(858  / 4 /2 ) * 1 / 10
#define 	thr_2 		(858  / 4 /2 ) * 2 / 10
#define 	thr_3 		(858  / 4 /2 ) * 3 / 10
#define 	thr_4 		(858  / 4 /2 ) * 4 / 10
#define 	thr_5 		(858  / 4 /2 ) * 5 / 10

#define 	X_mean		1024  / 4 /2
#define 	Y_mean		720  / 4 /2

extern int EN_Facedetect;
extern int numface;
extern int x;
extern int y;
extern int FACE_X;
extern int FACE_Y;
///////////////////////////////////////////////////////////////////////////////


