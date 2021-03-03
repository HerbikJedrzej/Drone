#include <gtest/gtest.h>
#include <BatteryObserver.hh>
#include <TestMemory.hh>

TEST(BatteryObserver_Test, init){
	uint16_t measure[3];
    TestMocks::TestMemory memory;
    double rezistors[3] = {47000, 4700, 3029};
    auto& memoryRegs = memory.i2c.devicesMap[0xA0];
    memoryRegs[memoryMap::rezistor_1_U_HSB] = 0;
    memoryRegs[memoryMap::rezistor_1_U_MSB] = 0;
    memoryRegs[memoryMap::rezistor_1_U_LSB] = 0;
    memoryRegs[memoryMap::rezistor_2_U_HSB] = 0;
    memoryRegs[memoryMap::rezistor_2_U_MSB] = 0;
    memoryRegs[memoryMap::rezistor_2_U_LSB] = 0;
	BatteryObserver battery1(&memory.memory, measure, rezistors, 2, 0xfff, 1.5, 4.0);
	BatteryObserver battery2(&memory.memory, measure, rezistors, 3, 0xfff, 1.5, 4.0);
	EXPECT_NO_THROW(EXPECT_TRUE(battery1.init()));
	EXPECT_ANY_THROW(EXPECT_TRUE(battery2.init()));
}

TEST(BatteryObserver_Test, batteryPercentVal){
	uint16_t measure[2];
    TestMocks::TestMemory memory;
    uint32_t r1 = 30000;
    uint32_t r2 = 20000;
    double minV = 1.5;
    double maxV = 4.5;
    double rezistors[2] = {47000, 4700};
    auto& memoryRegs = memory.i2c.devicesMap[0xA0];
    memoryRegs[memoryMap::rezistor_1_U_HSB] = (r1 >> 16) & 0xff;
    memoryRegs[memoryMap::rezistor_1_U_MSB] = (r1 >> 8) & 0xff;
    memoryRegs[memoryMap::rezistor_1_U_LSB] = r1 & 0xff;
    memoryRegs[memoryMap::rezistor_2_U_HSB] = (r2 >> 16) & 0xff;
    memoryRegs[memoryMap::rezistor_2_U_MSB] = (r2 >> 8) & 0xff;
    memoryRegs[memoryMap::rezistor_2_U_LSB] = r2 & 0xff;
	BatteryObserver battery(&memory.memory, measure, rezistors, 2, 0xfff, minV, maxV);
    EXPECT_TRUE(battery.init());
    measure[0] = 0xfff * 3 / 5;
    measure[1] = 0xfff * 2 / 5;
    double voltageCell1 = 3.3 * 3 / 5 * ((double(r1) + rezistors[0]) / rezistors[0]);
    double voltageCell2 = 3.3 * 2 / 5 * ((double(r2) + rezistors[1]) / rezistors[1]);
    uint8_t procentValue1 = uint8_t(100 * (voltageCell1 - minV) / (maxV - minV));
    uint8_t procentValue2 = uint8_t(100 * (voltageCell2 - minV - voltageCell1) / (maxV - minV));
    EXPECT_EQ(battery.batteryPercentVal(0), procentValue1);
    EXPECT_EQ(battery.batteryPercentVal(1), procentValue2);
    EXPECT_ANY_THROW(EXPECT_EQ(battery.batteryPercentVal(2), 0));
    EXPECT_EQ(battery.batteryPercentVal(), (procentValue1 < procentValue2)? procentValue1 : procentValue2);
}

TEST(BatteryObserver_Test, batteryVoltageVal){
	uint16_t measure[2];
    TestMocks::TestMemory memory;
    uint32_t r1 = 30000;
    uint32_t r2 = 20000;
    double minV = 1.5;
    double maxV = 4.5;
    double rezistors[2] = {47000, 4700};
    auto& memoryRegs = memory.i2c.devicesMap[0xA0];
    memoryRegs[memoryMap::rezistor_1_U_HSB] = (r1 >> 16) & 0xff;
    memoryRegs[memoryMap::rezistor_1_U_MSB] = (r1 >> 8) & 0xff;
    memoryRegs[memoryMap::rezistor_1_U_LSB] = r1 & 0xff;
    memoryRegs[memoryMap::rezistor_2_U_HSB] = (r2 >> 16) & 0xff;
    memoryRegs[memoryMap::rezistor_2_U_MSB] = (r2 >> 8) & 0xff;
    memoryRegs[memoryMap::rezistor_2_U_LSB] = r2 & 0xff;
	BatteryObserver battery(&memory.memory, measure, rezistors, 2, 0xfff, minV, maxV);
    EXPECT_TRUE(battery.init());
    measure[0] = 0xfff * 3 / 5;
    measure[1] = 0xfff * 2 / 5;
    double voltageCell1 = 3.3 * 3 / 5 * ((double(r1) + rezistors[0]) / rezistors[0]);
    double voltageCell2 = 3.3 * 2 / 5 * ((double(r2) + rezistors[1]) / rezistors[1]);
    EXPECT_DOUBLE_EQ(battery.batteryVoltageVal(0), voltageCell1);
    EXPECT_DOUBLE_EQ(battery.batteryVoltageVal(1), voltageCell2);
    EXPECT_ANY_THROW(EXPECT_DOUBLE_EQ(battery.batteryVoltageVal(2), 0.0));
    EXPECT_DOUBLE_EQ(battery.batteryVoltageVal(), voltageCell2);
}

