#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <MeasurementManager.hh>

using Drivers::MemoryIfc;

#define EXPECT_THROW_WITH_MSG(MSG, FUNCTION) try{FUNCTION;FAIL() << "Expected exception haven't happened";} \
    catch(const std::invalid_argument& e){EXPECT_STREQ(MSG,e.what());} \
    catch(const std::out_of_range& e){EXPECT_STREQ(MSG,e.what());}

namespace {
class MemoryMock : public MemoryIfc {
public:
    MemoryMock():MemoryIfc([](uint32_t)->void{}){}
    MOCK_METHOD(bool, write, (uint16_t, uint8_t*, uint16_t), (override));
	MOCK_METHOD(bool, read, (uint16_t, uint8_t*, uint16_t), (override));
	MOCK_METHOD(void, writeDMA, (uint16_t, uint8_t*, uint16_t, bool*), (override));
	MOCK_METHOD(void, writeDMAwithoutDataAlocate, (uint16_t, uint8_t*, uint16_t, bool*), (override));
	MOCK_METHOD(bool, readDMA, (uint16_t, uint8_t*, uint16_t, bool*), (override));
	MOCK_METHOD(bool, init, (const RegPair*, uint16_t), (override));
	MOCK_METHOD(void, lockMemory, (), (override));
	MOCK_METHOD(void, unlockMemory, (), (override));
	MOCK_METHOD(MemoryIfc::SizeModel, getSizeModel, (), (override));
};
class MeasurementManagerTestHelper : public MeasurementManager {
public:
	MeasurementManagerTestHelper(MemoryIfc* const memory, MemorySlot* const memorySlots, const uint8_t& slotsNumber):
        MeasurementManager(memory, memorySlots, slotsNumber){}
    uint8_t* getRawData(){
        return converter.rawData;
    }
    double* getMeasurements(){
        return converter.measurements;
    }
};
}

using namespace ::testing;

TEST(MeasurementManager, initWithNullDriver){
    EXPECT_THROW_WITH_MSG(
        "MeasurementManager: Memory driver cannot be null.",
        MeasurementManager mgr(nullptr, nullptr, 0);
        mgr.init();
    );
}

TEST(MeasurementManager, initWithWrongDriver){
    MemoryMock memory;
    EXPECT_CALL(memory, getSizeModel())
    .Times(1).WillOnce(Return(MemoryIfc::BytePagesNotDefined));
    EXPECT_CALL(memory, lockMemory()).Times(0);
    EXPECT_CALL(memory, unlockMemory()).Times(0);
    MeasurementManager::MemorySlot slot(0,0xf);
    MeasurementManager mgr(&memory, &slot, 1);
    mgr.init();
    EXPECT_NO_THROW(mgr.increment());
    mgr.setMeasureValue1(0);
    mgr.setMeasureValue2(0);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_NO_THROW(mgr.startMeasurement(0));
    EXPECT_THROW_WITH_MSG("MeasurementManager: Memory driver do not provide enough memory space.",
        mgr.increment()
    );
}

TEST(MeasurementManager, initWithNoSlots){
    MemoryMock memory;
    EXPECT_CALL(memory, getSizeModel())
    .Times(1).WillOnce(Return(MemoryIfc::BytePages256));
    EXPECT_CALL(memory, lockMemory()).Times(0);
    EXPECT_CALL(memory, unlockMemory()).Times(0);
    MeasurementManager mgr(&memory, nullptr, 0);
    mgr.init();
    EXPECT_NO_THROW(mgr.increment());
    mgr.setMeasureValue1(0);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_THROW_WITH_MSG("MeasurementManager: Cannot measure unknown slot",
        mgr.startMeasurement(0)
    );
}

