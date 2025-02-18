#ifndef SensorHub_H
#define SensorHub_H

#include "Arduino.h"

#ifdef __cplusplus

class SensorHub
{
protected:
    SensorHub();
    void i2c_execute(uint8_t address, uint8_t reg, uint8_t data);
    bool i2c_readByte(uint8_t address, uint8_t reg, uint8_t *const data, uint8_t length);
    bool i2c_readByte(uint8_t addr, uint8_t reg, int8_t *const data, uint8_t length);

    template <typename T>
    bool i2c_read_Xbit_LE(uint8_t addr, uint8_t reg, T *const data, uint8_t length);

    template <typename T>
    bool i2c_read_Xbit(uint8_t addr, uint8_t reg, T *const data, uint8_t length);

    void set_i2c_mode(uint8_t mode);
    bool is_sensor_connected(uint8_t address);
    virtual bool isConnected() = 0;
    virtual void printLog(String log) = 0;
    void startTransmission(uint8_t addr, uint8_t reg);
    virtual ~SensorHub();
};

#endif // __cplusplus
#endif // SensorHub_H