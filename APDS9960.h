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

    /**
     * @brief Prepare the sensor for measurements.
     * @returns The setup status. If false, the sensor configuration has failed.
     */
    bool begin();

    /**
     * @brief Prepare the sensor for measurements and enable all three sensing modes.
     * @returns The setup status. If false, the sensor configuration has failed.
     */
    bool beginAll();

    /**
     * @brief Gets the PID of the sensor.
     * @returns The PID of the sensor.
     */
    uint8_t getPID();

    /**
     * @brief Enables or disables all sensor-related interrupts.
     * @param state True to enable, false to disable.
     */
    void enableAllInterrupts(bool state);

    /**
     * @brief Enables or disables all sensing functionalities.
     * @param state True to enable, false to disable.
     */
    void enableAllSensors(bool state);

    /**
     * @brief Enables or disables gesture sensing mode.
     * @param state True to enable, false to disable.
     */
    void enableGestureSensing(bool state);

    /**
     * @brief Enables or disables proximity sensing mode.
     * @param state True to enable, false to disable.
     */
    void enableProximitySensing(bool state);

    /**
     * @brief Enables or disables ambient light and color sensing mode.
     * @param state True to enable, false to disable.
     */
    void enableLightSensing(bool state);

    /**
     * @brief Enables or disables the wait timer.
     * @param state True to enable, false to disable.
     * @param waitTime Wait time in clock cycles.
     * @param WLONG True for extended wait time, false for normal wait time.
     */
    void enableWaitTimer(bool state, uint8_t waitTime, bool WLONG);

    /**
     * @brief Enables or disables the proximity sensor interrupt.
     * @param state True to enable, false to disable.
     */
    void enableProximityInterrupt(bool state);

    /**
     * @brief Enables or disables the ambient light sensor interrupt.
     * @param state True to enable, false to disable.
     */
    void enableLightInterrupt(bool state);

    /**
     * @brief Enables or disables the gesture sensing interrupt.
     * @param state True to enable, false to disable.
     */
    void enableGestureInterrupt(bool state);

    /**
     * @brief Sets the sensitivity of the light sensor.
     * @param shutterSpeed The integration time for light sensing.
     */
    void setLightSensitivity(uint8_t shutterSpeed);

    /**
     * @brief Sets the gain factor for light sensing.
     * @param gainFactor The gain multiplier (e.g., 1x, 4x, 16x, 64x).
     */
    void setLightGain(uint8_t gainFactor);

    /**
     * @brief Sets the range of the proximity sensor.
     * @param level The range level (higher values increase sensitivity).
     */
    void setProximitySensorRange(uint8_t level);

    /**
     * @brief Reads the proximity sensor value.
     * @returns The detected proximity value (0-255).
     */
    uint8_t readProximity();

    /**
     * @brief Applies correction factors to proximity sensor readings.
     * @param upRight Correction factor for upward and right directions.
     * @param downLeft Correction factor for downward and left directions.
     */
    void correctProximity(int8_t upRight, int8_t downLeft);

    /**
     * @brief Sets interrupt threshold values for ambient light sensing.
     * @param low The lower threshold value.
     * @param high The upper threshold value.
     */
    void setLightSensingInterruptThreshold(uint16_t low, uint16_t high);

    /**
     * @brief Sets interrupt threshold values for proximity sensing.
     * @param low The lower threshold value.
     * @param high The upper threshold value.
     */
    void setProximitySensingInterruptThreshold(uint8_t low, uint8_t high);

    /**
     * @brief Sets the persistence filters for light and proximity sensing.
     * @param light Number of consecutive light readings before an interrupt is triggered.
     * @param proximity Number of consecutive proximity readings before an interrupt is triggered.
     */
    void setPersistence(uint8_t light, uint8_t proximity);

    /**
     * @brief Sets the sensitivity level for proximity sensing.
     * @param sensitivity The proximity sensitivity level (higher values increase sensitivity).
     */
    void setProximitySensitivity(uint8_t sensitivity);

    /**
     * @brief Reads the RGB color and clear channel data.
     * @returns A Color struct containing red, green, blue, and clear values.
     */
    Color readColorData();

    /**
     * @brief Sets the gesture sensitivity by configuring pulse length and count.
     * @param pulseLength The duration of each pulse (4, 8, 16, or 32 µs).
     * @param pulseCount The number of pulses per gesture event.
     */
    void setGestureSensitivity(uint8_t pulseLength, uint8_t pulseCount);

    /**
     * @brief Sets the gain level for gesture detection.
     * @param factor The gain factor (1x, 2x, 4x, or 8x).
     */
    void setGestureGain(uint8_t factor);

    /**
     * @brief Sets the gesture detection mode.
     * @param mode The gesture detection mode.
     */
    void setGestureDetectorMode(uint8_t mode);

    /**
     * @brief Reads gesture data and determines the direction.
     * @returns A Gesture struct containing movement values for up, down, left, and right.
     */
    Gesture readGesture();

    /**
     * @brief Resolves the gesture direction as a string based on primary and secondary movements.
     * @param gesture The Gesture struct containing movement values.
     * @param threshold The minimum required difference between primary and secondary movements.
     * @returns A string representing the resolved gesture (e.g., "Up", "Right + Down").
     */
    String resolveGesture(Gesture gesture, uint8_t threshold);

private:
    SensorHub sensorHub;
    uint8_t setupByte = 0x01, ctrl = 0x00, gesCtrl = 0x00;

    /**
     * @brief Checks if the sensor is connected.
     * @returns True if the sensor is detected, false otherwise.
     */
    bool isConnected();
};

/**
 * @brief Struct representing RGB color data from the sensor.
 */
struct Color
{
public:
    uint16_t red, green, blue, clear;
};

/**
 * @brief Struct representing gesture movement data.
 */
struct Gesture
{
public:
    uint8_t up, down, left, right;
};

#endif
#endif