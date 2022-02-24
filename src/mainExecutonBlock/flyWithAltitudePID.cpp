#include "flyWithAltitudePID.hh"
#include <OwnExceptions.hh>
#include "TrybeTools.hh"

namespace flyWithAltitudePIDSpace{
	double pidOutX = {0};
	double pidOutY = {0};
	double pidOutZ = {0};
	double newPower = {0.0};
	double yawl = 0.0;
	double altitude = 0.0;
	uint8_t dataToSave[4];
}

void flyWithAltitudePID(GlobalStruct& globals, DriversGroup& driversGroup, void(*)(uint32_t)){
	globals.altitude = driversGroup.altitudeProvider->getAltitude();
	if(driversGroup.radioParser->getSpecialButtonOption()){
		globals.sumPidX = driversGroup.pidX->getSum();
		globals.sumPidY = driversGroup.pidY->getSum();
		int16_t eSumPidX = globals.sumPidX;
		int16_t eSumPidY = globals.sumPidY;
		flyWithAltitudePIDSpace::dataToSave[0] = uint8_t((eSumPidX >> 8) & 0xff);
		flyWithAltitudePIDSpace::dataToSave[1] = uint8_t(eSumPidX & 0xff);
		flyWithAltitudePIDSpace::dataToSave[2] = uint8_t((eSumPidY >> 8) & 0xff);
		flyWithAltitudePIDSpace::dataToSave[3] = uint8_t(eSumPidY & 0xff);
		driversGroup.memory->writeDMAwithoutDataAlocate(memoryMap::PID_X_sum_msb, flyWithAltitudePIDSpace::dataToSave, 4, nullptr);
	}
	if(driversGroup.radioParser->getFlyOnOption()){
		driversGroup.pidX->setY((*driversGroup.ahrs)[0]);
		driversGroup.pidY->setY((*driversGroup.ahrs)[1]);
		driversGroup.pidZ->setY((*driversGroup.ahrs)[2]);
		driversGroup.pidH->setY(globals.altitude);
		driversGroup.pidX->setR(-roundRadioSetValueToDouble(driversGroup.radioParser->getPitchValue(), 6));
		driversGroup.pidY->setR(roundRadioSetValueToDouble(driversGroup.radioParser->getRollValue(), 6));
		flyWithAltitudePIDSpace::yawl -= roundRadioSetValueToDouble(driversGroup.radioParser->getYawlValue(), 100.0);
		flyWithAltitudePIDSpace::altitude += roundRadioSetValueToDouble(driversGroup.radioParser->getAltitudeIncremetionValue(), 220.0);
		driversGroup.pidZ->setR(flyWithAltitudePIDSpace::yawl);
		driversGroup.pidH->setR(flyWithAltitudePIDSpace::altitude);
		globals.power = driversGroup.pidH->calculate() + globals.basePower;
		driversGroup.enginesControl->set(globals.power, driversGroup.pidX, driversGroup.pidY, driversGroup.pidZ);
	}
	else if(driversGroup.radioParser->getEngineOnOption()){
		driversGroup.enginesControl->setAll(15);
		driversGroup.pidX->reset(globals.sumPidX);
		driversGroup.pidY->reset(globals.sumPidY);
		driversGroup.pidZ->reset(globals.sumPidZ);
		driversGroup.pidH->reset(0.0);
		flyWithAltitudePIDSpace::yawl = (*driversGroup.ahrs)[2];
		flyWithAltitudePIDSpace::altitude = globals.altitude + 0.5;
	}
	else{
		driversGroup.enginesControl->setAll(0);
	}
}
