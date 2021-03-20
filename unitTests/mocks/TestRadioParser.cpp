#include "TestRadioParser.hh"
#include "TestDelay.hh"

namespace TestMocks{
    TestRadioParser::TestRadioParser():
        RadioParser(&radioMock, Drivers::RadioMode::Drone){}

    void TestRadioParser::pushDataToQueue(){
        radioMock.rx.push({
            composeFirstByte(),
            uint8_t(R),
            uint8_t(P),
            uint8_t(Y),
            uint8_t(alt),
            mNr,
            uint8_t(mVal)
        });
    }

    uint8_t TestRadioParser::composeFirstByte(){
        uint8_t toReturn = 0;
        if(flyOn)
            toReturn |= (1 << 7);
        if(engOn)
            toReturn |= (1 << 6);
        if(jR)
            toReturn |= (1 << 5);
        if(jL)
            toReturn |= (1 << 4);
        if(sOpt)
            toReturn |= (1 << 3);
        toReturn |= (counter++) & 0x07;
        if(counter == 8)
            counter = 0;
        return toReturn;
    }
}