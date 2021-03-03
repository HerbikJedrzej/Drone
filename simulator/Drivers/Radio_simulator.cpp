#include "Radio_simulator.hh"
#include <stdexcept>
#include <fcntl.h> 
#include <sys/stat.h> 
#include <sys/types.h> 
#include <unistd.h>

#include <iostream>
using namespace std::chrono_literals;

namespace Drivers{

uint8_t HexToUint(const char c1, const char c2){
	uint8_t signVal = 0;
	signVal = (c1 - ((c1 >= 'A')? 55 : '0')) * 16;
    signVal |= c2 - ((c2 >= 'A')? 55 : '0');
	return signVal;
}

RadioSimulator::RadioSimulator(const uint8_t _key, unsigned int& _time, std::vector<uint8_t>& _radioData, const std::string& pipeName):
	RadioIfc(_key, _time),
	keyIsCorrect(false),
	radioData(_radioData),
	correctRadioDataOut(new uint8_t [_radioData.size() - 1]),
	correctRadioDataIn(new uint8_t [_radioData.size() - 1]),
	exit(false),
	reader(nullptr){
		for(uint8_t i = 0; i < radioData.size(); i++){
			correctRadioDataOut[i] = 0;
			correctRadioDataIn[i] = 0;
		}
		mkfifo(pipeName.c_str(), 0666);
        open(pipeName.c_str(), O_RDONLY); 
		fifoPipeHandler = open(pipeName.c_str(), O_RDONLY);
		reader.reset(new std::thread(
			[](bool* exit, int* fifoPipeHandler, std::vector<uint8_t>* data)->void{
				char* txt = new char[data->size() + 1];
				while(!(*exit)){
					read(*fifoPipeHandler, txt, data->size() * 3); 
				    std::this_thread::sleep_for(20ms);
					for(unsigned int i = 0; i < data->size(); i++){
						(*data)[i] =  HexToUint(txt[i * 3], txt[i * 3 + 1]);
					}
				}
				delete [] txt;
			},
			&exit, &fifoPipeHandler, &radioData
		));
	}

RadioSimulator::~RadioSimulator(){
	exit = true;
	delete [] correctRadioDataOut;
	delete [] correctRadioDataIn;
	reader->join();
	close(fifoPipeHandler);
}

bool RadioSimulator::isKyeCorrect(){
	return keyIsCorrect;
}

bool RadioSimulator::isAckCorrect(){
	return false;
}

bool RadioSimulator::isComunicationCorrect(){
	return isKyeCorrect();
}

void RadioSimulator::init(){
}

uint8_t RadioSimulator::getTx(unsigned int i) const{
	if(i > radioData.size() - 2)
		throw std::out_of_range("Trying to get vaule out of radio data. i = " + std::to_string(i));
	return correctRadioDataOut[i];
}

uint8_t RadioSimulator::getRx(unsigned int i) const{
	if(i > radioData.size() - 2)
		throw std::out_of_range("Trying to get vaule out of radio data. i = " + std::to_string(i));
	return correctRadioDataIn[i];
}

uint8_t& RadioSimulator::setTx(unsigned int i){
	if(i > radioData.size() - 2)
		throw std::out_of_range("Trying to get vaule out of radio data. i = " + std::to_string(i));
	return correctRadioDataOut[i];
}

uint8_t& RadioSimulator::setRx(unsigned int i){
	if(i > radioData.size() - 2)
		throw std::out_of_range("Trying to get vaule out of radio data. i = " + std::to_string(i));
	return correctRadioDataIn[i];
}

void RadioSimulator::handleTimeEvent(DriverIfc*){
	keyIsCorrect = (key == ((radioData[0] & 0xf0) | (radioData[radioData.size() - 1] & 0x0f)));
	if(!keyIsCorrect)
		return;
	for(uint8_t i = 1; i < radioData.size() - 1; i++)
		correctRadioDataIn[i] = radioData[i];
	correctRadioDataIn[0] = (radioData[0] & 0x0f) | (radioData[radioData.size() - 1] & 0xf0);
}

}