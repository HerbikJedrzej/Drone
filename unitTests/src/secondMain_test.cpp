#include <gtest/gtest.h>
#include <TestGPIO.hh>
#include <TestEngine.hh>
#include <TestRadioParser.hh>
#include <TestRadio.hh>
#include <TestIMUsensor.hh>
#include <TestMemory.hh>
#include <TestBatteryObserver.hh>
#include <QuatroEngineControl.hh>
#include <AHRS.hh>
#include <AltitudeProvider.hh>
#include <stdexcept>
#include <secondMain.hh>
#include <PID.hh>

namespace SecondMain_Test{
    Drivers::LED TestLed(TestMocks::gpio, TestMocks::delay);
    bool transmitUART(const char*, unsigned int){
        return true;
    }
}

TEST(SecondMain_Test, missingMemory){
    GlobalStruct globals{0, 0, 0};
    TestMocks::TestMemory memory;
    DriversGroup drivers{
        SecondMain_Test::TestLed,
        nullptr,    //     Drivers::RadioIfc* const radio;
        nullptr,    //     Drivers::RadioParser* const radioParser;
        nullptr,    //     Drivers::IMUsensorIfc* const dof10sensors;
        nullptr,    //     Drivers::Memory* const memory;
        nullptr,    //     EngineIfc* const enines;
        nullptr,    //     AHRS* const ahrs;
        nullptr,    //     PID* const pidX;
        nullptr,    //     PID* const pidY;
        nullptr,    //     PID* const pidZ;
        nullptr,    //     PID* const pidH;
        nullptr,    //     Barometer* const barometer;
        nullptr,    //     AltitudeProvider* const altitudeProvider;
        nullptr,    //     BatteryObserver* const battery;
        SecondMain_Test::transmitUART
    };
    TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect] = false;
    try{
        mainSetup(&TestMocks::gpio, globals, drivers, TestMocks::delay);
        EXPECT_TRUE(false);
    }catch(const std::invalid_argument& e){
        EXPECT_STREQ("Memory is not created.", e.what());
    }
    EXPECT_FALSE(TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect]);
}

TEST(SecondMain_Test, missingRadio){
    GlobalStruct globals{0, 0, 0};
    TestMocks::TestMemory memory;
    // std::map<uint16_t, uint8_t> memMap = TestMocks::TestI2C::devicesMap[0xA0];
    DriversGroup drivers{
        SecondMain_Test::TestLed,
        nullptr,    //     Drivers::RadioIfc* const radio;
        nullptr,    //     Drivers::RadioParser* const radioParser;
        nullptr,    //     Drivers::IMUsensorIfc* const dof10sensors;
        &memory.memory,    //     Drivers::Memory* const memory;
        nullptr,    //     EngineIfc* const enines;
        nullptr,    //     AHRS* const ahrs;
        nullptr,    //     PID* const pidX;
        nullptr,    //     PID* const pidY;
        nullptr,    //     PID* const pidZ;
        nullptr,    //     PID* const pidH;
        nullptr,    //     Barometer* const barometer;
        nullptr,    //     AltitudeProvider* const altitudeProvider;
        nullptr,    //     BatteryObserver* const battery;
        SecondMain_Test::transmitUART
    };
    TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect] = false;
    try{
        mainSetup(&TestMocks::gpio, globals, drivers, TestMocks::delay);
        EXPECT_TRUE(false);
    }catch(const std::invalid_argument& e){
        EXPECT_STREQ("Radio is not created.", e.what());
    }
    EXPECT_TRUE(TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect]);
}


