#include <unistd.h>
#include <thread>
#include <memory>

class KeyGetter{
    static std::unique_ptr<std::thread> keyReader;
    static unsigned int counter;
public:
    KeyGetter();
    ~KeyGetter();
    int getChar();
};
