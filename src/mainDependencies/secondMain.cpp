#include "secondMain.hh"
#include <mainFlyTrybe.hh>
#include <FlyTrybeWithAltitude.hh>
#include <tuneAxisTrybe.hh>
#include <PIDofAxisXandY.hh>
#include <flyWithAltitudePID.hh>
#include <EnginePercentValue.hh>
#include <OwnExceptions.hh>
#include <memoryMap.hh>
#include "processRadioData.hh"
#include <recognizeEngines.hh>
#include <cstdio>

using Drivers::GPIO;

#define INIT_LOG_BUFFER_MAX_SIZE 140
#define LOG_PRINT(BUFFOR, FORMAT, ...) {\
	const int size = sprintf(BUFFOR, FORMAT, ##__VA_ARGS__);\
	if(size > INIT_LOG_BUFFER_MAX_SIZE) {\
		THROW_out_of_range("Encrease init log buffer.");\
	}\
	driversGroup.transmitUART(BUFFOR, size);\
}

namespace{
	uint32_t filerForBattery = {0};
	void (*mainFunctionPtr[])(GlobalStruct& globals, DriversGroup& driversGroup, void(*delay)(uint32_t)) = {
		mainFlyTrybe,         // 0
		tuneAxisXTrybe,       // 1
		tuneAxisYTrybe,       // 2
		PIDofAxisX,           // 3
		PIDofAxisY,           // 4
		recognizeEngines,     // 5
		percentValEngine1,    // 6
		percentValEngine2,    // 7
		percentValEngine3,    // 8
		percentValEngine4,    // 9
		percentValEngine5,    // 10
		percentValEngine6,    // 11
		percentValEngine7,    // 12
		percentValEngine8,    // 13
		FlyTrybeWithAltitude, // 14
		flyWithAltitudePID    // 15
	};
	static_assert((sizeof(mainFunctionPtr)/sizeof(mainFunctionPtr[0])) == NUMBERS_OF_TRYBES, "Size of trybe table is not equal to variable NUMBERS_OF_TRYBES");
}

