#include "BatteryObserver.hh"
#include <OwnExceptions.hh>

BatteryObserver::BatteryObserver(Drivers::Memory* _memory, uint16_t const* _measurements, const double* _rezistorD, const uint8_t _numberOfCells, const uint16_t _adcMask, const double& _minVoltage, const double& _maxVoltage):
	memory(_memory),
	numberOfCells(_numberOfCells),
	adcMask(double(_adcMask)),
	minVoltage(_minVoltage),
	maxVoltage(_maxVoltage),
	r2(_rezistorD),
	rezistorU{0, 0, 0},
	measurements(_measurements){}

uint8_t BatteryObserver::batteryPercentVal(const uint8_t& i){
	const double baseVoltage = (i == 0)? minVoltage : batteryVoltageVal(i - 1) + minVoltage;
	double vol = batteryVoltageVal(i) - baseVoltage;
	if(vol < 0.0)
		return 0.0;
	return (100 * vol / (maxVoltage - minVoltage));
}

double BatteryObserver::batteryVoltageVal(const uint8_t& i){
	if(i >= numberOfCells)
		THROW_out_of_range("Trying to get voltage of cell out out of avaibles.");
	double r1 = rezistorU[i];
	double adc = measurements[i];
	return
		(double)(
			adc * 3.3 * (r1 + r2[i]) / (r2[i] * adcMask)
		);
}

BatteryObserver::~BatteryObserver(){
}

bool BatteryObserver::minimaliseBatteryUse(){
	if(batteryPercentVal() < 1)
		minimaliseBatteryUseFlag = true;
	else if(batteryPercentVal() > 5)
		minimaliseBatteryUseFlag = false;
	return minimaliseBatteryUseFlag;
}

void BatteryObserver::correctResistor(const uint8_t& cell, const uint8_t& referenceVoltage){
	double adc = measurements[cell];
	rezistorU[cell] = (uint32_t)(r2[cell] * (double(referenceVoltage) / 10 * adcMask / (adc * 3.3) - 1));
	uint8_t data[3] = {
		(uint8_t)((rezistorU[cell] >> 16) & 0xff),
		(uint8_t)((rezistorU[cell] >> 8) & 0xff),
		(uint8_t)(rezistorU[cell] & 0xff)
	};
	memory->write(memoryMap::rezistor_1_U_HSB + cell * 3, data, 3);
}

void BatteryObserver::correctResistor(const uint8_t& referenceVoltage){ // referenceVoltage 194 = 19.4[V]
	double reference = double(referenceVoltage) / 10;
	for(int8_t i = numberOfCells - 1; i >= 0 ; i--)
		if(reference > (maxVoltage * i + minVoltage))
			return correctResistor(i, referenceVoltage);
	THROW_invalid_argument("Voltage too low. Has to be higher than minVoltage.");
}

double BatteryObserver::batteryVoltageVal(){
	return batteryVoltageVal(numberOfCells - 1);
}

uint8_t BatteryObserver::batteryPercentVal(){
	double toReturn  = 100;
	double tmp = 0;
	for(uint8_t i = 0; i < numberOfCells; i++){
		tmp = batteryPercentVal(i);
		if(tmp < toReturn)
			toReturn = tmp;
	}
	return toReturn;
}

bool BatteryObserver::init(){
	uint8_t readedData[3];
	for(uint8_t i = 0; i < numberOfCells; i++){
		if(!memory->read(memoryMap::rezistor_1_U_HSB + i * 3, readedData, 3))
			return false;
		rezistorU[i] = (readedData[0] << 16) | (readedData[1] << 8) | readedData[2];
	}
	return true;
}
