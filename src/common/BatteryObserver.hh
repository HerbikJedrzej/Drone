#pragma once

#include <cstddef>
#include <cstdint>
#include <stdint.h>
#include <cstdlib>
#include <DriverIfc.hh>
#include <AT24C.hh>
#include <memoryMap.hh>

class BatteryObserver{
public:
	BatteryObserver(Drivers::Memory* _memory, uint16_t const* _measurements, const double* _rezistorD, const uint8_t _numberOfCells, const uint16_t _adcMask, const double& _minVoltage, const double& _maxVoltage);
	~BatteryObserver();
	bool init();
	double batteryVoltageVal();
	void correctResistor(const uint8_t& referenceVoltage);
	uint8_t batteryPercentVal(const uint8_t& i);
	double batteryVoltageVal(const uint8_t& i);
	virtual uint8_t batteryPercentVal();
	virtual bool minimaliseBatteryUse();
protected:
	void correctResistor(const uint8_t& cell, const uint8_t& referenceVoltage);
private:
	Drivers::Memory* const memory;
	const uint8_t numberOfCells;
	const double adcMask;
	const double minVoltage;
	const double maxVoltage;
	const double* r2;
	uint32_t rezistorU[3];
	uint16_t const* measurements;
	bool minimaliseBatteryUseFlag = {false};
};
