#include "TestI2C.hh"
#include "TestDelay.hh"

namespace TestMocks{
    std::map<uint8_t, std::map<uint16_t, uint8_t>> TestI2C::devicesMap;

    TestI2C::TestI2C():
    i2c(
        [](uint8_t DevAddress, uint16_t MemAddress, uint16_t, uint8_t *pData, uint16_t Size)->Drivers::BusStatus{
            std::map<uint16_t, uint8_t>& map = devicesMap.at(DevAddress);
            for(uint16_t i = 0; i < Size; i++)
                map[i + MemAddress] = pData[i];
            return Drivers::BusStatus::OK;
        },
        [](uint8_t DevAddress, uint16_t MemAddress, uint16_t, uint8_t *pData, uint16_t Size)->Drivers::BusStatus{
            std::map<uint16_t, uint8_t>& map = devicesMap.at(DevAddress);
            for(uint16_t i = 0; i < Size; i++)
                pData[i] = map.at(i + MemAddress);
            return Drivers::BusStatus::OK;
        },
        [](uint8_t DevAddress, uint16_t MemAddress, uint16_t, uint8_t *pData, uint16_t Size, uint32_t)->Drivers::BusStatus{
            std::map<uint16_t, uint8_t>& map = devicesMap.at(DevAddress);
            for(uint16_t i = 0; i < Size; i++)
                map[i + MemAddress] = pData[i];
            return Drivers::BusStatus::OK;
        },
        [](uint8_t DevAddress, uint16_t MemAddress, uint16_t, uint8_t *pData, uint16_t Size, uint32_t)->Drivers::BusStatus{
            std::map<uint16_t, uint8_t>& map = devicesMap.at(DevAddress);
            for(uint16_t i = 0; i < Size; i++)
                pData[i] = map.at(i + MemAddress);
            return Drivers::BusStatus::OK;
        },
        []()->bool{return false;},
        TestMocks::delay
    ){}
}