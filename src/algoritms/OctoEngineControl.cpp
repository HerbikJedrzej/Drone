#include "OctoEngineControl.hh"
#include <OwnExceptions.hh>

#define sqrt2 1.4142135623730950488016887242097

OctoEngineControl::OctoEngineControl(EngineIfc* const _engines):
    EngineControlIfc(_engines){
        if(engines->size() != 8)
            THROW_invalid_argument("OctoEngineControl needs exacly 8 engines.");
    }

void OctoEngineControl::set(const double& power, const double& pidX, const double& pidY, const double& pidZ){
	engines->set(0, power +   pidX         / 4         - pidZ);
	engines->set(1, power + ( pidX + pidY) / 8 + pidZ);
	engines->set(2, power +          pidY  / 4         - pidZ);
	engines->set(3, power + (-pidX + pidY) / 8 + pidZ);
	engines->set(4, power -   pidX         / 4         - pidZ);
	engines->set(5, power + (-pidX - pidY) / 8 + pidZ);
	engines->set(6, power -          pidY  / 4         - pidZ);
	engines->set(7, power + ( pidX - pidY) / 8 + pidZ);
}

void OctoEngineControl::setAxisX(const double& power, const double& pid){
	engines->set(0, power + pid         / 4);
	engines->set(1, power + pid / 8);
	engines->set(2, 0.0);
	engines->set(3, power - pid / 8);
	engines->set(4, power - pid         / 4);
	engines->set(5, power - pid / 8);
	engines->set(6, 0.0);
	engines->set(7, power + pid / 8);
}

void OctoEngineControl::setAxisY(const double& power, const double& pid){
	engines->set(0, 0.0);
	engines->set(1, power + pid / 8);
	engines->set(2, power + pid         / 4);
	engines->set(3, power + pid / 8);
	engines->set(4, 0.0);
	engines->set(5, power - pid / 8);
	engines->set(6, power - pid         / 4);
	engines->set(7, power - pid / 8);
}

void OctoEngineControl::setAxisZ(const double& power, const double& pid){
	engines->set(0, power - pid/2);
	engines->set(1, power + pid/2);
	engines->set(2, power - pid/2);
	engines->set(3, power + pid/2);
	engines->set(4, power - pid/2);
	engines->set(5, power + pid/2);
	engines->set(6, power - pid/2);
	engines->set(7, power + pid/2);
}
