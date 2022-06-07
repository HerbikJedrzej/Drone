#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <fstream>
#include "../plot.hh"
#include <AltitudeProvider.hh>
#include <math.h>

#include <SumUpDataContainer.hh>
#include <TestBarometer.hh>
#include <VarianceProvider.hh>

using TestHelpers::SumUpDataContainer;
using TestHelpers::VarianceProvider;
using TestMocks::BarometerMock;

#define TestName ::testing::UnitTest::GetInstance()->current_test_info()->name()
#define TestOutputName std::string("./moduleTestsOutputData/Filtration/") + TestName + ".png"

namespace AltitudeFiltration{

class DataProvider{
    class uncorrectFile{};
    std::ifstream dataFile;
    std::vector<double> dataAcc;
    std::vector<double> dataBarometer;
    std::vector<double> time;
    void readData(){
        double localData = 0.0;
        double localTime = 0.0;
        while(true){
            dataFile >> localData;
            if(dataFile.eof())
                return;
            dataBarometer.push_back(localData);
            dataFile >> localData;
            dataAcc.push_back(localData);
            // dataAcc.push_back(0);
            time.push_back(localTime);
            localTime += 0.01;
        }
    }
public:
    DataProvider(const std::string& name){
        std::string tmp;
        dataFile.open(name);
        if(!dataFile.good())
            throw uncorrectFile();
        for(unsigned int i = 0; i < 2; i++){
            std::getline(dataFile, tmp);
        }
        readData();
    }
    ~DataProvider(){
        dataFile.close();
    }
    std::vector<double> getTime() const{
        return time;
    }
    std::vector<double> getBarometerData() const{
        return dataBarometer;
    }
    std::vector<double> getAccelerometerData() const{
        return dataAcc;
    }
};

void filterData(const DataProvider& dataProvider){
    constexpr double dt = 0.01;
    std::vector<double> time = dataProvider.getTime();
    std::vector<double> altitude = dataProvider.getBarometerData();
    std::vector<double> result;
    BarometerMock barometer;
    AltitudeProvider filter(&barometer, 0.3, dt);
    filter.init(altitude[0]);
    barometer.altitudeVariance = 0.3;
    for(unsigned int i = 0; i < time.size(); i++){
        barometer.altitude = altitude[i];
        filter.run();
        result.push_back(filter.getAltitude());
    }
    plot::named_plot("altitude", time, altitude);
    plot::named_plot("result", time, result);
}

TEST(AltitudeFiltration_test, MesuredDataSlot1){
    plot::figure_size(1200, 780);
    plot::title("Filtred altitude.");
    filterData(DataProvider("./dataFiles/moduleTests/altitudeMesurements/data1.txt"));
    filterData(DataProvider("./dataFiles/moduleTests/altitudeMesurements/flydata1.txt"));
    plot::legend();
    plot::save(TestOutputName);
    plot::figure_close();
}

TEST(AltitudeFiltration_test, MesuredDataSlot2){
    plot::figure_size(1200, 780);
    plot::title("Filtred altitude.");
    filterData(DataProvider("./dataFiles/moduleTests/altitudeMesurements/data2.txt"));
    filterData(DataProvider("./dataFiles/moduleTests/altitudeMesurements/flydata2.txt"));
    plot::legend();
    plot::save(TestOutputName);
    plot::figure_close();
}

TEST(AltitudeFiltration_test, MesuredDataSlot3){
    plot::figure_size(1200, 780);
    plot::title("Filtred altitude.");
    filterData(DataProvider("./dataFiles/moduleTests/altitudeMesurements/data3.txt"));
    plot::legend();
    plot::save(TestOutputName);
    plot::figure_close();
}

TEST(AltitudeFiltration_test, MesuredDataSlot4){
    plot::figure_size(1200, 780);
    plot::title("Filtred altitude.");
    filterData(DataProvider("./dataFiles/moduleTests/altitudeMesurements/data4.txt"));
    plot::legend();
    plot::save(TestOutputName);
    plot::figure_close();
}

TEST(AltitudeFiltration_test, MesuredDataSlot5){
    plot::figure_size(1200, 780);
    plot::title("Filtred altitude.");
    filterData(DataProvider("./dataFiles/moduleTests/altitudeMesurements/data5.txt"));
    filterData(DataProvider("./dataFiles/moduleTests/altitudeMesurements/flydata5.txt"));
    plot::legend();
    plot::save(TestOutputName);
    plot::figure_close();
}
}