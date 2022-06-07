#include <gtest/gtest.h>
#include <iostream>
#include "../plot.hh"
#include "../Models.hh"
#include "../../src/algoritms/PID.hh"
#include <memory>
#include <iostream>
#include <fstream>

#define TestName ::testing::UnitTest::GetInstance()->current_test_info()->name()
#define TestOutputName std::string("./moduleTestsOutputData/PID/") + TestName + ".png"

namespace{

class ModelAxisH{
private:
    std::unique_ptr<models::Model> model;
    double output;
public:
    ModelAxisH() = delete;
    ModelAxisH& operator=(const ModelAxisH&) = default;
    ModelAxisH(ModelAxisH&&) = default;
    ModelAxisH(std::unique_ptr<models::Model> model):
        model(std::move(model)),
        output(0){}
    ~ModelAxisH() = default;
    void run(const double& input){
        this->model->input(input);
        this->output = this->model->output();
    }
    double getOutput(){
        return this->output;
    }
};


class DataProvider{
    class uncorrectFile{};
    std::ifstream dataFile;
    std::vector<double> dataSet;
    std::vector<double> dataMeasured;
    std::vector<double> time;
    void readData(){
        double localData = 0.0;
        double localTime = 0.0;
        while(true){
            dataFile >> localData;
            if(dataFile.eof())
                return;
            dataSet.push_back(localData);
            dataFile >> localData;
            dataMeasured.push_back(localData);
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
    std::vector<double> getMeasuredData() const{
        return dataMeasured;
    }
    std::vector<double> getSetData() const{
        return dataSet;
    }
};

ModelAxisH getModel(const double& offset, const double& p0 = 0.0){
    constexpr double T1 = 0.25;
    constexpr double T2 = 0.18;
    constexpr double mathAirObj = 0.93;
    return ModelAxisH(std::make_unique<models::RealIntegral>(
        new models::Offset(
            new models::Inercion(
                new models::Inercion(
                    10.0/30.0, T1, p0
                ),
                mathAirObj, T2, p0
            ), 
            offset
        ),
        1.0, 0.8
    ));
}

void filterData(const DataProvider& dataProvider, const double& setOffset){
    std::vector<double> time = dataProvider.getTime();
    std::vector<double> altitude = dataProvider.getMeasuredData();
    std::vector<double> set = dataProvider.getSetData();
    std::vector<double> altitudeZeroLevel, setTmp, result;

    ModelAxisH modelSet(std::make_unique<models::Proporcional>(0.1));
    ModelAxisH model = getModel(setOffset, 10.0/30.0 * set[0]);
    for(const double& elem : set){
        model.run(elem);
        modelSet.run(elem);
        result.push_back(model.getOutput());
        setTmp.push_back(modelSet.getOutput());
    }
    const double offset = altitude[0];
    for(const double& elem : altitude)
        altitudeZeroLevel.push_back(elem - offset);
    plot::named_plot("altitude", time, altitudeZeroLevel);
    plot::named_plot("set", time, setTmp);
    plot::named_plot("result", time, result);
}

TEST(PID_measurements_axis_H, modelsAndDataSlot2){
    plot::figure_size(1200, 780);
    plot::title("Filtred altitude.");
    filterData(DataProvider("./dataFiles/moduleTests/altitudeMesurements/measureH2.txt"), 9.9);
    plot::legend();
    plot::save(TestOutputName);
    plot::figure_close();
}

TEST(PID_measurements_axis_H, modelsAndDataSlot3){
    plot::figure_size(1200, 780);
    plot::title("Filtred altitude.");
    filterData(DataProvider("./dataFiles/moduleTests/altitudeMesurements/measureH3.txt"), 10.6);
    plot::legend();
    plot::save(TestOutputName);
    plot::figure_close();
}

TEST(PID_measurements_axis_H, modelsAndDataSlot5){
    plot::figure_size(1200, 780);
    plot::title("Filtred altitude.");
    filterData(DataProvider("./dataFiles/moduleTests/altitudeMesurements/measureH5.txt"), 10.6);
    plot::legend();
    plot::save(TestOutputName);
    plot::figure_close();
}

void responsData(const double& setOffset){
    auto result = models::simulate(new models::Step(1.0, 1.0), 0.0, 15.0);
    std::vector<double>& time = result.first;
    std::vector<double>& stepSignal = result.second;
    std::vector<double> response;
    PID pid;
    ModelAxisH model = getModel(setOffset);

    pid.reset(0.0);
    pid.P = 4.0;
    pid.I = 0.009;
    pid.D = 100.0;
    pid.setY(0.0);
    for(const double& elem : stepSignal){
        pid.setR(elem);
        model.run(pid.calculate());
        pid.setY(model.getOutput());
        response.push_back(model.getOutput());
    }
    plot::named_plot("set", time, stepSignal);
    plot::named_plot("response", time, response);
}

TEST(PID_measurements_axis_H, responsPID){
    plot::figure_size(1200, 780);
    plot::title("Filtred altitude.");
    responsData(10.6);
    plot::legend();
    plot::save(TestOutputName);
    plot::figure_close();
}
}
