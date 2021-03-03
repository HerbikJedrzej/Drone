#include "AltitudeProvider.hh"
#include <OwnExceptions.hh>

AltitudeProvider::AltitudeProvider(const double& _barometerVariance, const double& _dt):
    result(0),
    dt(_dt),
    barometerVariance(_barometerVariance){
        for(unsigned int _x = 0; _x < 3; _x++){
            for(unsigned int _y = 0; _y < 3; _y++)
                P[_x][_y] = 0.0;
            X[_x] = 0.0;
        }
    }

void AltitudeProvider::run(const double& acceleration, const double& _accelerationVariance, const double& altitude){
	accelerationVariance = _accelerationVariance;
	countKalmanGain();
	countPredictedX();
	countFiltredX(acceleration, altitude);
	countFiltredP();
	countPredictedP();
}

double AltitudeProvider::getAltitude(){
    return X[0];
}

void AltitudeProvider::countKalmanGain(){
    double n[2][2] = {
        {P[0][0] + barometerVariance, P[0][2]},
        {P[2][0], P[2][2] + accelerationVariance}
    };
    inverse(n);
    for(unsigned int _x = 0; _x < 3; _x++)
        for(unsigned int _y = 0; _y < 2; _y++){
            K[_x][_y] = 0.0;
            for(unsigned int _z = 0; _z < 2; _z++)
                K[_x][_y] += P[_x][_z * 2] * n[_z][_y];
        }
}

void AltitudeProvider::countPredictedX(){
    X[0] = X[0] + X[1] * dt + X[2] * dt * dt / 2;
    X[1] = X[1] + X[2] * dt;
}

void AltitudeProvider::countFiltredX(const double& acceleration, const double& altitude){
    const double deltaAltitude = altitude - X[0];
    const double deltaAcceleration = acceleration - X[2];
    for(unsigned int i = 0; i < 3; i++)
        X[i] = X[i] + K[i][0] * deltaAltitude + K[i][1] * deltaAcceleration;
}

void AltitudeProvider::countFiltredP(){
    const double tmp[3][3] = {
        {1.0 - K[0][0], 0.0,     - K[0][1]},
        {    - K[1][0], 1.0,     - K[1][1]},
        {    - K[2][0], 0.0, 1.0 - K[2][1]}
    };
    const double tmpP[3][3] = {
        {P[0][0], P[0][1], P[0][2]},
        {P[1][0], P[1][1], P[1][2]},
        {P[2][0], P[2][1], P[2][2]}
    };
    for(unsigned int _x = 0; _x < 3; _x++)
        for(unsigned int _y = 0; _y < 3; _y++){
            P[_x][_y] = 0.0;
            for(unsigned int _z = 0; _z < 3; _z++)
                P[_x][_y] += tmp[_x][_z] * tmpP[_z][_y];
        }
}

void AltitudeProvider::countPredictedP(){ // A*P*A' + Q
    double n[3][3];
    const double dt2 = dt * dt / 2;
    for(unsigned int i = 0; i < 3; i++){
        n[0][i] = P[0][i] + P[1][i] * dt + P[2][i] * dt2;
        n[1][i] = P[1][i] + P[2][i] * dt;
        n[2][i] = P[2][i];
    }
    for(unsigned int i = 0; i < 3; i++){
        P[i][0] = n[i][0] + n[i][1] * dt + n[i][2] * dt2;
        P[i][1] = n[i][1] + n[i][2] * dt;
        P[i][2] = n[i][2];
    }
    P[2][2] += 1;
}

void AltitudeProvider::inverse(double m[2][2]){
    const double det = m[0][0] * m[1][1] - m[0][1] * m[1][0];
    const double tmp[2][2] = {{m[1][1], -m[0][1]},{-m[1][0], m[0][0]}};
    for(unsigned int i = 0; i < 2; i++)
        for(unsigned int j = 0; j < 2; j++)
            m[i][j] = tmp[i][j] / det;
}
