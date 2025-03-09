#include <BME688.h>
#include <APDS9960.h>
#include <ML8511.h>
#include <ADS111S.h>
#include <MAX30100.h>
#include <MQGas.h>
#include <MQGASDEFS.h>
#include <MPM10.h>

BME688 bme;
APDS9960 apds;
ADS111S ads;
ML8511 ml(2, 4);
MQGas gasInterface(&ads);
MAX30100 mx;
MPM10 mpm(17, 16);
MPM10_DATA data;

typedef struct {
  float temp;
  float pres;
  float hum;
  float gasResistance;
  float uvIntensity;
  float uvIndex;
  Color color;
  float aqi1;
  float aqi2;
} ENV_VAR;

ENV_VAR envVar;
float baselineResistance = 30000.0;

void setup() {
  Serial.begin(9600);
  bme.begin();
  apds.begin();
  apds.enableLightSensing(true);
  ads.begin();
  gasInterface.begin();
}

void loop() {
  envVar.temp = bme.readTemperature();
  envVar.pres = bme.readPressure();
  envVar.hum = bme.readHumidity();
  envVar.gasResistance = bme.readGasForTemperature(400);
  envVar.uvIntensity = ml.getUVIntensity();
  envVar.uvIndex = ml.getUVIndex();
  apds.setLightGain(2);
  envVar.color = apds.readColorData();
  envVar.aqi1 = gasInterface.read(MQ_TYPE_MQ2, 0);
  envVar.aqi2 = gasInterface.read(MQ_TYPE_MQ135, 1);
  data = mpm.readData();

  summarize();
  Serial.println("\n");

  delay(2000);
}

