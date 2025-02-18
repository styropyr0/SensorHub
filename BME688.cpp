#include <BME688.h>
#include "SensorHub.h"
#include "SensorHubTemplates.h"
#include <Wire.h>

BME688::BME688()
{
}

bool BME688::begin()
{
    if (isConnected())
    {
        i2c_execute(bme688, BME_688_CTRL_MEAS_HUM_REG, hum_oss);
        i2c_execute(bme688, BME_688_CTRL_MEAS_REG, temp_oss << 5 | press_oss << 2 | BME_688_FORCED_MODE);
        i2c_execute(bme688, BME_688_IIR_FILTER_REG, BME_688_IIR_FILTER_C15);
        readCalibParams();
    }
    else
        printLog(BME_688_CHECK_CONN_ERR);
    return isConnected();
}

bool BME688::begin(uint8_t mode)
{
    if (isConnected())
    {
        if (mode <= BME_688_PARALLEL_MODE)
        {
            i2c_execute(bme688, BME_688_CTRL_MEAS_HUM_REG, BME_688_OSS_1);
            i2c_execute(bme688, BME_688_CTRL_MEAS_REG, BME_688_OSS_1 << 5 | BME_688_OSS_1 << 2 | mode);
            i2c_execute(bme688, BME_688_IIR_FILTER_REG, BME_688_IIR_FILTER_C15);
            readCalibParams();
        }
        else
        {
            printLog(BME_688_VALUE_INVALID);
            return false;
        }
    }
    else
        printLog(BME_688_CHECK_CONN_ERR);
    return isConnected();
}

bool BME688::begin(uint8_t mode, uint8_t oss)
{
    if (isConnected())
    {
        if (mode <= BME_688_PARALLEL_MODE && oss <= BME_688_OSS_16)
        {
            i2c_execute(bme688, BME_688_CTRL_MEAS_HUM_REG, oss);
            i2c_execute(bme688, BME_688_CTRL_MEAS_REG, oss << 5 | oss << 2 | mode);
            i2c_execute(bme688, BME_688_IIR_FILTER_REG, BME_688_IIR_FILTER_C15);
            readCalibParams();
        }
        else
        {
            printLog(BME_688_VALUE_INVALID);
            return false;
        }
    }
    else
        printLog(BME_688_CHECK_CONN_ERR);
    return isConnected();
}

BME688::~BME688()
{
}

void BME688::printLog(String log)
{
    if (printLogs)
        Serial.println(log);
}

void BME688::showLogs(bool show)
{
    printLogs = show;
}

