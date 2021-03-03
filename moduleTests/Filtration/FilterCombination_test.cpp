#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <fstream>
#include "../plot.hh"
#include "../../src/algoritms/StepingMeanFilter.hh"
#include "../../src/algoritms/Kalman.hh"
#include "../../src/algoritms/ComplementaryFilter.hh"

namespace FilterCombinationTests{

class FilterCombinationProvider{
    class uncorrectFile{};
    std::ifstream pureSignals, noiseSignals, expectedAngles;
    double dataN[9];
    double dataP[9];
    double dataE[3];
public:
    FilterCombinationProvider(){
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
            std::getline(pureSignals, tmp);
            std::getline(noiseSignals, tmp);
            std::getline(expectedAngles, tmp);
        }
    }
    ~FilterCombinationProvider(){
        expectedAngles.close();
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
    double *getExpectedAngles(){
        if (expectedAngles.eof())
            return nullptr;
        for (unsigned int i = 0; i < 3; i++)
            expectedAngles >> dataE[i];
        return dataE;
    }
};

TEST(FilterCombinationProvider, mainTest){
    FilterCombinationProvider dataProvider;
    StepingMeanFilter filterStepingMean;
    KalmanAHRS filterKalman;
    ComplementaryFilter filterComplementary;
    std::vector<double> t,
        p1, p2, p3, p4, p5, p6,
        f1, f2, f3, f4, f5, f6,
        e1, e2, e3,
        n1, n2, n3, n4, n5, n6;
    double *dataPure = dataProvider.getPureData();
    double *dataExpect = dataProvider.getExpectedAngles();
    double *dataNoise = dataProvider.getNoiseData();
    unsigned int i = 0;
    while (
        (dataPure != nullptr) ||
        (dataExpect != nullptr) ||
        (dataNoise != nullptr)){
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

            filterStepingMean.run(dataNoise);
            if(i > 8)
                filterKalman.changeR(filterStepingMean.getVariance());
            filterComplementary.run(
                filterKalman.run(
                    filterStepingMean.getFiltredValues(),
                    &dataNoise[3],
                    filterComplementary[0],
                    filterComplementary[1]
                ),
                &dataNoise[3],
                // nullptr
                &dataNoise[6],
                0.0,
                0.0
            );

            f1.push_back(filterKalman[0]);
            f2.push_back(filterKalman[1]);
            f3.push_back(filterKalman[2]);
            f4.push_back(filterComplementary[0]);
            f5.push_back(filterComplementary[1]);
            f6.push_back(filterComplementary[2]);
            e1.push_back(dataExpect[0]);
            e2.push_back(dataExpect[1]);
            e3.push_back(dataExpect[2]);
            dataExpect = dataProvider.getExpectedAngles();
            dataPure = dataProvider.getPureData();
            dataNoise = dataProvider.getNoiseData();
    }

    plot::figure_size(1200, 780);
    plot::title("Filtred roll angle.");
    plot::named_plot("Filtred", t, f4);
    plot::named_plot("Expected", t, e1);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/CombinationFilterRoll.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Filtred pitch angle.");
    plot::named_plot("Filtred", t, f5);
    plot::named_plot("Expected", t, e2);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/CombinationFilterPitch.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Filtred yawl angle.");
    plot::named_plot("Filtred", t, f6);
    plot::named_plot("Expected", t, e3);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/CombinationFilterYawl.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Filtred X axis.");
    plot::named_plot("noise", t, n1);
    plot::named_plot("Filtred", t, f1);
    plot::named_plot("Pure", t, p1);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/CombinationFilterXAxis.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Filtred X axis.");
    plot::named_plot("noise", t, n2);
    plot::named_plot("Filtred", t, f2);
    plot::named_plot("Pure", t, p2);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/CombinationFilterYAxis.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Filtred X axis.");
    plot::named_plot("noise", t, n3);
    plot::named_plot("Filtred", t, f3);
    plot::named_plot("Pure", t, p3);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/CombinationFilterZAxis.png");
    plot::figure_close();
}

}
