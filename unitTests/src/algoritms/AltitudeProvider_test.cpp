#include <gtest/gtest.h>
#include <vector>
#include "../../src/algoritms/AltitudeProvider.hh"
#include "Matrix.hh"

namespace {

Matrix<2,2> inverse(const Matrix<2,2>& m){
    const double det = m.get(0,0) * m.get(1,1) - m.get(0,1) * m.get(1,0);
    Matrix<2,2> toReturn;
    toReturn(0,0) =  m.get(1,1) / det;
    toReturn(0,1) = -m.get(0,1) / det;
    toReturn(1,0) = -m.get(1,0) / det;
    toReturn(1,1) =  m.get(0,0) / det;
    return std::move(toReturn);
}

class AltitudeAHRS_testHelper{
public:
	AltitudeAHRS_testHelper(const double& barometerVariance, const double& _dt):
		result(0),
		dt(_dt),
		_A{
			{1,dt,dt*dt/2},
			{0,1,dt},
			{0,0,1}
		},
		_H{
			{1,0,0},
			{0,0,1}
		},
		_Q{
			{0,0,0},
			{0,0,0},
			{0,0,1}
		},
		A(_A),
		H(_H),
		Q(_Q),
		I{
			{1,0,0},
			{0,1,0},
			{0,0,1}        
		}{
			R(0,0) = barometerVariance;
			R(0,1) = 0.0;
			R(1,0) = 0.0;
			R(1,1) = 0.0;
		}
    double getAltitude(){
	    return X.get(0,0);
	}
	void countKalmanGain(){
		K = multiplication<3,2,2>(
			multiplication<3,3,2>(P, transformation<3,2>(H)), // to do
			inverse(addition<2,2>(multiplication<2,3,2>(H, multiplication<3,3,2>(P, transformation<3,2>(H))), R))
		);
	}
	void countPredictedX(){
		X = multiplication<3, 3, 1>(A, X);
	}
	void countFiltredX(const double& acceleration, const double& altitude){
		Matrix<2,1> measure;
		measure(0,0) = altitude;
		measure(1,0) = acceleration;
		X = addition<3,1>(X, multiplication<3,2,1>(K, subtraction<2,1>(measure, multiplication<2,3,1>(H, X))));
	}
	void countPredictedP(){
		P = addition<3,3>(multiplication<3,3,3>(A, multiplication<3,3,3>(P, transformation<3,3>(A))), Q);
	}
	void countFiltredP(){
		P = multiplication<3,3,3>(subtraction<3,3>(Matrix<3,3>(I), multiplication<3,2,3>(K,H)), P);
	}
    void run(const double& acceleration, const double& accelerationVariance, const double& altitude){
		R(1,1) = accelerationVariance;
		countKalmanGain();
		countPredictedX();
		countFiltredX(acceleration, altitude);
		countFiltredP();
		countPredictedP();
	}
    double result;
    const double dt;
    const double _A[3][3];
    const double _H[2][3];
    const double _Q[3][3];
    const Matrix<3,3> A;
    const Matrix<2,3> H;
    const Matrix<3,3> Q;
    const double I[3][3];
    Matrix<2,2> R;
    Matrix<3,1> X;
    Matrix<3,3> P;
    Matrix<3,2> K;
};

class AltitudeProvider_test : public AltitudeProvider{
	AltitudeAHRS_testHelper matrixAHRS;
public:
	AltitudeProvider_test(const double& barometerVariance, const double& _dt):
		AltitudeProvider(barometerVariance, _dt),
		matrixAHRS(barometerVariance, _dt){}
	void countKalmanGain_test(){
		P[0][0] = 12.4;
		P[0][1] = 2.6;
		P[0][2] = -25.6;
		P[1][0] = 2.1132;
		P[1][1] = 0.25;
		P[1][2] = 0.923;
		P[2][0] = 1.215;
		P[2][1] = 4.523;
		P[2][2] = -0.92;
		accelerationVariance = 71.1324;
		matrixAHRS.R(0,0) = barometerVariance;
		matrixAHRS.R(0,1) = 0.0;
		matrixAHRS.R(1,0) = 0.0;
		matrixAHRS.R(1,1) = accelerationVariance;
		for(unsigned int i = 0; i < 3; i++)
			for(unsigned int j = 0; j < 3; j++)
				matrixAHRS.P(i,j) = P[i][j];
		countKalmanGain();
		matrixAHRS.countKalmanGain();
		for(unsigned int i = 0; i < 3; i++)
			for(unsigned int j = 0; j < 2; j++)
				EXPECT_DOUBLE_EQ(matrixAHRS.K.get(i,j), K[i][j]) << "j = " << j << " i = " << i << std::endl;
	}
	void countPredictedX_test(){
		X[0] = 2.14;
		X[1] = 2.12;
		X[2] = 2.46;
		for(unsigned int j = 0; j < 3; j++)
			matrixAHRS.X(j,0) = X[j];
		countPredictedX();
		matrixAHRS.countPredictedX();
		for(unsigned int i = 0; i < 3; i++)
			EXPECT_DOUBLE_EQ(matrixAHRS.X.get(i,0), X[i]) << "i = " << i << std::endl;
	}
	void countFiltredX_test(){
		K[0][0] = 12.4;
		K[0][1] = 2.6;
		K[1][0] = 2.1132;
		K[1][1] = 0.25;
		K[2][0] = 1.215;
		K[2][1] = 4.523;
		X[0] = 12.4;
		X[1] = 2.6;
		X[2] = -25.6;
		for(unsigned int i = 0; i < 3; i++){
			matrixAHRS.X(i,0) = X[i];
			for(unsigned int j = 0; j < 2; j++)
				matrixAHRS.K(i,j) = K[i][j];
		}
		countFiltredX(0.124, 0.245);
		matrixAHRS.countFiltredX(0.124, 0.245);
		for(unsigned int j = 0; j < 3; j++)
			EXPECT_DOUBLE_EQ(matrixAHRS.X.get(j,0), X[j]) << "j = " << j << std::endl;
	}
	void countPredictedP_test(){
		P[0][0] = 12.4;
		P[0][1] = 2.6;
		P[0][2] = -25.6;
		P[1][0] = 2.1132;
		P[1][1] = 0.25;
		P[1][2] = 0.923;
		P[2][0] = 1.215;
		P[2][1] = 4.523;
		P[2][2] = -0.92;
		for(unsigned int i = 0; i < 3; i++){
			for(unsigned int j = 0; j < 3; j++)
				matrixAHRS.P(i,j) = P[i][j];
		}
		countPredictedP();
		matrixAHRS.countPredictedP();
		for(unsigned int i = 0; i < 3; i++)
			for(unsigned int j = 0; j < 3; j++)
				EXPECT_DOUBLE_EQ(matrixAHRS.P.get(i,j), P[i][j]) << "i = " << i << "j = " << j << std::endl;
	}
	void countFiltredP_test(){
		P[0][0] = 12.4;
		P[0][1] = 2.6;
		P[0][2] = -25.6;
		P[1][0] = 2.1132;
		P[1][1] = 0.25;
		P[1][2] = 0.923;
		P[2][0] = 1.215;
		P[2][1] = 4.523;
		P[2][2] = -0.92;
		for(unsigned int i = 0; i < 3; i++){
			for(unsigned int j = 0; j < 3; j++)
				matrixAHRS.P(i,j) = P[i][j];
		}
		K[0][0] = 12.4;
		K[0][1] = 2.6;
		K[1][0] = 2.1132;
		K[1][1] = 0.25;
		K[2][0] = 1.215;
		K[2][1] = 4.523;
		for(unsigned int i = 0; i < 3; i++)
			for(unsigned int j = 0; j < 2; j++)
				matrixAHRS.K(i,j) = K[i][j];
		countFiltredP();
		matrixAHRS.countFiltredP();
		for(unsigned int i = 0; i < 3; i++)
			for(unsigned int j = 0; j < 3; j++)
				EXPECT_DOUBLE_EQ(matrixAHRS.P.get(i,j), P[i][j]) << "i = " << i << "j = " << j << std::endl;
	}
    void run_test(){
		std::vector<std::vector<double>> sampleData({
			{12.41, 21.51, 2.51},
			{-2.41, 0.12, -0.41},
			{24.15, 2.41, 0.24},
			{0.221, 0.124, -0.214},
			{24.024, 0.23, 0.11},
			{21.42, 24.124, 12.12},
			{4.24, 25.124, 124.24},
		});
		for(auto& packet : sampleData){
			matrixAHRS.run(packet[0], packet[1], packet[2]);
			run(packet[0], packet[1], packet[2]);
			EXPECT_NEAR(matrixAHRS.X.get(0, 0), X[0], 0.00001);
			EXPECT_NEAR(matrixAHRS.X.get(1, 0), X[1], 0.00001);
			EXPECT_NEAR(matrixAHRS.X.get(2, 0), X[2], 0.00001);
		}

	}
};

TEST(AltitudeProvider_test, countKalmanGain){
	AltitudeProvider_test tester(0.03, 0.01);
	tester.countKalmanGain_test();
}

TEST(AltitudeProvider_test, countPredictedX){
	AltitudeProvider_test tester(0.03, 0.01);
	tester.countPredictedX_test();
}

TEST(AltitudeProvider_test, countFiltredX){
	AltitudeProvider_test tester(0.03, 0.01);
	tester.countFiltredX_test();
}

TEST(AltitudeProvider_test, countPredictedP){
	AltitudeProvider_test tester(0.03, 0.01);
	tester.countPredictedP_test();
}

TEST(AltitudeProvider_test, countFiltredP){
	AltitudeProvider_test tester(0.03, 0.01);
	tester.countFiltredP_test();
}

TEST(AltitudeProvider_test, run){
	AltitudeProvider_test tester(0.03, 0.01);
	tester.run_test();
}

}