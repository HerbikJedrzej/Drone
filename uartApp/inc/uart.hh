#pragma once
#include <cstdlib>
#include <cstdint>
#include <termios.h> /* POSIX Terminal Control Definitions */
#include <queue>
#include <thread>

class UART{
    int fd;
    bool runFlag;
    std::thread th;
    std::queue<uint8_t> buffor;
public:
    enum Barude{
        b9600 = B9600,
        b115200 = B115200,
    };
    UART(const char* port, const Barude barude);
    ~UART();
    unsigned int size();
    bool empty();
    void clear();
    uint8_t getChar();
private:
    void run();
};
