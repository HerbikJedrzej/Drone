#include "esdStructure.hh"
#include <OwnExceptions.hh>

EsdDriver::EsdDriver(uint16_t* list, uint16_t size, const uint16_t* _min, const uint16_t* _max):
    EngineIfc(size, list),
    min(new uint16_t[size]),
    max(new uint16_t[size]){
        for(uint8_t i = 0; i < sizeOfList; i++){
            listOfEngineRegisters[i] = 0;
            min[i] = _min[i];
            max[i] = _max[i];
        }
    }

EsdDriver::~EsdDriver(){
    delete [] min;
    delete [] max;
}

void EsdDriver::init(){
    for(uint8_t i = 0; i < sizeOfList; i++)
        listOfEngineRegisters[i] = min[i];
}

void EsdDriver::set(uint8_t engineId, const double powerPercent){
    if(engineId >= sizeOfList)
        THROW_out_of_range("Trying to get engine out of range.");
    if(powerPercent < 0.0){
        listOfEngineRegisters[engineId] = min[engineId];
        return;
    }
    if (powerPercent > 100.0)
    {
        listOfEngineRegisters[engineId] = max[engineId];
        return;
    }
    listOfEngineRegisters[engineId] = round(powerPercent / 100.0 * double(max[engineId] - min[engineId])) + min[engineId];
}
