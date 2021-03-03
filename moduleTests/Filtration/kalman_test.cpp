#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <fstream>
#include "../plot.hh"
#include "../../src/algoritms/Kalman.hh"
#include "../../src/algoritms/StepingMeanFilter.hh"

namespace KalmanTests{

class KalmanDataProvider{
    class uncorrectFile{};
    std::ifstream pureSignals, noiseSignals;
    double dataN[9];
    double dataP[9];
public:
    KalmanDataProvider(){
        std::string tmp;
        pureSignals.open("./dataFiles/moduleTests/dataForKalmanModuleTest.txt");
        noiseSignals.open("./dataFiles/moduleTests/dataForKalmanModuleTestWithNoise.txt");
        if (!pureSignals.good())
            throw uncorrectFile();
        if (!noiseSignals.good())
            throw uncorrectFile();
        std::getline(pureSignals, tmp);
        std::getline(pureSignals, tmp);
        std::getline(noiseSignals, tmp);
        std::getline(noiseSignals, tmp);
    }
    ~KalmanDataProvider(){
        pureSignals.close();
        noiseSignals.close();
    }
    double *getNoiseData(){
        if (noiseSignals.eof())
            return nullptr;
        for (unsigned int i = 0; i < 9; i++)
            noiseSignals >> dataN[i];
        return dataN;
    }
    double *getPureData(){
        if (pureSignals.eof())
            return nullptr;
        for (unsigned int i = 0; i < 9; i++)
            pureSignals >> dataP[i];
        return dataP;
    }
};

TEST(Kalman, mainTest){
    double angleRoll = 0;
    double anglePitch = 0;
    KalmanAHRS kalman;
    KalmanDataProvider dataProvider;
    StepingMeanFilter warianceProvider;
    std::vector<double> t,
        p1, p2, p3, p4, p5, p6,
        f1, f2, f3, f4, f5, f6,
        n1, n2, n3, n4, n5, n6;
    double *dataPure = dataProvider.getPureData();
    double *dataNoise = dataProvider.getNoiseData();
    unsigned int i = 0;
    while ((dataPure != nullptr) || (dataNoise != nullptr)){
        double Y1[] = {dataNoise[0], dataNoise[1], dataNoise[2]};
        double Y2[] = {dataNoise[3], dataNoise[4], dataNoise[5]};
        t.push_back(i++);
        p1.push_back(dataPure[0]);
        p2.push_back(dataPure[1]);
        p3.push_back(dataPure[2]);
        p4.push_back(dataPure[3]);
        p5.push_back(dataPure[4]);
        p6.push_back(dataPure[5]);
        n1.push_back(dataNoise[0]);
        n2.push_back(dataNoise[1]);
        n3.push_back(dataNoise[2]);
        n4.push_back(dataNoise[3]);
        n5.push_back(dataNoise[4]);
        n6.push_back(dataNoise[5]);
        warianceProvider.run(dataNoise);
        kalman.changeR(warianceProvider.getVariance());
        kalman.run(Y1, Y2, angleRoll, anglePitch);
        angleRoll = atan2(-kalman[1], -kalman[2]);
        anglePitch = atan2(cos(angleRoll) * (kalman[0]), -(kalman[2]));
        f1.push_back(kalman[0]);
        f2.push_back(kalman[1]);
        f3.push_back(kalman[2]);
        f4.push_back(kalman[3]);
        f5.push_back(kalman[4]);
        f6.push_back(kalman[5]);
        dataPure = dataProvider.getPureData();
        dataNoise = dataProvider.getNoiseData();
    }
    plot::figure_size(1200, 780);
    plot::title("Kalman filtration of samples data. Akcelerometr X.");
    plot::named_plot("Noise signal", t, n1);
    plot::named_plot("Filtred signal", t, f1);
    plot::named_plot("Pure signal", t, p1);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/KalmanAkceletomertX.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Kalman filtration of samples data. Akcelerometr Y.");
    plot::named_plot("Noise signal", t, n2);
    plot::named_plot("Filtred signal", t, f2);
    plot::named_plot("Pure signal", t, p2);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/KalmanAkceletomertY.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Kalman filtration of samples data. Akcelerometr Z.");
    plot::named_plot("Noise signal", t, n3);
    plot::named_plot("Filtred signal", t, f3);
    plot::named_plot("Pure signal", t, p3);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/KalmanAkceletomertZ.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Kalman filtration of samples data. Gyroskope X.");
    plot::named_plot("Noise signal", t, n4);
    plot::named_plot("Filtred signal", t, f4);
    plot::named_plot("Pure signal", t, p4);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/KalmanGyroskopeX.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Kalman filtration of samples data. Gyroskope Y.");
    plot::named_plot("Noise signal", t, n5);
    plot::named_plot("Filtred signal", t, f5);
    plot::named_plot("Pure signal", t, p5);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/KalmanGyroskopeY.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Kalman filtration of samples data. Gyroskope Z.");
    plot::named_plot("Noise signal", t, n6);
    plot::named_plot("Filtred signal", t, f6);
    plot::named_plot("Pure signal", t, p6);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/KalmanGyroskopeZ.png");
    plot::figure_close();
}

}
