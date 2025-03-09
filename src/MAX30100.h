#ifndef MAX30100_H
#define MAX30100_H

#include <Arduino.h>
#include "MAX30100DEFS.h"
#include "SensorHub.h"

#ifdef __cplusplus

/**
 * @class MAX30100
 * @brief Driver for the MAX30100 pulse oximeter and heart-rate sensor.
 *
 * The MAX30100 class provides functionality to interact with the MAX30100
 * pulse oximeter and heart-rate sensor. It includes methods to initialize the
 * sensor, read raw data, and calculate heart rate and SpO2 values.
 *
 * @note This class uses the ADS111S ADC to read analog sensor data with higher precision.
 *
 * @author Saurav Sajeev
 */
class MAX30100
{
public:
    MAX30100();
    bool begin();
    bool begin(uint8_t mode);
    bool isConnected();
    uint16_t getRed();
    uint16_t getIR();

private:
    SensorHub sensorHub;
    uint16_t red = 0, ir = 0;

    void readRawData();
    void setupSensor(uint8_t mode);
    void resetFIFO();
};

#endif // __cplusplus
#endif // MAX30100_H