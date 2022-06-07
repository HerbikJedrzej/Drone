#include "MeasurementManager.hh"
#include <OwnExceptions.hh>

namespace {
    class MemoryNullObject : public Drivers::MemoryIfc {
        MemoryNullObject():MemoryIfc([](uint32_t)->void{}){}
        static MemoryNullObject singleInstance;
        bool throwSmallMemoryException(){
            THROW_invalid_argument("MeasurementManager: Memory driver do not provide enough memory space.");
            return false;
        }
    public:
        bool write(uint16_t , uint8_t* , uint16_t) override {return throwSmallMemoryException();}
        bool read(uint16_t , uint8_t* , uint16_t) override {return throwSmallMemoryException();}
        void writeDMA(uint16_t , uint8_t* , uint16_t , bool*) override {throwSmallMemoryException();}
        void writeDMAwithoutDataAlocate(uint16_t , uint8_t* , uint16_t , bool*) override {throwSmallMemoryException();}
        bool readDMA(uint16_t , uint8_t* , uint16_t , bool*) override {return throwSmallMemoryException();}
        bool init(const RegPair*, uint16_t) override {return false;}
        void lockMemory() override {throwSmallMemoryException();}
        void unlockMemory() override {throwSmallMemoryException();}
        MemoryIfc::SizeModel getSizeModel() override {return MemoryIfc::BytePagesNotDefined;}
        static MemoryIfc* provideDriver(MemoryIfc* const memory){
            if(memory == nullptr)
                THROW_invalid_argument("MeasurementManager: Memory driver cannot be null.");
            if(memory->getSizeModel() == MemoryIfc::BytePages256)
                return memory;
            return &singleInstance;
        }
    };
    MemoryNullObject MemoryNullObject::singleInstance;
}

MeasurementManager::MeasurementManager(Drivers::MemoryIfc* const memory, MemorySlot* const memorySlots, const uint8_t& slotsNumber)
    : memorySlots(memorySlots)
    , slotsNumber(slotsNumber)
    , memory(memory)
    , measurementCounter(0)
    , memoryCellAddres(0)
    , writeFinish(true)
    {
        converter.measurements[0] = 0;
        converter.measurements[1] = 0;
        for(uint8_t i = 0; i < slotsNumber; i++)
            if(!memorySlots[i].validate())
                THROW_invalid_argument("MeasurementManager: Memory slot doesn't pass validation");
    }

void MeasurementManager::setMeasureValue1(const double& val){
    converter.measurements[0] = val;
}

bool MeasurementManager::init(){
    memory = MemoryNullObject::provideDriver(memory);
    return true;
}

void MeasurementManager::setMeasureValue2(const double& val){
    converter.measurements[1] = val;
}

void MeasurementManager::startMeasurement(const uint8_t& slot){
    if(slot >= slotsNumber)
        THROW_out_of_range("MeasurementManager: Cannot measure unknown slot");
    // memory->unlockMemory();
    measurementCounter = memorySlots[slot].getSize();
    memoryCellAddres = memorySlots[slot].getEnd();
}

void MeasurementManager::increment(){
    if(measurementCounter > 0){
        if(!writeFinish)
            THROW_out_of_range("MeasurementManager: Previous memory writting not finished.");
        memory->writeDMAwithoutDataAlocate(memoryCellAddres - (0x10 * measurementCounter--) + 1, converter.rawData, 16, &writeFinish);
    } else {
        // memory->lockMemory();
        converter.measurements[0] = 0;
        converter.measurements[1] = 0;
    }
}
