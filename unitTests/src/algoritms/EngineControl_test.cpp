#include <gtest/gtest.h>
#include "../../src/algoritms/QuatroEngineControl.hh"
#include "../../src/algoritms/OctoEngineControl.hh"
#include <EngineIfc.hh>
#include <cstdlib>

class EnginesFotTest : public EngineIfc{
	uint16_t* values;
	static constexpr uint16_t max = {2500};
public:
	EnginesFotTest(const uint8_t size):
		EngineIfc(size, values),
		values(new uint16_t[size]){}

	~EnginesFotTest(){
		delete [] values;
	}
    void init() override{
		for(uint8_t i = 0; i < sizeOfList; i++)
			values[i] = 0;
	}
    void set(uint8_t engineId, const double powerPercent) override{
		if(engineId >= sizeOfList)
			throw std::out_of_range("Trying to get argument out of values.");
		values[engineId] = (powerPercent * max) / 100;
	}
	uint16_t& operator[](const uint8_t i){
		if(i < sizeOfList)
			return values[i];
		throw std::out_of_range("Trying to get argument out of values.");
	}
};

TEST(EngineControlIfc, setAll){
	EnginesFotTest engines(4);
	QuatroEngineControl controlerInstance(&engines);
	EngineControlIfc* controler = &controlerInstance;
	engines.init();
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
	controler->setAll(100);
	EXPECT_EQ(engines[0], 2500);
	EXPECT_EQ(engines[1], 2500);
	EXPECT_EQ(engines[2], 2500);
	EXPECT_EQ(engines[3], 2500);
	controler->setAll(50);
	EXPECT_EQ(engines[0], 1250);
	EXPECT_EQ(engines[1], 1250);
	EXPECT_EQ(engines[2], 1250);
	EXPECT_EQ(engines[3], 1250);
	controler->setAll(25);
	EXPECT_EQ(engines[0], 625);
	EXPECT_EQ(engines[1], 625);
	EXPECT_EQ(engines[2], 625);
	EXPECT_EQ(engines[3], 625);
	controler->setAll(0);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
}

TEST(EngineControlIfc, incrementEngine){
	EnginesFotTest engines(4);
	QuatroEngineControl controlerInstance(&engines);
	EngineControlIfc* controler = &controlerInstance;
	engines.init();
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
	controler->incrementEngine(50);
	EXPECT_EQ(engines[0], 1250);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
	controler->incrementEngine(100);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 2500);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
	controler->incrementEngine(10);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 250);
	EXPECT_EQ(engines[3], 0);
	controler->incrementEngine(1);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 25);
	controler->incrementEngine(50);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
	controler->incrementEngine(5);
	EXPECT_EQ(engines[0], 125);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
}

TEST(QuatroEngineControl, tooLessEngines){
	EnginesFotTest engines(3);
	try{
		QuatroEngineControl controler(&engines);
	}catch(std::invalid_argument& e){
		EXPECT_STREQ(e.what(), "QuatroEngineControl needs exacly 4 engines.");
	}catch(...){
		EXPECT_TRUE(false);
	}
}

TEST(QuatroEngineControl, tooMuchEngines){
	EnginesFotTest engines(5);
	try{
		QuatroEngineControl controler(&engines);
	}catch(std::invalid_argument& e){
		EXPECT_STREQ(e.what(), "QuatroEngineControl needs exacly 4 engines.");
	}catch(...){
		EXPECT_TRUE(false);
	}
}

TEST(QuatroEngineControl, setX){
	EnginesFotTest engines(4);
	QuatroEngineControl controlerInstance(&engines);
	EngineControlIfc* controler = &controlerInstance;
	PID pid;
	pid.P = 1;
	pid.I = 0;
	pid.D = 0;
	engines.init();
	pid.setY(0);
	pid.setR(2.0);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
	controler->setAxisX(50.0, &pid);
	EXPECT_EQ(engines[0], 1275);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 1225);
	EXPECT_EQ(engines[3], 0);
	pid.setY(-2.0);
	pid.setR(8.0);
	controler->setAxisX(50.0, &pid);
	EXPECT_EQ(engines[0], 1375);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 1125);
	EXPECT_EQ(engines[3], 0);
}

TEST(QuatroEngineControl, setY){
	EnginesFotTest engines(4);
	QuatroEngineControl controlerInstance(&engines);
	EngineControlIfc* controler = &controlerInstance;
	PID pid;
	pid.P = 1;
	pid.I = 0;
	pid.D = 0;
	engines.init();
	pid.setY(0);
	pid.setR(-2.0);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
	controler->setAxisY(50.0, &pid);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 1275);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 1225);
	pid.setY(2.0);
	pid.setR(-8.0);
	controler->setAxisY(50.0, &pid);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 1375);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 1125);
}

