#include "processRadioData.hh"
#include <OwnExceptions.hh>

#define M_PI 3.14159265358979323846 // pi

void ProcessRadioData::recordMeasurements(GlobalStruct& globals, DriversGroup& driversGroup){
	uint8_t data[2];
	const uint8_t option = driversGroup.radioParser->getMainOptionValue();
	switch(option){
		case 1:
			driversGroup.memory->read(memoryMap::slot1_sinusSignalParam1, data, 2);
			break;
		case 2:
			driversGroup.memory->read(memoryMap::slot2_sinusSignalParam1, data, 2);
			break;
		case 3:
			driversGroup.memory->read(memoryMap::slot3_sinusSignalParam1, data, 2);
			break;
		case 4:
			driversGroup.memory->read(memoryMap::slot4_sinusSignalParam1, data, 2);
			break;
		case 5:
			driversGroup.memory->read(memoryMap::slot5_sinusSignalParam1, data, 2);
			break;
		default:
			THROW_invalid_argument("There are olny slots [1...5] get measure.");
	}
	globals.measuredSignalParam1 = double(data[0]) / 8 + 0.25;
	globals.measuredSignalParam2 = double(data[1]) * 9.9 / 255 + 0.1;
	globals.measureTime = 0;
	driversGroup.measurementManager->startMeasurement(option - 1);
}

void ProcessRadioData::procesMeasurements(GlobalStruct& globals, DriversGroup& driversGroup){
	globals.measureTime += 0.01;
	driversGroup.measurementManager->increment();
}

void ProcessRadioData::sendMeasuredData(DriversGroup& driversGroup){
	memoryMap begin = memoryMap::slot1_begin;
	memoryMap end = memoryMap::slot1_end;
	const uint8_t option = driversGroup.radioParser->getMainOptionValue();
	uint8_t data;
	char dataUartInfo[8] = {'S', 'l', 'o', 't', ':', ' ', char('0' + option), '\n'};
	char dataUart[3] = {'0', '0', '\n'};
	switch(option){
		case 1:
			begin = memoryMap::slot1_begin;
			end = memoryMap::slot1_end;
			break;
		case 2:
			begin = memoryMap::slot2_begin;
			end = memoryMap::slot2_end;
			break;
		case 3:
			begin = memoryMap::slot3_begin;
			end = memoryMap::slot3_end;
			break;
		case 4:
			begin = memoryMap::slot4_begin;
			end = memoryMap::slot4_end;
			break;
		case 5:
			begin = memoryMap::slot5_begin;
			end = memoryMap::slot5_end;
			break;
		default:
			THROW_invalid_argument("There are olny slots [1...5] to read measure.");
	}
	if(!driversGroup.transmitUART(dataUartInfo, 8))
		THROW_invalid_argument("Sending data finished with failure.");
	for(uint32_t addr = begin; addr <= end; addr++){
		driversGroup.memory->read(addr, &data, 1);
		dataUart[0] = '0' + ((data >> 4) & 0x0f);
		dataUart[1] = '0' + (data & 0x0f);
		if(!driversGroup.transmitUART(dataUart, 3))
			THROW_invalid_argument("Sending data finished with failure.");
	}
}

void ProcessRadioData::setRegisterInMap(DriversGroup& driversGroup, const memoryMap& addr){
	uint8_t data = driversGroup.radioParser->getMainOptionValue();
	driversGroup.memory->write(addr, &data, 1);
}

