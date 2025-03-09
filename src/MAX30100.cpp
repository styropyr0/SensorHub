#include "MAX30100.h"

MAX30100::MAX30100() : sensorHub(MAX30100_I2C_ADDR) {}

bool MAX30100::begin()
{
    if (isConnected())
    {
        setupSensor(MAX30100_MODE_ALL);
    }
    // else
    //     printLog(BME_688_CHECK_CONN_ERR);
    return isConnected();
}

bool MAX30100::begin(uint8_t mode)
{
    if (isConnected())
    {
        if (mode == MAX30100_MODE_HR_ONLY || mode == MAX30100_MODE_SPO2_EN || mode == MAX30100_MODE_TEMP_ONLY || mode == MAX30100_MODE_ALL)
            setupSensor(mode);
        else
            return false;
    }
    // else
    //     printLog(BME_688_CHECK_CONN_ERR);
    return isConnected();
}

bool MAX30100::isConnected()
{
    uint8_t data;
    if (sensorHub.is_sensor_connected() && sensorHub.i2c_readByte(MAX30100_CHIP_ID_REG, &data, 1))
        return data == MAX30100_CHIP_ID;
    return false;
}

void MAX30100::setupSensor(uint8_t mode)
{
    sensorHub.i2c_execute(MAX30100_MODE_CONFIG_REG, mode);
    sensorHub.i2c_execute(MAX30100_LED_CONFIG_REG, LED_CURRENT_11MA << 4 | LED_CURRENT_11MA);
    sensorHub.i2c_execute(MAX30100_SPO2_CONFIG_REG, SPO2_SAMPLE_RATE_100 << 2 | LED_PW_400US);
    sensorHub.i2c_execute(MAX30100_INT_ENABLE_REG, 0x40);
    resetFIFO();
}

void MAX30100::resetFIFO()
{
    sensorHub.i2c_execute(MAX30100_FIFO_WR_PTR_REG, 0x00);
    sensorHub.i2c_execute(MAX30100_OVF_COUNTER_REG, 0x00);
    sensorHub.i2c_execute(MAX30100_FIFO_RD_PTR_REG, 0x00);
}

void MAX30100::readRawData()
{
    resetFIFO();
    uint8_t data[4];
    sensorHub.i2c_readByte(MAX30100_FIFO_DATA_REG, data, 4);
    red = (data[0] << 8) | data[1];
    ir = (data[2] << 8) | data[3];
}

uint16_t MAX30100::getRed()
{
    readRawData();
    return red;
}

uint16_t MAX30100::getIR()
{
    readRawData();
    return ir;
}