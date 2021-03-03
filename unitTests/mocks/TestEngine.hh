#pragma once
#include <map>
#include "TestDelay.hh"
#include <EngineIfc.hh>

namespace TestMocks{
    class TestEngine : public EngineIfc{
    public:
        std::map<uint8_t, double> map;
        TestEngine(const uint8_t size);
        void init() override;
        void set(uint8_t engineId, const double powerPercent) override;
    };
}