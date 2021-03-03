#pragma once
#include <BatteryObserver.hh>
#include <TestMemory.hh>

namespace TestMocks{
    class TestBatteryObserver : public BatteryObserver{
        TestMocks::TestMemory memory;
        uint16_t measurements[3];
    public:
        TestBatteryObserver(const double* _rezistorD, const uint8_t _numberOfCells, const double& _minVoltage, const double& _maxVoltage);
    };
}