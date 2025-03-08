#ifndef BME688DEFS
#define BME688DEFS

#ifdef __cplusplus

#define ADC_MAX 1023.0

#if defined(ESP32) || defined(ARDUINO_ARCH_RP2040) || defined(STM32F1) || defined(STM32F4) || defined(__SAM3X8E__)
#undef ADC_MAX
#define ADC_MAX 4095.0
#endif

// Calibration constants
#define ML8511_V_ZERO 0.87
#define ML8511_V_10 2.08
#define ML8511_V_15 2.75
#define ML8511_UV_IRRADIANCE_FAC 1.6
#define ML8511_CALIB_INTENSITY 10.0
#define ML8511_CALIB_VOLTAGE 2.2
#define ML8511_V_REF 3.3

#define ML8511_MED_DEFAULT 250

// Over-sampling settings
#define ML8511_OSS_0 0x01
#define ML8511_OSS_1 0x05
#define ML8511_OSS_2 0x0A
#define ML8511_OSS_3 0x0F

// Units
#define mW_Per_Sq_cm 0x01
#define W_Per_Sq_m 0x02
#define microW_Per_Sq_cm 0x03
#define Joules_s_Per_Sq_m 0x04
#define mJoules_s_Per_Sq_cm 0x05
#define Joules_s_Per_Sq_cm 0x06
#define MED 0x07

// Units string values
#define MWPSQCM "mW/cm2"
#define WPSQM "W/m2"
#define MUWPSQCM "μW/cm2"
#define JSPSQM "Js/m2"
#define MJSPSQCM "mJs/cm2"
#define JSPSQCM "Js/cm2"
#define MED_U 'MED'

#endif // __cplusplus
#endif // ML8511DEFS
