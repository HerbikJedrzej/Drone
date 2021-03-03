#include <toggleOnUserInput.hh>
#include <iostream>

void ToggleOnUserInput::run(){
    std::cin.get();
    flag = !initState;
}

ToggleOnUserInput::operator bool() const{
    return flag;
}

void ToggleOnUserInput::reset(){
    th.join();
    flag = initState;
    th = std::thread(&ToggleOnUserInput::run, this);
}

ToggleOnUserInput::ToggleOnUserInput(const bool& init):
    flag(init),
    initState(init),
    th(&ToggleOnUserInput::run, this){}

ToggleOnUserInput::~ToggleOnUserInput(){
    th.join();
}

