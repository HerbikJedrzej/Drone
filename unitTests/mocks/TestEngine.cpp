#include "TestEngine.hh"
#include "TestDelay.hh"

namespace TestMocks{

    TestEngine::TestEngine(const uint8_t size):EngineIfc(size, nullptr){
        for(uint8_t i = 0; i < sizeOfList; i++)
            map[i] = 0;
    }

    void TestEngine::init(){
    }

    void TestEngine::set(uint8_t engineId, const double powerPercent){
        map.at(engineId) = powerPercent;
    }

}