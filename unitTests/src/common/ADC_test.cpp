#include <gtest/gtest.h>
#include <ADC.hh>
#include <cstdlib>

#define AMOUNT_OF_CELLS 10
#define AMOUNT_OF_TEST 10

namespace ADCTest{
    void setTest(uint16_t* originalValues, uint16_t* newValues){
        for(unsigned int i = 0; i < AMOUNT_OF_CELLS; i++)
            originalValues[i] = newValues[i];
    }
    void doTest(ADCcontaine& adc, uint16_t* testValues){
        for(unsigned int i = 0; i < AMOUNT_OF_CELLS; i++)
            EXPECT_EQ(adc[i], testValues[i]);
    }
    void doTest(uint16_t* orginalVariables, uint16_t* testValues){
        for(unsigned int i = 0; i < AMOUNT_OF_CELLS; i++)
            EXPECT_EQ(orginalVariables[i], testValues[i]);
    }
}

TEST(ADC, ADC){
    uint16_t variable[AMOUNT_OF_CELLS];
    ADCcontaine adc(variable, AMOUNT_OF_CELLS);
    EXPECT_EQ(adc.size(), AMOUNT_OF_CELLS);
    for(unsigned int i = 0; i < AMOUNT_OF_TEST; i++){
        uint16_t data[AMOUNT_OF_CELLS];
        for(unsigned int j = 0; j < AMOUNT_OF_CELLS; j++)
            data[j] = (uint16_t)std::rand();
        ADCTest::setTest(variable, data);
        ADCTest::doTest(adc, data);
    }    
}