TEST(SecondMain_Test, missingRadioParser){
    GlobalStruct globals{0, 0, 0};
    TestMocks::TestRadio radio;
    TestMocks::TestMemory memory;
    // std::map<uint16_t, uint8_t> memMap = TestMocks::TestI2C::devicesMap[0xA0];
    DriversGroup drivers{
        SecondMain_Test::TestLed,
        &radio,    //     Drivers::RadioIfc* const radio;
        nullptr,    //     Drivers::RadioParser* const radioParser;
        nullptr,    //     Drivers::IMUsensorIfc* const dof10sensors;
        &memory.memory,    //     Drivers::Memory* const memory;
        nullptr,    //     EngineIfc* const enines;
        nullptr,    //     AHRS* const ahrs;
        nullptr,    //     PID* const pidX;
        nullptr,    //     PID* const pidY;
        nullptr,    //     PID* const pidZ;
        nullptr,    //     PID* const pidH;
        nullptr,    //     Barometer* const barometer;
        nullptr,    //     AltitudeProvider* const altitudeProvider;
        nullptr,    //     BatteryObserver* const battery;
        SecondMain_Test::transmitUART
    };
    TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect] = false;
    try{
        mainSetup(&TestMocks::gpio, globals, drivers, TestMocks::delay);
        EXPECT_TRUE(false);
    }catch(const std::invalid_argument& e){
        EXPECT_STREQ("RadioParser is not created.", e.what());
    }
    EXPECT_TRUE(TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect]);
}

TEST(SecondMain_Test, missingDOF10Sensor){
    GlobalStruct globals{0, 0, 0};
    TestMocks::TestRadio radio;
    TestMocks::TestRadioParser radioParser;
    TestMocks::TestMemory memory;
    // std::map<uint16_t, uint8_t> memMap = TestMocks::TestI2C::devicesMap[0xA0];
    DriversGroup drivers{
        SecondMain_Test::TestLed,
        &radio,    //     Drivers::RadioIfc* const radio;
        &radioParser,    //     Drivers::RadioParser* const radioParser;
        nullptr,    //     Drivers::IMUsensorIfc* const dof10sensors;
        &memory.memory,    //     Drivers::Memory* const memory;
        nullptr,    //     EngineIfc* const enines;
        nullptr,    //     AHRS* const ahrs;
        nullptr,    //     PID* const pidX;
        nullptr,    //     PID* const pidY;
        nullptr,    //     PID* const pidZ;
        nullptr,    //     PID* const pidH;
        nullptr,    //     Barometer* const barometer;
        nullptr,    //     AltitudeProvider* const altitudeProvider;
        nullptr,    //     BatteryObserver* const battery;
        SecondMain_Test::transmitUART
    };
    TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect] = false;
    try{
        mainSetup(&TestMocks::gpio, globals, drivers, TestMocks::delay);
        EXPECT_TRUE(false);
    }catch(const std::invalid_argument& e){
        EXPECT_STREQ("DOF 10 sensor is not created.", e.what());
    }
    EXPECT_TRUE(TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect]);
}

TEST(SecondMain_Test, missingEngines){
    GlobalStruct globals{0, 0, 0};
    TestMocks::TestRadio radio;
    TestMocks::TestRadioParser radioParser;
    TestMocks::TestIMU imu;
    TestMocks::TestMemory memory;
    // std::map<uint16_t, uint8_t> memMap = TestMocks::TestI2C::devicesMap[0xA0];
    DriversGroup drivers{
        SecondMain_Test::TestLed,
        &radio,    //     Drivers::RadioIfc* const radio;
        &radioParser,    //     Drivers::RadioParser* const radioParser;
        &imu,    //     Drivers::IMUsensorIfc* const dof10sensors;
        &memory.memory,    //     Drivers::Memory* const memory;
        nullptr,    //     EngineIfc* const enines;
        nullptr,    //     AHRS* const ahrs;
        nullptr,    //     PID* const pidX;
        nullptr,    //     PID* const pidY;
        nullptr,    //     PID* const pidZ;
        nullptr,    //     PID* const pidH;
        nullptr,    //     Barometer* const barometer;
        nullptr,    //     AltitudeProvider* const altitudeProvider;
        nullptr,    //     BatteryObserver* const battery;
        SecondMain_Test::transmitUART
    };
    TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect] = false;
    try{
        mainSetup(&TestMocks::gpio, globals, drivers, TestMocks::delay);
        EXPECT_TRUE(false);
    }catch(const std::invalid_argument& e){
        EXPECT_STREQ("EnginesControl is not created.", e.what());
    }
    EXPECT_TRUE(TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect]);
}

