#pragma once

#include <GPIO.hh>
#include "../common/esdStructure.hh"
#include <GPIOconf.hh>
#include <AT24C.hh>
#include <globalClass.hh>
#include "../common/ADC.hh"

void flyWithAltitudePID(GlobalStruct& globals, DriversGroup& diversGroup, void(*delay)(uint32_t));
