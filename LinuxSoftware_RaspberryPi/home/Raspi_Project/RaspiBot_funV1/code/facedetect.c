#include "include.h"

int EN_Facedetect=3;
int pantit;
int FACE_X=0;
int FACE_Y=13;

int	Old_Camset_x;
int	Old_Camset_y;
int FACE_timeout;
int numface;
int x;
int y;
int old_x;
int old_y;
int w;
int h;
int old_w;
int old_h;

int t;
int s=3;

int newframe=0;
	MMAL_COMPONENT_T *camera = 0;
	MMAL_ES_FORMAT_T *format;
	MMAL_STATUS_T status;
	MMAL_PORT_T  *camera_video_port = NULL, *camera_still_port = NULL;
	MMAL_PORT_T *preview_input_port = NULL;
	MMAL_POOL_T *camera_video_port_pool;

typedef struct 
				{
					int video_width;
					int video_height;
					int preview_width;
					int preview_height;
					int opencv_width;
					int opencv_height;
					float video_fps;
					MMAL_POOL_T *camera_video_port_pool;
					CvHaarClassifierCascade *cascade;
					CvMemStorage* storage;
					IplImage* image;
					IplImage* image2;
					VCOS_SEMAPHORE_T complete_semaphore;
				} PORT_USERDATA;

PORT_USERDATA userdata={
							.video_width = 1024   / 1,
							.video_height = 720   / 1,
							.opencv_width = 1024  / 4,
							.opencv_height = 720  / 4,
						};
			
			

static void video_buffer_callback(MMAL_PORT_T *port, MMAL_BUFFER_HEADER_T *buffer)
{
    static int frame_count = 0;
    static int frame_post_count = 0;
    static struct timespec t1;
    struct timespec t2;
    MMAL_BUFFER_HEADER_T *new_buffer;
    PORT_USERDATA * userdata = (PORT_USERDATA *) port->userdata;
    MMAL_POOL_T *pool = userdata->camera_video_port_pool;


    mmal_buffer_header_mem_lock(buffer);
    memcpy(userdata->image->imageData, buffer->data, userdata->video_width * userdata->video_height);
    mmal_buffer_header_mem_unlock(buffer);
   
    if (vcos_semaphore_trywait(&(userdata->complete_semaphore)) != VCOS_SUCCESS) 
		{
			vcos_semaphore_post(&(userdata->complete_semaphore));
			frame_post_count++;
		}

    mmal_buffer_header_release(buffer);
    // and send one back to the port (if still open)
    if (port->is_enabled) 
	{
        MMAL_STATUS_T status;

        new_buffer = mmal_queue_get(pool->queue);

        if (new_buffer)
            status = mmal_port_send_buffer(port, new_buffer);

        if (!new_buffer || status != MMAL_SUCCESS)
            printf("Unable to return a buffer to the video port\n");
    }
}