TEST(SecondMain_Test, missingAHRS){
    GlobalStruct globals{0, 0, 0};
    TestMocks::TestRadio radio;
    TestMocks::TestRadioParser radioParser;
    TestMocks::TestEngine engines(4);
    QuatroEngineControl enginesControl(&engines);
    TestMocks::TestIMU imu;
    TestMocks::TestMemory memory;
    // std::map<uint16_t, uint8_t> memMap = TestMocks::TestI2C::devicesMap[0xA0];
    DriversGroup drivers{
        SecondMain_Test::TestLed,
        &radio,    //     Drivers::RadioIfc* const radio;
        &radioParser,    //     Drivers::RadioParser* const radioParser;
        &imu,    //     Drivers::IMUsensorIfc* const dof10sensors;
        &memory.memory,    //     Drivers::Memory* const memory;
        &enginesControl,    //     EngineIfc* const enines;
        nullptr,    //     AHRS* const ahrs;
        nullptr,    //     PID* const pidX;
        nullptr,    //     PID* const pidY;
        nullptr,    //     PID* const pidZ;
        nullptr,    //     PID* const pidH;
        nullptr,    //     Barometer* const barometer;
        nullptr,    //     AltitudeProvider* const altitudeProvider;
        nullptr,    //     BatteryObserver* const battery;
        SecondMain_Test::transmitUART
    };
    TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect] = false;
    try{
        mainSetup(&TestMocks::gpio, globals, drivers, TestMocks::delay);
        EXPECT_TRUE(false);
    }catch(const std::invalid_argument& e){
        EXPECT_STREQ("AHRS is not created.", e.what());
    }
    EXPECT_TRUE(TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect]);
}

TEST(SecondMain_Test, missingPIDforX){
    GlobalStruct globals{0, 0, 0};
    TestMocks::TestRadio radio;
    TestMocks::TestRadioParser radioParser;
    TestMocks::TestEngine engines(4);
    QuatroEngineControl enginesControl(&engines);
    TestMocks::TestIMU imu;
    double imuData[3];
    AHRS ahrs(imuData, imuData, nullptr);
    TestMocks::TestMemory memory;
    // std::map<uint16_t, uint8_t> memMap = TestMocks::TestI2C::devicesMap[0xA0];
    DriversGroup drivers{
        SecondMain_Test::TestLed,
        &radio,    //     Drivers::RadioIfc* const radio;
        &radioParser,    //     Drivers::RadioParser* const radioParser;
        &imu,    //     Drivers::IMUsensorIfc* const dof10sensors;
        &memory.memory,    //     Drivers::Memory* const memory;
        &enginesControl,    //     EngineIfc* const enines;
        &ahrs,    //     AHRS* const ahrs;
        nullptr,    //     PID* const pidX;
        nullptr,    //     PID* const pidY;
        nullptr,    //     PID* const pidZ;
        nullptr,    //     PID* const pidH;
        nullptr,    //     Barometer* const barometer;
        nullptr,    //     AltitudeProvider* const altitudeProvider;
        nullptr,    //     BatteryObserver* const battery;
        SecondMain_Test::transmitUART
    };
    TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect] = false;
    try{
        mainSetup(&TestMocks::gpio, globals, drivers, TestMocks::delay);
        EXPECT_TRUE(false);
    }catch(const std::invalid_argument& e){
        EXPECT_STREQ("PID for axis X is not created.", e.what());
    }
    EXPECT_TRUE(TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect]);
}

TEST(SecondMain_Test, missingPIDforY){
    GlobalStruct globals{0, 0, 0};
    TestMocks::TestRadio radio;
    TestMocks::TestRadioParser radioParser;
    TestMocks::TestEngine engines(4);
    QuatroEngineControl enginesControl(&engines);
    TestMocks::TestIMU imu;
    double imuData[3];
    AHRS ahrs(imuData, imuData, nullptr);
    TestMocks::TestMemory memory;
    PID pidX;
    // std::map<uint16_t, uint8_t> memMap = TestMocks::TestI2C::devicesMap[0xA0];
    DriversGroup drivers{
        SecondMain_Test::TestLed,
        &radio,    //     Drivers::RadioIfc* const radio;
        &radioParser,    //     Drivers::RadioParser* const radioParser;
        &imu,    //     Drivers::IMUsensorIfc* const dof10sensors;
        &memory.memory,    //     Drivers::Memory* const memory;
        &enginesControl,    //     EngineIfc* const enines;
        &ahrs,    //     AHRS* const ahrs;
        &pidX,    //     PID* const pidX;
        nullptr,    //     PID* const pidY;
        nullptr,    //     PID* const pidZ;
        nullptr,    //     PID* const pidH;
        nullptr,    //     Barometer* const barometer;
        nullptr,    //     AltitudeProvider* const altitudeProvider;
        nullptr,    //     BatteryObserver* const battery;
        SecondMain_Test::transmitUART
    };
    TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect] = false;
    try{
        mainSetup(&TestMocks::gpio, globals, drivers, TestMocks::delay);
        EXPECT_TRUE(false);
    }catch(const std::invalid_argument& e){
        EXPECT_STREQ("PID for axis Y is not created.", e.what());
    }
    EXPECT_TRUE(TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect]);
}

