#include "FlyTrybeWithAltitude.hh"
#include "mainFlyTrybe.hh"

void FlyTrybeWithAltitude(GlobalStruct& globals, DriversGroup& driversGroup, void(*delay)(uint32_t)){
	mainFlyTrybe(globals, driversGroup, delay);
	driversGroup.measurementManager->setMeasureValue1(driversGroup.altitudeProvider->getAltitude());
	driversGroup.measurementManager->setMeasureValue2(globals.power * driversGroup.ahrs->getHorizontalFactor());
}
