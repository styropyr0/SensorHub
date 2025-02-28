#ifndef BME688DEFS
#define BME688DEFS

#include "Arduino.h"

#ifdef __cplusplus

#define ADC_MAX 1023.0

#if defined(ESP32) || defined(ARDUINO_ARCH_RP2040) || defined(STM32F1) || defined(STM32F4) || defined(__SAM3X8E__)
#undef ADC_MAX
#define ADC_MAX 4095.0
#endif

#define ML8511_V_ZERO 1.0
#define ML8511_UV_IRRADIANCE_FAC 1.6
#define ML8511_CALIB_INTENSITY 10.0
#define ML8511_CALIB_VOLTAGE 2.08

#endif // __cplusplus
#endif // ML8511DEFS