TEST(SecondMain_Test, missingPIDforZ){
    GlobalStruct globals{0, 0, 0};
    TestMocks::TestRadio radio;
    TestMocks::TestRadioParser radioParser;
    TestMocks::TestEngine engines(4);
    QuatroEngineControl enginesControl(&engines);
    TestMocks::TestIMU imu;
    double imuData[3];
    AHRS ahrs(imuData, imuData, nullptr);
    TestMocks::TestMemory memory;
    PID pidX;
    PID pidY;
    // std::map<uint16_t, uint8_t> memMap = TestMocks::TestI2C::devicesMap[0xA0];
    DriversGroup drivers{
        SecondMain_Test::TestLed,
        &radio,    //     Drivers::RadioIfc* const radio;
        &radioParser,    //     Drivers::RadioParser* const radioParser;
        &imu,    //     Drivers::IMUsensorIfc* const dof10sensors;
        &memory.memory,    //     Drivers::Memory* const memory;
        &enginesControl,    //     EngineIfc* const enines;
        &ahrs,    //     AHRS* const ahrs;
        &pidX,    //     PID* const pidX;
        &pidY,    //     PID* const pidY;
        nullptr,    //     PID* const pidZ;
        nullptr,    //     PID* const pidH;
        nullptr,    //     Barometer* const barometer;
        nullptr,    //     AltitudeProvider* const altitudeProvider;
        nullptr,    //     BatteryObserver* const battery;
        SecondMain_Test::transmitUART
    };
    TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect] = false;
    try{
        mainSetup(&TestMocks::gpio, globals, drivers, TestMocks::delay);
        EXPECT_TRUE(false);
    }catch(const std::invalid_argument& e){
        EXPECT_STREQ("PID for axis Z is not created.", e.what());
    }
    EXPECT_TRUE(TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect]);
}

TEST(SecondMain_Test, missingPIDforH){
    GlobalStruct globals{0, 0, 0};
    TestMocks::TestRadio radio;
    TestMocks::TestRadioParser radioParser;
    TestMocks::TestEngine engines(4);
    QuatroEngineControl enginesControl(&engines);
    TestMocks::TestIMU imu;
    double imuData[3];
    AHRS ahrs(imuData, imuData, nullptr);
    AltitudeProvider altitudeProvider(0.1);
    TestMocks::TestMemory memory;
    PID pidX;
    PID pidY;
    PID pidZ;
    DriversGroup drivers{
        SecondMain_Test::TestLed,
        &radio,    //     Drivers::RadioIfc* const radio;
        &radioParser,    //     Drivers::RadioParser* const radioParser;
        &imu,    //     Drivers::IMUsensorIfc* const dof10sensors;
        &memory.memory,    //     Drivers::Memory* const memory;
        &enginesControl,    //     EngineIfc* const enines;
        &ahrs,    //     AHRS* const ahrs;
        &pidX,    //     PID* const pidX;
        &pidY,    //     PID* const pidY;
        &pidZ,    //     PID* const pidZ;
        nullptr,    //     PID* const pidH;
        nullptr,    //     Barometer* const barometer;
        nullptr,    //     AltitudeProvider* const altitudeProvider;
        nullptr,    //     BatteryObserver* const battery;
        SecondMain_Test::transmitUART
    };
    TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect] = false;
    try{
        mainSetup(&TestMocks::gpio, globals, drivers, TestMocks::delay);
        EXPECT_TRUE(false);
    }catch(const std::invalid_argument& e){
        EXPECT_STREQ("PID for axis H is not created.", e.what());
    }
    EXPECT_TRUE(TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect]);
}

