#pragma once

#include <cstdint>
#include <PID.hh>
#include <AHRS.hh>
#include <GPIO.hh>
#include <AT24C.hh>
#include <ADC.hh>
#include <EngineIfc.hh>
#include <EngineControlIfc.hh>
#include <LSM6DS33.hh>
#include <RadioIfc.hh>
#include <RadioParser.hh>
#include <MPU6050.hh>
#include <LED.hh>
#include <memoryMap.hh>
#include <BatteryObserver.hh>
#include <AltitudeProvider.hh>
#include <BarometerIfc.hh>
#include <MeasurementManager.hh>

struct Barometer{
    int16_t AC1;
    int16_t AC2;
    int16_t AC3;
    uint16_t AC4;
    uint16_t AC5;
    uint16_t AC6;
    int16_t B1;
    int16_t B2;
    int16_t MB;
    int16_t MC;
    int16_t MD;
};

struct  DriversGroup{
    Drivers::LED& led;
    Drivers::RadioIfc* const radio;
    Drivers::RadioParser* const radioParser;
    Drivers::IMUsensorIfc* const dof10sensors;
    Drivers::Memory* const memory;
    EngineControlIfc* const enginesControl;
    AHRS* const ahrs;
    PID* const pidX;
    PID* const pidY;
    PID* const pidZ;
    PID* const pidH;
    Drivers::BarometerIfc* const barometer;
    AltitudeProvider* const altitudeProvider;
    BatteryObserver* battery;
    MeasurementManager* measurementManager;
    bool (*transmitUART)(const char* text, unsigned int size);
};

struct GlobalStruct{
	double basePower = 0.0;
    double power = 0;
	uint8_t mainLoopTrybeFlag = 0;
	double enginesCorrectionX = 0;
	double enginesCorrectionY = 0;
    double PIDcofactor = 1;
    double pidPaxisXY = 1;
    double pidIaxisXY = 0;
    double pidDaxisXY = 0;
    double pidPaxisZ = 1;
    double pidIaxisZ = 0;
    double pidDaxisZ = 0;
    double pidPaxisH = 1;
    double pidIaxisH = 0;
    double pidDaxisH = 0;
    double measureTime = 0;
    uint16_t measuredSignalParam1 = 0;
    uint16_t measuredSignalParam2 = 0;
    double angleOffsetAxisX = 0;
    double angleOffsetAxisY = 0;
	double percentValueOfEngine = 0;
    double sumPidX = 0;
    double sumPidY = 0;
    double sumPidZ = 0;
    double altitude = 0;
};
