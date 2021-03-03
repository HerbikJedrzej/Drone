#include "MiniDronEngine.hh"
#include <OwnExceptions.hh>

MiniDronEngine::MiniDronEngine(uint16_t* list, uint8_t size):
    EngineIfc(size, list){}

void MiniDronEngine::init(){
    for(uint8_t i = 0; i < sizeOfList; i++)
        listOfEngineRegisters[i] = 0;
}

void MiniDronEngine::set(uint8_t engineId, const double powerPercent){
    if(engineId >= sizeOfList)
        THROW_out_of_range("Trying to get engine out of range.");
    if(powerPercent < 0.0){
        listOfEngineRegisters[engineId] = 0;
        return;
    }
    if (powerPercent > 100.0)
    {
        listOfEngineRegisters[engineId] = 10000;
        return;
    }
    listOfEngineRegisters[engineId] = round(powerPercent * 100.0);
}