TEST(SecondMain_Test, missingAltitudeProvider){
    GlobalStruct globals{0, 0, 0};
    TestMocks::TestRadio radio;
    TestMocks::TestRadioParser radioParser;
    TestMocks::TestEngine engines(4);
    QuatroEngineControl enginesControl(&engines);
    TestMocks::TestIMU imu;
    double imuData[3];
    AHRS ahrs(imuData, imuData, nullptr);
    TestMocks::TestMemory memory;
    PID pidX;
    PID pidY;
    PID pidZ;
    PID pidH;
    DriversGroup drivers{
        SecondMain_Test::TestLed,
        &radio,    //     Drivers::RadioIfc* const radio;
        &radioParser,    //     Drivers::RadioParser* const radioParser;
        &imu,    //     Drivers::IMUsensorIfc* const dof10sensors;
        &memory.memory,    //     Drivers::Memory* const memory;
        &enginesControl,    //     EngineIfc* const enines;
        &ahrs,    //     AHRS* const ahrs;
        &pidX,    //     PID* const pid;
        &pidY,    //     PID* const pid;
        &pidZ,    //     PID* const pidZ;
        &pidH,    //     PID* const pidH;
        nullptr,    //     Barometer* const barometer;
        nullptr,    //     AltitudeProvider* const altitudeProvider;
        nullptr,    //     BatteryObserver* const battery;
        SecondMain_Test::transmitUART
    };
    TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect] = false;
    try{
        mainSetup(&TestMocks::gpio, globals, drivers, TestMocks::delay);
        EXPECT_TRUE(false);
    }catch(const std::invalid_argument& e){
        EXPECT_STREQ("AltitudeProvider is not created.", e.what());
    }
    EXPECT_TRUE(TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect]);
}

TEST(SecondMain_Test, missingBatteryObserver){
    GlobalStruct globals{0, 0, 0};
    TestMocks::TestRadio radio;
    TestMocks::TestRadioParser radioParser;
    TestMocks::TestEngine engines(4);
    QuatroEngineControl enginesControl(&engines);
    TestMocks::TestIMU imu;
    double imuData[3];
    AHRS ahrs(imuData, imuData, nullptr);
    AltitudeProvider altitudeProvider(0.1);
    TestMocks::TestMemory memory;
    PID pidX;
    PID pidY;
    PID pidZ;
    PID pidH;
    DriversGroup drivers{
        SecondMain_Test::TestLed,
        &radio,    //     Drivers::RadioIfc* const radio;
        &radioParser,    //     Drivers::RadioParser* const radioParser;
        &imu,    //     Drivers::IMUsensorIfc* const dof10sensors;
        &memory.memory,    //     Drivers::Memory* const memory;
        &enginesControl,    //     EngineIfc* const enines;
        &ahrs,    //     AHRS* const ahrs;
        &pidX,    //     PID* const pid;
        &pidY,    //     PID* const pid;
        &pidZ,    //     PID* const pidZ;
        &pidH,    //     PID* const pidH;
        nullptr,    //     Barometer* const barometer;
        &altitudeProvider,    //     AltitudeProvider* const altitudeProvider;
        nullptr,    //     BatteryObserver* const battery;
        SecondMain_Test::transmitUART
    };
    TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect] = false;
    try{
        mainSetup(&TestMocks::gpio, globals, drivers, TestMocks::delay);
        EXPECT_TRUE(false);
    }catch(const std::invalid_argument& e){
        EXPECT_STREQ("Battery observer is not created.", e.what());
    }
    EXPECT_TRUE(TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect]);
}

