#include "DroneVisualisation.h"
#include <ncurses.h>
#include <stdexcept>

DroneVisualisation::DroneVisualisation(){
  system("setterm -cursor off");
  system("clear");
  printf("     Cyrcle nr: xxxxx\n");
  printf("o------------------------------------------------------------------o\n");
  printf("|  4 - xxx         1 - xxx  |           Write to memory            |\n");
  printf("|                           | Addr |Trybe|         Data            |\n");
  printf("|                           | 0x---- | | 0x-- 0x-- 0x-- 0x-- 0x--  |\n");
  printf("| xxxxOxxxx       xxxxOxxxx | 0x---- | | 0x-- 0x-- 0x-- 0x-- 0x--  |\n");
  printf("|      \\             /      | 0x---- | | 0x-- 0x-- 0x-- 0x-- 0x--  |\n");
  printf("|       \\  _______  /       | 0x---- | | 0x-- 0x-- 0x-- 0x-- 0x--  |\n");
  printf("|        \\/       \\/        | 0x---- | | 0x-- 0x-- 0x-- 0x-- 0x--  |\n");
  printf("|        /         \\        | 0x---- | | 0x-- 0x-- 0x-- 0x-- 0x--  |\n");
  printf("|       |           |       | 0x---- | | 0x-- 0x-- 0x-- 0x-- 0x--  |\n");
  printf("|        \\         /        | 0x---- | | 0x-- 0x-- 0x-- 0x-- 0x--  |\n");
  printf("|        /\\_______/\\        | 0x---- | | 0x-- 0x-- 0x-- 0x-- 0x--  |\n");
  printf("|       /           \\       | 0x---- | | 0x-- 0x-- 0x-- 0x-- 0x--  |\n");
  printf("|      /             \\      | 0x---- | | 0x-- 0x-- 0x-- 0x-- 0x--  |\n");
  printf("| xxxxOxxxx       xxxxOxxxx |--------------------------------------|\n");
  printf("|                           |                 LEDS                 |\n");
  printf("|                           |   1 - XXX 2 - XXX 3 - XXX 4 - XXX    |\n");
  printf("|  3 - xxx         2 - xxx  |   5 - XXX 6 - XXX 7 - XXX 8 - XXX    |\n");
  printf("|------------------------------------------------------------------|\n");
  printf("|                     Radio data                     |   Rotation  |\n");
  printf("|        Transmit                      Recive        |          o  |\n");
  printf("| 1. 0b--------  ---  0x--  1. 0b--------  ---  0x-- | Z: xxx.xx   |\n");
  printf("| 2. 0b--------  ---  0x--  2. 0b--------  ---  0x-- |  ^          |\n");
  printf("| 3. 0b--------  ---  0x--  3. 0b--------  ---  0x-- |  |          |\n");
  printf("| 4. 0b--------  ---  0x--  4. 0b--------  ---  0x-- |  |         o|\n");
  printf("| 5. 0b--------  ---  0x--  5. 0b--------  ---  0x-- |  |X: xxx.xx |\n");
  printf("| 6. 0b--------  ---  0x--  6. 0b--------  ---  0x-- |  o--------->|\n");
  printf("| 7. 0b--------  ---  0x--  7. 0b--------  ---  0x-- | /          o|\n");
  printf("| 8. 0b--------  ---  0x--  8. 0b--------  ---  0x-- |v  Y: xxx.xx |\n");
  printf("o------------------------------------------------------------------o\n");
  breakToNextPrint = 0;
}

DroneVisualisation::~DroneVisualisation(){
  system("clear");
  system("setterm -cursor on");
}

void DroneVisualisation::printToCoordinates(unsigned int x, unsigned int y, char element){
  printf("\033[%d;%dH%c\n", x, y, element);
  printf("\033[0;0H");
}

void DroneVisualisation::printToCoordinates(unsigned int x, unsigned int y, const std::string& text){
  printf("\033[%d;%dH%s\n", x, y, text.c_str());
  printf("\033[0;0H");
}

const std::string DroneVisualisation::uintToHex(uint8_t v){
  std::string toReturn("00");
  uint8_t signVal = v / 16;
  if(signVal > 9)
    toReturn[0] = 55;
  toReturn[0] += signVal;
  signVal = v % 16;
  if(signVal > 9)
    toReturn[1] = 55;
  toReturn[1] += signVal;
  return toReturn;
}

const std::string DroneVisualisation::uintToInt(uint8_t v){
  int16_t val = (int16_t)((int8_t)v);
  std::string toReturn(" 000");
  if(val < 0){
    toReturn[0] = '-';
    val = -val;
  }
  toReturn[3] += val % 10;
  val /= 10;
  toReturn[2] += val % 10;
  val /= 10;
  toReturn[1] += val % 10;
  return toReturn;
}

