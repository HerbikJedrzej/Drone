#pragma once

namespace TestHelpers{

template<int size>
class VarianceProvider {
public:
    VarianceProvider(){
        for(int i = 0; i < size; i++)
            dataBuffor[i] = 0;
    }
    double newData(const double& val){
        dataBuffor[bufforCounter++] = val;
        if(bufforCounter == size)
            bufforCounter = 0;
        calculateVariance();
        return val;
    }
    double getVariance(){
        return variance;
    }
private:
    void calculateVariance(){
        double mean = 0;
        for(int i = 0; i < size; i++)
            mean += dataBuffor[i];
        mean /= double(size);
        variance = 0;
        for(int i = 0; i < size; i++){
            const double diff = mean - dataBuffor[i];
            variance += diff*diff;
        }
        variance /= double(size);
    }
    double dataBuffor[size];
    unsigned int bufforCounter = 0;
    double variance = 0;
};

}