TEST(MeasurementManager, slotValidate){
    EXPECT_FALSE(MeasurementManager::MemorySlot(4,0).validate());
    EXPECT_FALSE(MeasurementManager::MemorySlot(0,0).validate());
    EXPECT_FALSE(MeasurementManager::MemorySlot(0,0xe).validate());
    EXPECT_FALSE(MeasurementManager::MemorySlot(0,0x1e).validate());
    EXPECT_FALSE(MeasurementManager::MemorySlot(0x14,0x5f).validate());
    EXPECT_TRUE(MeasurementManager::MemorySlot(0,0xf).validate());
    EXPECT_TRUE(MeasurementManager::MemorySlot(0x10,0x5f).validate());
    EXPECT_TRUE(MeasurementManager::MemorySlot(0x1010,0xffff).validate());
}

TEST(MeasurementManager, slotSize){
    EXPECT_EQ(1, MeasurementManager::MemorySlot(0,0xf).getSize());
    EXPECT_EQ(5, MeasurementManager::MemorySlot(0x10,0x5f).getSize());
    EXPECT_EQ(3839, MeasurementManager::MemorySlot(0x1010,0xffff).getSize());
}

TEST(MeasurementManager, initWithSlots){
    MeasurementManager::MemorySlot wrongSlot(0x14,0x5f);
    MeasurementManager::MemorySlot correctSlot(0,0xf);
    MeasurementManager::MemorySlot combination1[2] = {correctSlot, wrongSlot};
    MeasurementManager::MemorySlot combination2[2] = {wrongSlot, correctSlot};
    MeasurementManager::MemorySlot combination3[5] = {correctSlot, correctSlot, correctSlot, correctSlot, wrongSlot};

    ASSERT_FALSE(wrongSlot.validate());
    ASSERT_TRUE(correctSlot.validate());

    MemoryMock memory;
    EXPECT_CALL(memory, getSizeModel()).Times(2);
    EXPECT_CALL(memory, lockMemory()).Times(0);
    EXPECT_CALL(memory, unlockMemory()).Times(0);
    EXPECT_THROW_WITH_MSG("MeasurementManager: Memory slot doesn't pass validation",
        MeasurementManager mgr(&memory, combination2, 1);
    );
    EXPECT_NO_THROW(
        MeasurementManager mgr(&memory, combination1, 1);
        mgr.init();
    );
    EXPECT_THROW_WITH_MSG("MeasurementManager: Memory slot doesn't pass validation",
        MeasurementManager mgr(&memory, combination2, 2);
    );
    EXPECT_THROW_WITH_MSG("MeasurementManager: Memory slot doesn't pass validation",
        MeasurementManager mgr(&memory, combination1, 2);
    );
    EXPECT_NO_THROW(
        MeasurementManager mgr(&memory, combination3, 4);
        mgr.init();
    );
    EXPECT_THROW_WITH_MSG("MeasurementManager: Memory slot doesn't pass validation",
        MeasurementManager mgr(&memory, combination3, 5);
    );
}

