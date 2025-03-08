#include "SensorHub.h"

/**
 * Parent class for all sensors
 */

SensorHub::SensorHub(uint8_t addr) : ADDR(addr)
{
    mode = 1;
    Wire.begin();
}

#if SOC_I2S_SUPPORTS_ADC

SensorHub::SensorHub(uint8_t ws, uint8_t bclock, uint8_t data, uint32_t samplingRate, uint8_t CHANNEL)
{
    // mode = 2;

    // i2s_channel_fmt_t channelFormat;

    // if (CHANNEL == I2S_CHANNEL_LEFT)
    //     channelFormat = I2S_CHANNEL_FMT_ONLY_LEFT;
    // else if (CHANNEL == I2S_CHANNEL_RIGHT)
    //     channelFormat = I2S_CHANNEL_FMT_ONLY_RIGHT;
    // else if (CHANNEL == I2S_CHANNEL_STEREO)
    //     channelFormat = I2S_CHANNEL_FMT_RIGHT_LEFT;
    // else
    //     channelFormat = I2S_CHANNEL_FMT_ALL_LEFT;

    // const i2s_config_t i2s_config = {
    //     .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),
    //     .sample_rate = samplingRate,
    //     .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
    //     .channel_format = channelFormat,
    //     .communication_format = I2S_COMM_FORMAT_I2S,
    //     .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    //     .dma_buf_count = 8,
    //     .dma_buf_len = 512,
    //     .use_apll = false,
    // };

    // const i2s_pin_config_t pin_config = {
    //     .bck_io_num = bclock,
    //     .ws_io_num = ws,
    //     .data_out_num = I2S_PIN_NO_CHANGE,
    //     .data_in_num = data,
    // };

    // i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL);
    // i2s_set_pin(I2S_NUM_0, &pin_config);
}

#endif

void SensorHub::i2c_execute(uint8_t reg, uint8_t data)
{
    Wire.beginTransmission(ADDR);
    Wire.write(reg);
    Wire.write(data);
    Wire.endTransmission(true);
}

void SensorHub::i2c_execute_16bit(uint8_t reg, uint16_t data)
{
    Wire.beginTransmission(ADDR);
    Wire.write(reg);
    Wire.write(data >> 8);
    Wire.write(data & 0xFF);
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