const std::string DroneVisualisation::uintToUint(uint8_t val){
  std::string toReturn("000");
  toReturn[2] += val % 10;
  val /= 10;
  toReturn[1] += val % 10;
  val /= 10;
  toReturn[0] += val % 10;
  return toReturn;
}

const std::string DroneVisualisation::uintToBits(uint8_t bits){
  std::string bitsTab("00000000");
  for(int8_t i = 7; i >= 0; i--){
    if((bits & 1) != 0)
      bitsTab[i] = '1';
    bits = bits >> 1;
  }
  return bitsTab;
}

const std::string DroneVisualisation::doubleToDouble(double val){
  std::string toReturn(" 000.00");
  uint32_t intVal = 0;
  if(val < 0.0){
    toReturn[0] = '-';
    intVal =  val * -100.0;
  }else{
    intVal =  val * 100.0;
  }
  for(unsigned int i = 6; i > 0; i--){
    if(i == 4)
      i--;
    toReturn[i] += intVal % 10;
    intVal /= 10;
  }
  return toReturn;
}

void DroneVisualisation::printRadioTransmision(const std::vector<uint8_t>& data){
  for(unsigned int i = 0; i < 8; i++)
    printToCoordinates(23 + i, 34, 
      uintToBits(data[i]) + "  " +
      uintToUint(data[i]) + "  0x" +
      uintToHex(data[i]));
}

void DroneVisualisation::printCoordinates(const double& roll, const double& pitch, const double& yawl){
  printToCoordinates(23, 59, doubleToDouble(yawl));
  printToCoordinates(27, 61, doubleToDouble(roll));
  printToCoordinates(30, 61, doubleToDouble(pitch));
}

void DroneVisualisation::printLeds(const std::vector<bool>& leds){
  for(unsigned int i = 0; i < leds.size(); i++){
    std::string base("1 - OXX");
    if(leds[i]){
      base[5] = 'N';
      base[6] = ' ';
    }else{
      base[5] = 'F';
      base[6] = 'F';
    }
    base[0] += i;
    printToCoordinates((i < 4)? 18 : 19, 33 + (i % 4) * 8, base);
  }
}

void DroneVisualisation::printCycleNumber(uint32_t val){
  std::string toPrint("00000");
  toPrint[4] += val % 10;
  val /= 10;
  toPrint[3] += val % 10;
  val /= 10;
  toPrint[2] += val % 10;
  val /= 10;
  toPrint[1] += val % 10;
  val /= 10;
  toPrint[0] += val % 10;
  printToCoordinates(0, 17, toPrint);
}

void DroneVisualisation::addToMemory(const std::vector<Drivers::Memory_Simulator::MemoryPair>& data){
  for(const auto& elem : data)
    for(uint16_t i = 0; i < elem.data.size(); i++){
      if(i % 5 == 0)
        memoryToPrint.push({uint16_t(elem.addr + i), {}, elem.reading});
      std::vector<uint8_t>& vec = memoryToPrint.back().data;
      vec.push_back(elem.data[i]);
    }
}

void DroneVisualisation::printMemoryHistory(){
  if(breakToNextPrint > 0){
    breakToNextPrint--;
    return;
  }
  for(uint8_t i = 0; i < 11; i++)
    if(memoryToPrint.empty()){
      printToCoordinates(5 + i, 33, "---- | | 0x-- 0x-- 0x-- 0x-- 0x--");
    }else{
      auto& elem = memoryToPrint.front();
      printToCoordinates(5 + i, 33, uintToHex(elem.addr >> 8) + uintToHex(elem.addr & 0xff));
      printToCoordinates(5 + i, 39, (elem.reading)? 'R' : 'W');
      for(uint16_t j = 0; j < elem.data.size(); j++)
        printToCoordinates(5 + i, 44 + j * 5, uintToHex(elem.data[j]));
      memoryToPrint.pop();
      breakToNextPrint = 100;
    }
}

void DroneVisualisation::printEngines(uint16_t engineRegisters[4]){
  printToCoordinates(3, 24, uintToUint(uint8_t(engineRegisters[0] / 100)));
  printToCoordinates(19, 24, uintToUint(uint8_t(engineRegisters[1] / 100)));
  printToCoordinates(19, 8, uintToUint(uint8_t(engineRegisters[2] / 100)));
  printToCoordinates(3, 8, uintToUint(uint8_t(engineRegisters[3] / 100)));
}