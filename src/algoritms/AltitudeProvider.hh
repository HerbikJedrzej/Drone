#pragma once

class AltitudeProvider{
public:
    AltitudeProvider(const double& barometerVariance, const double& _dt = 0.01);
    void run(const double& acceleration, const double& velocity, const double& position);
    double getAltitude();
protected:
	void countKalmanGain();
	void countPredictedX();
	void countFiltredX(const double& acceleration, const double& altitude);
	void countFiltredP();
	void countPredictedP();

    double result;
    const double dt;
    const double barometerVariance;
    double accelerationVariance;
    double X[3];
    double P[3][3];
    double K[3][2];
private:
    void inverse(double m[2][2]);
};
