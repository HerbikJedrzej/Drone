#pragma once

#include <AT24C.hh>
#include <map>
#include <string>
#include <vector>

namespace Drivers{

class Memory_Simulator : public Memory
{
	std::map<uint16_t, uint8_t> registers;
public:
	struct MemoryPair{
		uint16_t addr;
		std::vector<uint8_t> data;
		bool reading;
	};
	Memory_Simulator(const std::string& _memoryFileName);
	~Memory_Simulator();
	bool write(uint16_t addr, uint8_t* data, uint16_t dataSize) final;
	bool read(uint16_t addr, uint8_t* data, uint16_t dataSize) final;
	void writeDMA(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish) final;
	void writeDMAwithoutDataAlocate(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish) final;
	bool readDMA(uint16_t addr, uint8_t* data, uint16_t dataSize, bool* finish) final;
	bool init(const RegPair* initTable = nullptr, uint16_t size = 0) final;
	void lockMemory() final;
	void unlockMemory() final;
	void saveFile(const std::string&);
	const std::vector<Drivers::Memory_Simulator::MemoryPair> getMemoryChanges();
private:
	std::vector<Drivers::Memory_Simulator::MemoryPair> history;
};

}
