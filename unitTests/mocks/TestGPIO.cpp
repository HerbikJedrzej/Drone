#include "TestGPIO.hh"
#include "TestDelay.hh"

namespace TestMocks{
    namespace TestGPIO{
        std::map<InputList, bool> inputs;
        std::map<OutputList, bool> outputs;
        void write(OutputList pin, bool state){
            outputs[pin] = state;
        }
        bool read(OutputList pin){
            return outputs.at(pin);
        }
        bool readInput(InputList pin){
            return inputs.at(pin);
        }
    }
    Drivers::GPIO gpio(TestGPIO::write, TestGPIO::read, TestGPIO::readInput, TestMocks::delay);
}