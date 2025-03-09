# SensorHub Library (BETA)

The SensorHub library is an Arduino library designed to interface with a variety of environmental sensors. It provides a unified API to read data from multiple sensors, including temperature, humidity, pressure, gas concentration, UV intensity, and particulate matter. This library simplifies the process of integrating multiple sensors into a single project, making it easier to develop comprehensive environmental monitoring systems.

## Supported Sensors

- **BME688**: Temperature, pressure, humidity, and gas resistance sensor.
- **APDS9960**: Color, proximity, ambient light, and gesture sensor.
- **ML8511**: UV intensity sensor.
- **ADS111S**: 16-bit ADC for high-precision analog readings. Microcontroller could delegate the ADC processing to ADS111 series ADC for higher resolution data.
- **MAX30100**: Pulse oximeter and heart-rate sensor.
- **MQGas**: Various MQ series gas sensors (e.g., MQ2, MQ135).
- **MPM10**: Particulate matter sensor (PM1.0, PM2.5, PM10).

## Features

- Unified API for multiple sensors.
- High-precision analog readings using ADS111S.
- Support for I2C and UART communication.
- Comprehensive environmental data collection.
- Easy integration with Arduino projects.

## Installation

1. Download the SensorHub library.
2. Extract the contents to your Arduino libraries folder.
3. Include the library in your Arduino sketch:

## Example Usage

```cpp
#include <BME688.h>
#include <APDS9960.h>
#include <ML8511.h>
#include <ADS111S.h>
#include <MAX30100.h>
#include <MQGas.h>
#include <MPM10.h>

#include <BME688.h>
#include <APDS9960.h>
#include <ML8511.h>
#include <ADS111S.h>
#include <MAX30100.h>
#include <MQGas.h>
#include <MPM10.h>

BME688 bme;
APDS9960 apds;
ADS111S ads;
ML8511 ml(2, 4);
MQGas gasInterface(&ads);
MAX30100 mx;
MPM10 mpm(17, 16);
MPM10_DATA data;

void setup() {
  Serial.begin(9600);
  bme.begin();
  apds.begin();
  apds.enableLightSensing(true);
  ads.begin();
  gasInterface.begin();
}

void loop() {
  float temp = bme.readTemperature();
  float pres = bme.readPressure();
  float hum = bme.readHumidity();
  float gasResistance = bme.readGasForTemperature(400);
  float uvIntensity = ml.getUVIntensity();
  float uvIndex = ml.getUVIndex();
  Color color = apds.readColorData();
  float aqi1 = gasInterface.read(MQ_TYPE_MQ2, 0);
  float aqi2 = gasInterface.read(MQ_TYPE_MQ135, 1);
  data = mpm.readData();

  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" C");

  Serial.print("Pressure: ");
  Serial.print(pres);
  Serial.println(" Pa");

  Serial.print("Humidity: ");
  Serial.print(hum);
  Serial.println(" %");

  Serial.print("Gas Resistance: ");
  Serial.print(gasResistance);
  Serial.println(" Ohm");

  Serial.print("UV Intensity: ");
  Serial.print(uvIntensity);
  Serial.println(" mW/cm2");

  Serial.print("UV Index: ");
  Serial.println(uvIndex);

  Serial.print("Color (RGB): ");
  Serial.print("Red: ");
  Serial.print(color.red);
  Serial.print(" Green: ");
  Serial.print(color.green);
  Serial.print(" Blue: ");
  Serial.println(color.blue);

  Serial.print("Air Quality 1 (MQ2): ");
  Serial.print(aqi1);
  Serial.println(" ppm");

  Serial.print("Air Quality 2 (MQ135): ");
  Serial.print(aqi2);
  Serial.println(" ppm");

  if (data.valid) {
    Serial.print("PM1.0: ");
    Serial.print(data.pm1_0);
    Serial.println(" µg/m³");

    Serial.print("PM2.5: ");
    Serial.print(data.pm2_5);
    Serial.println(" µg/m³");

    Serial.print("PM10: ");
    Serial.print(data.pm10);
    Serial.println(" µg/m³");
  }

  delay(2000);
}