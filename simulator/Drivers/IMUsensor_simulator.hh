#pragma once
#include <IMUsensorIfc.hh>

namespace Drivers{

class IMU_Symulator : public IMUsensorIfc{
public:
    IMU_Symulator(bool& akcelerometerAndGyroskopeDataReadyFlag, bool& magnetometerDataReadyFlag);
    void handleTimeEvent(DriverIfc*) final;
    void handleFinish(DriverIfc*) final;
    bool init() final;
};
}