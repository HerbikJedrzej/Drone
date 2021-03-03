#include <gtest/gtest.h>
#include <esdStructure.hh>
#include <cstdlib>

#define AMOUNT_OF_CELLS 6

TEST(ServoStructure, checkRangeOfEsdStruct){
    uint16_t variable[AMOUNT_OF_CELLS];
    const uint16_t enginesMin[6] = {2000, 3000, 4000, 5000, 6000, 8000};
    const uint16_t enginesMax[6] = {2500, 3800, 5000, 10000, 8000, 9000};
    EsdDriver engines(variable, AMOUNT_OF_CELLS, enginesMin, enginesMax);
    EXPECT_EQ(engines.size(), 6);
    engines.init();
    for (unsigned int i = 0; i < AMOUNT_OF_CELLS; i++)
        EXPECT_EQ(variable[i], enginesMin[i]);
    for (unsigned int i = 0; i < AMOUNT_OF_CELLS; i++)
        engines.set(i, 50.0 + ((double)i) * 10.0);
    for (unsigned int i = 0; i < AMOUNT_OF_CELLS; i++)
        EXPECT_EQ(variable[i], enginesMin[i] + (0.5 + i * 0.1) * (enginesMax[i] - enginesMin[i]));
    for (unsigned int i = 0; i < AMOUNT_OF_CELLS; i++)
        engines.set(i, 0);
    for (unsigned int i = 0; i < AMOUNT_OF_CELLS; i++)
        EXPECT_EQ(variable[i], enginesMin[i]);
    engines.set(0, 100.1);
    EXPECT_EQ(variable[0], 2500);
    engines.set(1, 100000.1);
    EXPECT_EQ(variable[0], 2500);
    EXPECT_EQ(variable[1], 3800);
    engines.set(0, -0.1);
    EXPECT_EQ(variable[0], 2000);
    EXPECT_EQ(variable[1], 3800);
    engines.set(1, -10.1);
    EXPECT_EQ(variable[0], 2000);
    EXPECT_EQ(variable[1], 3000);
}