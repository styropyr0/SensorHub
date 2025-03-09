#ifndef MQGASDEFS
#define MQGASDEFS

#ifdef __cplusplus

#define MQ_TYPE_MQ2 0     // Combustible gases like LPG, propane, methane
#define MQ_TYPE_MQ3 1     // Alcohol, ethanol, benzine
#define MQ_TYPE_MQ4 2     // Methane, natural gas
#define MQ_TYPE_MQ5 3     // LPG, natural gas, methane
#define MQ_TYPE_MQ6 4     // LPG, butane, isobutane
#define MQ_TYPE_MQ7 5     // Carbon monoxide (CO)
#define MQ_TYPE_MQ8 6     // Hydrogen
#define MQ_TYPE_MQ9 7     // Carbon monoxide (CO) and flammable gases
#define MQ_TYPE_MQ131 8   // Ozone (O3)
#define MQ_TYPE_MQ135 9   // Air quality (ammonia, sulfur, benzene, etc.)
#define MQ_TYPE_MQ136 10  // Hydrogen sulfide (H2S)
#define MQ_TYPE_MQ137 11  // Ammonia (NH3)
#define MQ_TYPE_MQ138 12  // Alcohol, toluene, acetone
#define MQ_TYPE_MQ303A 13 // Alcohol (used in breathalyzers)
#define MQ_TYPE_MQ309A 14 // Carbon monoxide (CO)

// Conversion constants
#define MQ_VCC 3.3
#define MQ_ADC_RES_16 65535
#define MQ_ADC_RES_12 4095
#define MQ_ADC_RES_10 1023
#define MQ_MQ2_A 171.39
#define MQ_MQ2_B -1.46
#define MQ_MQ135_A 5.91
#define MQ_MQ135_B -0.5

// Calibration constants
#define MQ_MQ2_CAL_R0 110.0
#define MQ_MQ135_CAL_R0 30.0

#endif // __cplusplus
#endif // MQGASDEFS
