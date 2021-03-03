#include "IMUsensor_simulator.hh"

namespace Drivers{

IMU_Symulator::IMU_Symulator(bool& akcelerometerAndGyroskopeDataReadyFlag, bool& magnetometerDataReadyFlag):
    IMUsensorIfc(akcelerometerAndGyroskopeDataReadyFlag, magnetometerDataReadyFlag, RottatedAxis::none, [](uint32_t)->void{}){
}

void IMU_Symulator::handleTimeEvent(DriverIfc*){
}

void IMU_Symulator::handleFinish(DriverIfc*){
}

bool IMU_Symulator::init(){
    return true;
}

}