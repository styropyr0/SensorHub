#ifndef BME688_H
#define BME688_H

#include "Arduino.h"
#include "BME688DEFS.h"
#include "SensorHub.h"

#ifdef __cplusplus

class BME688
{
public:
    BME688();
    ~BME688();
    bool begin();
    bool begin(uint8_t mode);
    bool begin(uint8_t mode, uint8_t oss);
    double readTemperature();
    double readPressure();
    double readHumidity();
    double readGasForTemperature(uint16_t temperature);
    double readGas(uint8_t profile);
    void showLogs(bool show);
    bool setTemperatureOversampling(uint8_t oss);
    bool setPressureOversampling(uint8_t oss);
    bool setHumidityOversampling(uint8_t oss);
    void ignoreUnsafeTemperatureWarnings(bool ignore);
    // int readPressure();
    // int readHumidity();
    // int readGas();
    // int readAltitude();
    // int readAll();
    // int setPressureOversampling();
    // int setHumidityOversampling();
    // int setIIRFilter();
    // int setGasHeater();
    // int setGasHeaterTemperature();
    // int setGasHeaterDuration();
    // int setProfile();
    // int setPowerMode();
    // int setForcedMode();
    // int setNormalMode();
    // int setSleepMode();
    // int setGasStatus();
    // int setNewData();
    // int setGasHeaterProfile();
    // int setGasHeaterEnable();
    // int setGasHeaterDisable();
    // int setGasHeaterStatus();
    // int setGasHeaterDuration();
    // int setGasHeaterTemperature();
    // ~BME688();

    bool isConnected();

private:
    SensorHub sensorHub;
    uint8_t temp_oss = BME_688_OSS_1,
            press_oss = BME_688_OSS_1,
            hum_oss = BME_688_OSS_1,
            mode = BME_688_FORCED_MODE;

    bool printLogs = true;
    // iir_filter = 0,
    // gas_heater = 0,
    // gas_heater_temperature = 0,
    // gas_heater_duration = 0,
    // profile = 0,
    // power_mode = 0,
    // forced_mode = 0,
    // normal_mode = 0,
    // sleep_mode = 0,
    // gas_status = 0,
    // new_data = 0,
    // gas_heater_profile = 0,
    // gas_heater_enable = 0,
    // gas_heater_disable = 0,
    // gas_heater_status = 0;

    // CALIBRATION CONSTANTS
    int16_t par_t16[2] = {0};
    int8_t par_t3 = 0;
    uint16_t par_p1 = 0;
    int16_t par_p16[6] = {0};
    int8_t par_p8[4] = {0};
    uint16_t par_h16[2] = {0};
    int8_t par_h8[5] = {0};
    uint8_t par_h6 = 0;
    uint8_t par_p10 = 0, res_heat_range = 0;
    int8_t par_g1 = 0, par_g3 = 0, res_heat_val = 0;
    int16_t par_g2 = 0;
    bool allowHighTemps = false;

    // CALIBRATED READINGS
    double t_fine = 0, p_fine = 0, h_fine = 0, g_fine = 0, g_res = 0;

    // Gas Sensor Profile data
    uint8_t measProfile = 0, targetTemp = 0, targetWaitTime = 0;

    int32_t readRawTemp();
    int32_t readRawPres();
    int16_t readRawHum();
    int16_t readRawGas();
    double readUCTemp(int32_t adc_T);
    double readUCPres(int32_t adc_P);
    double readUCHum(int16_t adc_H);
    uint8_t readUCGas(uint16_t adc_G);
    double startGasMeasurement(uint8_t profile, uint8_t waitTime);
    bool setHeatProfiles();
    bool checkGasMeasurementCompletion();
    void printLog(String log);
    void readCalibParams();
};

#endif // __cplusplus
#endif // BME688_H