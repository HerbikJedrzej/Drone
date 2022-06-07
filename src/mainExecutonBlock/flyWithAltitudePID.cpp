#include "flyWithAltitudePID.hh"
#include <OwnExceptions.hh>
#include "TrybeTools.hh"

namespace flyWithAltitudePIDSpace{
	double yawl = 0.0;
	double altitude = 0.0;
	uint8_t dataToSave[4];
	bool pidOn = false;
}

void flyWithAltitudePID(GlobalStruct& globals, DriversGroup& driversGroup, void(*)(uint32_t)){
	globals.altitude = driversGroup.altitudeProvider->getAltitude();
	if(driversGroup.radioParser->getJoyRightButtonOption()){
		flyWithAltitudePIDSpace::pidOn = true;
	}
	if(driversGroup.radioParser->getJoyLeftButtonOption()){
		flyWithAltitudePIDSpace::pidOn = false;
	}
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
		driversGroup.pidH->outputFactor = 1.0 / driversGroup.ahrs->getHorizontalFactor();
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
		if(flyWithAltitudePIDSpace::pidOn){
			globals.power = driversGroup.pidH->calculate() + globals.basePower;
		} else {
			globals.power = roundRadioSetValueToDouble(driversGroup.radioParser->getAltitudeIncremetionValue(), 5) + globals.basePower;
			flyWithAltitudePIDSpace::altitude = globals.altitude;
		}
		driversGroup.enginesControl->set(globals.power, driversGroup.pidX, driversGroup.pidY, driversGroup.pidZ);
	}
	else if(driversGroup.radioParser->getEngineOnOption()){
		driversGroup.pidH->outputFactor = 1.0;
		driversGroup.enginesControl->setAll(15);
		driversGroup.pidX->reset(globals.sumPidX);
		driversGroup.pidY->reset(globals.sumPidY);
		driversGroup.pidZ->reset(globals.sumPidZ);
		driversGroup.pidH->reset(0.0);
		flyWithAltitudePIDSpace::yawl = (*driversGroup.ahrs)[2];
		flyWithAltitudePIDSpace::altitude = globals.altitude;
		flyWithAltitudePIDSpace::pidOn = false;
	}
	else{
		driversGroup.pidH->outputFactor = 1.0;
		driversGroup.enginesControl->setAll(0);
		driversGroup.led.set(LedsList::angleGreaterThan15deg,
			((*driversGroup.ahrs)[0] > 15.0) || ((*driversGroup.ahrs)[0] < -15.0) ||
			((*driversGroup.ahrs)[1] > 15.0) || ((*driversGroup.ahrs)[1] < -15.0)
		);
		flyWithAltitudePIDSpace::pidOn = false;
	}
	driversGroup.measurementManager->setMeasureValue1(globals.altitude);
	driversGroup.measurementManager->setMeasureValue2(flyWithAltitudePIDSpace::altitude);
}
