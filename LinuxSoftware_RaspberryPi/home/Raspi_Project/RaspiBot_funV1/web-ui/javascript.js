
var command = 
				{
				  'UP' : 	 		0,
				  'DOWN' :			0,
				  'LEFT' : 			0,
				  'RIGHT' : 		0,
				  'ULTRASONIC' : 	1,
				  'SPEED' : 		1,
				  'DISTANCE' : 		1,
				  'CAMERA' : 		1,
				  'LED' : 			1,		
				  'POWER':			0,
				  'FUNCTION':		0,
				}


var CONTROL_PORT = 3000;
var WEBCAM_PORT = 8080;

function load() 
{
	createImageLayer();
 	setInterval(updateSensorData, 100);
	onkeydown = KeyDown;
	onkeyup = KeyUp;
	onkeypress = KeyPress;
	send() ;
	Canvas() ;
}

function FunctionSwitch (v)
{
	$('span.FaceDetect_button').html("_");
	$('span.Autonomy_button').html("_");
	$('span.ObjectDetect_button').html("_");
	if(command['FUNCTION'] != v)		set('FUNCTION',v);
		else							set('FUNCTION',0);
	if (command['FUNCTION'] == 1)			$('span.FaceDetect_button').html("X");
	else if (command['FUNCTION'] == 2)		$('span.Autonomy_button').html("X");
	else if (command['FUNCTION'] == 3)		$('span.ObjectDetect_button').html("X");
}

var enCAM_MOUSE=1;
var CAMx=0;
var CAMy=0;
var X=100;
var Y=100;
var message;
var res;
function Canvas() 
{
		var canvas = document.getElementById('myCanvas');
		var context = canvas.getContext('2d');

		canvas.addEventListener('mousemove', function(evt) 
			{
			
				var rect = canvas.getBoundingClientRect();
				
	
				X = evt.clientX - rect.left;
				Y = evt.clientY - rect.top;
		

			var context = canvas.getContext('2d');
			context.clearRect(0, 0, canvas.width, canvas.height);
			context.font = '10pt Calibri';
			context.fillStyle = 'black';
				
				
				context.fillRect(99,0,1,200);
				context.fillRect(0,99,200,1);
				
				context.fillText(message, 8, 10);
				context.fillText('X', (CAMx+45)*res-3, (-CAMy+45)*res+4);
			}, false);
	
}
function CAM_MOUSE() 
{
	if (enCAM_MOUSE) 
	{
				res = (200-1)/90;
				CAMx = (X / res)-45;
				CAMy = 45-(Y/ res);
			message = (CAMx|0) + ':' + (CAMy|0);
		$.get(window.location.protocol+'//'+window.location.host + ':' + CONTROL_PORT + "?cam" + ((CAMx+45)|0) + 'x' + ((CAMy+45)|0) + 'y');
	}
		
}

		
var speed= 1;
var min_speed=0;
var max_speed=9999;
function SPEEDUpdate(v)
{
	if(v==0)		
		{
			speed=parseInt(SPEED.value);
			SPEEDpreview.innerHTML =speed ;
		}
		else 
			{	
				if(v>0)
					if (speed +v< max_speed) speed=speed+v;
						else speed=max_speed;
				if (v<0)
					if (speed +v> min_speed) speed = speed+v;
						else speed=min_speed;
				SPEED.value = speed ;
				SPEEDpreview.innerHTML =SPEED.value ;
			}
	SPEEDpreview.innerHTML =speed ;
	  $.get(window.location.protocol+'//'+window.location.host + ':' + CONTROL_PORT + "?spe" + speed + "|");
}

function set (name, value)    
{	
	command[name] = value;
	send();
  	return true;
}

function send() 
{
	  var commandBits = "";
	  for (var name in command)     commandBits = commandBits + command[name];
	  $.get(window.location.protocol+'//'+window.location.host + ':' + CONTROL_PORT + "?set" + commandBits);
}

