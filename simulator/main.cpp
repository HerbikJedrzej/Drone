#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#include "GPIOconf.hh"
#include "DroneVisualisation.h"
#include "KeyboardCharProvider.h"
#include "KeyboardInterpreter.h"
#include "Drivers/Radio_simulator.hh"
#include "Drivers/Memory_simulator.hh"
#include <RadioParser.hh>
#include <QuatroEngineControl.hh>
#include <memoryMap.hh>
#include <MiniDronEngine.hh>
#include "Drivers/IMUsensor_simulator.hh"
#include <AHRS.hh>
#include <globalClass.hh>
#include <PID.hh>
#include <AltitudeProvider.hh>
#include <BatteryObserver.hh>
#include <secondMain.hh>

using namespace std::chrono_literals;

namespace Drivers{

int readedChar = 0;
uint32_t timerCounts = 0;

void timerMainLoop(bool* exit, bool* mainLoop){
  KeyGetter key;
  while(!(*exit)){
    readedChar = key.getChar();
    if(readedChar == ']')
      *exit = true;
    std::this_thread::sleep_for(20ms);
    *mainLoop = true;
    timerCounts++;
  }
}

}

namespace {
  void HAL_Delay(uint32_t){}
  std::vector<bool> leds(LED_LAST_LED, true);
}

int main(){
  bool exit = false;
  bool doCircle = true;
  Drivers::GPIO gpio(
    [](OutputList i, bool s)->void{
      if(i == OutputList::none)
        return;
      if(uint8_t(i) >= leds.size())
        throw std::out_of_range("Trying to reach led out of list. i = " + std::to_string(i));
      leds[i] = s;
    },
    [](OutputList i)->bool{
      if(i == OutputList::none)
        return false;
      if(uint8_t(i) >= leds.size())
        throw std::out_of_range("Trying to reach led out of list. i = " + std::to_string(i));
      return leds[i];
        return false;
    },
    [](InputList)->bool{
        return false;
    },
    HAL_Delay
  );
  uint16_t adcDataForBattery[2] = {1000, 1000};
  DroneVisualisation visual;
  std::thread timer(Drivers::timerMainLoop, &exit, &doCircle);
  Drivers::Memory_Simulator memory("DroneMemoryRegisters.json");
  std::vector<uint8_t> radioData = {0xde, 255, 12, 51, 23, 0x34, 12, 0x55};
  Drivers::RadioSimulator radio(0x46, Drivers::timerCounts, radioData, "/tmp/radioStream");
  Drivers::RadioParser radioParser(&radio, Drivers::RadioTrybe::Drone);
  KeyboardInterpreter interpreter(visual, &radioParser);
  Drivers::LED led(gpio, HAL_Delay);
  gpio.pin(OutputList::LED_R, false);
  gpio.pin(OutputList::LED_G1, false);
  gpio.pin(OutputList::LED_G2, false);
  gpio.pin(OutputList::LED_Y1, false);
  gpio.pin(OutputList::LED_Y2, false);
  gpio.pin(OutputList::LED_B, false);
  led.addLED(LedsList::batteryOut, OutputList::LED_Y1, true);
  led.addLED(LedsList::initFailure, OutputList::LED_R, true);
  led.addLED(LedsList::optionProcess, OutputList::LED_B, true);
  led.addLED(LedsList::angleGreaterThan15deg, OutputList::LED_Y2, true);
  uint16_t enginesPower[4] = {0, 0, 0, 0};
  double batteryRezistors[3] = {10000, 22000};
  BatteryObserver battery(&memory, adcDataForBattery, batteryRezistors, 2, 0xfff, 3.0, 3.7);
  bool akcelerometerAndGyroskopeDataReadyFlag;
  bool magnetometerDataReadyFlag;
  Drivers::IMU_Symulator imu(akcelerometerAndGyroskopeDataReadyFlag, magnetometerDataReadyFlag);
  PID pidX;
  PID pidY;
  PID pidZ;
  PID pidH;
  AHRS ahrs(imu.akcelerometr, imu.gyroskope, nullptr);
  AltitudeProvider altitudeProvider(0.1);
  MiniDronEngine engines(enginesPower, 4);
  QuatroEngineControl engineControl(&engines);
  srand( time( NULL ) );
  GlobalStruct globalStruct = {30.0};
  DriversGroup driversGroup{
    led,
    &radio,
    &radioParser,
    &imu,
    &memory,
    &engineControl,
    &ahrs,
    &pidX,
    &pidY,
    &pidZ,
    &pidH,
    nullptr,
    &altitudeProvider,
    &battery,
    [](const char*, unsigned int){
      return true;
    }
  };
  mainSetup(&gpio, globalStruct, driversGroup, HAL_Delay);
  while(!exit){
	  if(doCircle){
		  doCircle = false;
      if(Drivers::timerCounts)
        leds[OutputList::LED_G1] = !leds[OutputList::LED_G1];
      leds[OutputList::LED_G2] = radio.isKyeCorrect();
      led.handleTimeEvent(nullptr);
      radio.handleTimeEvent(nullptr);
      radioParser.run();
      altitudeProvider.run(0.0, 0.0, 0.0);
      mainLoop(&gpio, globalStruct, driversGroup, HAL_Delay);
      visual.printEngines(enginesPower);
      visual.printCycleNumber(Drivers::timerCounts);
      visual.printRadioTransmision(radioData);
      // visual.printCoordinates(1124.51, 24.1, -23.15);
      visual.printLeds(leds);
      visual.addToMemory(memory.getMemoryChanges());
      visual.printMemoryHistory();
    }
  }
  memory.saveFile("DroneMemoryRegisters.json");
  timer.join();
  return 0;
}
