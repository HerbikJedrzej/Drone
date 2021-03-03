#include "FlyTrybeWithAltitude.hh"
#include "mainFlyTrybe.hh"

void FlyTrybeWithAltitude(GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t)){
	mainFlyTrybe(globals, diversGroup, delay);
	globals.measuredVal1 = uint16_t(100 * globals.power);
	globals.measuredVal2 = uint16_t(diversGroup.altitudeProvider->getAltitude() * 100); // altitude;
}
