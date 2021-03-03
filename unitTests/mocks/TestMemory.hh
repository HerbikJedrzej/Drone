#pragma once
#include <AT24C.hh>
#include "TestI2C.hh"

namespace TestMocks{
    class TestMemory{
    public:
        TestMocks::TestI2C i2c;
        Drivers::Memory memory;
        TestMemory();
    };
}