TEST(QuatroEngineControl, setZ){
	EnginesFotTest engines(4);
	QuatroEngineControl controlerInstance(&engines);
	EngineControlIfc* controler = &controlerInstance;
	PID pid;
	pid.P = 1;
	pid.I = 0;
	pid.D = 0;
	engines.init();
	pid.setY(0);
	pid.setR(-1.0);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
	controler->setAxisZ(50.0, &pid);
	EXPECT_EQ(engines[0], 1225);
	EXPECT_EQ(engines[1], 1275);
	EXPECT_EQ(engines[2], 1225);
	EXPECT_EQ(engines[3], 1275);
	pid.setY(1.0);
	pid.setR(-4.0);
	controler->setAxisZ(50.0, &pid);
	EXPECT_EQ(engines[0], 1125);
	EXPECT_EQ(engines[1], 1375);
	EXPECT_EQ(engines[2], 1125);
	EXPECT_EQ(engines[3], 1375);
}

TEST(QuatroEngineControl, set){
	EnginesFotTest engines(4);
	QuatroEngineControl controlerInstance(&engines);
	EngineControlIfc* controler = &controlerInstance;
	PID pidX;
	PID pidY;
	PID pidZ;
	pidX.P = 1;
	pidX.I = 0;
	pidX.D = 0;
	pidY.P = 1;
	pidY.I = 0;
	pidY.D = 0;
	pidZ.P = 1;
	pidZ.I = 0;
	pidZ.D = 0;
	engines.init();
	pidX.setY(0);
	pidX.setR(4.0);
	pidY.setY(0);
	pidY.setR(6.0);
	pidZ.setY(0);
	pidZ.setR(-1.0);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
	controler->set(40.0, &pidX, &pidY, &pidZ);
	EXPECT_EQ(engines[0], 1000 + 50 - 25);
	EXPECT_EQ(engines[1], 1000 - 75 + 25);
	EXPECT_EQ(engines[2], 1000 - 50 - 25);
	EXPECT_EQ(engines[3], 1000 + 75 + 25);
	pidX.setY(-2.0);
	pidX.setR(4.0);
	pidY.setY(-2.0);
	pidY.setR(6.0);
	pidZ.setY(4.0);
	pidZ.setR(-1.0);
	controler->set(40.0, &pidX, &pidY, &pidZ);
	EXPECT_EQ(engines[0], 1000 +  75 - 125);
	EXPECT_EQ(engines[1], 1000 - 100 + 125);
	EXPECT_EQ(engines[2], 1000 -  75 - 125);
	EXPECT_EQ(engines[3], 1000 + 100 + 125);
}

TEST(OctoEngineControl, tooLessEngines){
	EnginesFotTest engines(4);
	try{
		OctoEngineControl controler(&engines);
	}catch(std::invalid_argument& e){
		EXPECT_STREQ(e.what(), "OctoEngineControl needs exacly 8 engines.");
	}catch(...){
		EXPECT_TRUE(false);
	}
}

TEST(OctoEngineControl, tooMuchEngines){
	EnginesFotTest engines(9);
	try{
		OctoEngineControl controler(&engines);
	}catch(std::invalid_argument& e){
		EXPECT_STREQ(e.what(), "OctoEngineControl needs exacly 8 engines.");
	}catch(...){
		EXPECT_TRUE(false);
	}
}

TEST(OctoEngineControl, setX){
	EnginesFotTest engines(8);
	OctoEngineControl controlerInstance(&engines);
	EngineControlIfc* controler = &controlerInstance;
	PID pid;
	pid.P = 1;
	pid.I = 0;
	pid.D = 0;
	engines.init();
	pid.setY(0);
	pid.setR(2.0);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
	EXPECT_EQ(engines[4], 0);
	EXPECT_EQ(engines[5], 0);
	EXPECT_EQ(engines[6], 0);
	EXPECT_EQ(engines[7], 0);
	controler->setAxisX(50.0, &pid);
	EXPECT_EQ(engines[0], 1262);
	EXPECT_EQ(engines[1], 1256);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 1243);
	EXPECT_EQ(engines[4], 1237);
	EXPECT_EQ(engines[5], 1243);
	EXPECT_EQ(engines[6], 0);
	EXPECT_EQ(engines[7], 1256);
	pid.setY(-2.0);
	pid.setR(8.0);
	controler->setAxisX(50.0, &pid);
	EXPECT_EQ(engines[0], 1312);
	EXPECT_EQ(engines[1], 1281);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 1218);
	EXPECT_EQ(engines[4], 1187);
	EXPECT_EQ(engines[5], 1218);
	EXPECT_EQ(engines[6], 0);
	EXPECT_EQ(engines[7], 1281);
}

