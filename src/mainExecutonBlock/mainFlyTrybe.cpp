#include "mainFlyTrybe.hh"
#include <OwnExceptions.hh>
#include "TrybeTools.hh"

namespace mainFlyTrybeSpace{
	double pidOutX = {0};
	double pidOutY = {0};
	double pidOutZ = {0};
	double newPower = {0.0};
	double yawlInStady = 0.0;
	uint8_t dataToSave[4];
}

void mainFlyTrybe(GlobalStruct& globals, DriversGroup& driversGroup, void(*)(uint32_t)){
	if(driversGroup.radioParser->getSpecialButtonOption()){
		globals.sumPidX = driversGroup.pidX->getSum();
		globals.sumPidY = driversGroup.pidY->getSum();
		int16_t eSumPidX = globals.sumPidX;
		int16_t eSumPidY = globals.sumPidY;
		mainFlyTrybeSpace::dataToSave[0] = uint8_t((eSumPidX >> 8) & 0xff);
		mainFlyTrybeSpace::dataToSave[1] = uint8_t(eSumPidX & 0xff);
		mainFlyTrybeSpace::dataToSave[2] = uint8_t((eSumPidY >> 8) & 0xff);
		mainFlyTrybeSpace::dataToSave[3] = uint8_t(eSumPidY & 0xff);
		driversGroup.memory->writeDMAwithoutDataAlocate(memoryMap::PID_X_sum_msb, mainFlyTrybeSpace::dataToSave, 4, nullptr);
	}
	globals.power = roundRadioSetValueToDouble(driversGroup.radioParser->getAltitudeIncremetionValue(), 5) + globals.basePower;
	if(driversGroup.radioParser->getFlyOnOption()){
		driversGroup.pidX->setY((*driversGroup.ahrs)[0]);
		driversGroup.pidY->setY((*driversGroup.ahrs)[1]);
		driversGroup.pidZ->setY((*driversGroup.ahrs)[2]);
		driversGroup.pidX->setR(-roundRadioSetValueToDouble(driversGroup.radioParser->getPitchValue(), 6));
		driversGroup.pidY->setR(roundRadioSetValueToDouble(driversGroup.radioParser->getRollValue(), 6));
		mainFlyTrybeSpace::yawlInStady -= roundRadioSetValueToDouble(driversGroup.radioParser->getYawlValue(), 100.0);
		driversGroup.pidZ->setR(mainFlyTrybeSpace::yawlInStady);
		driversGroup.enginesControl->set(globals.power, driversGroup.pidX, driversGroup.pidY, driversGroup.pidZ);
	}
	else if(driversGroup.radioParser->getEngineOnOption()){
		driversGroup.enginesControl->setAll(15);
		driversGroup.pidX->reset(globals.sumPidX);
		driversGroup.pidY->reset(globals.sumPidY);
		driversGroup.pidZ->reset(globals.sumPidZ);
		driversGroup.led.set(LedsList::angleGreaterThan15deg, false);
		mainFlyTrybeSpace::yawlInStady = (*driversGroup.ahrs)[2];
	}
	else{
		driversGroup.enginesControl->setAll(0);
		driversGroup.led.set(LedsList::angleGreaterThan15deg,
			((*driversGroup.ahrs)[0] > 15.0) || ((*driversGroup.ahrs)[0] < -15.0) ||
			((*driversGroup.ahrs)[1] > 15.0) || ((*driversGroup.ahrs)[1] < -15.0)
		);
	}
}
