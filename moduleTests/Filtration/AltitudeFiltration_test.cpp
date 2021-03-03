#include <gtest/gtest.h>
#include <iostream>
#include <string>
#include <fstream>
#include "../plot.hh"
#include <AltitudeProvider.hh>
#include <math.h>

namespace AltitudeFiltration{

class Suming{
    const double dt;
    double sum;
    std::vector<double> data;
public:
    Suming(const double& _dt):
        dt(_dt),
        sum(0){}
    void addMeasure(const double& val){
        sum += val * dt;
        data.push_back(sum);
    }
    const std::vector<double> getSum(){
        return data;
    }
};

std::vector<double> addPrecissionError(const std::vector<double>& v, const double& precission){
    std::vector<double> toReturn;
    auto realNumericAround = [](const double& val)->int{
        double difference = val - int(val);
        difference *= difference;
        if(difference < 0.25)
            return int(val);
        else
            return int(val) + ((val < 0)? -1 : 1);
    };
    for(const auto& elem : v)
        toReturn.push_back(realNumericAround(elem / precission) * precission);
    return std::move(toReturn);
}

TEST(AltitudeFiltration_test, mainTest){
    constexpr double dt = 0.01;
    Suming h(dt);
    Suming hn(dt);
    Suming v(dt);
    Suming vn(dt);
    std::vector<double> time, a, an;
    constexpr unsigned int noiseFactor = 12;
    constexpr unsigned int noiseFactorV = 2.5;
    for(unsigned int i = 0; i < 500; i++){
        double t = double(i) * dt;
        time.push_back(t);
        a.push_back((t - 3) * (t - 3) + 2*t - 8);
        an.push_back(a.back() + double(rand() % (noiseFactor * 1000)) / 1000 - noiseFactor * 0.5 + 1.2);
        v.addMeasure(a.back());
        vn.addMeasure(an.back());
    }
    for(const auto& elem : v.getSum())
        h.addMeasure(elem);
    for(const auto& elem : vn.getSum())
        hn.addMeasure(elem + double(rand() % (noiseFactorV * 1000)) / 1000 - noiseFactorV * 0.5);

    auto measureFromAkcelerometer = hn.getSum();
    auto barometerHmeasure = addPrecissionError(h.getSum(), 1.25);

    std::vector<double> result, velocity = vn.getSum();
    AltitudeProvider filter(0.03, dt);
    for(unsigned int i = 0; i < barometerHmeasure.size(); i++){
        filter.run(an[i], 0.012, barometerHmeasure[i]);
        result.push_back(filter.getAltitude());
    }

    plot::figure_size(1200, 780);
    plot::title("Filtred altitude.");
    plot::named_plot("a", time, a);
    plot::named_plot("an", time, an);
    plot::named_plot("v", time, v.getSum());
    plot::named_plot("result", time, result);
    plot::named_plot("vn", time, vn.getSum());
    plot::named_plot("h", time, measureFromAkcelerometer);
    plot::named_plot("hn", time, hn.getSum());
    plot::named_plot("barometer", time, barometerHmeasure);
    plot::legend();
    plot::save("./moduleTestsOutputData/Filtration/Filtred_altitude.png");
    plot::figure_close();

}

}
