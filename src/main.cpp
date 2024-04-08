#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <HTTPClient.h>

#define DHTPIN 21
#define DHTTYPE DHT11

DHT_Unified dht(DHTPIN, DHTTYPE);

uint32_t delayMS;

const char* ssid = "SSID";
const char* password = "PASSWORD";
const char* url = "https://hook.eu2.make.com/id";

unsigned long previousMillis = 0;
const long interval = 600000; // 30 minutes

void sendData(float temperature, float humidity);

void setup() {
  Serial.begin(9600);
  dht.begin();
  Serial.println(F("DHTxx Unified Sensor Example"));
  sensor_t sensor;
  dht.temperature().getSensor(&sensor);
  Serial.println(F("------------------------------------"));
  Serial.println(F("Temperature Sensor"));
  Serial.print(F("Sensor:       ")); Serial.println(sensor.name);
  Serial.println(F("------------------------------------"));
  dht.humidity().getSensor(&sensor);
  Serial.println(F("Humidity Sensor"));
  Serial.print(F("Sensor:       ")); Serial.println(sensor.name);
  Serial.println(F("------------------------------------"));
  delayMS = sensor.min_delay / 1000;
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sensors_event_t temp_event, humidity_event;
    dht.temperature().getEvent(&temp_event);
    if (isnan(temp_event.temperature)) {
      Serial.println(F("Error reading temperature!"));
    }
    else {
      Serial.print(F("Temperature: "));
      Serial.print(temp_event.temperature);
      Serial.println(F("°C"));
    }
    dht.humidity().getEvent(&humidity_event);
    if (isnan(humidity_event.relative_humidity)) {
      Serial.println(F("Error reading humidity!"));
    }
    else {
      Serial.print(F("Humidity: "));
      Serial.print(humidity_event.relative_humidity);
      Serial.println(F("%"));
      sendData(temp_event.temperature, humidity_event.relative_humidity); // Envoyer les données à Google Sheets
    }
  }
}

void sendData(float temperature, float humidity) {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  HTTPClient http;
  String urlWithParams = String(url) + "?temp=" + String(temperature, 2) + "&humi=" + String(humidity, 2);
  http.begin(urlWithParams);

  int httpResponseCode = http.GET();
  if(httpResponseCode > 0) {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
  } else {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }

  http.end();
}