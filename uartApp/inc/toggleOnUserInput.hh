#pragma once
#include <thread>

class ToggleOnUserInput{
    bool flag;
    const bool initState;
    std::thread th;
    void run();
public:
    ToggleOnUserInput(const bool& init);
    ~ToggleOnUserInput();
    void reset();
    operator bool() const;
};
