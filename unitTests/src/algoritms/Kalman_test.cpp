#include <gtest/gtest.h>
#include "../../src/algoritms/Kalman.hh"
#include <cstdlib>

#define ROLL 0
#define PITCH 1
#define YAW 2
#define CONVERT_MESURE_TO_DEGRES 16.384
#define CONVERT_MESURE_TO_RADIANS 939.2102
#define CONVERT_RADIANS_TO_DEGRES 0.0174

namespace KalmanTests
{

class KalmanTester : public KalmanAHRS
{
public:
	KalmanTester():KalmanAHRS(){
		double _Q[6] = {
			0.01,
			0.01,
			0.01,
			0.01,
			0.01,
			0.01};
		double _R[3] = {
			1.25,
			1.25,
			1.25};
		changeQ(_Q);
		changeR(_R);
	}
	void countErrorForNewPreditedStateTest(){ // Pp = A * P * A' + Q
		double result[6][6];
		double expected[6][6] = {
			0.0100, 0.0000, 0.0000, 0.0000, 0.0000, 0.0000,
    		0.0000, 0.0100, 0.0000, 0.0000, 0.0000, 0.0000,
			0.0000, 0.0000, 0.0100, 0.0000, 0.0000, 0.0000,
			0.0000, 0.0000, 0.0000, 1.0100, 1.0000, 1.0000,
			0.0000, 0.0000, 0.0000, 1.0000, 1.0100, 1.0000,
			0.0000, 0.0000, 0.0000, 1.0000, 1.0000, 1.0100
		};
		countErrorForNewPreditedState(result);
		for (unsigned int i = 0; i < 6; i++)
			for (unsigned int j = 0; j < 6; j++)
				EXPECT_DOUBLE_EQ(result[i][j], expected[i][j]);
	}
	void countPreditedStateTest(){ // Xp=A*X+w
		double roll = 1.6;
		double pitch = -0.46;
		double Y[6] = {
			-5.03,
			0.013,
			-9.01,
			14.53,
			1.03,
			-2.45
		};
		double expected[6] = {
			-4.99453,
			0.0817651,
			-9.79742,
			14.53,
			1.03,
			-2.45
		};
		X[0] = Y[0];
		X[1] = Y[1];
		X[2] = Y[2];
		X[3] = Y[3];
		X[4] = Y[4];
		X[5] = Y[5];
		acceleretionVectorLengthSquared = Y[0] * Y[0] + Y[1] * Y[1] + Y[2] * Y[2];
		acceleretionVectorLength = sqrt(acceleretionVectorLengthSquared);
		countMatrixA(Y, roll, pitch);
		countPreditedState();
		for(unsigned int i = 0; i < 6; i++)
				EXPECT_NEAR(X[i], expected[i], 0.00005);
	}
	void countMatrixATest(){
		const double Y[] = {-5.03, 0.013, -9.01, 14.53, 1.03, -2.45};
		double expected[6][6] = {
			0.7248,     0.0006,   -0.4046,       0,      0.0026,   0.0879,
			0.0006,     0.9507,    0.0010,   -0.0026,      0,     -0.0436,
			-0.4046,    0.0010,    0.2259,   -0.0879,    0.0436,      0,
				0,         0,         0,      1.0000,      0,         0,
				0,         0,         0,         0,      1.0000,      0,
				0,         0,         0,         0,        0,      1.0000};
		acceleretionVectorLengthSquared = Y[0] * Y[0] + Y[1] * Y[1] + Y[2] * Y[2];
		acceleretionVectorLength = sqrt(acceleretionVectorLengthSquared);
		countMatrixA(Y, 1.6, -0.46);
		for (unsigned int i = 0; i < 6; i++)
			for (unsigned int j = 0; j < 6; j++)
				EXPECT_NEAR(A[i][j], expected[i][j], 0.00005);
	}
	void countNewInputDataTest(){ // Yk=Y+z
		double Y1[] = {-5.03, 0.013, -9.01};
		double Y2[] = {14.53, 1.03, -2.45};
		double result[6];
		double expected[6] = {118.2100, -24.1270, -6.8600, -38.5700, 13.5300, -2.3970};
		z[0] = 123.24;
		z[1] = -24.14;
		z[2] = 2.15;
		z[3] = -53.1;
		z[4] = 12.5;
		z[5] = 0.053;
		countNewInputData(Y1, Y2, result);
		for(unsigned int i = 0; i < 6; i++)
			EXPECT_DOUBLE_EQ(result[i], expected[i]);
	}
	void countInverseCovarianceTest(){ // P=1/(Pp+R)
		double Pkp[6][6] = {
			0.244, 12.24, 0.21, -1.21, -2.51, 12.34,
			21.42, -35.1, -0.2, 1.425, -5.12, 1.004,
			25.12, -12.2, -4.1, 2.511, 2.251, -2.12,
			3.125, 23.35, 12.4, -12.1, 1.551, 24.12,
			-24.1, -5.12, 11.5, 0.125, 0.012, 25.12,
			47.12, 26.23, -3.2, 23.36, 6.231, 236.1
		};
		double expected[6][6] = {
			0.010164, 0.00377311, 0.0151532, 0.00393076, -0.0149981, 0.000782936,
			0.0440562, -0.0236858, 0.00597609, -0.00906379, 0.00382146, -0.00162885,
			0.0624316, -0.0190085, 0.0545331, -0.0111579, 0.0563428, -0.00754712,
			0.102242, -0.0531206, 0.0569275, -0.0888479, 0.051927, -0.00105482,
			-0.226849, -0.0343108, 0.0354454, 0.05397, -0.0759041, 0.0148826,
			-0.0102057, 0.00778192, -0.00951674, 0.00743745, 0.000197849, 0.00386942
		};
		R[0] = 12.34;
		R[1] = 1.004;
		R[2] = 12.34;
		countInverseCovariance(Pkp);
		for (unsigned int i = 0; i < 6; i++)
			for (unsigned int j = 0; j < 6; j++)
				EXPECT_NEAR(P[i][j], expected[i][j], 0.00005);
	}
	void countKalmanGainTest(){ // K=Pp/(Pp+R)
		double Pkp[6][6] = {
			0.244, 12.24, 0.21, -1.21, -2.51, 12.34,
			21.42, -35.1, -0.2, 1.425, -5.12, 1.004,
			25.12, -12.2, -4.1, 2.511, 2.251, -2.12,
			3.125, 23.35, 12.4, -12.1, 1.551, 24.12,
			-24.1, -5.12, 11.5, 0.125, 0.012, 25.12,
			47.12, 26.23, -3.2, 23.36, 6.231, 236.1
		};
		double expected[6][6] = {
			0.874577, -0.0465602, -0.18699, -0.0485055, 0.185076, -0.00966143,
			-0.0442324, 1.02378, -0.00599999, 0.00910005, -0.00383674, 0.00163537,
			-0.770406, 0.234565, 0.327061, 0.137688, -0.69527, 0.0931315,
			-0.000511212, 0.000265603, -0.000284637, 1.00044, -0.000259635, 5.27409e-06,
			0.00113424, 0.000171554, -0.000177227, -0.00026985, 1.00038, -7.44131e-05,
			5.10287e-05, -3.89096e-05, 4.75837e-05, -3.71873e-05, -9.89243e-07, 0.999981};
		double result[6][6];
		R[0] = 12.34;
		R[1] = 1.004;
		R[2] = 12.34;
		countInverseCovariance(Pkp);
		countKalmanGain(Pkp, result);
		for (unsigned int i = 0; i < 6; i++)
			for (unsigned int j = 0; j < 6; j++)
				EXPECT_NEAR(result[i][j], expected[i][j], 0.00005);
	}
	void countNewStateTest(){ // X=Xp+K*(Yk-Xp)
		double K[6][6] = {
  			 21.7272,    2.7535,   -3.0707,   -5.3476,    7.2619,   -1.3432,
  			 -0.5285,    0.9593,    0.0604,    0.1327,   -0.1683,    0.0326,
  			-78.8184,  -10.2494,   11.1243,   19.9679,  -27.1821,    5.0845,
  			 -6.4625,   -0.7952,    0.8165,    2.7105,   -2.2119,    0.4073,
  			111.0321,   14.9277,  -15.3741,  -28.1979,   38.6763,   -7.1024,
  			  0.0361,   -0.0049,    0.0065,   -0.0145,    0.0087,    0.9945
		};		
		X[0] = -5.03;
		X[1] = 0.013;
		X[2] = -9.01;
		X[3] = 14.53;
		X[4] = 1.03;
		X[5] = -2.45;
		double Yk[6] = {
			-4.5693,
			-0.0570,
			-7.3338,
			14.5300,
			1.0300,
			-2.4500
		};
		double expected[6] = {
			-0.3601,
			-0.1964,
			-25.9576,
			12.9770,
			25.3675,
			-2.4221
		};
		countNewState(K, Yk);
		for(unsigned int i = 0; i < 6; i++)
			EXPECT_NEAR(X[i], expected[i], 0.00005);
	}
	void countNewErrorForStateTest(){ // P=(eye-K)*Pp
		double Pkp[6][6] = {
			0.244, 12.24, 0.21, -1.21, -2.51, 12.34,
			21.42, -35.1, -0.2, 1.425, -5.12, 1.004,
			25.12, -12.2, -4.1, 2.511, 2.251, -2.12,
			3.125, 23.35, 12.4, -12.1, 1.551, 24.12,
			-24.1, -5.12, 11.5, 0.125, 0.012, 25.12,
			47.12, 26.23, -3.2, 23.36, 6.231, 236.1
		};
		double K[6][6] = {
		  	  1.7272,    2.7535,   -3.0707,   -5.3476,    7.2619,   -1.3432,
		  	 -0.5285,    0.9593,    0.0604,    0.1327,   -0.1683,    0.0326,
		  	-78.8184,  -10.2494,    1.1243,    1.9679,   -2.1821,    5.0845,
		  	 -6.4625,   -0.7952,    0.8165,    2.7105,   -2.2119,    0.4073,
		  	111.0321,   14.9277,  -15.3741,   -8.1979,    3.6763,   -7.1024,
		  	  0.0361,   -0.0049,    0.0065,   -0.0145,    0.0087,    0.9945
		};
		double expected[6][6] = {
			272.9932,  247.5639,  -33.6917,  -29.5698,   39.4118,  245.4467,
			 -6.5233,    0.9618,    0.7448,    0.1320,   -2.0778,    0.0207,
			-62.6685,  416.0105,   31.9742, -175.7666, -285.2985, -209.9289,
			-79.7445,   -0.7978,   10.0757,    2.7223,  -27.2946,    0.4187,
			464.1366, -631.2151,  -35.2162,  218.0642,  446.6654,  389.6647,
			  0.4470,   -0.0072,    0.0802,   -0.0137,    0.1075,    1.0030
		};
		countNewErrorForState(K, Pkp);
		for (unsigned int i = 0; i < 6; i++)
			for (unsigned int j = 0; j < 6; j++)
				// EXPECT_DOUBLE_EQ(P[i][j], expected[i][j]);
				EXPECT_NEAR(P[i][j], expected[i][j], 0.00005);
	}
	void runAll(){
		double roll = 1.6;
		double pitch = -0.46;
		double Y1[] = {
			-5.03,
			0.013,
			-9.01
		};
		double Y2[] = {
			14.53,
			1.03,
			-2.45};
		double expectedX[] = {
			1.72646,
			3.89298,
			-1.02521,
			11.1301,
			2.13009,
			-0.189912
		};
		double expectedP[6][6] = {
			0.0107451, 0.00181698, -0.000445134, 0.00067371, 0.00067371, 0.00067371,
			0.00181698, 0.013925, -0.000981022, 0.00148477, 0.00148477, 0.00148477,
			-0.000445134, -0.000981022, 0.010161, -0.000363748, -0.000363748, -0.000363748,
			0.00067371, 0.00148477, -0.000363748, 0.00388387, 0.000550532, 0.000550532,
			0.00067371, 0.00148477, -0.000363748, 0.000550532, 0.00388387, 0.000550532,
			0.00067371, 0.00148477, -0.000363748, 0.000550532, 0.000550532, 0.00388387
		};
		run(Y1, Y2, roll, pitch);
		for (unsigned int i = 0; i < 6; i++)
		{
			for (unsigned int j = 0; j < 6; j++)
				EXPECT_NEAR(P[i][j], expectedP[i][j], 0.00005);
			EXPECT_NEAR(X[i], expectedX[i], 0.00005);
		}
	}
};

TEST(Kalman, countMatrixA)
{
	KalmanTester kalmanTester;
	kalmanTester.countMatrixATest();
}

TEST(Kalman, countPreditedState)
{
	KalmanTester kalmanTester;
	kalmanTester.countPreditedStateTest();
}

TEST(Kalman, countErrorForNewPreditedState)
{
	KalmanTester kalmanTester;
	kalmanTester.countErrorForNewPreditedStateTest();
}

TEST(Kalman, countNewInputData)
{
	KalmanTester kalmanTester;
	kalmanTester.countNewInputDataTest();
}

TEST(Kalman, countInverseCovariance)
{
	KalmanTester kalmanTester;
	kalmanTester.countInverseCovarianceTest();
}

TEST(Kalman, countKalmanGain)
{
	KalmanTester kalmanTester;
	kalmanTester.countKalmanGainTest();
}

TEST(Kalman, countNewState)
{
	KalmanTester kalmanTester;
	kalmanTester.countNewStateTest();
}

TEST(Kalman, countNewErrorForState)
{
	KalmanTester kalmanTester;
	kalmanTester.countNewErrorForStateTest();
}

TEST(Kalman, runAll)
{
	KalmanTester kalmanTester;
	kalmanTester.runAll();
}

}
