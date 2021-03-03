#pragma once
#include <I2C.hh>
#include <map>

namespace TestMocks{
    class TestI2C{
    public:
        TestI2C();
        Drivers::I2C i2c;
        static std::map<uint8_t, std::map<uint16_t, uint8_t>> devicesMap;
    };
}