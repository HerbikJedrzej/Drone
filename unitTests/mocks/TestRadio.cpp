#include "TestRadio.hh"
#include "TestDelay.hh"

namespace TestMocks{
    constexpr uint8_t radioKey = 0x16;
    unsigned int localRadiotime = 0;
    TestRadio::TestRadio():
        RadioIfc(radioKey, localRadiotime){}

    bool TestRadio::isKyeCorrect(){
        return keyCorrect;
    }

    bool TestRadio::isAckCorrect(){
        return ackCorrect;
    }

    bool TestRadio::isComunicationCorrect(){
        return comunictationCorrect;
    }

    void TestRadio::init(){
    }

    uint8_t TestRadio::getTx(unsigned int i) const{
        return dataToSend[i];
    }

    uint8_t TestRadio::getRx(unsigned int i) const{
        return dataToRead[i];
    }

    uint8_t& TestRadio::setTx(unsigned int i){
        return dataToSend[i];
    }

    uint8_t& TestRadio::setRx(unsigned int i){
        return dataToRead[i];
    }

    void TestRadio::handleTimeEvent(Drivers::DriverIfc*){
        tx.push(std::array<uint8_t, size>{0});
        if(rx.size() == 0)
            throw std::invalid_argument("Empty incoming radio data.");
        for(uint8_t i = 0; i < size; i++){
            dataToRead[i] = rx.front()[i];
            tx.back()[i] = dataToSend[i];
        }
        rx.pop();
    }

}