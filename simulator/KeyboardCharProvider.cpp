#include <iostream>
#include <thread>
#include <termios.h> 
#include <unistd.h>
#include <KeyboardCharProvider.h>

using namespace std::chrono_literals;

namespace{
    int key=0;
    bool finishKeyLissinning=false;

    int mygetch(){ 
        struct termios old, newone;
        int toReturn;
        tcgetattr( STDIN_FILENO, &old );
        newone = old;
        newone.c_lflag &= ~( ICANON | ECHO );
        tcsetattr( STDIN_FILENO, TCSANOW, &newone );
        toReturn = getchar();
        tcsetattr( STDIN_FILENO, TCSANOW, &old );
        return toReturn;
    }

    void readKeynboard(){
        finishKeyLissinning=false;
        while(!finishKeyLissinning && key != ']'){
            key=mygetch();
        }
    }
}

unsigned int KeyGetter::counter = 0;
std::unique_ptr<std::thread> KeyGetter::keyReader = nullptr;

KeyGetter::KeyGetter(){
    if(!keyReader){
        keyReader.reset(new std::thread(readKeynboard));
        counter++;
    }
}

KeyGetter::~KeyGetter(){
    if(--counter == 0){
        finishKeyLissinning = true;
        keyReader->join();
    }
}

int KeyGetter::getChar(){
    int tmp = key;
    key = 0;
    return tmp;
}
