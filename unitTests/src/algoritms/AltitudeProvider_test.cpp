#include <gtest/gtest.h>
#include <vector>
#include <AltitudeProvider.hh>
#include <SumUpDataContainer.hh>
#include <TestBarometer.hh>
#include <VarianceProvider.hh>

namespace {

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
    return toReturn;
}

constexpr double barometerPrecision = 0.3;
constexpr double dt = 0.01;

}

TEST(AltitudeProvider_test, init){
    TestMocks::BarometerMock barometer;
    AltitudeProvider altitudeProvider(&barometer, barometerPrecision, dt);
    ASSERT_NEAR(0.0, altitudeProvider.getAltitude(), 0.0);
    altitudeProvider.init(100.0);
    ASSERT_NEAR(100.0, altitudeProvider.getAltitude(), 0.0);
    barometer.altitude = 0.0;
    barometer.altitudeVariance = 10.0;
    altitudeProvider.run();
    ASSERT_GT(altitudeProvider.getAltitude(), 0.0);
    ASSERT_LT(altitudeProvider.getAltitude(), 100.0);
}

TEST(AltitudeProvider_test, compare){
    TestHelpers::SumUpDataContainer h(dt);
    TestHelpers::SumUpDataContainer hn(dt);
    TestHelpers::SumUpDataContainer v(dt);
    std::vector<double> time, a, an;
    constexpr unsigned int noiseFactor = 1;
    constexpr unsigned int noiseFactorV = 250;
    for(unsigned int i = 0; i < 680; i++){
        double t = double(i) * dt;
        time.push_back(t);
        a.push_back((t - 3) * (t - 3) + 2*t - 8);
        an.push_back(a.back() + double(rand() % (noiseFactor * 1000)) / 1000 - noiseFactor * 0.5);
        v.addMeasure(a.back());
    }
    for(const auto& elem : v.getSum())
        h.addMeasure(elem);
    for(const auto& elem : v.getSum())
        hn.addMeasure(elem + double(rand() % (noiseFactorV * 1000)) / 1000 - noiseFactorV * 0.5);

    auto measureFromAkcelerometer = hn.getSum();
    auto barometerHmeasure = addPrecissionError(h.getSum(), 1.25);

    TestMocks::BarometerMock barometer;
    AltitudeProvider filter(&barometer, barometerPrecision, dt);
    for(unsigned int i = 0; i < barometerHmeasure.size(); i++){
        barometer.altitude = barometerHmeasure[i];
        filter.run();
        ASSERT_NEAR(barometerHmeasure[i], filter.getAltitude(), 2.5) << "Sample " << i << " out of " << barometerHmeasure.size();
    }
}
