#include "QuatroEngineControl.hh"
#include <OwnExceptions.hh>

QuatroEngineControl::QuatroEngineControl(EngineIfc* const _engines):
    EngineControlIfc(_engines){
        if(engines->size() != 4)
            THROW_invalid_argument("QuatroEngineControl needs exacly 4 engines.");
    }

void QuatroEngineControl::set(const double& power, const double& pidX, const double& pidY, const double& pidZ){
	engines->set(0, power + pidX / 2 + pidZ);
	engines->set(1, power - pidY / 2 - pidZ);
	engines->set(2, power - pidX / 2 + pidZ);
	engines->set(3, power + pidY / 2 - pidZ);
}

void QuatroEngineControl::setAxisX(const double& power, const double& pid){
	engines->set(0, power + pid / 2);
	engines->set(1, 0);
	engines->set(2, power - pid / 2);
	engines->set(3, 0);
}

void QuatroEngineControl::setAxisY(const double& power, const double& pid){
	engines->set(0, 0);
	engines->set(1, power - pid / 2);
	engines->set(2, 0);
	engines->set(3, power + pid / 2);
}

void QuatroEngineControl::setAxisZ(const double& power, const double& pid){
	engines->set(0, power + pid);
	engines->set(1, power - pid);
	engines->set(2, power + pid);
	engines->set(3, power - pid);
}
