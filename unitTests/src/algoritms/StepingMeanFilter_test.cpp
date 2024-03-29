#include <gtest/gtest.h>
#include "../../src/algoritms/StepingMeanFilter.hh"
#include <cstdlib>

#define ROLL 0
#define PITCH 1
#define YAW 2
#define NUMBERS_OF_TESTS 30

namespace StepingMeanFilterTests
{

TEST(StepingMeanFilter, positiveSamples)
{
	StepingMeanFilter filter;
	double data[] = {
	    12.53, 25.33, 23.75, 12.356, 23.76, 43.54, 25.46, 35.35124, 14.214, 12.15,
		23.53, 25.33, 43.75,  1.356,  3.76, 43.54, 35.46, 25.35124, 34.214, 11.15,
		23.15, 46.23, 23.15,  23.12, 124.1, 24.51, 35.12,    35.12,   5.12, 1.12
   	};
	double expected[] = {
		0.6265, 1.893, 3.0805, 3.6983, 4.8863, 7.0633, 8.3363, 10.1039, 10.8146,11.4221,
        12.5986, 13.8651, 16.0526, 16.1204, 16.3084, 18.4854, 20.2584, 21.5259, 23.2366, 23.7941,
        24.3251, 25.3701, 25.3401, 25.8783, 30.8953, 29.9438, 30.4268, 30.4153, 29.9606, 29.4091
	};
	for(unsigned int i = 0; i < NUMBERS_OF_TESTS; i++){
		const double tmp[3] = {
			data[i],
			data[i],
			data[i]
		};
		filter.run(tmp);
		for(unsigned int j = 0; j < 3; j++)
			EXPECT_NEAR(expected[i], filter[j], 0.00005);
	}
}

TEST(StepingMeanFilter, negativeSamples)
{
	StepingMeanFilter filter;
	double data[] = {
		-12.53, -25.33, -23.75, -12.356, -23.76, -43.54, -25.46, -35.35124, -14.214, -12.15,
		-23.53, -25.33, -43.75, -1.356, -3.76, -43.54, -35.46, -25.35124, -34.214, -11.15,
		-23.15, -46.23, -23.15, -23.12, -124.1, -24.51, -35.12, -35.12, -5.12, -1.12
	};
	double expected[] = {
		-0.6265, -1.893, -3.0805, -3.6983, -4.8863, -7.0633, -8.3363, -10.1039, -10.8146,-11.4221,
        -12.5986, -13.8651, -16.0526, -16.1204, -16.3084, -18.4854, -20.2584, -21.5259, -23.2366, -23.7941,
        -24.3251, -25.3701, -25.3401, -25.8783, -30.8953, -29.9438, -30.4268, -30.4153, -29.9606, -29.4091
	};
	for (unsigned int i = 0; i < NUMBERS_OF_TESTS; i++)
	{
		const double tmp[3] = {
			data[i],
			data[i],
			data[i]};
		filter.run(tmp);
		for (unsigned int j = 0; j < 3; j++)
			EXPECT_NEAR(expected[i], filter[j], 0.00005);
	}
}

TEST(StepingMeanFilter, samplesGivingZero)
{
	StepingMeanFilter filter;
	double data1[] = {
		12.53, 25.33, 23.75, 12.356, 23.76, 43.54, 25.46, 35.35124, 14.214, 12.15,
		23.53, 25.33, 43.75, 1.356, 3.76, 43.54, 35.46, 25.35124, 34.214, 11.15,
		23.15, 46.23, 23.15, 23.12, 124.1, 24.51, 35.12, 35.12, 5.12, 1.12
	};
	double data2[] = {
		-12.53, -25.33, -23.75, -12.356, -23.76, -43.54, -25.46, -35.35124, -14.214, -12.15,
		-23.53, -25.33, -43.75, -1.356, -3.76, -43.54, -35.46, -25.35124, -34.214, -11.15,
		-23.15, -46.23, -23.15, -23.12, -124.1, -24.51, -35.12, -35.12, -5.12, -1.12
	};
	for (unsigned int i = 0; i < NUMBERS_OF_TESTS * 2; i++)
	{
		if (i % 2 == 0){
			const double tmp[] = {data1[i / 2], data1[i / 2], data1[i / 2]};
			filter.run(tmp);
		}
		else
		{
			const double tmp[] = {data2[i / 2], data2[i / 2], data2[i / 2]};
			filter.run(tmp);
			EXPECT_NEAR(0, filter[0], 0.00005);
			EXPECT_NEAR(0, filter[1], 0.00005);
			EXPECT_NEAR(0, filter[2], 0.00005);
		}
	}
}

TEST(StepingMeanFilter, Variacion)
{
	StepingMeanFilter filter;
	double data[] = {
	    2.53, 5.33, 2.75, 2.356, 3.76, 3.54, 2.46, 3.35124, 1.214, 2.15,
		3.53, 5.33, 3.75, 1.356, 3.36, 4.54, 3.46, 2.35124, 3.214, 1.15,
		3.15, 6.23, 2.15, 3.122, 4.15, 2.51, 3.12, 3.12241, 5.121, 1.12
   	};
	double expectedVariacion[] = {
		0.304043, 1.58604, 1.83718, 1.97586, 2.40363, 2.70283, 2.74101, 2.89368,
		2.80539, 2.73158, 2.80384, 3.27457, 3.2244, 3.02663, 2.83364, 2.75108,
		2.43211, 2.05085, 1.62362, 1.33953, 1.32406, 1.56277, 1.60388, 1.57322,
		1.6039, 1.617, 1.59371, 1.59181, 1.56127, 1.73458
	};
	for (unsigned int i = 0; i < NUMBERS_OF_TESTS; i++){
		const double tmp[] = {data[i], data[i], data[i]};
		filter.run(tmp);
		const double *resultVariance = filter.getVariance();
		EXPECT_NEAR(resultVariance[0], expectedVariacion[i], 0.00005);
		EXPECT_NEAR(resultVariance[1], expectedVariacion[i], 0.00005);
		EXPECT_NEAR(resultVariance[2], expectedVariacion[i], 0.00005);
	}
}

}