int FacedetectPrepare(int rotation)
{
	EN_Facedetect=0;
    // bcm_host_init();
    
    /* setup opencv */
    userdata.cascade = (CvHaarClassifierCascade*) cvLoad("/home/pi/Raspi_Project/Raspibot/haarcascade_frontalface_alt_tree.xml", NULL, NULL, NULL);
    userdata.storage = cvCreateMemStorage(0);
    userdata.image = cvCreateImage(cvSize(userdata.video_width, userdata.video_height), IPL_DEPTH_8U, 1);
    userdata.image2 = cvCreateImage(cvSize(userdata.opencv_width, userdata.opencv_height), IPL_DEPTH_8U, 1);
    
	if (!userdata.cascade) 			printf("Error: unable to load harrcascade\n");
	
	/////////////////////////////////////////////////////////////////////////////////////
	status = mmal_component_create(MMAL_COMPONENT_DEFAULT_CAMERA, &camera);
	if (status != MMAL_SUCCESS) 
		{
			printf("Error: create camera %x\n", status);
			return 0;
		}

    camera_video_port = camera->output[MMAL_CAMERA_VIDEO_PORT];
    camera_still_port = camera->output[MMAL_CAMERA_CAPTURE_PORT];
			
	/////////////////////////////////////////////////////////////////////////////////////    

	
	MMAL_PARAMETER_CAMERA_CONFIG_T cam_config = {
													{ MMAL_PARAMETER_CAMERA_CONFIG, sizeof (cam_config)},
													.max_stills_w = 1280,
													.max_stills_h = 720,
													.stills_yuv422 = 0,
													.one_shot_stills = 0,
													.max_preview_video_w = 1280,
													.max_preview_video_h = 720,
													.num_preview_video_frames = 2,
													.stills_capture_circular_buffer_height = 0,
													.fast_preview_resume = 1,
													.use_stc_timestamp = MMAL_PARAM_TIMESTAMP_MODE_RESET_STC
												};

	mmal_port_parameter_set(camera->control, &cam_config.hdr);
   

	// if(0)
		// {
			// MMAL_PARAMETER_MIRROR_T mirror = 	{	
													// {MMAL_PARAMETER_MIRROR, sizeof(MMAL_PARAMETER_MIRROR_T)},
													// .value= MMAL_PARAM_MIRROR_VERTICAL
												// };
											
			// mmal_port_parameter_set(camera->output[0], &mirror.hdr);
			// mmal_port_parameter_set(camera->output[1], &mirror.hdr);
			// mmal_port_parameter_set(camera->output[2], &mirror.hdr);
		// }
		if(rotation)
		{
	
    rotation = ((rotation % 360 ) / 90) * 90;

   mmal_port_parameter_set_int32(camera->output[0], MMAL_PARAMETER_ROTATION, rotation);
   mmal_port_parameter_set_int32(camera->output[1], MMAL_PARAMETER_ROTATION, rotation);
   mmal_port_parameter_set_int32(camera->output[2], MMAL_PARAMETER_ROTATION, rotation);
		}
	/////////////////////////////////////////////////////////////////////////////////////
    format = camera_video_port->format;

    format->encoding = MMAL_ENCODING_I420;
    format->encoding_variant = MMAL_ENCODING_I420;

    format->es->video.width = userdata.video_width;
    format->es->video.height = userdata.video_width;
    format->es->video.crop.x = 0;
    format->es->video.crop.y = 0;
    format->es->video.crop.width = userdata.video_width;
    format->es->video.crop.height = userdata.video_height;
    format->es->video.frame_rate.num = 30;
    format->es->video.frame_rate.den = 1;

    camera_video_port->buffer_size = userdata.preview_width * userdata.preview_height * 12 / 8;
    camera_video_port->buffer_num = 1;

    
	status = mmal_port_format_commit(camera_video_port);
	if (status != MMAL_SUCCESS) 
		{
			printf("Error: unable to commit camera video port format (%u)\n", status);
			return 0;
		}
	/////////////////////////////////////////////////////////////////////////////////////
	// crate pool form camera video port
    camera_video_port_pool = (MMAL_POOL_T *) mmal_port_pool_create(camera_video_port, camera_video_port->buffer_num, camera_video_port->buffer_size);
    userdata.camera_video_port_pool = camera_video_port_pool;
    camera_video_port->userdata = (struct MMAL_PORT_USERDATA_T *) &userdata;

    
	status = mmal_port_enable(camera_video_port, video_buffer_callback);
	if (status != MMAL_SUCCESS) 
		{
			printf("Error: unable to enable camera video port (%u)\n", status);
			return 0;
		}



    status = mmal_component_enable(camera);
	
	
	// Send all the buffers to the camera video port
	int num = mmal_queue_length(camera_video_port_pool->queue);
	int q;

	for (q = 0; q < num; q++) 
	{
		MMAL_BUFFER_HEADER_T *buffer = mmal_queue_get(camera_video_port_pool->queue);
		

		if (!buffer)
			{
				printf("Unable to get a required buffer %d from pool queue\n", q);
			}
		if (mmal_port_send_buffer(camera_video_port, buffer) != MMAL_SUCCESS)
			{
				printf("Unable to send a buffer to encoder output port (%d)\n", q);
			}
	}

     
		
	if (mmal_port_parameter_set_boolean(camera_video_port, MMAL_PARAMETER_CAPTURE, 1) != MMAL_SUCCESS) 
		{
			printf("%s: Failed to start capture\n", __func__);
		}

    vcos_semaphore_create(&userdata.complete_semaphore, "mmal_opencv_demo-sem", 0);
	
	EN_Facedetect=1;
	return 1;
}

int CloseFacedetect()
{
	EN_Facedetect=2;
	vcos_semaphore_delete(&userdata.complete_semaphore);
	if (camera_video_port && camera_video_port->is_enabled)
		{
			if (mmal_port_disable(camera_video_port)	!=	MMAL_SUCCESS)	
				{printf("fail to disable video port\n"); return 0;}
		}
	
	mmal_port_pool_destroy(camera_video_port,camera_video_port_pool);
				

	if (camera)
		 {
			if (mmal_component_disable(camera)	!=	MMAL_SUCCESS)	
				{printf("fail to  disable camera\n"); return 0;}
			if (mmal_component_destroy(camera)	!=	MMAL_SUCCESS)	
				{printf("fail to  destroy camera\n"); return 0;}
		}
	EN_Facedetect=3;
	return 1;
}

