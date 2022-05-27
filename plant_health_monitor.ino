#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN 2

#define LIGHTPIN 8
#define SOILHUMIDITYPIN 4

DHT dht(DHTPIN, DHTTYPE);
float air_humidity, air_temperature, light_intensity, soil_humidity;

void setup() {
  pinMode(LIGHTPIN, INPUT);
  pinMode(SOILHUMIDITYPIN, INPUT);
  Serial.begin(9600);
  dht.begin();
}

void loop()
{
  // Reading temperature (celsius) and humidity (%)
  air_humidity = dht.readHumidity();
  air_temperature = dht.readTemperature();
  light_intensity = digitalRead(LIGHTPIN);
  soil_humidity = digitalRead(SOILHUMIDITYPIN);

  // Check if any reads failed and exit early (to try again).
  if (isnan(air_humidity) || isnan(air_temperature)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  Serial.println("Air humidity: ");
  Serial.println(air_humidity);

  Serial.println("Air temperature: ");
  Serial.println(air_temperature);

  Serial.println("Light intensity: ");
  Serial.println(light_intensity);

  Serial.println("Soil humidity: ");
  Serial.println(soil_humidity);

  // Compute heat index in Fahrenheit (the default)
  //float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  //float hic = dht.computeHeatIndex(t, h, false);

  delay(5000);
}