TEST(BatteryObserver_Test, correctResistor){	uint16_t measure[3];
    TestMocks::TestMemory memory;
    const uint32_t r1 = 10;
    const uint32_t r2 = 2000;
    const uint32_t r3 = 60000;
    double minV = 1.2;
    double maxV = 3.1;
    double rezistors[3] = {100, 2000, 30000};
    auto& memoryRegs = memory.i2c.devicesMap[0xA0];
    memoryRegs[memoryMap::rezistor_1_U_HSB] = 11;
    memoryRegs[memoryMap::rezistor_1_U_MSB] = 12;
    memoryRegs[memoryMap::rezistor_1_U_LSB] = 13;
    memoryRegs[memoryMap::rezistor_2_U_HSB] = 14;
    memoryRegs[memoryMap::rezistor_2_U_MSB] = 15;
    memoryRegs[memoryMap::rezistor_2_U_LSB] = 16;
    memoryRegs[memoryMap::rezistor_3_U_HSB] = 17;
    memoryRegs[memoryMap::rezistor_3_U_MSB] = 18;
    memoryRegs[memoryMap::rezistor_3_U_LSB] = 19;
	BatteryObserver battery(&memory.memory, measure, rezistors, 3, 0xfff, minV, maxV);
    EXPECT_TRUE(battery.init());
    measure[0] = 0xfff * 3.0 * (rezistors[0] / (double(r1) + rezistors[0])) / 3.3;
    measure[1] = 0xfff * 5.8 * (rezistors[1] / (double(r2) + rezistors[1])) / 3.3;
    measure[2] = 0xfff * 8.8 * (rezistors[2] / (double(r3) + rezistors[2])) / 3.3;
    battery.correctResistor(30);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_1_U_HSB], (r1 >> 16) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_1_U_MSB], (r1 >> 8) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_1_U_LSB], r1 & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_2_U_HSB], 14);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_2_U_MSB], 15);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_2_U_LSB], 16);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_3_U_HSB], 17);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_3_U_MSB], 18);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_3_U_LSB], 19);
    battery.correctResistor(88);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_1_U_HSB], (r1 >> 16) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_1_U_MSB], (r1 >> 8) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_1_U_LSB], r1 & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_2_U_HSB], 14);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_2_U_MSB], 15);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_2_U_LSB], 16);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_3_U_HSB], (r3 >> 16) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_3_U_MSB], (r3 >> 8) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_3_U_LSB], r3 & 0xff);
    battery.correctResistor(58);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_1_U_HSB], (r1 >> 16) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_1_U_MSB], (r1 >> 8) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_1_U_LSB], r1 & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_2_U_HSB], (r2 >> 16) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_2_U_MSB], (r2 >> 8) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_2_U_LSB], r2 & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_3_U_HSB], (r3 >> 16) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_3_U_MSB], (r3 >> 8) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_3_U_LSB], r3 & 0xff);
    EXPECT_ANY_THROW(battery.correctResistor(12));
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_1_U_HSB], (r1 >> 16) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_1_U_MSB], (r1 >> 8) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_1_U_LSB], r1 & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_2_U_HSB], (r2 >> 16) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_2_U_MSB], (r2 >> 8) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_2_U_LSB], r2 & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_3_U_HSB], (r3 >> 16) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_3_U_MSB], (r3 >> 8) & 0xff);
    EXPECT_EQ(memoryRegs[memoryMap::rezistor_3_U_LSB], r3 & 0xff);
    EXPECT_NO_THROW(battery.correctResistor(13));
}

TEST(BatteryObserver_Test, minimaliseBatteryUse){
	uint16_t measure[2];
    TestMocks::TestMemory memory;
    uint32_t r1 = 3000;
    uint32_t r2 = 20000;
    double minV = 1.5;
    double maxV = 4.5;
    double rezistors[2] = {6000, 4700};
    auto& memoryRegs = memory.i2c.devicesMap[0xA0];
    memoryRegs[memoryMap::rezistor_1_U_HSB] = (r1 >> 16) & 0xff;
    memoryRegs[memoryMap::rezistor_1_U_MSB] = (r1 >> 8) & 0xff;
    memoryRegs[memoryMap::rezistor_1_U_LSB] = r1 & 0xff;
    memoryRegs[memoryMap::rezistor_2_U_HSB] = (r2 >> 16) & 0xff;
    memoryRegs[memoryMap::rezistor_2_U_MSB] = (r2 >> 8) & 0xff;
    memoryRegs[memoryMap::rezistor_2_U_LSB] = r2 & 0xff;
	BatteryObserver battery(&memory.memory, measure, rezistors, 2, 0xfff, minV, maxV);
    EXPECT_TRUE(battery.init());
    measure[0] = 0xfff;
    measure[1] = 0xfff;
    for(uint i = 0; i < 0xfff; i++){
        EXPECT_FALSE(battery.minimaliseBatteryUse());
        measure[0]--;
        if(battery.batteryPercentVal() < 1)
            break;
    }
    EXPECT_TRUE(battery.minimaliseBatteryUse());
    for(uint i = 0; i < 0xfff; i++){
        EXPECT_TRUE(battery.minimaliseBatteryUse());
        measure[0]++;
        if(battery.batteryPercentVal() > 5)
            break;
    }
    EXPECT_FALSE(battery.minimaliseBatteryUse());
}
