#include <stdio.h>
#include <wiringPi.h>

#define LEDG    0
#define LEDB    2
#define LEDW	3

////////////////////////////////////////////

void LEDInit(void)
{
    pinMode (LEDG, OUTPUT) ;
    pinMode (LEDB, OUTPUT) ;
    pinMode (LEDW, OUTPUT) ;
}


////////////////////////////////////////
void main ()
{
	wiringPiSetup();
	LEDInit();
       
        digitalWrite (LEDB, LOW) ;
        digitalWrite (LEDG, LOW) ;
        digitalWrite (LEDW, HIGH) ;
		delay(100);
        digitalWrite (LEDB, HIGH) ;
        digitalWrite (LEDG, LOW) ;
        digitalWrite (LEDW, LOW) ;
		delay(100);
        digitalWrite (LEDB, LOW) ;
        digitalWrite (LEDG, HIGH) ;
        digitalWrite (LEDW, LOW) ;
		delay(100);
        digitalWrite (LEDB, LOW) ;
        digitalWrite (LEDG, LOW) ;
        digitalWrite (LEDW, LOW) ;
		delay(100);
        digitalWrite (LEDB, HIGH) ;
        digitalWrite (LEDG, HIGH) ;
        digitalWrite (LEDW, HIGH) ;
}
