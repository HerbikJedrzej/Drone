#include "ADC.hh"

ADCcontaine::ADCcontaine(const uint16_t* list, uint8_t size):
    sizeOfList(size),
    listOfAdcRegisters(list)
    {}

uint16_t ADCcontaine::operator[](unsigned int i) const{
    return listOfAdcRegisters[i];
}

uint8_t ADCcontaine::size() const{
    return sizeOfList;
}
