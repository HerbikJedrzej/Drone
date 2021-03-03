#include "PIDofAxisXandY.hh"
#include <OwnExceptions.hh>
#include <memoryMap.hh>

void mainLoopOfPIDofAxisXandY(GlobalStruct& globals, DriversGroup& driversGroup, const bool& itIsAxisX, void(*)(uint32_t)){
	if(driversGroup.radioParser->getFlyOnOption()){
		if(itIsAxisX){
			driversGroup.pidX->setY((*driversGroup.ahrs)[0]);
			driversGroup.pidX->setR(double(driversGroup.radioParser->getRollValue() / 5));
			driversGroup.enginesControl->setAxisX(globals.power, driversGroup.pidX);
		}else{
			driversGroup.pidY->setY((*driversGroup.ahrs)[1]);
			driversGroup.pidY->setR(double(driversGroup.radioParser->getPitchValue() / 5));
			driversGroup.enginesControl->setAxisY(globals.power, driversGroup.pidY);
		}
	}
	else if(driversGroup.radioParser->getEngineOnOption()){
		if(itIsAxisX)
			driversGroup.enginesControl->setAxisX(25.0);
		else
			driversGroup.enginesControl->setAxisY(25.0);
	}
	else{
		driversGroup.enginesControl->setAll(0);
	}
}

void PIDofAxisX(GlobalStruct& globals, DriversGroup& driversGroup, void(*delay)(uint32_t)){
	mainLoopOfPIDofAxisXandY(globals, driversGroup, true, delay);
}

void PIDofAxisY(GlobalStruct& globals, DriversGroup& driversGroup, void(*delay)(uint32_t)){
	mainLoopOfPIDofAxisXandY(globals, driversGroup, false, delay);
}
