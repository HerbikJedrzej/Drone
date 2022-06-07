#pragma once

#include "globalClass.hh"
#include <memoryMap.hh>

#define NUMBERS_OF_TRYBES 16
#define MACRO_TO_STRING(X) #X
#define MERGE_FORTRYBE_OUT_OF_RANGE "There are only " MACRO_TO_STRING(NUMBERS_OF_TRYBES) " trybes of main loop."

class ProcessRadioData{
public:
	static void procesMeasurements(GlobalStruct& globals, DriversGroup& driversGroup);
	static void processRadioMainData(GlobalStruct& globals, DriversGroup& driversGroup);
protected:
	static void recordMeasurements(GlobalStruct& globals, DriversGroup& driversGroup);
	static void sendMeasuredData(DriversGroup& driversGroup);
	static void applyPIDparams(GlobalStruct& globals, DriversGroup& driversGroup);
	static void setRegisterInMap(DriversGroup& driversGroup, const memoryMap& addr);
};
