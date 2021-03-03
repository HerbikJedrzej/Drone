#pragma once
#include <RadioParser.hh>
#include "TestRadio.hh"

namespace TestMocks{
    class TestRadioParser : public Drivers::RadioParser{
        uint8_t counter = {0};
        bool flyOn = {false};
        bool engOn = {false};
        bool jR = {false};
        bool jL = {false};
        bool sOpt = {false};
        int8_t R = {0};
        int8_t P = {0};
        int8_t Y = {0};
        int8_t alt = {0};
        uint8_t mNr = {0};
        int8_t mVal = {0};
        uint8_t composeFirstByte();
    public:
        TestRadio radioMock;
        TestRadioParser();
        void pushDataToQueue();
    };
}