void BME688::readCalibParams()
{
    if (!(i2c_read_Xbit_LE(bme688, BME_688_TEMP_CALIB1_REG, &par_t16[0], 16) &&
          i2c_read_Xbit_LE(bme688, BME_688_TEMP_CALIB2_REG, &par_t16[1], 16) &&
          i2c_readByte(bme688, BME_688_TEMP_CALIB3_REG, &par_t3, 1)))
        printLog(BME_688_TEMP_CAL_EXCEPT);

    if (!(i2c_read_Xbit_LE(bme688, BME_688_PRES_CALIB1_REG, &par_p1, 16) &&
          i2c_read_Xbit_LE(bme688, BME_688_PRES_CALIB2_REG, &par_p16[1], 16) &&
          i2c_readByte(bme688, BME_688_PRES_CALIB3_REG, &par_p8[0], 1) &&
          i2c_read_Xbit_LE(bme688, BME_688_PRES_CALIB4_REG, &par_p16[2], 16) &&
          i2c_read_Xbit_LE(bme688, BME_688_PRES_CALIB5_REG, &par_p16[3], 16) &&
          i2c_readByte(bme688, BME_688_PRES_CALIB6_REG, &par_p8[1], 1) &&
          i2c_readByte(bme688, BME_688_PRES_CALIB7_REG, &par_p8[2], 1) &&
          i2c_read_Xbit_LE(bme688, BME_688_PRES_CALIB8_REG, &par_p16[4], 16) &&
          i2c_read_Xbit_LE(bme688, BME_688_PRES_CALIB9_REG, &par_p16[5], 16) &&
          i2c_readByte(bme688, BME_688_PRES_CALIB10_REG, &par_p10, 1)))
        printLog(BME_688_PRES_CAL_EXCEPT

    if (!(i2c_read_Xbit_LE(bme688, BME_688_HUM_CALIB1_REG, &par_h16[0], 12) &&
          i2c_read_Xbit(bme688, BME_688_HUM_CALIB2_REG, &par_h16[1], 12) &&
          i2c_readByte(bme688, BME_688_HUM_CALIB3_REG, &par_h8[0], 1) &&
          i2c_readByte(bme688, BME_688_HUM_CALIB4_REG, &par_h8[1], 1) &&
          i2c_readByte(bme688, BME_688_HUM_CALIB5_REG, &par_h8[2], 1) &&
          i2c_readByte(bme688, BME_688_HUM_CALIB6_REG, &par_h6, 1) &&
          i2c_readByte(bme688, BME_688_HUM_CALIB7_REG, &par_h8[4], 1)))
        printLog(BME_688_HUM_CAL_EXCEPT);
}

bool BME688::isConnected()
{
    uint8_t pid = 0;
    if (is_sensor_connected(bme688) && i2c_readByte(bme688, BME_688_CHIP_ID_REG, &pid, 1))
        return pid == BME_688_CHIP_ID;
    return false;
}

bool BME688::setTemperatureOversampling(uint8_t oss)
{
    if (oss <= BME_688_OSS_16)
        temp_oss = oss;
    else
    {
        printLog(BME_688_VALUE_INVALID);
        return false;
    }
    return true;
}

int32_t BME688::readRawTemp()
{
    int32_t raw = 0;
    if (!i2c_read_Xbit(bme688, BME_688_TEMP_RAW_REG, &raw, 20))
        printLog(BME_688_READ_FAILURE);
    return raw;
}

int32_t BME688::readRawPres()
{
    int32_t raw = 0;
    if (!i2c_read_Xbit(bme688, BME_688_PRES_RAW_REG, &raw, 20))
        printLog(BME_688_READ_FAILURE);
    return raw;
}

int32_t BME688::readRawHum()
{
    int32_t raw = 0;
    if (!i2c_read_Xbit(bme688, BME_688_HUM_RAW_REG, &raw, 16))
        printLog(BME_688_READ_FAILURE);
    return raw;
}

double BME688::readUCTemp(int32_t adc_T)
{
    double var1 = (((double)adc_T / 16384.0) - ((double)par_t16[0] / 1024.0)) * (double)par_t16[1];
    double var2 = ((((double)adc_T / 131072.0) - ((double)par_t16[0] / 8192.0)) *
                   (((double)adc_T / 131072.0) - ((double)par_t16[0] / 8192.0))) *
                  ((double)par_t3 * 16.0);

    t_fine = var1 + var2;
    return t_fine / 5120.0;
}

double BME688::readUCPres(int32_t adc_P)
{
    double var1 = 0.0, var2 = 0.0, var3 = 0.0;
    double press_comp = 0.0;

    var1 = ((double)t_fine / 2.0) - 64000.0;
    var2 = var1 * var1 * ((double)par_p8[1] / 131072.0);
    var2 = var2 + (var1 * (double)par_p16[3] * 2.0);
    var2 = (var2 / 4.0) + ((double)par_p16[2] * 65536.0);
    var1 = ((((double)par_p8[0] * var1 * var1) / 16384.0) + ((double)par_p16[1] * var1)) / 524288.0;
    var1 = (1.0 + (var1 / 32768.0)) * (double)par_p1;
    press_comp = 1048576.0 - (double)adc_P;
    press_comp = ((press_comp - (var2 / 4096.0)) * 6250.0) / var1;
    var1 = ((double)par_p16[5] * press_comp * press_comp) / 2147483648.0;
    var2 = press_comp * ((double)par_p16[4] / 32768.0);
    var3 = (press_comp / 256.0) * (press_comp / 256.0) * (press_comp / 256.0) * ((double)par_p10 / 131072.0);
    p_fine = press_comp + (var1 + var2 + var3 + ((double)par_p8[2] * 128.0)) / 16.0;
    return p_fine;
}

double BME688::readUCHum(int32_t adc_H)
{
    double t_fine = this->t_fine / 5120.0;
    double var1 = 0, var2 = 0, var3 = 0, var4 = 0;

    var1 = adc_H - (((double)par_h16[0] * 16.0) + (((double)par_h8[0] / 2.0) * t_fine));
    var2 = var1 * (((double)par_h16[1] / 262144.0) *
                   (1.0 + (((double)par_h8[1] / 16384.0) * t_fine) +
                    (((double)par_h8[2] / 1048576.0) * t_fine * t_fine)));
    var3 = (double)par_h6 / 16384.0;
    var4 = (double)par_h8[4] / 2097152.0;
    h_fine = var2 + ((var3 + (var4 * t_fine)) * var2 * var2);
    return h_fine;
}

double BME688::readTemperature()
{
    i2c_execute(bme688, BME_688_CTRL_MEAS_REG, temp_oss << 5 | press_oss << 2 | mode);
    delay(500);
    return readUCTemp(readRawTemp());
}

double BME688::readPressure()
{
    i2c_execute(bme688, BME_688_CTRL_MEAS_REG, temp_oss << 5 | press_oss << 2 | mode);
    delay(500);
    return readUCPres(readRawPres());
}

double BME688::readHumidity()
{
    i2c_execute(bme688, BME_688_CTRL_MEAS_HUM_REG, hum_oss);
    i2c_execute(bme688, BME_688_CTRL_MEAS_REG, temp_oss << 5 | press_oss << 2 | mode);
    delay(500);
    return readUCHum(readRawHum());
}