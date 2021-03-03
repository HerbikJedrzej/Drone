#include "TestIMUsensor.hh"
#include "TestDelay.hh"

namespace TestMocks{
    TestIMU::TestIMU():
        Drivers::IMUsensorIfc(akcAndGyroDataReadyFlag, magDataReadyFlag, Drivers::IMUsensorIfc::RottatedAxis::none, TestMocks::delay){}

    void TestIMU::handleTimeEvent(DriverIfc*){
    }

    void TestIMU::handleFinish(DriverIfc*){
    }

    bool TestIMU::init(){
        return initReturn;
    }
}