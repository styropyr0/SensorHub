#include "APDS9960.h"

APDS9960::APDS9960() : sensorHub(APDS_I2C_ADDR) {}

bool APDS9960::begin()
{
    if (isConnected())
        sensorHub.i2c_execute(APDS_ENABLE_REG, setupByte);
    return isConnected();
}

bool APDS9960::beginAll()
{
    if (isConnected())
    {
        enableAllSensors(true);
        enableAllInterrupts(true);
        sensorHub.i2c_execute(APDS_ENABLE_REG, setupByte);
    }
    return isConnected();
}

uint8_t APDS9960::getPID()
{
    uint8_t pid = 0;
    sensorHub.i2c_readByte(APDS_PID_REG, &pid, 1);
    return pid;
}

void APDS9960::enableAllSensors(bool state)
{
    enableLightSensing(state);
    enableProximitySensing(state);
    enableGestureSensing(state);
}

void APDS9960::enableAllInterrupts(bool state)
{
    enableLightInterrupt(state);
    enableProximityInterrupt(state);
}

void APDS9960::enableLightSensing(bool state)
{
    setupByte = (setupByte & ~APDS_AIL_MASK) | (-state & APDS_AIL_MASK);
    sensorHub.i2c_execute(APDS_ENABLE_REG, setupByte);
}

void APDS9960::enableProximitySensing(bool state)
{
    setupByte = (setupByte & ~APDS_PIL_MASK) | (-state & APDS_PIL_MASK);
    sensorHub.i2c_execute(APDS_ENABLE_REG, setupByte);
}

void APDS9960::enableGestureSensing(bool state)
{
    setupByte = (setupByte & ~APDS_GES_MASK) | (-state & APDS_GES_MASK);
    sensorHub.i2c_execute(APDS_ENABLE_REG, setupByte);
}

void APDS9960::enableLightInterrupt(bool state)
{
    setupByte = (setupByte & ~APDS_AIL_INT_MASK) | (-state & APDS_AIL_INT_MASK);
    sensorHub.i2c_execute(APDS_ENABLE_REG, setupByte);
}

void APDS9960::enableProximityInterrupt(bool state)
{
    setupByte = (setupByte & ~APDS_PIL_INT_MASK) | (-state & APDS_PIL_INT_MASK);
    sensorHub.i2c_execute(APDS_ENABLE_REG, setupByte);
}

void APDS9960::enableWaitTimer(bool state, uint8_t waitTime, bool WLONG)
{
    setupByte = (setupByte & ~APDS_WEN_MASK) | (-state & APDS_WEN_MASK);
    sensorHub.i2c_execute(APDS_ENABLE_REG, setupByte);
    sensorHub.i2c_execute(APDS_WTIME_REG, waitTime);
    sensorHub.i2c_execute(APDS_CONFIG_REG_1, WLONG ? 0x62 : 0x60);
}

bool APDS9960::isConnected()
{
    uint8_t pid = 0x00;
    if (sensorHub.is_sensor_connected() && sensorHub.i2c_readByte(APDS_PID_REG, &pid, 1))
        return pid == APDS_PID_1 || pid == APDS_PID_2;
    return false;
}

void APDS9960::setSensitivity(uint8_t shutterSpeed)
{
    sensorHub.i2c_execute(APDS_ATIME_REG, 255 - shutterSpeed);
}

uint8_t APDS9960::readProximity()
{
    uint8_t pData = 0;
    sensorHub.i2c_readByte(APDS_PDATA_REG, &pData, 1);
    return pData;
}

void APDS9960::correctProximity(int8_t upRight, int8_t downLeft)
{
    sensorHub.i2c_execute(APDS_POFFSET_UR_REG, upRight);
    sensorHub.i2c_execute(APDS_POFFSET_DL_REG, downLeft);
}

void APDS9960::setLightSensingInterruptThreshold(uint16_t low, uint16_t high)
{
    sensorHub.i2c_execute(APDS_AILTL_REG, 0xFF & low);
    sensorHub.i2c_execute(APDS_AILTH_REG, low >> 8);
    sensorHub.i2c_execute(APDS_AIHTL_REG, 0xFF & high);
    sensorHub.i2c_execute(APDS_AIHTH_REG, high >> 8);
}

void APDS9960::setProximitySensingInterruptThreshold(uint8_t low, uint8_t high)
{
    sensorHub.i2c_execute(APDS_PILT_REG, low);
    sensorHub.i2c_execute(APDS_PIHT_REG, high);
}

void APDS9960::setPersistence(uint8_t light, uint8_t proximity)
{
    if (light > 15 || proximity > 15)
        return;
    sensorHub.i2c_execute(APDS_PERS_REG, light << 4 | proximity);
}