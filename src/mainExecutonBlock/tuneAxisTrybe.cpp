#include "tuneAxisTrybe.hh"
#include <OwnExceptions.hh>

void mainLoopOfEngineManagement(GlobalStruct& globals, DriversGroup& driversGroup, const bool& itIsAxisX){
	double& correctionBetweenEngines = (itIsAxisX)? globals.enginesCorrectionX : globals.enginesCorrectionY;
	globals.measuredVal1 = ((*driversGroup.ahrs)[(itIsAxisX)? 0 : 1] + 180.0) * 65535.0 / 360.0; // 0x0000 = -180 deg | 0xFFFF = +180 deg
	globals.measuredSignal = globals.measuredSignalParam1 * sin(6.28318530718 * globals.measuredSignalParam2 * globals.measureTime);
	if(driversGroup.radioParser->getSpecialButtonOption()){
		uint8_t tmpP = uint8_t(globals.power) * 3;
		driversGroup.memory->write(memoryMap::initEnginePower, &tmpP, 1);
		uint8_t tmpC = int8_t(correctionBetweenEngines) * 6;
		driversGroup.memory->write((itIsAxisX)? memoryMap::initEngineCorrectionX : memoryMap::initEngineCorrectionY, &tmpC, 1);
	}
	correctionBetweenEngines += double(driversGroup.radioParser->getYawlValue() / 25) / 100;
	globals.power += double(driversGroup.radioParser->getAltitudeIncremetionValue() / 25) / 100;
	if(globals.power > 85.0)
		globals.power = 85.0;
	else if(globals.power < 0.0)
		globals.power = 0.0;
	if(correctionBetweenEngines > 21.0)
		correctionBetweenEngines = 21.0;
	else if(correctionBetweenEngines < -21.0)
		correctionBetweenEngines = -21.0;
	if(driversGroup.radioParser->getFlyOnOption()){
		if(itIsAxisX)
			driversGroup.enginesControl->setAxisX(globals.power, correctionBetweenEngines + double(driversGroup.radioParser->getRollValue() / 25) / 10 + globals.measuredSignal);
		else
			driversGroup.enginesControl->setAxisY(globals.power, -correctionBetweenEngines - double(driversGroup.radioParser->getRollValue() / 25) / 10 - globals.measuredSignal);
	}
	else if(driversGroup.radioParser->getEngineOnOption()){
		if(itIsAxisX)
			driversGroup.enginesControl->setAxisX(25.0);
		else
			driversGroup.enginesControl->setAxisY(25.0);
	}
	else{
		driversGroup.enginesControl->setAll(0.0);
	}
}

void tuneAxisXTrybe(GlobalStruct& globals, DriversGroup& driversGroup, void(*)(uint32_t)){
	mainLoopOfEngineManagement(globals, driversGroup, true);
}

void tuneAxisYTrybe(GlobalStruct& globals, DriversGroup& driversGroup, void(*)(uint32_t)){
	mainLoopOfEngineManagement(globals, driversGroup, false);
}