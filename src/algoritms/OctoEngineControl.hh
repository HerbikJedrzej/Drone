#pragma once
#include <EngineControlIfc.hh>

class OctoEngineControl : public EngineControlIfc{
public:
    OctoEngineControl(EngineIfc* const _engines);
    void set(const double& power, const double& pidX, const double& pidY, const double& pidZ) final;
    void setAxisX(const double& power, const double& pid = 0) final;
    void setAxisY(const double& power, const double& pid = 0) final;
    void setAxisZ(const double& power, const double& pid = 0) final;
};