void mainSetup(Drivers::GPIO*, GlobalStruct& globals, DriversGroup& driversGroup, void(*delay)(uint32_t)){
	uint8_t dataToRead;
	delay(100);
	if(!driversGroup.memory)
		THROW_invalid_argument("Memory is not created.");
	driversGroup.memory->lockMemory();
	if(!driversGroup.radio)
		THROW_invalid_argument("Radio is not created.");
	if(!driversGroup.radioParser)
		THROW_invalid_argument("RadioParser is not created.");
	if(!driversGroup.dof10sensors)
		THROW_invalid_argument("DOF 10 sensor is not created.");
	if(!driversGroup.enginesControl)
		THROW_invalid_argument("EnginesControl is not created.");
	if(!driversGroup.ahrs)
		THROW_invalid_argument("AHRS is not created.");
	if(!driversGroup.pidX)
		THROW_invalid_argument("PID for axis X is not created.");
	if(!driversGroup.pidY)
		THROW_invalid_argument("PID for axis Y is not created.");
	if(!driversGroup.pidZ)
		THROW_invalid_argument("PID for axis Z is not created.");
	if(!driversGroup.pidH)
		THROW_invalid_argument("PID for axis H is not created.");
	if(!driversGroup.altitudeProvider)
		THROW_invalid_argument("AltitudeProvider is not created.");
	if(!driversGroup.battery)
		THROW_invalid_argument("Battery observer is not created.");
	if(!driversGroup.measurementManager)
		THROW_invalid_argument("Measurement manager is not created.");
	driversGroup.pidX->setLimit(-25.0, 25.0);
	driversGroup.pidY->setLimit(-25.0, 25.0);
	driversGroup.pidH->setLimit(-20.0, 30.0);
	driversGroup.pidH->AntiWindup = 5.0;
	uint8_t dataOfPIDsum[2];
	driversGroup.memory->read(memoryMap::PID_X_sum_msb, dataOfPIDsum, 2);
	globals.sumPidX = int16_t((dataOfPIDsum[0] << 8) | dataOfPIDsum[1]);
	driversGroup.memory->read(memoryMap::PID_Y_sum_msb, dataOfPIDsum, 2);
	globals.sumPidY = int16_t((dataOfPIDsum[0] << 8) | dataOfPIDsum[1]);

	driversGroup.memory->read(memoryMap::commonFactorForAxisXYZ, &dataToRead, 1);		
	const double factor = double(dataToRead) * 0.19 / 255 + 0.01; // [0.01 - 0.20]
	driversGroup.memory->read(memoryMap::pid_P_XY_00, &dataToRead, 1);		
	const double paramP = double(dataToRead) * 7.0  / 255 + 1.0 ; // [8.0 - 1.0 ]
	driversGroup.memory->read(memoryMap::pid_I_XY_00, &dataToRead, 1);		
	const double paramI = double(dataToRead) * 0.03 / 255 + 0.005 ; // [0.035 - 0.005 ]
	driversGroup.memory->read(memoryMap::pid_D_XY_00, &dataToRead, 1);		
	const double paramD = double(dataToRead) * 180.0 / 255 + 20.0; // [200 - 20]
	driversGroup.memory->read(memoryMap::pid_P_Z, &dataToRead, 1);
	const double pidZparamP = double(dataToRead) * 7.0 / 255 + 1.0; // [1.0 - 8.0]
	driversGroup.memory->read(memoryMap::pid_I_Z, &dataToRead, 1);
	const double pidZparamI = double(dataToRead) * 1.0 / 255; // [0.0 - 1.0]
	driversGroup.memory->read(memoryMap::pid_D_Z, &dataToRead, 1);
	const double pidZparamD = double(dataToRead) * 1.0 / 255; // [0.0 - 1.0]
	driversGroup.memory->read(memoryMap::pid_P_H, &dataToRead, 1);
	const double pidHparamP = double(dataToRead) * 8.0 / 255 + 4.0; // [12.0 - 4.0]
	driversGroup.memory->read(memoryMap::pid_I_H, &dataToRead, 1);
	const double pidHparamI = double(dataToRead) * 0.495 / 255 + 0.005; // [0.5 - 0.005]
	driversGroup.memory->read(memoryMap::pid_D_H, &dataToRead, 1);
	const double pidHparamD = double(dataToRead) * 100.0 / 255 + 50.0; // [150.0 - 50.0]

	globals.PIDcofactor = factor; 
	globals.pidPaxisXY = paramP;
	globals.pidIaxisXY = paramI;
	globals.pidDaxisXY = paramD;
	driversGroup.pidX->P = paramP * factor;
	driversGroup.pidX->I = paramI * factor;
	driversGroup.pidX->D = paramD * factor;
	driversGroup.pidY->P = paramP * factor;
	driversGroup.pidY->I = paramI * factor;
	driversGroup.pidY->D = paramD * factor;
	globals.pidPaxisZ = pidZparamP;
	globals.pidIaxisZ = pidZparamI;
	globals.pidDaxisZ = pidZparamD;
	driversGroup.pidZ->P = pidZparamP * factor;
	driversGroup.pidZ->I = pidZparamI * factor;
	driversGroup.pidZ->D = pidZparamD * factor;
	globals.pidPaxisH = pidHparamP;
	globals.pidIaxisH = pidHparamI;
	globals.pidDaxisH = pidHparamD;
	driversGroup.pidH->P = pidHparamP;
	driversGroup.pidH->I = pidHparamI;
	driversGroup.pidH->D = pidHparamD;

	driversGroup.memory->unlockMemory();
	delay(10);
	if(driversGroup.barometer)
		if(!driversGroup.barometer->init())
			THROW_invalid_argument("Barometer sensors uncorrect initialisation.");
	if(!driversGroup.dof10sensors->init())
		THROW_invalid_argument("DOF 10 sensors uncorrect initialisation.");
	if(!driversGroup.battery->init())
		THROW_invalid_argument("Battery observer initialisation error.");
	const Drivers::Memory::RegPair initMemoryRegisters[] = {
		{memoryMap::mainLoopTrybe, 0},
	};
	if(!driversGroup.memory->init(initMemoryRegisters, sizeof(initMemoryRegisters) / sizeof(initMemoryRegisters[0])))
		THROW_invalid_argument("Memory initialisation error");
	driversGroup.measurementManager->init();
	driversGroup.radio->init();
	driversGroup.memory->read(memoryMap::mainLoopTrybe, &globals.mainLoopTrybeFlag, 1);
	uint8_t dataForAngleOffsets[2];
	driversGroup.memory->read(memoryMap::angleOffsetX, dataForAngleOffsets, 2);
	globals.angleOffsetAxisX = double(dataForAngleOffsets[0])* 30.0 / 255 - 15.0;
	globals.angleOffsetAxisY = double(dataForAngleOffsets[1])* 30.0 / 255 - 15.0;
	uint8_t dataForInitEngines[3];
	driversGroup.memory->read(memoryMap::initEnginePower, dataForInitEngines, 3);
	// globals.basePower = double(dataForInitEngines[0]) / 3;
	globals.enginesCorrectionX = double(int8_t(dataForInitEngines[1])) / 6;
	globals.enginesCorrectionY = double(int8_t(dataForInitEngines[2])) / 6;
	delay(10);
	printInitInfo(globals, driversGroup);
	if(globals.mainLoopTrybeFlag >= NUMBERS_OF_TRYBES){
		globals.mainLoopTrybeFlag = 0;
		driversGroup.memory->writeDMAwithoutDataAlocate(memoryMap::mainLoopTrybe, &globals.mainLoopTrybeFlag, 1, nullptr);
		THROW_out_of_range("Unknown main loop trybe mode");
	}
}

