#pragma once

#include <vector>

namespace TestHelpers{

class SumUpDataContainer{
    const double dt;
    double sum;
    std::vector<double> data;
public:
    SumUpDataContainer(const double& _dt):
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

}
