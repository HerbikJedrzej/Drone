#include <gtest/gtest.h>
#include "Matrix.hh"

TEST(Martix_test, multiplication)
{
	Matrix<3,2> m1;
	Matrix<2,4> m2;
	m1(0,0) = 2.42;
	m1(0,1) = -0.24;
	m1(1,0) = 2.521;
	m1(1,1) = -24;
	m1(2,0) = 12.41;
	m1(2,1) = 1.12;
	m2(0,0) = 23.2;
	m2(0,1) = 1.12;
	m2(0,2) = 21.12;
	m2(0,3) = 0.12;
	m2(1,0) = -24;
	m2(1,1) = 15;
	m2(1,2) = 1.62;
	m2(1,3) = 24.12;
	Matrix<3,4> result = multiplication<3, 2, 4>(m1, m2);
	EXPECT_NEAR(result(0,0), 7738.0/125, 0.0001);
	EXPECT_NEAR(result(0,1), -556.0/625, 0.0001);
	EXPECT_NEAR(result(0,2), 31701.0/625, 0.0001);
	EXPECT_NEAR(result(0,3), -6873.0/1250, 0.0001);
	EXPECT_NEAR(result(1,0), 793109.0/1250, 0.0001);
	EXPECT_NEAR(result(1,1), -2232353.0/6250, 0.0001);
	EXPECT_NEAR(result(1,2), 44886.0/3125, 0.0001);
	EXPECT_NEAR(result(1,3), -14464437.0/25000, 0.0001);
	EXPECT_NEAR(result(2,0), 32629.0/125, 0.0001);
	EXPECT_NEAR(result(2,1), 19187.0/625, 0.0001);
	EXPECT_NEAR(result(2,2), 164946.0/625, 0.0001);
	EXPECT_NEAR(result(2,3), 71259.0/2500, 0.0001);
}

TEST(Martix_test, subtraction)
{
	Matrix<3,2> m1;
	Matrix<3,2> m2;
	m1(0,0) = 2.42;
	m1(0,1) = -0.24;
	m1(1,0) = 2.521;
	m1(1,1) = -24;
	m1(2,0) = 12.41;
	m1(2,1) = 1.12;
	m2(0,0) = 23;
	m2(0,1) = 1.12;
	m2(1,0) = -2.2;
	m2(1,1) = -0.12;
	m2(2,0) = 2.2;
	m2(2,1) = 0.25;
	Matrix<3,2> result = subtraction<3, 2>(m1, m2);
	EXPECT_NEAR(result(0,0), -1029.0/50, 0.0001);
	EXPECT_NEAR(result(0,1), -34.0/25, 0.0001);
	EXPECT_NEAR(result(1,0), 4721.0/1000, 0.0001);
	EXPECT_NEAR(result(1,1), -597.0/25, 0.0001);
	EXPECT_NEAR(result(2,0), 1021.0/100, 0.0001);
	EXPECT_NEAR(result(2,1), 87.0/100, 0.0001);
}

TEST(Martix_test, addition)
{
	Matrix<3,2> m1;
	Matrix<3,2> m2;
	m1(0,0) = 2.42;
	m1(0,1) = -0.24;
	m1(1,0) = 2.521;
	m1(1,1) = -24;
	m1(2,0) = 12.41;
	m1(2,1) = 1.12;
	m2(0,0) = 23;
	m2(0,1) = 1.12;
	m2(1,0) = -2.2;
	m2(1,1) = -0.12;
	m2(2,0) = 2.2;
	m2(2,1) = 0.25;
	Matrix<3,2> result = addition<3, 2>(m1, m2);
	EXPECT_NEAR(result(0,0), 1271.0/50, 0.0001);
	EXPECT_NEAR(result(0,1), 22.0/25, 0.0001);
	EXPECT_NEAR(result(1,0), 321.0/1000, 0.0001);
	EXPECT_NEAR(result(1,1), -603.0/25, 0.0001);
	EXPECT_NEAR(result(2,0), 1461.0/100, 0.0001);
	EXPECT_NEAR(result(2,1), 137.0/100, 0.0001);
}
