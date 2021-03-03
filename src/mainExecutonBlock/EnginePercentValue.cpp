#include "EnginePercentValue.hh"
#include <OwnExceptions.hh>

void enginePercentValue(GlobalStruct& globals, DriversGroup& driversGroup, const uint8_t& engNr){
	driversGroup.enginesControl->setAll(0.0);
	if(driversGroup.radioParser->getFlyOnOption())
		(*driversGroup.enginesControl)(engNr, globals.percentValueOfEngine);
	else if(driversGroup.radioParser->getEngineOnOption())
		(*driversGroup.enginesControl)(engNr, 25.0);
}

void percentValEngine1(GlobalStruct& globals, DriversGroup& diversGroup, void(*)(uint32_t)){
	enginePercentValue(globals, diversGroup, 0);
}
void percentValEngine2(GlobalStruct& globals, DriversGroup& diversGroup, void(*)(uint32_t)){
	enginePercentValue(globals, diversGroup, 1);
}
void percentValEngine3(GlobalStruct& globals, DriversGroup& diversGroup, void(*)(uint32_t)){
	enginePercentValue(globals, diversGroup, 2);
}
void percentValEngine4(GlobalStruct& globals, DriversGroup& diversGroup, void(*)(uint32_t)){
	enginePercentValue(globals, diversGroup, 3);
}
void percentValEngine5(GlobalStruct& globals, DriversGroup& diversGroup, void(*)(uint32_t)){
	enginePercentValue(globals, diversGroup, 4);
}
void percentValEngine6(GlobalStruct& globals, DriversGroup& diversGroup, void(*)(uint32_t)){
	enginePercentValue(globals, diversGroup, 5);
}
void percentValEngine7(GlobalStruct& globals, DriversGroup& diversGroup, void(*)(uint32_t)){
	enginePercentValue(globals, diversGroup, 6);
}
void percentValEngine8(GlobalStruct& globals, DriversGroup& diversGroup, void(*)(uint32_t)){
	enginePercentValue(globals, diversGroup, 7);
}
