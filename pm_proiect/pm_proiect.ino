#include <LiquidCrystal_I2C.h>
#include "DHT.h"

#define DHTTYPE DHT11
#define DHTPIN 2
#define LIGHTPIN 8
#define SOILHUMIDITYPIN A0
#define LEDPIN 13
#define MAXDARKTIME 36000000 // 10 h

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
DHT dht(DHTPIN, DHTTYPE);

// Values of parameters
float air_humidity, air_temperature, light_intensity, soil_humidity;
// State of parameters
bool good_temp, good_light, good_soil, plant_mood;
int dark_start_time = 0;

void setup()
{
  // Initialize the lcd
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);

  // Initialize the DHT sensor
  dht.begin();

  // Set the pin for light presence on input
  pinMode(LIGHTPIN, INPUT);

  // Set the pin for soil humidity reading on input
  pinMode(SOILHUMIDITYPIN, INPUT);

  // Set the pin for the led on output
  pinMode(LEDPIN, OUTPUT);
  
  Serial.begin(9600);
}

void print_details_on_lcd(bool good_temp, bool good_light, bool good_soil, bool plant_mood) {
  lcd.setCursor(1,0);
  
  // Air temperature details
  if (good_temp) {
    lcd.print("t-good  ");
  } else {
    lcd.print("t->bad  ");
  }

  // Light presence details
  if (good_light) {
    lcd.print("l-good  ");
  } else {
    lcd.print("l->bad  ");
  }
  
  lcd.setCursor(0,1);

  // Soil humidity details
  if (good_soil) {
    lcd.print("s-good  ");
  } else {
    lcd.print("s->bad  ");
  }

  // Plant mood details
  if (plant_mood) {
    lcd.print("plant-:)");
  } else {
    lcd.print("plant-:(");
  }
}

void loop()
{
  // Read air temperature (celsius) and humidity (%)
  air_humidity = dht.readHumidity();
  air_temperature = dht.readTemperature();
  
  // Check for light presence
  light_intensity = digitalRead(LIGHTPIN);
  
  // Read soil humidity considering reading error
  float sum = 0;
  for (int i = 0; i < 500; ++i) {
    sum += analogRead(SOILHUMIDITYPIN);
  }
  soil_humidity = sum / 500;

  Serial.println("Air temperature: ");
  Serial.println(air_temperature);

  Serial.println("Light intensity: ");
  Serial.println(light_intensity);

  Serial.println("Soil humidity: ");
  Serial.println(soil_humidity);

  // Set air temperature state
  // Air temperature should be between 18 and 28 Celsius
  if (air_temperature < 18.0 || air_temperature > 28.0)
    good_temp = false;
  else
    good_temp = true;

  // Set the light presence state
  // Measuring current time and see if the plant has spent more than 10 hours in darkness
  int current_time = millis();
  if (light_intensity != 1 && dark_start_time + MAXDARKTIME >= current_time) {
    good_light = false;
  }
  else {
    good_light = true;
    dark_start_time = millis();
  }

  // Set the soil humidity state
  // If the soil humidity is bigger than 150 the plant needs water
  if (soil_humidity < 150)
    good_soil = true;
  else
    good_soil = false;

  // Set the plant state
  plant_mood = good_temp & good_light & good_soil;

  // Print on LCD
  print_details_on_lcd(good_temp, good_light, good_soil, plant_mood);

  // Turn on/off the led based on plant state
  if (!plant_mood) {
    digitalWrite(LEDPIN, HIGH);
  } else {
    digitalWrite(LEDPIN, LOW);
  }

  // Delay measuring to 30 min
  delay(1800000);
}
