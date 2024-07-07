#include "car.h"
#include "xunji.h"
#include "Buzzer.h"
void main()
{
  Timer0_Init();
	
	while(1)
	{
    car_go();
	}	
}	