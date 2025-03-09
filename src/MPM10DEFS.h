#ifndef MPM10DEFS
#define MPM10DEFS

#ifdef __cplusplus

#define MPM10_BUFFER_SIZE 32
#define MPM10_FRAME_HEADER_1 0x42
#define MPM10_FRAME_HEADER_2 0x4D
#define MPM10_DATA_LENGTH 30

#define MPM10_MODE_UART 0x00
#define MPM10_MODE_I2C 0x01

#define MPM_10_BAUD_RATE 9600
#define MPM10_TIMEOUT 1000

#endif // __cplusplus
#endif // MPM10DEFS