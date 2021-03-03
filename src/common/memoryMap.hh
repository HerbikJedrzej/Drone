#pragma once

enum memoryMap{
	// BEGIN initial test 0x0000
	// END initial test 0x0009

	// BEGIN ADC1 battery 0x000A
		rezistor_1_U_HSB = 0x000A,
		rezistor_1_U_MSB,
		rezistor_1_U_LSB, // (uint) resistance = HSB | MSB | LSB
		rezistor_2_U_HSB,
		rezistor_2_U_MSB,
		rezistor_2_U_LSB, // (uint) resistance = HSB | MSB | LSB
		rezistor_3_U_HSB,
		rezistor_3_U_MSB,
		rezistor_3_U_LSB, // (uint) resistance = HSB | MSB | LSB
	// END ADC3 battery 0x0012

	// BEGIN Angle offset of axises X and Y 0x0029
		angleOffsetX = 0x0029,
		angleOffsetY = 0x002A,
	// END Angle offset of axises X and Y 0x002A

	// BEGIN Trybe of main loop 0x002B
		mainLoopTrybe = 0x002B,
		// value:
		//		0 - casual trybe - flying
		//		1 - tuning trybe - axis X
		//		2 - tuning trybe - axis Y
	// END ADC3 battery 0x002B

	// BEGIN Trybe of main loop 0x002C
		initEnginePower = 0x002C, // engines[%] = (uint8) value / 3
		initEngineCorrectionX, // engines[%] = (int8) value / 6
		initEngineCorrectionY = 0x002E, // engines[%] = (int8) value / 6
	// END ADC3 battery 0x002E

	// BEGIN PID parameters 0x002F
		commonFactorForAxisXYZ = 0x002F,
		pid_P_XY_00,
		pid_I_XY_00,
		pid_D_XY_00,
		pid_P_XY_01,
		pid_I_XY_01,
		pid_D_XY_01,
		pid_P_XY_10,
		pid_I_XY_10,
		pid_D_XY_10,
		pid_P_XY_11,
		pid_I_XY_11,
		pid_D_XY_11,
		pid_P_Z,
		pid_I_Z,
		pid_D_Z,
		pid_P_H,
		pid_I_H,
		pid_D_H = 0x0041,
	// END PID parameters 0x0041

	// BEGIN PID sum 0x0042
		PID_X_sum_msb = 0x0042,
		PID_X_sum_lsb,
		PID_Y_sum_msb,
		PID_Y_sum_lsb,
		PID_Z_sum_msb,
		PID_Z_sum_lsb,
		PID_H_sum_msb,
		PID_H_sum_lsb = 0x0049,
	// END PID sum 0x0049

	// BEGIN measurements for matlab drone model. 0xE49E
	// SLOT 1
		slot1_sinusSignalParam1 = 0x0300,
		slot1_sinusSignalParam2,
		slot1_measuredData1MSB = 0x0302,
		slot1_measuredData1LSB,
		slot1_measuredData500MSB = 0x06E8,
		slot1_measuredData500LSB,
	// SLOT 2
		slot2_sinusSignalParam1 = 0x06EA,
		slot2_sinusSignalParam2,
		slot2_measuredData1MSB = 0x06EC,
		slot2_measuredData1LSB,
		slot2_measuredData500MSB = 0x0AD2,
		slot2_measuredData500LSB,
	// SLOT 3
		slot3_sinusSignalParam1 = 0x0AD4,
		slot3_sinusSignalParam2,
		slot3_measuredData1MSB = 0x0AD6,
		slot3_measuredData1LSB,
		slot3_measuredData500MSB = 0x0EBC,
		slot3_measuredData500LSB,
	// SLOT 4
		slot4_sinusSignalParam1 = 0x0EBE,
		slot4_sinusSignalParam2,
		slot4_measuredData1MSB = 0x0EC0,
		slot4_measuredData1LSB,
		slot4_measuredData1000MSB = 0x168E,
		slot4_measuredData1000LSB,
	// SLOT 5
		slot5_sinusSignalParam1 = 0x1690,
		slot5_sinusSignalParam2,
		slot5_measuredData1MSB = 0x1692,
		slot5_measuredData1LSB,
		slot5_measuredData1000MSB = 0x1E60,
		slot5_measuredData1000LSB,
	// END measurements for matlab drone model. Slot 5. 0xFFFF
};
