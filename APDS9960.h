#ifndef APDS9960_H
#define APDS9960_H

#include <Arduino.h>
#include "APDS9960DEFS.h"
#include "SensorHub.h"

#ifdef __cplusplus

struct Color;
struct Gesture;

/**
 * @brief APDS9960 Color, Proximity, and Gesture sensor.
 * @author Saurav Sajeev
 */
class APDS9960
{
public:
    APDS9960();
    bool begin();
    bool beginAll();
    uint8_t getPID();
    void enableAllInterrupts(bool state);
    void enableAllSensors(bool state);
    void enableGestureSensing(bool state);
    void enableProximitySensing(bool state);
    void enableLightSensing(bool state);
    void enableWaitTimer(bool state, uint8_t waitTime, bool WLONG);
    void enableProximityInterrupt(bool state);
    void enableLightInterrupt(bool state);
    void enableGestureInterrupt(bool state);
    void setLightSensitivity(uint8_t shutterSpeed);
    void setLightGain(uint8_t gainFactor);
    void setProximitySensorRange(uint8_t level);
    uint8_t readProximity();
    void correctProximity(int8_t upRight, int8_t downLeft);
    void setLightSensingInterruptThreshold(uint16_t low, uint16_t high);
    void setProximitySensingInterruptThreshold(uint8_t low, uint8_t high);
    void setPersistence(uint8_t light, uint8_t proximity);
    void setProximitySensitivity(uint8_t sensitivity);
    Color readColorData();
    void setGestureSensitivity(uint8_t pulseLength, uint8_t pulseCount);
    void setGestureGain(uint8_t factor);
    void setGestureDetectorMode(uint8_t mode);
    Gesture readGesture();
    String resolveGesture(Gesture gesture, uint8_t threshold);

private:
    SensorHub sensorHub;
    uint8_t setupByte = 0x01, ctrl = 0x00, gesCtrl = 0x00;

    bool isConnected();
};

struct Color
{
public:
    uint16_t red, green, blue, clear;
};

struct Gesture
{
public:
    uint8_t up, down, left, right;
};

#endif
#endif