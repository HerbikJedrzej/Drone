#pragma once
#include <BarometerIfc.hh>

class AltitudeProvider{
public:
    AltitudeProvider(Drivers::BarometerIfc* const _barometer, const double& _barometerPrecision, const double& _dt = 0.01);
    void run();
    double getAltitude();
    void init(const double& altitude);
protected:
    const double dt;
private:
    Drivers::BarometerIfc* const barometer;
    double X = 0.0;
    double P = 0.0;
    const double barometerPrecision;
};
