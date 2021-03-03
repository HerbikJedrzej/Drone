#include "KeyboardInterpreter.h"
#include <stdexcept>
#include <string>

KeyboardInterpreter::KeyboardInterpreter(DroneVisualisation& _visual, Drivers::RadioParser* const _radioParser):
    visual(_visual),
    radioParser(_radioParser){
}

KeyboardInterpreter::~KeyboardInterpreter(){
}

void KeyboardInterpreter::run(const unsigned int& key){
    switch(key){
        case 0:
            return;
        case ']':
            return;
        case '[':
            return;
        case 'i':
            break;
        default:
            throw std::invalid_argument("Unknown char input = " + std::to_string(key));
    }
}
