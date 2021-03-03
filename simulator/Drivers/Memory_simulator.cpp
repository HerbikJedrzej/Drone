#include "Memory_simulator.hh"
#include <fstream>
#include <stdexcept>
#include <iostream>

namespace Drivers{

Memory_Simulator::Memory_Simulator(const std::string& memoryFileName):
	Memory(nullptr, nullptr, 0xA0, OutputList::MemoryWriteProtect, [](uint32_t)->void{}){
		std::ifstream file;
		file.open(memoryFileName, std::fstream::app);
		if(!file.is_open())
			throw std::invalid_argument("Memory file '" + memoryFileName + "' cannot be loaded.");
		for(std::string line; std::getline(file, line); ){
			uint16_t reg = std::stoi(line);
			uint8_t val = std::stoi(
				line.substr(
					line.find(std::to_string(reg)) + std::to_string(reg).size()
					)
				);
			registers[reg] = val;
		}
		file.close();
	}

Memory_Simulator::~Memory_Simulator(){
}

void Memory_Simulator::saveFile(const std::string& memoryFileName){
	std::ofstream file;
	file.open(memoryFileName);
	if(!file.is_open())
		throw std::invalid_argument("Memory file '" + memoryFileName + "' cannot be saved.");
	for(const auto& elem : registers)
		file << elem.first << " " << uint16_t(elem.second) << std::endl;
	file.close();
}

bool Memory_Simulator::write(uint16_t addr, uint8_t* data, uint16_t dataSize){
	std::vector<uint8_t> elem;
	for(uint16_t i = 0; i < dataSize; i++){
		registers[addr + i] = data[i];
		elem.push_back(data[i]);
	}
	history.push_back({uint16_t(addr), std::move(elem), false});
	return true;
}

bool Memory_Simulator::read(uint16_t addr, uint8_t* data, uint16_t dataSize){
	std::vector<uint8_t> elem;
	for(uint16_t i = 0; i < dataSize; i++){
		try{
			data[i] = registers.at(addr + i);
		}catch(...){
			registers[addr + i] = 255;
			data[i] = 255;
		}
		elem.push_back(data[i]);
	}
	history.push_back({uint16_t(addr), std::move(elem), true});
	return true;
}

void Memory_Simulator::writeDMA(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish){
	write(addr, data, dataSize);
	if(finish)
		*finish = true;
}

void Memory_Simulator::writeDMAwithoutDataAlocate(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish){
	write(addr, data, dataSize);
	if(finish)
		*finish = true;
}

bool Memory_Simulator::readDMA(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish){
	read(addr, data, dataSize);
	if(finish)
		*finish = true;
	return true;
}

bool Memory_Simulator::init(const RegPair* initTable, uint16_t size){
	uint8_t data[10]      = {0xac, 0x53, 0x35, 0xba, 0xc8, 0xc2, 0xaa, 0x26, 0xff, 0x00};
	uint8_t dataClear[10] = {0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff};
	uint8_t dataCheck[10] = {0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0x00};
	while(!read(0x00, dataCheck, 10));
	const bool firstUseOfMemory = (
		dataCheck[0] != 0x00 ||
		dataCheck[1] != 0xff ||
		dataCheck[2] != 0x00 ||
		dataCheck[3] != 0xff ||
		dataCheck[4] != 0x00 ||
		dataCheck[5] != 0xff ||
		dataCheck[6] != 0x00 ||
		dataCheck[7] != 0xff ||
		dataCheck[8] != 0x00 ||
		dataCheck[9] != 0xff
	);
	while(!write(0x00, data, 10));
	delay(2);
	while(!read(0x00, dataCheck, 10));
	const bool first = (
		dataCheck[0] == 0xac &&
		dataCheck[1] == 0x53 &&
		dataCheck[2] == 0x35 &&
		dataCheck[3] == 0xba &&
		dataCheck[4] == 0xc8 &&
		dataCheck[5] == 0xc2 &&
		dataCheck[6] == 0xaa &&
		dataCheck[7] == 0x26 &&
		dataCheck[8] == 0xff &&
		dataCheck[9] == 0x00
	);
	while(!write(0x00, dataClear, 10));
	delay(2);
	while(!read(0x00, dataCheck, 10));
	const bool second = (
		dataCheck[0] == 0x00 &&
		dataCheck[1] == 0xff &&
		dataCheck[2] == 0x00 &&
		dataCheck[3] == 0xff &&
		dataCheck[4] == 0x00 &&
		dataCheck[5] == 0xff &&
		dataCheck[6] == 0x00 &&
		dataCheck[7] == 0xff &&
		dataCheck[8] == 0x00 &&
		dataCheck[9] == 0xff
	);
	if(firstUseOfMemory && first && second){
		uint8_t val;
		for(uint16_t i = 0; i < size; i++){
			val = initTable[i].value;
			if(!write(initTable[i].addr, &val, 1))
				return false;
		}
		return true;
	}
	return (first && second);
}

void Memory_Simulator::lockMemory(){
}

void Memory_Simulator::unlockMemory(){
}

const std::vector<Drivers::Memory_Simulator::MemoryPair> Memory_Simulator::getMemoryChanges(){
	auto toReturn = std::move(history);
	return toReturn;
}

}