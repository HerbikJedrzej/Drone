#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <fstream>
#include "../plot.hh"
#include "../../src/algoritms/ComplementaryFilter.hh"

namespace ComplementaryFilterTests{

class ComplementaryFilterProvider{
    class uncorrectFile{};
    std::ifstream pureSignals, noiseSignals, expectedAngles;
    double dataN[9];
    double dataP[9];
    double dataE[3];
public:
    ComplementaryFilterProvider(){
        std::string tmp;
        expectedAngles.open("./dataFiles/moduleTests/dataForKalmanModuleTestExpectedAngles.txt");
        pureSignals.open("./dataFiles/moduleTests/dataForKalmanModuleTest.txt");
        noiseSignals.open("./dataFiles/moduleTests/dataForKalmanModuleTestWithNoise.txt");
        if(!pureSignals.good())
            throw uncorrectFile();
        if(!expectedAngles.good())
            throw uncorrectFile();
        if(!noiseSignals.good())
            throw uncorrectFile();
        for(unsigned int i = 0; i < 2; i++){
            std::getline(expectedAngles, tmp);
            std::getline(pureSignals, tmp);
            std::getline(noiseSignals, tmp);
        }
    }
    ~ComplementaryFilterProvider(){
        expectedAngles.close();
        pureSignals.close();
        noiseSignals.close();
    }
    double *getNoiseData(){
        if(noiseSignals.eof())
            return nullptr;
        for(unsigned int i = 0; i < 9; i++)
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
    double *getExpectedAngles(){
        if (expectedAngles.eof())
            return nullptr;
        for (unsigned int i = 0; i < 3; i++)
            expectedAngles >> dataE[i];
        return dataE;
    }
};

TEST(ComplementaryFilter, mainTest){
    ComplementaryFilterProvider dataProvider;
    ComplementaryFilter filter;
    std::vector<double> t,
        // p1, p2, p3, p4, p5, p6,
        f1, f2, f3, e1, e2, e3,
        n1, n2, n3, n4, n5, n6;
    double *dataExpect = dataProvider.getExpectedAngles();
    // double *dataPure = dataProvider.getPureData();
    double *dataNoise = dataProvider.getNoiseData();
    unsigned int i = 0;
    while(
        // (dataPure != nullptr) ||
        (dataNoise != nullptr) ||
        (dataExpect != nullptr)){
            t.push_back(i++);
            // p1.push_back(dataPure[0]);
            // p2.push_back(dataPure[1]);
            // p3.push_back(dataPure[2]);
            // p4.push_back(dataPure[3]);
            // p5.push_back(dataPure[4]);
            // p6.push_back(dataPure[5]);
            n1.push_back(dataNoise[0]);
            n2.push_back(dataNoise[1]);
            n3.push_back(dataNoise[2]);
            n4.push_back(dataNoise[3]);
            n5.push_back(dataNoise[4]);
            n6.push_back(dataNoise[5]);
            filter.run(dataNoise, &dataNoise[3], &dataNoise[6], 0.0, 0.0);
            f1.push_back(filter[0]);
            f2.push_back(filter[1]);
            f3.push_back(filter[2]);
            e1.push_back(dataExpect[0]);
            e2.push_back(dataExpect[1]);
            e3.push_back(dataExpect[2]);
            dataExpect = dataProvider.getExpectedAngles();
            // dataPure = dataProvider.getPureData();
            dataNoise = dataProvider.getNoiseData();
    }

    plot::figure_size(1200, 780);
    plot::title("Filtred roll angle.");
    plot::named_plot("Filtred", t, f1);
    plot::named_plot("Expected", t, e1);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/FiltredRoll.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Filtred pitch angle.");
    plot::named_plot("Filtred", t, f2);
    plot::named_plot("Expected", t, e2);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/FiltredPitch.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Filtred yawl angle.");
    plot::named_plot("Filtred", t, f3);
    plot::named_plot("Expected", t, e3);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/FiltredYawl.png");
    plot::figure_close();
}

}
