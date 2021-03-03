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
		0.020378112091141584, 0.010191959018691781, -0.0017830024368430094,
		0.050834454870195729, 0.030623698211107813, 0.0048502038836360159,
		0.091338053814984499, 0.061194598855808316, 0.021048623254859709,
		0.14185924560163479, 0.10181649638553522, 0.047096905735932219,
		0.20236938848139821, 0.15241293004531398, 0.083112669384005938,
		0.27284041789383334, 0.21291876503079746, 0.12915899144406653,
		0.35324422535641836, 0.28328039911881586, 0.18527150340230264,
		0.44355185773767586, 0.3634567948317286, 0.25146494010132392,
		0.54373256579590601, 0.45342170641555019, 0.32773467560723896,
		0.65375277277637933, 0.55316771030423528, 0.41405923445846121
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
