#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <Adafruit_BMP280.h>
#include <Wire.h>
#include "DHT.h"

#define DHTPIN 15
#define DHTTYPE DHT11

const char* ssid = "TIM-38844234";
const char* password = "B&BACASADILIDIA3";
Adafruit_BMP280 bmp;
WebServer server(80);
DHT dht(DHTPIN, DHTTYPE);
int startLed = 26;
int workingLed = 27;

void getHumidity() {
  server.send(200, "text/plain", String(dht.readHumidity()));
}

void getTemperature() {
  server.send(200, "text/plain", String(dht.readTemperature()));
}

void getPascal() {
  server.send(200, "text/plain", String(bmp.readPressure()));
}

void getAltitude() {
  server.send(200, "text/plain", String(bmp.readAltitude()));
}

void handleNotFound() {
  server.send(404, "text/plain", "endPoint sbagliato");
}

void setup(void) {
  pinMode(startLed, OUTPUT);
  pinMode(workingLed, OUTPUT);
  digitalWrite(workingLed, HIGH);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }
  digitalWrite(workingLed, LOW);
  digitalWrite(startLed, HIGH);
  
  bmp.begin(0x76);
  dht.begin();

  server.on("/humidity", getHumidity);
  server.on("/temperature", getTemperature);
  server.on("/pascal", getPascal);
  server.on("/altitude", getAltitude);
  server.onNotFound(handleNotFound);
  server.begin();
}

void loop(void) {
  server.handleClient();
  delay(2);//allow the cpu to switch to other tasks
}
