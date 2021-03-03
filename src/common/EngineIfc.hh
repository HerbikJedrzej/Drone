#pragma once

#include <cstddef>
#include <cstdint>
#include <stdint.h>
#include <cstdlib>

class EngineIfc{
public:
    EngineIfc(const uint8_t size, uint16_t* regList):
        sizeOfList(size),
        listOfEngineRegisters{regList}
        {}
    virtual void init() = 0;
    virtual void set(uint8_t engineId, const double powerPercent) = 0;
    uint8_t size(){return sizeOfList;}
protected:
    const uint8_t sizeOfList;
    uint16_t* listOfEngineRegisters;
    uint16_t round(const double& val){
        uint16_t toReturn = uint16_t(val);
        if((val - double(toReturn)) >= 0.5)
            toReturn++;
        return toReturn;
    }
};
