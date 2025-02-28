#include "ML8511.h"

ML8511::ML8511(uint8_t analog, uint8_t enable) : analog(analog), enable(enable)
{
    pinMode(enable, OUTPUT);
    digitalWrite(enable, LOW);
    pinMode(analog, INPUT);
}

float ML8511::getUVIntensity()
{
    return max(0.0, (getVoltage() - ML8511_V_ZERO) * ML8511_CALIB_INTENSITY / (ML8511_CALIB_VOLTAGE - 1.0));
}

float ML8511::getVoltage()
{
    digitalWrite(enable, HIGH);
    delay(5);

    float voltage = analogRead(analog) * (3.3 / ADC_MAX);
    digitalWrite(enable, LOW);
    return voltage;
}

float ML8511::getUVIndex()
{
    return getUVIntensity() * ML8511_UV_IRRADIANCE_FAC * 40;
}