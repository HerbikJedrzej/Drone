#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <fstream>
#include "../plot.hh"
#include "../../src/algoritms/StepingMeanFilter.hh"

namespace StepingMeanFilterTests{

class StepingMeanFilterProvider{
    class uncorrectFile{};
    std::ifstream pureSignals, noiseSignals;
    double dataN[9];
    double dataP[9];
public:
    StepingMeanFilterProvider(){
        std::string tmp;
        pureSignals.open("./dataFiles/moduleTests/dataForKalmanModuleTest.txt");
        noiseSignals.open("./dataFiles/moduleTests/dataForKalmanModuleTestWithNoise.txt");
        if(!pureSignals.good())
            throw uncorrectFile();
        if(!noiseSignals.good())
            throw uncorrectFile();
        for(unsigned int i = 0; i < 2; i++){
            std::getline(pureSignals, tmp);
            std::getline(noiseSignals, tmp);
        }
    }
    ~StepingMeanFilterProvider(){
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

TEST(StepingMeanFilter, mainTest){
    StepingMeanFilterProvider dataProvider;
    StepingMeanFilter filter;
    std::vector<double> t,
        p1, p2, p3,
        f1, f2, f3,
        n1, n2, n3;
    double *dataPure = dataProvider.getPureData();
    double *dataNoise = dataProvider.getNoiseData();
    unsigned int i = 0;
    while (
        (dataPure != nullptr) ||
        (dataNoise != nullptr)){
            t.push_back(i++);
            p1.push_back(dataPure[0]);
            p2.push_back(dataPure[1]);
            p3.push_back(dataPure[2]);
            n1.push_back(dataNoise[0]);
            n2.push_back(dataNoise[1]);
            n3.push_back(dataNoise[2]);
            filter.run(dataNoise);
            f1.push_back(filter[0]);
            f2.push_back(filter[1]);
            f3.push_back(filter[2]);
            dataPure = dataProvider.getPureData();
            dataNoise = dataProvider.getNoiseData();
    }

    plot::figure_size(1200, 780);
    plot::title("Filtred X axis.");
    plot::named_plot("noise", t, n1);
    plot::named_plot("Filtred", t, f1);
    plot::named_plot("Pure", t, p1);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/StepingMeanXAxis.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Filtred X axis.");
    plot::named_plot("noise", t, n2);
    plot::named_plot("Filtred", t, f2);
    plot::named_plot("Pure", t, p2);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/StepingMeanYAxis.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Filtred X axis.");
    plot::named_plot("noise", t, n3);
    plot::named_plot("Filtred", t, f3);
    plot::named_plot("Pure", t, p3);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/StepingMeanZAxis.png");
    plot::figure_close();
}

}
