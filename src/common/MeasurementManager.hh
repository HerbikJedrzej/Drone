#pragma once

#include <MemoryIfc.hh>

class MeasurementManager {
public:
    class MemorySlot{
        const uint16_t begin;
        const uint16_t end;
        const uint16_t size;
    public:
        MemorySlot(const uint16_t& begin, const uint16_t& end)
            : begin(begin)
            , end(end)
            , size((end - begin + 1) / 0x10)
            {}
        const uint16_t& getBegin(){return begin;}
        const uint16_t& getEnd(){return end;}
        const uint16_t& getSize(){return size;}
        bool validate() const{
            if(end <= begin)
                return false;
            return (((end - begin + 1) % 0x10) == 0);
        }
    };
	MeasurementManager(Drivers::MemoryIfc* const memory, MemorySlot* const memorySlots, const uint8_t& slotsNumber);
    void startMeasurement(const uint8_t& slot);
    void setMeasureValue1(const double& val);
    void setMeasureValue2(const double& val);
    void increment();
    bool init();
    MemorySlot* const memorySlots;
    const uint8_t slotsNumber;
private:
    Drivers::MemoryIfc* memory;
    uint16_t measurementCounter;
    uint16_t memoryCellAddres;
    bool writeFinish;
protected:
    union {
        double measurements[2];
        uint8_t rawData[16];
    } converter;
};
