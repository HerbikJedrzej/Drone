#pragma once

#include "globalClass.hh"
#include <AT24C.hh>
#include <GPIO.hh>

void mainSetup(Drivers::GPIO* gpio, GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t));
void mainLoop(Drivers::GPIO* gpio, GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t));
