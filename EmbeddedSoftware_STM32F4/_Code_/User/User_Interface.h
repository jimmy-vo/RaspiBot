#include "BalaPi_BoardConf.h"

typedef	void Display;
typedef char string[];
typedef struct 
{
	Display 				*Function;
	string					Text;
}Menu_Choose;


Display Display_Prompt(Display (*DisplayA)() , Display (*DisplayB)() , Display (*DisplayC)() , Display (*DisplayD)() );
Display Display_StartUp(void);
void Initialize_UserInterface (uint16_t input);
Display Display_RTC(void);
Display Display_VoltFB(void);

void Display_Servotest(uint32_t step);
void Display_Motortest(void);
void Display_VoltFBtest(void);
void Display_SERIALtest(void);
void Display_IMU (void);
void Display_EEPROMtest(void);
void Display_EEPROMtest(void);
void Display_SONAtest(void);
