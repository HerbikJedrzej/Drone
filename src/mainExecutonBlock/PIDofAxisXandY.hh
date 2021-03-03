#include <GPIO.hh>
#include "../common/esdStructure.hh"
#include <GPIOconf.hh>
#include <AT24C.hh>
#include <globalClass.hh>
#include "../common/ADC.hh"

/*
Interface of radio data:
	Main option:
	14 = Common factor for axises X and Y.	
	15 = Value of msb P param of PID axis X.
	16 = Value of msb I param of PID axis X.
	17 = Value of msb D param of PID axis X.
	18 = Value of msb P param of PID axis Y.
	19 = Value of msb I param of PID axis Y.
	20 = Value of msb D param of PID axis Y.
	21 = Value of msb P param of PID axis z.
	22 = Value of msb I param of PID axis z.
	23 = Value of msb D param of PID axis z.
	24 = Value of msb P param of PID axis H.
	25 = Value of msb I param of PID axis H.
	26 = Value of msb D param of PID axis H.
*/

void PIDofAxisX(GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t));
void PIDofAxisY(GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t));
