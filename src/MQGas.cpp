#include "MQGas.h"

MQGas::MQGas()
{
    adc = nullptr;
    i2c_mode = false;
}

MQGas::MQGas(ADS111S *adc) : adc(adc)
{
    i2c_mode = true;
}

bool MQGas::begin()
{
    if (i2c_mode)
    {
        return adc->begin();
    }
    else
    {
        pinMode(pin, INPUT);
        return true;
    }
}

float MQGas::read(uint8_t type, uint8_t pin)
{
    if (pin > 4 && i2c_mode)
        return -1.0;

    this->type = type;

    if (i2c_mode)
    {
        return getConcentration(getResistance(adc->readFrom(pin)));
    }
    else
    {
        return getConcentration(getResistance(analogRead(pin)));
    }
}

float MQGas::getResistance(uint16_t adcValue)
{
    float voltage = adcValue / (float)(i2c_mode ? ADS111S_ADC_MAX : getADCResolution()) * (i2c_mode ? adc->getVoltageGainInUse() : MQ_VCC);
    return 10 * (MQ_VCC - voltage) / voltage;
}

float MQGas::getConcentration(float resistance)
{
    switch (type)
    {
    case MQ_TYPE_MQ2:
        return MQ_MQ2_A * pow(resistance / MQ_MQ2_CAL_R0, MQ_MQ2_B);
    case MQ_TYPE_MQ3:
        return pow(resistance / 0.6, -1.0 / 0.3);
    case MQ_TYPE_MQ4:
        return pow(resistance / 0.3, -1.0 / 0.4);
    case MQ_TYPE_MQ5:
        return pow(resistance / 0.3, -1.0 / 0.42);
    case MQ_TYPE_MQ6:
        return pow(resistance / 0.3, -1.0 / 0.4);
    case MQ_TYPE_MQ7:
        return pow(resistance / 0.3, -1.0 / 0.4);
    case MQ_TYPE_MQ8:
        return pow(resistance / 0.3, -1.0 / 0.4);
    case MQ_TYPE_MQ9:
        return pow(resistance / 0.3, -1.0 / 0.4);
    case MQ_TYPE_MQ131:
        return pow(resistance / 0.3, -1.0 / 0.4);
    case MQ_TYPE_MQ135:
        return MQ_MQ135_A * pow(resistance / MQ_MQ135_CAL_R0, MQ_MQ135_B);
    case MQ_TYPE_MQ136:
        return pow(resistance / 0.3, -1.0 / 0.3);
    case MQ_TYPE_MQ137:
        return pow(resistance / 0.3, -1.0 / 0.3);
    case MQ_TYPE_MQ138:
        return pow(resistance / 0.3, -1.0 / 0.3);
    case MQ_TYPE_MQ303A:
        return pow(resistance / 0.3, -1.0 / 0.3);
    case MQ_TYPE_MQ309A:
        return pow(resistance / 0.3, -1.0 / 0.3);
    default:
        return -1.0;
    }
}

int MQGas::getADCResolution()
{
#if defined(ARDUINO_ESP32_DEV) || defined(ARDUINO_SAMD_MKRZERO) || defined(ARDUINO_NANO_RP2040_CONNECT)
    return MQ_ADC_RES_12;
#else
    return MQ_ADC_RES_10;
#endif
}
