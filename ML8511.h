#ifndef ML8511_H
#define ML8511_H

#include <Arduino.h>
#include "ML8511DEFS.h"

#ifdef __cplusplus

class ML8511
{
public:
    ML8511(uint8_t analog, uint8_t enable);
    float getUVIntensity();
    float getUVIndex();

private:
    uint8_t analog, enable;

    float getVoltage();
};

#endif
#endif