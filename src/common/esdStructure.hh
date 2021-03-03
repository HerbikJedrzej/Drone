#pragma once

#include <cstddef>
#include <cstdint>
#include <stdint.h>
#include <cstdlib>
#include "EngineIfc.hh"

class EsdDriver : public EngineIfc{
public:
    EsdDriver(uint16_t *list, const uint16_t size, const uint16_t* _min, const uint16_t* _max);
    ~EsdDriver();
    void init() override;
    void set(uint8_t engineId, const double powerPercent) override;
protected:
    uint16_t* min;
    uint16_t* max;
};
