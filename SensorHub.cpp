#include "SensorHub.h"
#include <Wire.h>

/**
 * Parent class for all sensors
 */

SensorHub::SensorHub(uint8_t addr) : ADDR(addr)
{
    Wire.begin();
}

void SensorHub::i2c_execute(uint8_t reg, uint8_t data)
{
    Wire.beginTransmission(ADDR);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission(true);
}

bool SensorHub::i2c_readByte(uint8_t reg, uint8_t *const data, uint8_t length)
{
    startTransmission(reg);
    Wire.requestFrom(ADDR, length);
    if (Wire.available() < length)
        return false;
    for (uint8_t i = 0; i < length; i++)
        data[i] = Wire.read();
    return true;
}

bool SensorHub::i2c_readByte(uint8_t reg, int8_t *const data, uint8_t length)
{
    startTransmission(reg);
    Wire.requestFrom(ADDR, length);
    if (Wire.available() < length)
        return false;
    for (uint8_t i = 0; i < length; i++)
        data[i] = Wire.read();
    return true;
}

void SensorHub::startTransmission(uint8_t reg)
{
    Wire.beginTransmission(ADDR);
    Wire.write(reg);
    Wire.endTransmission(false);
}

bool SensorHub::is_sensor_connected()
{
    Wire.beginTransmission(ADDR);
    return (Wire.endTransmission() == 0);
}

SensorHub::~SensorHub()
{
}