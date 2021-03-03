#include <cstdint>

namespace TestMocks{
    uint32_t sleepedTime = {0};
    void delay(uint32_t t){
        sleepedTime += t;
    }
}