TEST(OctoEngineControl, setY){
	EnginesFotTest engines(8);
	OctoEngineControl controlerInstance(&engines);
	EngineControlIfc* controler = &controlerInstance;
	PID pid;
	pid.P = 1;
	pid.I = 0;
	pid.D = 0;
	engines.init();
	pid.setY(0);
	pid.setR(-2.0);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
	EXPECT_EQ(engines[4], 0);
	EXPECT_EQ(engines[5], 0);
	EXPECT_EQ(engines[6], 0);
	EXPECT_EQ(engines[7], 0);
	controler->setAxisY(50.0, &pid);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 1256);
	EXPECT_EQ(engines[2], 1262);
	EXPECT_EQ(engines[3], 1256);
	EXPECT_EQ(engines[4], 0);
	EXPECT_EQ(engines[5], 1243);
	EXPECT_EQ(engines[6], 1237);
	EXPECT_EQ(engines[7], 1243);
	pid.setY(2.0);
	pid.setR(-8.0);
	controler->setAxisY(50.0, &pid);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 1281);
	EXPECT_EQ(engines[2], 1312);
	EXPECT_EQ(engines[3], 1281);
	EXPECT_EQ(engines[4], 0);
	EXPECT_EQ(engines[5], 1218);
	EXPECT_EQ(engines[6], 1187);
	EXPECT_EQ(engines[7], 1218);
}

TEST(OctoEngineControl, setZ){
	EnginesFotTest engines(8);
	OctoEngineControl controlerInstance(&engines);
	EngineControlIfc* controler = &controlerInstance;
	PID pid;
	pid.P = 1;
	pid.I = 0;
	pid.D = 0;
	engines.init();
	pid.setY(0);
	pid.setR(-1.0);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
	EXPECT_EQ(engines[4], 0);
	EXPECT_EQ(engines[5], 0);
	EXPECT_EQ(engines[6], 0);
	EXPECT_EQ(engines[7], 0);
	controler->setAxisZ(50.0, &pid);
	EXPECT_EQ(engines[0], 1237);
	EXPECT_EQ(engines[1], 1262);
	EXPECT_EQ(engines[2], 1237);
	EXPECT_EQ(engines[3], 1262);
	EXPECT_EQ(engines[4], 1237);
	EXPECT_EQ(engines[5], 1262);
	EXPECT_EQ(engines[6], 1237);
	EXPECT_EQ(engines[7], 1262);
	pid.setY(1.0);
	pid.setR(-4.0);
	controler->setAxisZ(50.0, &pid);
	EXPECT_EQ(engines[0], 1187);
	EXPECT_EQ(engines[1], 1312);
	EXPECT_EQ(engines[2], 1187);
	EXPECT_EQ(engines[3], 1312);
	EXPECT_EQ(engines[4], 1187);
	EXPECT_EQ(engines[5], 1312);
	EXPECT_EQ(engines[6], 1187);
	EXPECT_EQ(engines[7], 1312);
}

TEST(OctoEngineControl, set){
	EnginesFotTest engines(8);
	OctoEngineControl controlerInstance(&engines);
	EngineControlIfc* controler = &controlerInstance;
	PID pidX;
	PID pidY;
	PID pidZ;
	pidX.P = 1;
	pidX.I = 0;
	pidX.D = 0;
	pidY.P = 1;
	pidY.I = 0;
	pidY.D = 0;
	pidZ.P = 1;
	pidZ.I = 0;
	pidZ.D = 0;
	engines.init();
	pidX.setY(0);
	pidX.setR(4.0);
	pidY.setY(0);
	pidY.setR(6.0);
	pidZ.setY(0);
	pidZ.setR(-1.0);
	EXPECT_EQ(engines[0], 0);
	EXPECT_EQ(engines[1], 0);
	EXPECT_EQ(engines[2], 0);
	EXPECT_EQ(engines[3], 0);
	EXPECT_EQ(engines[4], 0);
	EXPECT_EQ(engines[5], 0);
	EXPECT_EQ(engines[6], 0);
	EXPECT_EQ(engines[7], 0);
	controler->set(40.0, &pidX, &pidY, &pidZ);
	EXPECT_EQ(engines[0], 1000 + 25      - 25);
	EXPECT_EQ(engines[1], 1000 + 12 - 19 + 25);
	EXPECT_EQ(engines[2], 1000 -      38 - 25);
	EXPECT_EQ(engines[3], 1000 - 13 - 19 + 25);
	EXPECT_EQ(engines[4], 1000 - 25      - 25);
	EXPECT_EQ(engines[5], 1000 - 13 + 19 + 25);
	EXPECT_EQ(engines[6], 1000 +      37 - 25);
	EXPECT_EQ(engines[7], 1000 + 12 + 19 + 25);
	pidX.setY(-2.0);
	pidX.setR(4.0);
	pidY.setY(-2.0);
	pidY.setR(6.0);
	pidZ.setY(4.0);
	pidZ.setR(-1.0);
	controler->set(40.0, &pidX, &pidY, &pidZ);
	EXPECT_EQ(engines[0], 1000 + 37      - 125);
	EXPECT_EQ(engines[1], 1000 + 18 - 25 + 125);
	EXPECT_EQ(engines[2], 1000 -      50 - 125);
	EXPECT_EQ(engines[3], 1000 - 19 - 25 + 125);
	EXPECT_EQ(engines[4], 1000 - 38      - 125);
	EXPECT_EQ(engines[5], 1000 - 19 + 25 + 125);
	EXPECT_EQ(engines[6], 1000 +      50 - 125);
	EXPECT_EQ(engines[7], 1000 + 18 + 25 + 125);
}