void printInitInfo(GlobalStruct& globals, DriversGroup& driversGroup){
	char dataToLog[INIT_LOG_BUFFER_MAX_SIZE] = "\n------------- init begin -------------";
	driversGroup.transmitUART(dataToLog, 40);
	LOG_PRINT(dataToLog, "\nMain loop is in trybe: %d", globals.mainLoopTrybeFlag);
	LOG_PRINT(dataToLog, "\nMemory size: %d", driversGroup.memory->getSizeModel() / 0x100 + 1);
	LOG_PRINT(dataToLog, "\nAvaible memory slots for measurements: %d", driversGroup.measurementManager->slotsNumber);
	for(uint8_t i = 0; i < driversGroup.measurementManager->slotsNumber; i++)
		LOG_PRINT(dataToLog, "\nSlot %d size = %.2fs", i + 1, driversGroup.measurementManager->memorySlots[i].getSize()/100.0);
	LOG_PRINT(dataToLog, "\nAxis X angle offset = %f", globals.angleOffsetAxisX);
	LOG_PRINT(dataToLog, "\nAxis Y angle offset = %f", globals.angleOffsetAxisY);
	LOG_PRINT(dataToLog, "\nAxis X PID sum = %f", globals.sumPidX);
	LOG_PRINT(dataToLog, "\nAxis Y PID sum = %f", globals.sumPidY);
	LOG_PRINT(dataToLog, "\nAxis X engines correction = %f", globals.enginesCorrectionX);
	LOG_PRINT(dataToLog, "\nAxis Y engines correction = %f", globals.enginesCorrectionY);
	LOG_PRINT(dataToLog, "\nPID axis XY factor = %f", globals.PIDcofactor);
	LOG_PRINT(dataToLog, "\nAxis XY PID. P = %f I = %f D = %f", globals.pidPaxisXY, globals.pidIaxisXY, globals.pidDaxisXY);
	LOG_PRINT(dataToLog, "\nAxis  Z PID. P = %f I = %f D = %f", globals.pidPaxisZ, globals.pidIaxisZ, globals.pidDaxisZ);
	if(driversGroup.pidH)
		LOG_PRINT(dataToLog, "\nAxis  H PID. P = %f I = %f D = %f", globals.pidPaxisH, globals.pidIaxisH, globals.pidDaxisH);
	LOG_PRINT(dataToLog, "\n------------- init end -------------\n");
}

void mainLoop(Drivers::GPIO*, GlobalStruct& globals, DriversGroup& driversGroup, void(*delay)(uint32_t)){
	ProcessRadioData::processRadioMainData(globals, driversGroup);
	mainFunctionPtr[globals.mainLoopTrybeFlag](globals, driversGroup, delay);
	ProcessRadioData::procesMeasurements(globals, driversGroup);
	filerForBattery = filerForBattery << 1;
	if(driversGroup.battery->minimaliseBatteryUse())
		filerForBattery |= 1;
	driversGroup.led.set(LedsList::batteryOut, filerForBattery == 0xffffffff);
}