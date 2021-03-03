#pragma once
#include <GPIO.hh>
#include <map>
#include "TestDelay.hh"

namespace TestMocks{
    namespace TestGPIO{
        extern std::map<InputList, bool> inputs;
        extern std::map<OutputList, bool> outputs;
        void write(OutputList pin, bool);
        bool read(OutputList pin);
        bool readInput(InputList pin);
    }
    extern Drivers::GPIO gpio;
}