void summarize() {
  Serial.println("---------------------------------------------------------");
  Serial.println("DETAILED ENVIRONMENT SUMMARY");
  Serial.println("---------------------------------------------------------");

  // Temperature Assessment
  Serial.print("Temperature: ");
  Serial.println(String(envVar.temp) + " C");
  if (envVar.temp < 18) {
    Serial.println("Warning: Temperature is too low; may cause discomfort or hypothermia.");
  } else if (envVar.temp > 30) {
    Serial.println("Warning: High temperature; may lead to heat-related stress.");
  }

  Serial.println("");

  // Pressure Assessment
  Serial.print("Pressure: ");
  Serial.println(String(envVar.pres) + " Pa");
  if (envVar.pres < 90000) {
    Serial.println("Warning: Low atmospheric pressure; may cause dizziness or fatigue.");
  } else if (envVar.pres > 105000) {
    Serial.println("Warning: High atmospheric pressure; may cause discomfort for sensitive individuals.");
  }

  Serial.println("");

  // Humidity Assessment
  Serial.print("Humidity: ");
  Serial.println(String(envVar.hum) + " %");
  if (envVar.hum < 30) {
    Serial.println("Warning: Low humidity; may cause dryness in skin and respiratory issues.");
  } else if (envVar.hum > 60) {
    Serial.println("Warning: High humidity; could promote mold growth and discomfort.");
  }

  Serial.println("");

  // UV Intensity Assessment
  Serial.print("UV Intensity: ");
  Serial.println(String(envVar.uvIntensity) + " " + ml.uvi_unit);
  Serial.print("UV Index: ");
  Serial.println(String(envVar.uvIndex));
  if (envVar.uvIndex > 5) {
    Serial.println("Warning: High UV index; prolonged exposure may cause skin damage and eye strain. Use protection.");
  }

  Serial.println("");

  // Color Intensity Assessment
  Serial.print("Color Intensity (RGB): ");
  Serial.println("Red: " + String(envVar.color.red) + " Green: " + String(envVar.color.green) + " Blue: " + String(envVar.color.blue));

  int totalIntensity = envVar.color.red + envVar.color.green + envVar.color.blue;

  if (totalIntensity > 700) {
    Serial.println("Warning: Excessive light intensity detected.");

    // Specific wavelength effects
    if (envVar.color.red > 1000) {
      Serial.println("   - High Red Intensity: May cause eye strain and headaches over prolonged exposure.");
    }
    if (envVar.color.green > 800) {
      Serial.println("   - High Green Intensity: Generally less harmful but could add to visual discomfort.");
    }
    if (envVar.color.blue > 400) {
      Serial.println("   - High Blue Intensity: Blue light overexposure may disrupt sleep patterns and strain eyes.");
    }

    if (envVar.color.red > 250 && envVar.color.blue > 250) {
      Serial.println("   - Combined Red and Blue Intensity: Prolonged exposure may affect photosensitive individuals.");
    }

    Serial.println("Suggestion: Reduce exposure or wear protective glasses, especially for blue light.");
  } else {
    Serial.println("Light intensity is within a safe range.");
  }

  Serial.println("");

  // Gas Resistance and Air Quality Assessment
  Serial.print("Gas resistance: ");
  Serial.println(String(envVar.gasResistance) + " Ohm");
  int aqi = calculateAQI(envVar.gasResistance);

  // Evaluate Overall AQI
  String airQuality, impact, action;
  if (aqi <= 50) {
    airQuality = "Excellent";
    impact = "Air is best for well-being.";
    action = "No actions needed.";
  } else if (aqi <= 100) {
    airQuality = "Good";
    impact = "No significant adverse impact.";
    action = "Maintain regular ventilation.";
  } else if (aqi <= 150) {
    airQuality = "Lightly Polluted";
    impact = "Mild reduction in well-being.";
    action = "Consider ventilation.";
  } else if (aqi <= 200) {
    airQuality = "Moderately Polluted";
    impact = "Irritation potential for sensitive individuals.";
    action = "Ensure proper ventilation.";
  } else if (aqi <= 250) {
    airQuality = "Heavily Polluted";
    impact = "Potential for headaches and discomfort.";
    action = "Increase ventilation and identify pollution sources.";
  } else if (aqi <= 350) {
    airQuality = "Severely Polluted";
    impact = "Severe health effects and harmful VOC presence.";
    action = "Maximize ventilation and check contamination sources.";
  } else {
    airQuality = "Extremely Polluted";
    impact = "Serious neurotoxic risks.";
    action = "Take immediate measures to improve air quality.";
  }

  Serial.print("Air Quality (BME688 AQI): ");
  Serial.println(aqi);
  Serial.print("Air Quality Description: ");
  Serial.println(airQuality);
  Serial.print("Impact: ");
  Serial.println(impact);
  Serial.print("Suggested Action: ");
  Serial.println(action);

  Serial.println("");

  // MQ2 Sensor (Smoke) Assessment
  Serial.print("Air quality 1 (MQ2, Smoke): ");
  Serial.println(String(envVar.aqi1) + " ppm");

  if (envVar.aqi1 <= 200) {
    Serial.println("Smoke Level: No Smoke. Clean ambient conditions.");
  } else if (envVar.aqi1 <= 280) {
    Serial.println("Smoke Level: Low Smoke. Possible presence of minor combustible gases.");
  } else if (envVar.aqi1 <= 350) {
    Serial.println("Smoke Level: Moderate Smoke. Check ventilation or potential sources.");
  } else {
    Serial.println("Smoke Level: High Smoke. Hazardous conditions—ensure safety measures.");
  }

  Serial.println("");

  // MQ135 Sensor (VOC) Assessment
  Serial.print("Air quality 2 (MQ135, VOC): ");
  Serial.println(String(envVar.aqi2) + " ppm");

  if (envVar.aqi2 <= 150) {
    Serial.println("VOC Level: Minimal.\nVOC concentration is minimal, and air quality is good.");
  } else if (envVar.aqi2 <= 250) {
    Serial.println("VOC Level: Low.\nLow VOC concentration, no significant impact on health.");
  } else if (envVar.aqi2 <= 300) {
    Serial.println("VOC Level: Moderate.\nSlightly elevated VOCs, consider improving ventilation.");
  } else if (envVar.aqi2 <= 400) {
    Serial.println("Warning: High VOC concentration. May cause respiratory discomfort. Ensure proper ventilation.");
  } else {
    Serial.println("Critical: Very high VOC concentration! Prolonged exposure could harm health. Take immediate action to improve ventilation or identify pollutant sources.");
  }

  Serial.println("");

  if (data.valid) {
    // PM1.0 Assessment
    Serial.print("PM1.0 (Ultra fine particles): ");
    Serial.println(String(data.pm1_0) + " µg/m³");
    if (data.pm1_0 <= 12) {
      Serial.println("Excellent: Air is exceptionally clean and healthy.");
    } else if (data.pm1_0 <= 60) {
      Serial.println("Good: Air quality is satisfactory, minimal fine particles present.");
    } else if (data.pm1_0 <= 105) {
      Serial.println("Moderate: Fine particle concentration may affect sensitive groups.");
    } else if (data.pm1_0 <= 150) {
      Serial.println("Unhealthy: Prolonged exposure may impact general health. Avoid prolonged outdoor activities.");
    } else if (data.pm1_0 <= 250) {
      Serial.println("Very Unhealthy: Serious health effects possible for the general population. Avoid exposure.");
    } else {
      Serial.println("Hazardous: Extremely poor air quality. Prolonged exposure can cause severe health issues.");
    }
    Serial.println("");

    // PM2.5 Assessment
    Serial.print("PM2.5 (Dust, smoke particles, etc.): ");
    Serial.println(String(data.pm2_5) + " µg/m³");
    if (data.pm2_5 <= 12) {
      Serial.println("Excellent: Very low fine particulate levels, air is safe for everyone.");
    } else if (data.pm2_5 <= 60) {
      Serial.println("Good: Air is clean, with minimal health risks.");
    } else if (data.pm2_5 <= 105) {
      Serial.println("Moderate: Sensitive individuals may experience slight irritation.");
    } else if (data.pm2_5 <= 150) {
      Serial.println("Unhealthy: Increased risk of respiratory issues, especially for sensitive groups.");
    } else if (data.pm2_5 <= 250) {
      Serial.println("Very Unhealthy: Avoid outdoor activities; particulate levels are hazardous.");
    } else {
      Serial.println("Hazardous: Prolonged exposure can lead to severe respiratory or cardiovascular issues.");
    }
    Serial.println("");

    // PM10 Assessment
    Serial.print("PM10 (Dust, pollen, mould, etc): ");
    Serial.println(String(data.pm10) + " µg/m³");
    if (data.pm10 <= 20) {
      Serial.println("Excellent: Coarse particulate concentration is very low, air quality is excellent.");
    } else if (data.pm10 <= 60) {
      Serial.println("Good: Low levels of coarse particles, air quality is acceptable.");
    } else if (data.pm10 <= 105) {
      Serial.println("Moderate: Some respiratory irritation may occur for sensitive individuals.");
    } else if (data.pm10 <= 200) {
      Serial.println("Unhealthy: Prolonged exposure may affect respiratory health, especially during physical activity.");
    } else if (data.pm10 <= 350) {
      Serial.println("Very Unhealthy: Elevated health risk for everyone; avoid outdoor activities.");
    } else {
      Serial.println("Hazardous: Critical air quality, particulate levels are dangerously high.");
    }
    Serial.println("");
  }

  Serial.println("---------------------------------------------------------");
}

int calculateAQI(float gasResistance) {
  float ratio = baselineResistance / gasResistance;
  int aqi;

  if (ratio > 1.5) {
    aqi = 50;
  } else if (ratio > 1.2) {
    aqi = 100;
  } else if (ratio > 1.0) {
    aqi = 150;
  } else if (ratio > 0.8) {
    aqi = 200;
  } else if (ratio > 0.5) {
    aqi = 250;
  } else if (ratio > 0.3) {
    aqi = 350;
  } else {
    aqi = 500;
  }

  return aqi;
}