void* launch_Facedetect() 
{
	if (EN_Facedetect==3)	
		{
			FacedetectPrepare(180);
			
			UART_OUT [0] = 0xFF;
			UART_OUT [1] = 0xFF;
			UART_OUT [2] = Motor_L1;
			UART_OUT [3] = Motor_L2;
			UART_OUT [4] = Motor_R1;
			UART_OUT [5] = Motor_R2;
			UART_OUT[TX_SIZE-1]=bit6;
		}
	
	while ((EN_Facedetect==1) && (Function==1))
		{
			
			if (vcos_semaphore_wait(&(userdata.complete_semaphore)) == VCOS_SUCCESS)
				{
					
					cvResize (userdata.image, userdata.image2, CV_INTER_LINEAR);
					CvSeq* objects = cvHaarDetectObjects	(	
															userdata.image2,
															userdata.cascade, 
															userdata.storage, 
															1.4, //fator scale
															3,//min neighbors 
															0, //flag
															cvSize(1024/11, 720/11),
															cvSize(20, 20)
															);
					CvRect* r;			
					if (objects != 0) 
					{
						if (objects->total > 0) 
							{
								int ii;
								for (ii = 0; ii < objects->total; ii++) 
								{
									r = (CvRect*) cvGetSeqElem(objects, ii);	
									
									w	=	r->width;
									h	=	r->height;
									x	=	r->x ;
									y	=	r->y ;
									pantit=1;	
								}
								numface = objects->total ;
								FACE_timeout=0;
							} 
							else 
								{
									FACE_timeout= FACE_timeout + CV_Rate;
									numface = 0 ;
									digitalWrite (LEDG, LOW);
									digitalWrite (LEDB, LOW);
									digitalWrite (LEDW, LOW);
								}	
					}
					else 
						{
							printf("error detected");
						}
					CV_Rate=(millis()-CV_Timing);
					CV_Timing=millis();
					newframe=1;
				}
		}  

	if (EN_Facedetect==1)	CloseFacedetect();
	
	pthread_exit(NULL);
}
	

void Facedetect_Process()
{
	int step_x;
	int step_y;
	int dis_x;
	int dis_y;
	int LED=0;
	
	Command_Process();
	if ((Camera_X!=Old_Camset_x)||(Camera_Y!=Old_Camset_y))
		{
			FACE_X=Camera_X;
			FACE_Y=Camera_Y;
			Old_Camset_x=Camera_X;
			Old_Camset_y=Camera_Y;
		}
		else if (FACE_timeout>3000) 
			FACESearch();
			else if (pantit==1)
				{
					x	=	x + (w/2);
					y	=	y + (h/2);
					
					dis_x =  abs(x - X_mean);
					dis_y =  abs(y - Y_mean);
					
					if 		(dis_x > thr_5)		{ step_x=5;  LED=6;}
					else if	(dis_x > thr_4)		{ step_x=4;  LED=5;}
					else if	(dis_x > thr_3)		{ step_x=3;	 LED=4;}
					else if	(dis_x > thr_2)		{ step_x=2;	 LED=3;}
					else if	(dis_x > thr_1)		{ step_x=1;	 LED=2;}
					else 						{ step_x=0;  LED=1;}
					
					if 		(dis_y > thr_5)		{ step_y=5; }
					else if	(dis_y > thr_4)		{ step_y=4;  }
					else if	(dis_y > thr_3)		{ step_y=3;	 }
					else if	(dis_y > thr_2)		{ step_y=2;	 }
					else if	(dis_y > thr_1)		{ step_y=1;	 }
					else 						{ step_y=0;  }
					
					
					if 		(x > X_mean)	{if	(FACE_X + step_x<= 45) 		FACE_X = FACE_X + step_x;}
					else if	(x < X_mean)	{if	(FACE_X - step_x>=-45) 		FACE_X = FACE_X - step_x;}
					if 		(y > Y_mean)	{if	(FACE_Y - step_y>=-45)  	FACE_Y = FACE_Y - step_y;}
					else if	(y < Y_mean)	{if	(FACE_Y + step_y<= 45)  	FACE_Y = FACE_Y + step_y;}
					
					if (LED==0)	{digitalWrite (LEDB, HIGH); digitalWrite (LEDG, HIGH); digitalWrite (LEDW, HIGH);}
						else 
							{
								if ((LED&0x1)==0x1) 	 digitalWrite (LEDB, HIGH);
								if ((LED&0x2)==0x2)      digitalWrite (LEDG, HIGH);
								if ((LED&0x4)==0x4)      digitalWrite (LEDW, HIGH);
							}
				}
			if (newframe)	
				{		
					printf("%2d face detected  x:%3d y:%3d  |THR = %1d|	   ",  numface, x, y,(LED-1));
					printf ("TX:%3d RX:%3d Se:%3d Ma:%3d CV:%3d",
													TX_Rate,
													RX_Rate,
													SensorPost_Rate,
													Manual_Rate, 
													CV_Rate				);
					printf("\n");
					newframe=0;
				}
			
			
	UART_OUT [6] = FACE_X;
	UART_OUT [7] = FACE_Y;	
			pantit=0;	
}

void FACESearch()
{
	if (newframe==1)
	{
		FACE_X =  FACE_X + s;
		if (FACE_X>45) {FACE_X=45; s=-3;}
		if (FACE_X<-45) {FACE_X=-45; s=3;}
		newframe=2;
	}
	
}