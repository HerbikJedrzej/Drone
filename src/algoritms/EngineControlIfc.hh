#pragma once
#include <EngineIfc.hh>
#include <PID.hh>

class EngineControlIfc{
public:
    EngineControlIfc(EngineIfc* const _engines):
        engines(_engines),
        pointedEngine(0){}

    virtual void set(const double& power, const double& pidXVal, const double& pidYVal, const double& pidZVal) = 0;
    virtual void setAxisX(const double& power, const double& pidVal = 0) = 0;
    virtual void setAxisY(const double& power, const double& pidVal = 0) = 0;
    virtual void setAxisZ(const double& power, const double& pidVal = 0) = 0;

    void set(const double& power, PID* const pidX, PID* const pidY, PID* const pidZ){
        this->set(power, pidX->calculate(), pidY->calculate(), pidZ->calculate());
    }

    void setAxisX(const double& power, PID* const pid){
        this->setAxisX(power, pid->calculate());
    }

    void setAxisY(const double& power, PID* const pid){
        this->setAxisY(power, pid->calculate());
    }

    void setAxisZ(const double& power, PID* const pid){
        this->setAxisZ(power, pid->calculate());
    }

    void setAll(const double percentVal){
        for(uint8_t i = 0; i < engines->size(); i++)
            engines->set(i, percentVal);
    }

    void incrementEngine(const double percentVal){
        if(pointedEngine >= engines->size()){
            setAll(0);
            pointedEngine = 0;
            return;
        }
        setAll(0);
        engines->set(pointedEngine++, percentVal);
    }

    void operator()(const uint8_t e, const double percentVal){
        engines->set(e, percentVal);
    }
protected:
    EngineIfc* const engines;
    uint8_t pointedEngine;
};
