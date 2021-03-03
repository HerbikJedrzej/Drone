#include "DroneVisualisation.h"
#include <RadioParser.hh>

class KeyboardInterpreter{
    DroneVisualisation& visual;
    Drivers::RadioParser* const radioParser;
public:
    KeyboardInterpreter(DroneVisualisation& _visual, Drivers::RadioParser* const _radioParser);
    ~KeyboardInterpreter();
    void run(const unsigned int& key);
};