void ProcessRadioData::applyPIDparams(GlobalStruct& globals, DriversGroup& driversGroup){
		driversGroup.pidX->P = globals.PIDcofactor * globals.pidPaxisXY;
		driversGroup.pidY->P = globals.PIDcofactor * globals.pidPaxisXY;
		driversGroup.pidX->I = globals.PIDcofactor * globals.pidIaxisXY;
		driversGroup.pidY->I = globals.PIDcofactor * globals.pidIaxisXY;
		driversGroup.pidX->D = globals.PIDcofactor * globals.pidDaxisXY;
		driversGroup.pidY->D = globals.PIDcofactor * globals.pidDaxisXY;
		driversGroup.pidZ->P = globals.PIDcofactor * globals.pidPaxisZ;
		driversGroup.pidZ->I = globals.PIDcofactor * globals.pidIaxisZ;
		driversGroup.pidZ->D = globals.PIDcofactor * globals.pidDaxisZ;
		driversGroup.pidH->P = globals.pidPaxisH;
		driversGroup.pidH->I = globals.pidIaxisH;
		driversGroup.pidH->D = globals.pidDaxisH;
}

void ProcessRadioData::processRadioMainData(GlobalStruct& globals, DriversGroup& driversGroup){
	uint8_t tmp = 0;
	if(driversGroup.radioParser->getMainOptionNumber() != 0)
		driversGroup.led.set(LedsList::optionProcess, true, 10);
	switch(driversGroup.radioParser->getMainOptionNumber()){
		case 0:
			return;
		case 1:
			globals.mainLoopTrybeFlag = (uint8_t)driversGroup.radioParser->getMainOptionValue();
			if(globals.mainLoopTrybeFlag >= NUMBERS_OF_TRYBES)
				THROW_out_of_range(MERGE_FORTRYBE_OUT_OF_RANGE);
			driversGroup.memory->writeDMAwithoutDataAlocate(memoryMap::mainLoopTrybe, &globals.mainLoopTrybeFlag, 1, nullptr);
			return;
		case 2:
			recordMeasurements(globals, driversGroup);
			return;
		case 3:
			sendMeasuredData(driversGroup);
			return;
		case 4:
			setRegisterInMap(driversGroup, memoryMap::slot1_sinusSignalParam1);
			return;
		case 5:
			setRegisterInMap(driversGroup, memoryMap::slot1_sinusSignalParam2);
			return;
		case 6:
			setRegisterInMap(driversGroup, memoryMap::slot2_sinusSignalParam1);
			return;
		case 7:
			setRegisterInMap(driversGroup, memoryMap::slot2_sinusSignalParam2);
			return;
		case 8:
			setRegisterInMap(driversGroup, memoryMap::slot3_sinusSignalParam1);
			return;
		case 9:
			setRegisterInMap(driversGroup, memoryMap::slot3_sinusSignalParam2);
			return;
		case 10:
			setRegisterInMap(driversGroup, memoryMap::slot4_sinusSignalParam1);
			return;
		case 11:
			setRegisterInMap(driversGroup, memoryMap::slot4_sinusSignalParam2);
			return;
		case 12:
			setRegisterInMap(driversGroup, memoryMap::slot5_sinusSignalParam1);
			return;
		case 13:
			setRegisterInMap(driversGroup, memoryMap::slot5_sinusSignalParam2);
			return;
		case 14:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.PIDcofactor = double(tmp) * 0.19 / 255 + 0.01; // [0.01 - 0.20]
			driversGroup.memory->write(memoryMap::commonFactorForAxisXYZ, &tmp, 1);		
			applyPIDparams(globals, driversGroup);
			return;
		case 15:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.pidPaxisXY = double(tmp) * 7.0  / 255 + 1.0 ; // [8.0 - 1.0 ]
			driversGroup.memory->write(memoryMap::pid_P_XY_00, &tmp, 1);		
			driversGroup.memory->write(memoryMap::pid_P_XY_11, &tmp, 1);		
			applyPIDparams(globals, driversGroup);
			return;
		case 16:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.pidIaxisXY = double(tmp) * 0.03  / 255 + 0.005 ; // [0.035 - 0.005 ]
			driversGroup.memory->write(memoryMap::pid_I_XY_00, &tmp, 1);		
			driversGroup.memory->write(memoryMap::pid_I_XY_11, &tmp, 1);		
			applyPIDparams(globals, driversGroup);
			return;
		case 17:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.pidDaxisXY = double(tmp) * 180.0 / 255 + 20.0; // [200 - 20]
			driversGroup.memory->write(memoryMap::pid_D_XY_00, &tmp, 1);		
			driversGroup.memory->write(memoryMap::pid_D_XY_11, &tmp, 1);		
			applyPIDparams(globals, driversGroup);
			return;
		case 18:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.pidPaxisXY = double(tmp) * 7.0  / 255 + 1.0 ; // [8.0 - 1.0 ]
			driversGroup.memory->write(memoryMap::pid_P_XY_00, &tmp, 1);		
			driversGroup.memory->write(memoryMap::pid_P_XY_11, &tmp, 1);		
			applyPIDparams(globals, driversGroup);
			return;
		case 19:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.pidIaxisXY = double(tmp) * 0.03  / 255 + 0.005 ; // [0.035 - 0.005 ]
			driversGroup.memory->write(memoryMap::pid_I_XY_00, &tmp, 1);		
			driversGroup.memory->write(memoryMap::pid_I_XY_11, &tmp, 1);		
			applyPIDparams(globals, driversGroup);
			return;
		case 20:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.pidDaxisXY = double(tmp) * 180.0 / 255 + 20.0; // [200 - 20]
			driversGroup.memory->write(memoryMap::pid_D_XY_00, &tmp, 1);		
			driversGroup.memory->write(memoryMap::pid_D_XY_11, &tmp, 1);		
			applyPIDparams(globals, driversGroup);
			return;
		case 21:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.pidPaxisZ = double(tmp) * 7.0 / 255 + 1.0; // [1.0 - 8.0]
			driversGroup.memory->write(memoryMap::pid_P_Z, &tmp, 1);		
			applyPIDparams(globals, driversGroup);
			return;
		case 22:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.pidIaxisZ = double(tmp) * 1.0 / 255; // [1.0 - 0.0]
			driversGroup.memory->write(memoryMap::pid_I_Z, &tmp, 1);		
			applyPIDparams(globals, driversGroup);
			return;
		case 23:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.pidDaxisZ = double(tmp) * 1.0 / 255; // [1.0 - 0.0]
			driversGroup.memory->write(memoryMap::pid_D_Z, &tmp, 1);		
			applyPIDparams(globals, driversGroup);
			return;
		case 24:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.pidPaxisH = double(tmp) * 8.0 / 255 + 4.0; // [12.0 - 4.0]
			driversGroup.memory->write(memoryMap::pid_P_H, &tmp, 1);		
			applyPIDparams(globals, driversGroup);
			return;
		case 25:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.pidIaxisH = double(tmp) * 0.495 / 255 + 0.005; // [0.5 - 0.005]
			driversGroup.memory->write(memoryMap::pid_I_H, &tmp, 1);		
			applyPIDparams(globals, driversGroup);
			return;
		case 26:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.pidDaxisH = double(tmp) * 100.0 / 255 + 50.0; // [150.0 - 50.0]
			driversGroup.memory->write(memoryMap::pid_D_H, &tmp, 1);		
			applyPIDparams(globals, driversGroup);
			return;
		case 27:
			driversGroup.battery->correctResistor(uint8_t(driversGroup.radioParser->getMainOptionValue()));
			return;
		case 28:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.angleOffsetAxisX = double(tmp) * 30.0 / 255 - 15.0; // [-15.0  -  15.0]
			driversGroup.memory->write(memoryMap::angleOffsetX, &tmp, 1);
			return;
		case 29:
			tmp = uint8_t(driversGroup.radioParser->getMainOptionValue());
			globals.angleOffsetAxisY = double(tmp) * 30.0 / 255 - 15.0; // [-15.0  -  15.0]
			driversGroup.memory->write(memoryMap::angleOffsetY, &tmp, 1);
			return;
		case 30:
			globals.percentValueOfEngine = double(uint8_t(driversGroup.radioParser->getMainOptionValue())) / 2.55;
			return;
		case 31:
			recordMeasurements(globals, driversGroup);
			return;
		default:
			THROW_invalid_argument("This value of main option number is not supported.");
	}
}
