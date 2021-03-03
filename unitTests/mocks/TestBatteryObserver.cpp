#include "TestBatteryObserver.hh"
#include "TestDelay.hh"

namespace TestMocks{
    TestBatteryObserver::TestBatteryObserver(const double* _rezistorD, const uint8_t _numberOfCells, const double& _minVoltage, const double& _maxVoltage):
        BatteryObserver(&memory.memory, measurements, _rezistorD, _numberOfCells, 0xfff, _minVoltage, _maxVoltage){}
}