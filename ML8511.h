#ifndef ML8511_H
#define ML8511_H

#include <Arduino.h>
#include "ML8511DEFS.h"

#ifdef __cplusplus

/**
 * @brief ML8511 Sensor class.
 * Read UV Intensity and UV Index from the sensor, with configurable units.
 * @author Saurav Sajeev
 */
class ML8511
{
public:
    // Holds the current UV intensity unit.
    String uvi_unit;

    /**
     * @brief Creates an ML8511 object..
     * @param analog The pin to which sensor's OUT pin is connected.
     * @param enable The pin to which sensor's EN pin is connected.
     */
    ML8511(uint8_t analog, uint8_t enable);

    /**
     * @brief Get the UV intensity value from the sensor.
     * @returns The ambient UV intensity.
     */
    float getUVIntensity();

    /**
     * @brief Get the UV Index corresponding to the UV radiation level.
     * @returns The UV Index factor.
     */
    uint8_t getUVIndex();

    /**
     * @brief Sets the Vzero voltage of the sensor. It is the voltage output by the sensor when the UV radiation level is zero.
     * @param v0 The voltage value (in V)
     */
    void set_V0(float v0);

    /**
     * @brief Sets the over-sampling value. It averages the data which improves consistency.
     * @param oss The over-sampling factor. Default value is 5 steps.
     */
    void setOverSampling(uint8_t oss);

    /**
     * @brief Calibrate the sensor with your own calibration voltage values.
     * @param UV0_voltage The voltage when the UV intensity is 0.
     * @param UV10_voltage The voltage when the UV intensity is 10.
     * @param UV15_voltage The voltage when the UV intensity is 15.
     * @returns True if the values satisfy calibration preferences, otherwise False.
     */
    bool calibrateSensor(float UV0_voltage, float UV10_voltage, float UV15_voltage);

    /**
     * @brief Sets the active unit which will be used by @fn getUVIntensity.
     * @param unit The unit which needs to be used. Please include ML8511DEFS.h for unit constants or refer documentation.
     */
    void setActiveUnit(uint8_t unit);

private:
    uint8_t analog, enable, OSS, unit;
    uint16_t med;
    float v_zero, x, y;

    float getVoltage();
    bool checkApproxEquality(float a, float b, float fac);
    float convert(float value);
};

#endif
#endif