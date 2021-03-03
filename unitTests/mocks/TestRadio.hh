#pragma once
#include <RadioIfc.hh>
#include <map>
#include "TestDelay.hh"
#include <array>
#include <queue>

namespace TestMocks{
    class TestRadio : public Drivers::RadioIfc{
        static constexpr unsigned int size = {7};
        uint8_t dataToRead[size];
        uint8_t dataToSend[size];
    public:
        bool keyCorrect = {true};
        bool ackCorrect = {true};
        bool comunictationCorrect = {true};
        std::queue<std::array<uint8_t, size>> tx;
        std::queue<std::array<uint8_t, size>> rx;
        TestRadio();
        bool isKyeCorrect() override;
        bool isAckCorrect() override;
        bool isComunicationCorrect() override;
        void init() override;
        uint8_t getTx(unsigned int i) const override;
        uint8_t getRx(unsigned int i) const override;
        uint8_t& setTx(unsigned int i) override;
        uint8_t& setRx(unsigned int i) override;
    protected:
        void handleTimeEvent(Drivers::DriverIfc*) override;
    };
}