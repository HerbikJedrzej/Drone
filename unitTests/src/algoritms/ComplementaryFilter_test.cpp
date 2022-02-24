#include <gtest/gtest.h>
#include "../../src/algoritms/ComplementaryFilter.hh"
#include <cstdlib>

#define ROLL 0
#define PITCH 1
#define YAW 2
#define NUMBERS_OF_TESTS 10

namespace ComplementaryFilterTests
{
double randDouble(){
	return ((double)std::rand()) / 100;
}

TEST(ComplementaryFilter, withoutMagnetometerMesurements){
	ComplementaryFilter filter1;
	ComplementaryFilter filter2;
	filter1.changeA(0);
	for(unsigned int i= 0; i < NUMBERS_OF_TESTS * 10; i++){
		const double akcelerometer[] = {randDouble(), randDouble(), randDouble()};
		const double gyroskope[] = {randDouble(), randDouble(), randDouble()};
		const double magnetometer[] = {randDouble(), randDouble(), randDouble()};
		filter1.run(akcelerometer, gyroskope, magnetometer, 0.0, 0.0);
		filter2.run(akcelerometer, gyroskope, nullptr, 0.0, 0.0);
		EXPECT_DOUBLE_EQ(filter1[2], filter2[2]);
	}
}
TEST(ComplementaryFilter, first)
{
	ComplementaryFilter filter;
	filter.changeA(0.005);
	double acceleromiterData[3], gyroskopeData[3], magnetometerData[3],
	expected[NUMBERS_OF_TESTS][3] = {
		0.004670148823192618072, -0.005516072483842712759, -0.001776127602645304494, 0.01949706815063754345,
		-0.000714505779671980451, 0.004778994827858663516, 0.04444939076107513876, 0.0143020812609009236,
		0.02081002062468989555, 0.07949706259504603356, 0.03944229201594327827, 0.04659961864219420991,
		0.1246110531218934114, 0.07462502119805596024, 0.08226370716341066536, 0.1797629109431771455,
		0.1197788341452529493, 0.1278667806241504667, 0.2449241426725664861, 0.174841748388166629,
		0.183451472855121922, 0.3200654121992942969, 0.2397615114431980443, 0.2490461983294190718,
		0.4051555892172674644, 0.3144965285888077222, 0.3246652266462810021, 0.5001607178126850073,
		0.3990177085356028663, 0.4103067519461778834
	};
	for (unsigned int i = 0; i < 10; i++)
	{
		acceleromiterData[0] = 12.532 + i;
		acceleromiterData[1] = -23.4512 + i;
		acceleromiterData[2] = 0.8734 + i;
		gyroskopeData[0] = 1.24 + i;
		gyroskopeData[1] = 0.2 + i;
		gyroskopeData[2] = -0.098 + i;
		magnetometerData[0] = 1.24 + i;
		magnetometerData[1] = 0.2 + i;
		magnetometerData[2] = -0.098 + i;
		filter.run(acceleromiterData, gyroskopeData, magnetometerData, 0.0, 0.0);
		for(unsigned int j = 0; j < 3; j++)
			EXPECT_DOUBLE_EQ(filter[j], expected[i][j]);
	}
}

}
