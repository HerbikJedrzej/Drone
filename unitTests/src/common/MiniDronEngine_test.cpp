#include <gtest/gtest.h>
#include <MiniDronEngine.hh>
#include <cstdlib>

#define AMOUNT_OF_CELLS 10
#define AMOUNT_OF_TEST 10

namespace servoTest{
    void setTest(MiniDronEngine& engine, uint16_t* testValues){
        for(unsigned int i = 0; i < AMOUNT_OF_CELLS; i++)
            engine.set(i, testValues[i]);
    }
    void doTest(uint16_t* orginalVariables, uint16_t* testValues){
        for(unsigned int i = 0; i < AMOUNT_OF_CELLS; i++)
            EXPECT_EQ(orginalVariables[i], testValues[i] * 100);
        }
}

TEST(EngineIfc, round){
    class EngineIfcRoundTest : EngineIfc{
        uint16_t list[1];
    public:
        EngineIfcRoundTest():
            EngineIfc(1, list){}
        void init() override{}
        void set(uint8_t, const double)override{}
        uint16_t r(const double& v){
            return round(v);
        }
    } c;
    EXPECT_EQ(c.r(15.0),  15);
    EXPECT_EQ(c.r(15.1),  15);
    EXPECT_EQ(c.r(15.2),  15);
    EXPECT_EQ(c.r(15.3),  15);
    EXPECT_EQ(c.r(15.4),  15);
    EXPECT_EQ(c.r(15.49), 15);
    EXPECT_EQ(c.r(15.5),  16);
    EXPECT_EQ(c.r(15.51), 16);
    EXPECT_EQ(c.r(15.6),  16);
    EXPECT_EQ(c.r(15.7),  16);
    EXPECT_EQ(c.r(15.8),  16);
    EXPECT_EQ(c.r(15.9),  16);
    EXPECT_EQ(c.r(16.0),  16);
}

TEST(MiniDronEngine, MiniDronEngine){
    uint16_t variable[AMOUNT_OF_CELLS];
    MiniDronEngine engine(variable, AMOUNT_OF_CELLS);
    EXPECT_EQ(engine.size(), AMOUNT_OF_CELLS);
    engine.init();
    for (unsigned int i = 0; i < AMOUNT_OF_CELLS; i++)
        EXPECT_EQ(variable[i], 0);
    for(unsigned int i = 0; i < AMOUNT_OF_TEST; i++){
        uint16_t data[AMOUNT_OF_CELLS];
        for(unsigned int j = 0; j < AMOUNT_OF_CELLS; j++)
            data[j] = (uint16_t)std::rand() % 100;
        servoTest::setTest(engine, data);
        servoTest::doTest(variable, data);
    }    
}