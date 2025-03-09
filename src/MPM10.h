#ifndef MPM10_H
#define MPM10_H

#include <Arduino.h>
#include "MPM10DEFS.h"

#ifdef __cplusplus

/**
 * @brief MPM10 Series PM Sensor class.
 * Read UV Intensity and UV Index from the sensor, with configurable units.
 * @author Saurav Sajeev
 */
class MPM10
{
    public:
    MPM10(uint8_t mode);
};

#endif
#endif