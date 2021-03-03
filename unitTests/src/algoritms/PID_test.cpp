#include <gtest/gtest.h>
#include "../../src/algoritms/PID.hh"
#include <cstdlib>

#define AMOUNT_OF_TESTS 1
#define xSizeOfData 4

namespace PIDTests{

void quickTest(const double& _P,  const double& _I,  const double& _D, double answers[xSizeOfData]){
    PID pid;
    pid.P = _P;
    pid.I = _I;
    pid.D = _D;
    double yData[xSizeOfData];
    double rData[xSizeOfData];
    yData[0] = 1.235;
    yData[1] = -0.24;
    yData[2] = 41.1;
    yData[3] = 0.3456;
    rData[0] = 34.52;
    rData[1] = -21;
    rData[2] = 24.1;
    rData[3] = 0.0241;
    for(unsigned int i = 0; i < xSizeOfData; i++){
        pid.setR(rData[i]);
        pid.setY(yData[i]);
        auto odp = pid.calculate();
        EXPECT_DOUBLE_EQ(answers[i], odp);
    }
}

TEST(PID, Ptest){
    double P = 1;
    double I = 0;
    double D = 0;
    double expectResult[xSizeOfData];
    expectResult[0] = 33.2850;
    expectResult[1] = -20.7600;
    expectResult[2] = -17.0000;
    expectResult[3] = -0.3215;
    quickTest(P, I, D, expectResult);
}

TEST(PID, Itest){
    double P = 0;
    double I = 0.01;
    double D = 0;
    double expectResult[xSizeOfData];
    expectResult[0] = 0.33285;
    expectResult[1] = 0.12525;
    expectResult[2] = -0.04475;
    expectResult[3] = -0.047965;
    quickTest(P, I, D, expectResult);
}

TEST(PID, Dtest){
    double P = 0;
    double I = 0;
    double D = 100;
    double expectResult[xSizeOfData];
    expectResult[0] = 3328.5;
    expectResult[1] = -5404.5;
    expectResult[2] = 376.0000;
    expectResult[3] = 1667.85;
    quickTest(P, I, D, expectResult);
}

TEST(PID, allPIDtest){
    double P = 1.35;
    double I = 0.0526;
    double D = 512.0;
    double expectResult[xSizeOfData];
    expectResult[0] = 17088.605541;
    expectResult[1] = -27698.407185;
    expectResult[2] = 1901.934615;
    expectResult[3] = 8538.7056791;
    quickTest(P, I, D, expectResult);
}

}
