#include <gtest/gtest.h>
#include <iostream>
#include "../plot.hh"
#include "../../src/algoritms/PID.hh"

namespace PIDTests{

class Model{
protected:
    double y;
    double yPast;
    const double dt{0.01};
public:
    Model(const double val1):
        y(val1),
        yPast(0){}
    virtual void giveFeedbacDataAndRun(const double value1){
        yPast = y;
        double vp1 = value1 - yPast;
        y += vp1 * dt;
    }
    double getOutput(){
        return y;
    }
};

TEST(PID_test, simpleInertion){
    Model model(-2);
    double P = 16.8294;
    double I = 84.3967;
    double D = -0.0090082;
    PID pid;
    pid.P = P;
    pid.I = I;
    pid.D = D;
    int n = 1000;
    std::vector<double> t(n), u1(n), r1(n);
    for (int i = 0; i < n; ++i){
        t.at(i) = i;
        if (i < 400)
            u1.at(i) = -1;
        else
            u1.at(i) = 0;
        pid.setR(u1[i]);
        double opd = pid.calculate();
        model.giveFeedbacDataAndRun(opd);
        r1.at(i) = model.getOutput();
        pid.setY(model.getOutput());
    }
    plot::figure_size(1200, 780);
    plot::named_plot("U(t)", t, u1);
    plot::named_plot("r(t)", t, r1);
    plot::title("PID single inercion.");
    plot::legend();
    plot::save("./moduleTestsOutputData/PID/PIDsimpleInrecion.png");
    plot::figure_close();
}

TEST(PID_test, simpleInertion_antiWindup){
    Model model(-2);
    double P = 16.8294;
    double I = 84.3967;
    double D = -0.0090082;
    PID pid;
    pid.P = P;
    pid.I = I;
    pid.D = D;
    pid.AntiWindup = 0.02;
    pid.setLimit(-25, 25);
    int n = 1000;
    std::vector<double> t(n), u1(n), r1(n);
    for (int i = 0; i < n; ++i){
        t.at(i) = i;
        if (i < 400)
            u1.at(i) = -1;
        else
            u1.at(i) = 0;
        pid.setR(u1[i]);
        double opd = pid.calculate();
        model.giveFeedbacDataAndRun(opd);
        r1.at(i) = model.getOutput();
        pid.setY(model.getOutput());
    }
    plot::figure_size(1200, 780);
    plot::named_plot("U(t)", t, u1);
    plot::named_plot("r(t)", t, r1);
    plot::title("PID single inercion.");
    plot::legend();
    plot::save("./moduleTestsOutputData/PID/PIDsimpleInrecionAntiWindup.png");
    plot::figure_close();
}

}
