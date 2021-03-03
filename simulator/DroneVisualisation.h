#pragma once
#include <string>
#include <vector>
#include <queue>
#include "Drivers/Memory_simulator.hh"

class DroneVisualisation{
protected:
  void printToCoordinates(unsigned int x, unsigned int y, char element);
  void printToCoordinates(unsigned int x, unsigned int y, const std::string& text);
  const std::string uintToHex(uint8_t);
  const std::string uintToInt(uint8_t);
  const std::string uintToBits(uint8_t);
  const std::string uintToUint(uint8_t);
  const std::string doubleToDouble(double);
  std::queue<Drivers::Memory_Simulator::MemoryPair> memoryToPrint;
  uint8_t breakToNextPrint;
public:
  DroneVisualisation();
  ~DroneVisualisation();
  void addToMemory(const std::vector<Drivers::Memory_Simulator::MemoryPair>& data);
  void printRadioTransmision(const std::vector<uint8_t>& data);
  void printMemoryHistory();
  void printCycleNumber(uint32_t data);
  void printCoordinates(const double& roll, const double& pitch, const double& yawl);
  void printEngines(uint16_t engineRegisters[4]);
  void printLeds(const std::vector<bool>& leds);
};