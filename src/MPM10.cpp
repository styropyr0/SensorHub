#include "MPM10.h"

MPM10::MPM10(uint8_t tx, uint8_t rx)
{
    this->mode = MPM10_MODE_UART;
    this->tx = tx;
    this->rx = rx;
}

MPM10::MPM10()
{
    this->mode = MPM10_MODE_I2C;
}

MPM10_DATA MPM10::readData()
{
    MPM10_DATA data;
    data.pm1_0 = 0;
    data.pm2_5 = 0;
    data.pm10 = 0;
    data.valid = false;

    if (this->mode == MPM10_MODE_UART)
    {
        Serial2.begin(MPM_10_BAUD_RATE, SERIAL_8N1, rx, tx);
        unsigned long startTime = millis();
        uint8_t index = 0;
        while (millis() - startTime < MPM10_TIMEOUT)
        {
            if (Serial2.available())
            {
                buffer[index] = Serial2.read();

                index++;
                startTime = millis();
                if (index >= MPM10_BUFFER_SIZE)
                {
                    break;
                }
            }
        }
        if (isValidData())
        {
            data.pm1_0 = (buffer[4] << 8) | buffer[5];
            data.pm2_5 = (buffer[6] << 8) | buffer[7];
            data.pm10 = (buffer[8] << 8) | buffer[9];
            data.valid = true;
        }
        Serial2.end();
    }
    else
    {
        // I2C mode
    }

    return data;
}

bool MPM10::isConnected()
{
    if (this->mode == MPM10_MODE_UART)
    {
        Serial1.begin(9600);
        Serial1.write(0x42);
        Serial1.write(0x4D);
        Serial1.end();
    }
    else
    {
        // I2C mode
    }
    return true;
}

bool MPM10::isValidData()
{
    if (buffer[0] == MPM10_FRAME_HEADER_1 && buffer[1] == MPM10_FRAME_HEADER_2)
    {
        uint16_t sum = 0;
        for (uint8_t i = 0; i < MPM10_DATA_LENGTH; i++)
            sum += buffer[i];

        uint16_t receivedChecksum = (buffer[30] << 8) | buffer[31];
        return sum == receivedChecksum;
    }

    return false;
}
