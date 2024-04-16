#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>

#define DHTPIN 21 
#define DHTTYPE DHT11 

DHT_Unified dht(DHTPIN, DHTTYPE); 

uint32_t delayMS; 

const char* ssid = "iPhone 13 Paul";
const char* password = "haricot9";
const char* url = "https://hook.eu2.make.com/ggtizg0u4y2el4bzaw9lc5wr68mf93e5";

unsigned long previousMillis = 0; 
const long interval = 30000; 

struct Data {
  float temperature;
  float humidity;
  float difference;
};

Data storedData[48];
int dataIndex = 0;

void recordData(float temperature, float humidity);
void sendData();

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
    dht.humidity().getEvent(&humidity_event);

    if(!isnan(temp_event.temperature) && !isnan(humidity_event.relative_humidity)){
      recordData(temp_event.temperature, humidity_event.relative_humidity);
      sendData();
    }
  }
}

void recordData(float temp, float humi) {
  storedData[dataIndex].temperature = temp;
  storedData[dataIndex].humidity = humi;
  storedData[dataIndex].difference = 0;
  if (dataIndex > 0) {
    for (int i = 0; i < dataIndex; i++) {
      storedData[i].difference += 30;
    }
  }
  dataIndex++;
  if (dataIndex == 48) {
    dataIndex = 47;
  }
  Serial.print("Index des données: ");
  Serial.print(dataIndex);
  Serial.print("Différence de l'index: ");
  Serial.println(storedData[dataIndex].difference);
  Serial.print(F("Temperature: "));
  Serial.print(temp);
  Serial.println(F("°C"));
  Serial.print(F("Humidity: "));
  Serial.print(humi);
  Serial.println(F("%"));
}

void sendData(){
  if (WiFi.status() != WL_CONNECTED){
    int tries = 0;
    while (WiFi.status() != WL_CONNECTED && tries < 10) {
      Serial.println("Tentative de connexion au réseau WiFi...");
      WiFi.begin(ssid, password);
      delay(1000);
      tries++;
    }
  }
  if(WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    for (int i = 0; i < dataIndex; i++) {
      float temp = storedData[i].temperature;
      float humi = storedData[i].humidity;
      float diff = storedData[i].difference;
      String urlWithParams = String(url) + "?temp=" + String(temp, 2) + "&humi=" + String(humi, 2) + "&diff=" + String(diff, 2);
      http.begin(urlWithParams);
      Serial.println(urlWithParams);
      int httpResponseCode = http.GET(); 
      if(httpResponseCode > 0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
        http.end();
      } else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
        http.end(); 
      }
    }
    dataIndex = 0;
  }
}