#pragma once

#include <cstddef>
#include <cstdint>
#include <stdint.h>
#include <cstdlib>
#include "EngineIfc.hh"

class MiniDronEngine : public EngineIfc{
public:
    MiniDronEngine(uint16_t* list, uint8_t size);
    void init() override;
    void set(uint8_t engineId, const double powerPercent) override;
};
