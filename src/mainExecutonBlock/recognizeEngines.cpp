#include "recognizeEngines.hh"
#include <OwnExceptions.hh>

namespace{
	uint16_t counterForEngines = {0};
}

void recognizeEngines(GlobalStruct&, DriversGroup& driversGroup, void(*)(uint32_t)){
	if(driversGroup.radioParser->getFlyOnOption() || driversGroup.radioParser->getEngineOnOption()){
		counterForEngines++;
		if(counterForEngines >= 100){
			counterForEngines = 0;
			driversGroup.enginesControl->incrementEngine(25.0);
		}
	}
	else{
		driversGroup.enginesControl->setAll(0.0);
	}
}
