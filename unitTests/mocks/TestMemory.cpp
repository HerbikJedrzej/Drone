#include "TestMemory.hh"
#include "TestDelay.hh"
#include "TestGPIO.hh"

namespace TestMocks{
    TestMemory::TestMemory():
        i2c(),
        memory(&i2c.i2c, &TestMocks::gpio, 0xA0, OutputList::MemoryWriteProtect, TestMocks::delay){}
}