#include <GPIO.hh>
#include "../common/esdStructure.hh"
#include <GPIOconf.hh>
#include <AT24C.hh>
#include <globalClass.hh>
#include "../common/ADC.hh"

/* 
	 1. Out of AHRS class use degres in AHRS use radians.
	 2. PID return value in %. -60 = min, 60 = max.
	 3. ESD input is the same as PID output.
*/

void mainFlyTrybe(GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t));