TEST(MeasurementManager, writeToSlotWithSingleCell){
    MemoryMock memory;
    EXPECT_CALL(memory, getSizeModel())
    .Times(1).WillOnce(Return(MemoryIfc::BytePages256));
    MeasurementManager::MemorySlot slot(0,0xf);
    ASSERT_EQ(1, slot.getSize());
    MeasurementManagerTestHelper mgr(&memory, &slot, 1);
    mgr.init();
    EXPECT_CALL(memory, writeDMAwithoutDataAlocate(0,mgr.getRawData(), 16,_)).Times(2);
    EXPECT_CALL(memory, lockMemory()).Times(0);
    EXPECT_CALL(memory, unlockMemory()).Times(0);
    mgr.setMeasureValue1(0.1234);
    mgr.setMeasureValue2(-12.5);
    // clear before send
    EXPECT_DOUBLE_EQ(0.1234, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(-12.5, mgr.getMeasurements()[1]);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_DOUBLE_EQ(0.0, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(0.0, mgr.getMeasurements()[1]);
    EXPECT_NO_THROW(mgr.startMeasurement(0));
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_DOUBLE_EQ(0.0, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(0.0, mgr.getMeasurements()[1]);
    // send proper values
    mgr.setMeasureValue1(0.1234);
    mgr.setMeasureValue2(-12.5);
    EXPECT_DOUBLE_EQ(0.1234, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(-12.5, mgr.getMeasurements()[1]);
    EXPECT_NO_THROW(mgr.startMeasurement(0));
    EXPECT_DOUBLE_EQ(0.1234, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(-12.5, mgr.getMeasurements()[1]);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_DOUBLE_EQ(0.1234, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(-12.5, mgr.getMeasurements()[1]);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_DOUBLE_EQ(0.0, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(0.0, mgr.getMeasurements()[1]);
}

TEST(MeasurementManager, writeToSlotsWithManyCell){
    MemoryMock memory;
    EXPECT_CALL(memory, getSizeModel())
    .Times(1).WillOnce(Return(MemoryIfc::BytePages256));
    EXPECT_CALL(memory, lockMemory()).Times(0);
    EXPECT_CALL(memory, unlockMemory()).Times(0);
    MeasurementManager::MemorySlot slots[] = {
        MeasurementManager::MemorySlot(0x000,0x02f),
        MeasurementManager::MemorySlot(0x100,0x14f),
    };
    ASSERT_EQ(3, slots[0].getSize());
    ASSERT_EQ(5, slots[1].getSize());
    MeasurementManagerTestHelper mgr(&memory, slots, 2);
    mgr.init();
    EXPECT_CALL(memory, writeDMAwithoutDataAlocate(0x00,mgr.getRawData(), 16,_)).Times(1);
    EXPECT_CALL(memory, writeDMAwithoutDataAlocate(0x10,mgr.getRawData(), 16,_)).Times(1);
    EXPECT_CALL(memory, writeDMAwithoutDataAlocate(0x20,mgr.getRawData(), 16,_)).Times(1);
    EXPECT_CALL(memory, writeDMAwithoutDataAlocate(0x100,mgr.getRawData(), 16,_)).Times(1);
    EXPECT_CALL(memory, writeDMAwithoutDataAlocate(0x110,mgr.getRawData(), 16,_)).Times(1);
    EXPECT_CALL(memory, writeDMAwithoutDataAlocate(0x120,mgr.getRawData(), 16,_)).Times(1);
    EXPECT_CALL(memory, writeDMAwithoutDataAlocate(0x130,mgr.getRawData(), 16,_)).Times(1);
    EXPECT_CALL(memory, writeDMAwithoutDataAlocate(0x140,mgr.getRawData(), 16,_)).Times(1);
    EXPECT_NO_THROW(mgr.startMeasurement(0));
    mgr.setMeasureValue1(0.1234);
    mgr.setMeasureValue2(-12.5);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_DOUBLE_EQ(0.1234, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(-12.5, mgr.getMeasurements()[1]);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_DOUBLE_EQ(0.1234, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(-12.5, mgr.getMeasurements()[1]);
    mgr.setMeasureValue1(0.12);
    mgr.setMeasureValue2(-1.5);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_DOUBLE_EQ(0.12, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(-1.5, mgr.getMeasurements()[1]);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_DOUBLE_EQ(0.0, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(0.0, mgr.getMeasurements()[1]);

    EXPECT_NO_THROW(mgr.startMeasurement(1));
    mgr.setMeasureValue1(0.1234);
    mgr.setMeasureValue2(-12.5);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_DOUBLE_EQ(0.1234, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(-12.5, mgr.getMeasurements()[1]);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_DOUBLE_EQ(0.1234, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(-12.5, mgr.getMeasurements()[1]);
    mgr.setMeasureValue1(0.12);
    mgr.setMeasureValue2(-1.5);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_DOUBLE_EQ(0.12, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(-1.5, mgr.getMeasurements()[1]);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_DOUBLE_EQ(0.12, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(-1.5, mgr.getMeasurements()[1]);
    mgr.setMeasureValue1(12);
    mgr.setMeasureValue2(1.5);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_DOUBLE_EQ(12, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(1.5, mgr.getMeasurements()[1]);
    EXPECT_NO_THROW(mgr.increment());
    EXPECT_DOUBLE_EQ(0.0, mgr.getMeasurements()[0]);
    EXPECT_DOUBLE_EQ(0.0, mgr.getMeasurements()[1]);
}
