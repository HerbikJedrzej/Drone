#pragma once

#include <BarometerIfc.hh>

namespace TestMocks{

class BarometerMock : public Drivers::BarometerIfc{
    void handleTimeEvent(DriverIfc*) override {}
    void handleFinish(DriverIfc*) override {}
    bool mockFlag;
public:
    BarometerMock():BarometerIfc(mockFlag, [](uint32_t)->void{}){}
    virtual bool init() override {return false;}

    double preasure;
    double temperature;
    double altitude;
    double altitudeVariance;

    double getPreasure() override {
        return preasure;
    }
    double getTemperature() override {
        return temperature;
    }
    double getAltitude() override {
        return altitude;
    }
};

}