function updateSensorData()
{
	if(command['ULTRASONIC']==1)
			{
				$.get(window.location.protocol+'//'+window.location.host + "/sona.txt", "", function(sona){
					if (sona!= "")   $('div.sona').html(sona);
					}, "html");
			}	
			else 
				{
				  $('div.sona').html('');
				}
	
	if(command['SPEED']==1)
			{	
				$.get(window.location.protocol+'//'+window.location.host + "/SPEED.txt", "", function(SPEED){
					if (SPEED!= "")  $('div.SPEED').html(SPEED);
					}, "html");
			}
			else 
				{
				  $('div.SPEED').html('');
				}
  
	if(command['DISTANCE']==1)
			{	
				$.get(window.location.protocol+'//'+window.location.host + "/DISTANCE.txt", "", function(DISTANCE){
					if (DISTANCE!= "") $('div.DISTANCE').html(DISTANCE);
					  }, "html");
			}
			else 
				{
				  $('div.DISTANCE').html('');
				}
}

function toggle(name) 
{
	if (command[name] == 0) 
		{
			command[name] = 1;
			if (name=='DISTANCE')			$('span.DISTANCE_button').html("X");
			if (name=='ULTRASONIC')			$('span.ULTRASONIC_button').html("X");
			if (name=='CAMERA')				$('span.CAMERA_button').html("X");
			if (name=='SPEED')				$('span.SPEED_button').html("X");
			if (name=='LED')				$('span.LED_button').html("X");
			
		} 
		else
			{
				command[name] = 0;
				if (name=='ULTRASONIC') 	$('span.ULTRASONIC_button').html("_");
				if (name=='CAMERA') 		$('span.CAMERA_button').html("_");
				if (name=='DISTANCE') 		$('span.DISTANCE_button').html("_");
				if (name=='SPEED')			$('span.SPEED_button').html("_");
				if (name=='LED')			$('span.LED_button').html("_");
			}
	send();
	send();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 function KeyUp(event) 
 {
	var key = 0;
	if (event)     
		{
			key = event.keyCode;
			
			if (key == 65)     set('LEFT',0);
			if (key == 68)     set('RIGHT',0);
			if (key == 87)    	 set('UP',0);
			if (key == 83)     set('DOWN',0);
			
			if (key == 52)    toggle('CAMERA') ;
			if (key == 53)    toggle('LED') ; 
			if (key == 51)    toggle('ULTRASONIC') ; 
			if (key == 50)    toggle('SPEED') ; 	 
			if (key == 49)    toggle('DISTANCE') ; 	
			
			if (key == 67)     {enCAM_MOUSE =0;CAM_MOUSE()}
		}
   return true;
 }
 
function KeyDown(event) 
 {
	var key = 0;
	if (event)     
		{
			key = event.keyCode;

			if (key == 65)     set('LEFT',1);
			if (key == 68)     set('RIGHT',1);
			if (key == 87)    	 {set('UP',1);}
			if (key == 83)     set('DOWN',1);
		 
			if (key == 189)     SPEEDUpdate(-1) ; 
			if (key == 187)     SPEEDUpdate(1) ; 
			
			if (key == 67)     {enCAM_MOUSE =1;CAM_MOUSE()}	
		}
	return true;
 }

 function KeyPress(event) 
 {
	var key = 0;
	if (event)     
		{
			key = event.keyCode;

			if (key == 65)     set('LEFT',1);
			if (key == 68)     set('RIGHT',1);
			if (key == 87)    	{set('UP',1);}
			if (key == 83)     set('DOWN',1);
		 
			if (key == 189)    SPEEDUpdate(-1) ; 
			if (key == 187)    SPEEDUpdate(1) ; 
						
			if (key == 67)     {enCAM_MOUSE =1;CAM_MOUSE()}
		}
	return true; 
 }
 
var imageNr = 0; // Serial number of current image
var finished = new Array(); // References to img objects which have finished downloading
function createImageLayer() 
{
  var img = new Image();
  img.style.position = "absolute";
  img.style.zIndex = -1;
  img.onload = imageOnload;
  img.src = window.location.protocol+'//'+window.location.host + ':' + WEBCAM_PORT + "/?action=snapshot&n=" + (++imageNr);
  var webcam = document.getElementById("webcam");
  webcam.insertBefore(img, webcam.firstChild);
}

// Two layers are always present (except at the very beginning), to avoid flicker
function imageOnload() 
{
  this.style.zIndex = imageNr; // Image finished, bring to front!
  while (1 < finished.length) {
    var del = finished.shift(); // Delete old image(s) from document
    del.parentNode.removeChild(del);
  }
  finished.push(this);
  createImageLayer();
}
 