TEST(SecondMain_Test, initPASS){
    GlobalStruct globals{0, 0, 0};
    TestMocks::TestRadio radio;
    TestMocks::TestRadioParser radioParser;
    TestMocks::TestEngine engines(4);
    QuatroEngineControl enginesControl(&engines);
    TestMocks::TestIMU imu;
    double imuData[3];
    AHRS ahrs(imuData, imuData, nullptr);
    AltitudeProvider altitudeProvider(0.1);
    TestMocks::TestMemory memory;
    PID pidX;
    PID pidY;
    PID pidZ;
    PID pidH;
    double rezistors[3] = {10000, 22000, 47000};
    TestMocks::TestBatteryObserver battery(rezistors, 3, 2.0, 3.7);
    DriversGroup drivers{
        SecondMain_Test::TestLed,
        &radio,    //     Drivers::RadioIfc* const radio;
        &radioParser,    //     Drivers::RadioParser* const radioParser;
        &imu,    //     Drivers::IMUsensorIfc* const dof10sensors;
        &memory.memory,    //     Drivers::Memory* const memory;
        &enginesControl,    //     EngineIfc* const enines;
        &ahrs,    //     AHRS* const ahrs;
        &pidX,    //     PID* const pidX;
        &pidY,    //     PID* const pidY;
        &pidZ,    //     PID* const pidZ;
        &pidH,    //     PID* const pidH;
        nullptr,    //     Barometer* const barometer;
        &altitudeProvider,    //     AltitudeProvider* const altitudeProvider;
        &battery,    //     BatteryObserver* const battery;
        SecondMain_Test::transmitUART
    };
    TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect] = false;
    auto& memoryRegs = memory.i2c.devicesMap[0xA0];
    for(uint8_t i = 0; i < 10; i++){
    	memoryRegs[i] = 0;
    }
	memoryRegs[memoryMap::commonFactorForAxisXYZ] = 0;
	memoryRegs[memoryMap::pid_P_XY_00] = 0;
	memoryRegs[memoryMap::pid_I_XY_00] = 0;
	memoryRegs[memoryMap::pid_D_XY_00] = 0;
	memoryRegs[memoryMap::pid_P_Z] = 0;
	memoryRegs[memoryMap::pid_I_Z] = 0;
	memoryRegs[memoryMap::pid_D_Z] = 0;
	memoryRegs[memoryMap::pid_P_H] = 0;
	memoryRegs[memoryMap::pid_I_H] = 0;
	memoryRegs[memoryMap::pid_D_H] = 0;
	memoryRegs[memoryMap::angleOffsetX] = 0;
	memoryRegs[memoryMap::angleOffsetY] = 0;
	memoryRegs[memoryMap::mainLoopTrybe] = 0;
	memoryRegs[memoryMap::initEnginePower] = 0;
	memoryRegs[memoryMap::initEnginePower + 1] = 0;
	memoryRegs[memoryMap::initEnginePower + 2] = 0;
	memoryRegs[memoryMap::rezistor_1_U_HSB] = 0;
	memoryRegs[memoryMap::rezistor_1_U_MSB] = 0;
	memoryRegs[memoryMap::rezistor_1_U_LSB] = 0;
	memoryRegs[memoryMap::rezistor_2_U_HSB] = 0;
	memoryRegs[memoryMap::rezistor_2_U_MSB] = 0;
	memoryRegs[memoryMap::rezistor_2_U_LSB] = 0;
	memoryRegs[memoryMap::rezistor_3_U_HSB] = 0;
	memoryRegs[memoryMap::rezistor_3_U_MSB] = 0;
	memoryRegs[memoryMap::rezistor_3_U_LSB] = 0;
	memoryRegs[memoryMap::PID_X_sum_msb] = 0;
	memoryRegs[memoryMap::PID_X_sum_lsb] = 0;
	memoryRegs[memoryMap::PID_Y_sum_msb] = 0;
	memoryRegs[memoryMap::PID_Y_sum_lsb] = 0;
	memoryRegs[memoryMap::PID_Z_sum_msb] = 0;
	memoryRegs[memoryMap::PID_Z_sum_lsb] = 0;
	memoryRegs[memoryMap::PID_H_sum_msb] = 0;
	memoryRegs[memoryMap::PID_H_sum_lsb] = 0;
    TestMocks::sleepedTime = 0;
    try{
        mainSetup(&TestMocks::gpio, globals, drivers, TestMocks::delay);
    }catch(const std::invalid_argument& e){
        EXPECT_STREQ("", e.what());
    }catch(...){
        EXPECT_TRUE(false);
    }
    EXPECT_EQ(TestMocks::sleepedTime, 124);
    EXPECT_FALSE(TestMocks::TestGPIO::outputs[OutputList::MemoryWriteProtect]);
}

// radioParser.pushDataToQueue();
