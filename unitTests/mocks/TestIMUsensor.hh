#pragma once
#include <IMUsensorIfc.hh>
#include <map>
#include "TestDelay.hh"
#include <array>
#include <queue>

namespace TestMocks{
    class TestIMU : public Drivers::IMUsensorIfc{
        bool akcAndGyroDataReadyFlag, magDataReadyFlag;
    public:
        bool initReturn = {true};
        TestIMU();
        void handleTimeEvent(DriverIfc*) override;
        void handleFinish(DriverIfc*) override;
        bool init() override;
    };
}