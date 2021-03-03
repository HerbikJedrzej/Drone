#include <GPIO.hh>
#include "../common/esdStructure.hh"
#include <GPIOconf.hh>
#include <AT24C.hh>
#include <globalClass.hh>
#include "../common/ADC.hh"

void percentValEngine1(GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t));
void percentValEngine2(GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t));
void percentValEngine3(GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t));
void percentValEngine4(GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t));
void percentValEngine5(GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t));
void percentValEngine6(GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t));
void percentValEngine7(GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t));
void percentValEngine8(GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t));
