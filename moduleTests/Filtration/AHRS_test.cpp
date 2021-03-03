#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <fstream>
#include "../plot.hh"
#include "../../src/algoritms/AHRS.hh"

namespace AHRStest{

class LSM6DS33{
    class uncorrectFile
    {};
    std::ifstream noiseSignals, expectedAngles;
    double dataN[9];
    double dataE[3];
    double complementaryPastAngles = {0};
public:
    LSM6DS33(){
        std::string tmp;
        expectedAngles.open("./dataFiles/moduleTests/dataForKalmanModuleTestExpectedAngles.txt");
        noiseSignals.open("./dataFiles/moduleTests/dataForKalmanModuleTestWithNoise.txt");
        if (!expectedAngles.good())
            throw uncorrectFile();
        if (!noiseSignals.good())
            throw uncorrectFile();
        for (unsigned int i = 0; i < 2; i++){
            std::getline(noiseSignals, tmp);
            std::getline(expectedAngles, tmp);
        }
    }
    ~LSM6DS33(){
        expectedAngles.close();
        noiseSignals.close();
    }
    double *getNoiseData(){
        if (noiseSignals.eof())
            return nullptr;
        for (unsigned int i = 0; i < 9; i++)
            noiseSignals >> dataN[i];
        return dataN;
    }
    double *getExpectedAngles(){
        if (expectedAngles.eof())
            return nullptr;
        for (unsigned int i = 0; i < 3; i++)
            expectedAngles >> dataE[i];
        return dataE;
    }
    double expandRangeOfAngle(const double mesured){
        int i = ((int)complementaryPastAngles) / (2 * M_PI);
        if ((2 * M_PI) * i + mesured - complementaryPastAngles > (1.7 * M_PI))
            i--;
        if ((2 * M_PI) * i + mesured - complementaryPastAngles < (-1.7 * M_PI))
            i++;
        complementaryPastAngles = (2 * M_PI) * i + mesured;
        return complementaryPastAngles;
    }
    double getAngle(unsigned int i){
        if(i == 0)
            return atan2(-dataN[1], -dataN[2]) * 180 / M_PI;
        if (i == 1)
            return atan2(cos(dataE[0]) * dataN[0], -dataN[2]) * 180 / M_PI;
        else
            return expandRangeOfAngle(atan2(
                    sin(dataE[0]) * dataN[8] - cos(dataE[0]) * dataN[7]
                ,
                    cos(dataE[1]) * dataN[6] +
                    sin(dataE[0]) * sin(dataE[1]) * dataN[7] +
                    cos(dataE[0]) * sin(dataE[1]) * dataN[8]
                ))
                * 180 / M_PI;
    }
};

TEST(AHRStest, mainTest)
{
    LSM6DS33 dataProvider;
    std::vector<double> t,
        f1, f2, f3,
        e1, e2, e3,
        n1, n2, n3;
    double *dataExpect = dataProvider.getExpectedAngles();
    double *dataNoise = dataProvider.getNoiseData();
    AHRS ahrs(dataNoise, &dataNoise[3], &dataNoise[6]);
    unsigned int i = 0;
    while(
        (dataExpect != nullptr) ||
        (dataNoise != nullptr)){
            ahrs.run();
            t.push_back(i++);
            n1.push_back(dataProvider.getAngle(0));
            n2.push_back(dataProvider.getAngle(1));
            n3.push_back(dataProvider.getAngle(2));
            f1.push_back(ahrs[0]);
            f2.push_back(ahrs[1]);
            f3.push_back(ahrs[2]);
            e1.push_back(dataExpect[0] * 180 / M_PI);
            e2.push_back(dataExpect[1] * 180 / M_PI);
            e3.push_back(dataExpect[2] * 180 / M_PI);
            dataExpect = dataProvider.getExpectedAngles();
            dataNoise = dataProvider.getNoiseData();
    }

    plot::figure_size(1200, 780);
    plot::title("Filtred roll angle.");
    plot::named_plot("Mesured", t, n1);
    plot::named_plot("Filtred", t, f1);
    plot::named_plot("Expected", t, e1);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/AHRS_Roll.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Filtred pitch angle.");
    plot::named_plot("Mesured", t, n2);
    plot::named_plot("Filtred", t, f2);
    plot::named_plot("Expected", t, e2);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/AHRS_Pitch.png");
    plot::figure_close();

    plot::figure_size(1200, 780);
    plot::title("Filtred yawl angle.");
    plot::named_plot("Mesured", t, n3);
    plot::named_plot("Filtred", t, f3);
    plot::named_plot("Expected", t, e3);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/AHRS_Yawl.png");
    plot::figure_close();
}

}
