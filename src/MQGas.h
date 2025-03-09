#ifndef MQGAS_H
#define MQGAS_H

#include <Arduino.h>
#include "ADS111S.h"
#include "MQGASDEFS.h"

#ifdef __cplusplus

/**
 * @class MQGas
 * @brief Driver for the MQ Series gas sensors.
 *
 * The MQGas class provides functionality to interact with MQ Series gas sensors.
 * It includes methods to initialize the sensor, read sensor resistance, calculate
 * gas concentrations for different gas types, and perform sensor calibration.
 *
 * @note This class uses the ADS111S ADC to read analog sensor data with higher precision.
 *
 * @author Saurav Sajeev
 */
class MQGas
{
public:
    /**
     * @brief Default Constructor for MQGas.
     *
     * Initializes an instance of the MQGas class with default settings.
     * Use this if you're not connecting to an external ADC like ADS111S.
     */
    MQGas();

    /**
     * @brief Parameterized Constructor for MQGas.
     *
     * Initializes an MQGas instance using an external ADC.
     *
     * @param adc Pointer to an ADS111S object to enable high-resolution ADC readings.
     * This constructor allows the user to use the ADS111S ADC library for precise measurements.
     */
    MQGas(ADS111S *adc);

    /**
     * @brief Initializes the MQGas sensor module.
     *
     * This method sets up any required configurations for the MQ gas sensors,
     * including initialization of the ADC (if applicable).
     *
     * @return `true` if initialization is successful, `false` otherwise.
     */
    bool begin();

    /**
     * @brief Reads the gas concentration.
     *
     * This method fetches the data from the MQ gas sensor connected to the specified pin.
     * The data is processed to return either the sensor resistance or a calculated
     * concentration based on the gas type.
     *
     * @param type The gas sensor type (e.g., MQ2, MQ135) defined in MQGASDEFS.h.
     *             Example: `MQ_TYPE_MQ2` for the MQ2 sensor.
     * @param pin The analog pin or ADC channel connected to the sensor's output.
     *
     * @return The sensor data, such as resistance in kOhms or gas concentration in ppm.
     */
    float read(uint8_t type, uint8_t pin);

private:
    uint8_t type, pin;
    ADS111S *adc;
    bool i2c_mode = false;

    float getResistance(uint16_t adcValue);
    int getADCResolution();
    float getConcentration(float resistance);
};

#endif // __cplusplus
#endif // MQGAS_H