#pragma once

#include <cstddef>
#include <cstdint>
#include <stdint.h>
#include <cstdlib>

class ADCcontaine{
public:
	ADCcontaine(const uint16_t* list, uint8_t size);
    uint16_t operator[](unsigned int) const;
    uint8_t size() const;
protected:
    const unsigned int sizeOfList;
    const uint16_t* listOfAdcRegisters;
private:
};
