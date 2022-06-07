#include "AltitudeProvider.hh"
#include <OwnExceptions.hh>

AltitudeProvider::AltitudeProvider(Drivers::BarometerIfc* const _barometer, const double& _barometerPrecision, const double& _dt):
    dt(_dt),
    barometer(_barometer),
    barometerPrecision(_barometerPrecision){}

double AltitudeProvider::getAltitude(){
    return X;
}

void AltitudeProvider::init(const double& altitude){
    X = altitude;
}

void AltitudeProvider::run(){
    const double Pp = P + barometerPrecision;
    const double K = Pp / (Pp + 40);
    X = X + K * (barometer->getAltitude() - X);
    P = (1 - K) * Pp;
}
