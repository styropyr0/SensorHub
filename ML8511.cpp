#include "ML8511.h"

ML8511::ML8511(uint8_t analog, uint8_t enable) : analog(analog), enable(enable)
{
    pinMode(enable, OUTPUT);
    digitalWrite(enable, LOW);
    pinMode(analog, INPUT);
    uvi_unit = MWPSQCM;
    OSS = ML8511_OSS_1;
    unit = mW_Per_Sq_cm;
    med = ML8511_MED_DEFAULT;
    calibrateSensor(ML8511_V_ZERO, ML8511_V_10, ML8511_V_15);
}

float ML8511::getUVIntensity()
{
    float intensity_mean = 0;
    for (int i = 0; i < OSS; i++)
        intensity_mean = getVoltage() * x + y;
    intensity_mean /= OSS;
    return intensity_mean > 0 ? convert(intensity_mean) : 0.0;
}

float ML8511::getVoltage()
{
    digitalWrite(enable, HIGH);
    delay(3);

    float voltage = analogRead(analog) * (ML8511_V_REF / ADC_MAX);
    digitalWrite(enable, LOW);
    return voltage;
}

uint8_t ML8511::getUVIndex()
{
    uint8_t u = unit;
    setActiveUnit(mW_Per_Sq_cm);

    float uvi = getUVIntensity() * 12;

    setActiveUnit(u);

    if (uvi > floor(uvi) + 0.5)
        return uvi + 1;
    return uvi;
}

void ML8511::set_V0(float v0)
{
    v_zero = v0;
}

void ML8511::setOverSampling(uint8_t oss)
{
    if (oss > ML8511_OSS_3)
        return;
    OSS = oss;
}

bool ML8511::calibrateSensor(float UV0_voltage, float UV10_voltage, float UV15_voltage)
{
    v_zero = UV0_voltage;
    x = 10.0 / (UV10_voltage - UV0_voltage);
    y = -UV0_voltage * x;

    return checkApproxEquality(UV15_voltage * x + y, UV15_voltage, 1);
}

bool ML8511::checkApproxEquality(float a, float b, float fac)
{
    return (a <= (int)b + fac / 2) && (a >= (int)b - fac / 2);
}

void ML8511::setActiveUnit(uint8_t unit)
{
    switch (unit)
    {
    case mW_Per_Sq_cm:
        uvi_unit = MWPSQCM;
        break;

    case W_Per_Sq_m:
        uvi_unit = WPSQM;
        break;

    case microW_Per_Sq_cm:
        uvi_unit = MUWPSQCM;
        break;

    case Joules_s_Per_Sq_m:
        uvi_unit = JSPSQM;
        break;

    case mJoules_s_Per_Sq_cm:
        uvi_unit = MJSPSQCM;

    case Joules_s_Per_Sq_cm:
        uvi_unit = JSPSQCM;
        break;

    case MED:
        uvi_unit = MED_U;
        break;

    default:
        uvi_unit = MWPSQCM;
        this->unit = mW_Per_Sq_cm;
        return;
    }

    this->unit = unit;
}

float ML8511::convert(float value)
{
    switch (unit)
    {
    case W_Per_Sq_m:
        return value * 10;

    case microW_Per_Sq_cm:
        return value * 1000;

    case Joules_s_Per_Sq_m:
        return value * 10;

    case Joules_s_Per_Sq_cm:
        return value / 1000;

    case MED:
        return (value * 10) / med;

    default:
        return value;
    }
}