// ============================================================================
// [2.3] Push data to the world : Adafruit IO (MQTT)
// Binome 21 - Albert & Brayane
//
// L'ESP32 PUBLIE la temperature du BME680 vers un "feed" Adafruit IO, via MQTT.
// Difference avec 2.2 (HTTP) : ici on ne "demande" rien, on PUBLIE sur un topic
// aupres d'un BROKER (io.adafruit.com) qui decouple l'emetteur des abonnes
// (publish/subscribe). Port 1883 = non chiffre (8883 = TLS).
//
// secrets.h doit contenir : WIFI_SSID, WIFI_PASS, AIO_USERNAME, AIO_KEY  (hors Git).
// Prerequis : creer un feed nomme "temperature" sur io.adafruit.com.
// ============================================================================
#include <Arduino.h>
#include <WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include "Adafruit_BME680.h"
#include "secrets.h"

const char* PREFIX = "[Binome 21 - Albert & Brayane]";

#define AIO_SERVER      "io.adafruit.com"
#define AIO_SERVERPORT  1883                 // 1883 = non chiffre ; 8883 = TLS

WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);

// Feed : <username>/feeds/temperature  (cree-le sur io.adafruit.com)
Adafruit_MQTT_Publish tempFeed = Adafruit_MQTT_Publish(&mqtt, AIO_USERNAME "/feeds/temperature");

Adafruit_BME680 bme;

void connectWiFi() {
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print(PREFIX); Serial.print(" Connexion Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) { delay(500); Serial.print("."); }
  Serial.println();
  Serial.print(PREFIX); Serial.print(" WiFi OK, IP = "); Serial.println(WiFi.localIP());
}

void connectMQTT() {
  if (mqtt.connected()) return;
  Serial.print(PREFIX); Serial.print(" Connexion a Adafruit IO");
  int8_t ret;
  while ((ret = mqtt.connect()) != 0) {        // 0 = connecte
    Serial.print(" ["); Serial.print(mqtt.connectErrorString(ret)); Serial.print("]");
    mqtt.disconnect();
    delay(3000);
  }
  Serial.println();
  Serial.print(PREFIX); Serial.println(" Connecte a Adafruit IO.");
}

void setup() {
  Serial.begin(115200);
  delay(300);
  if (!bme.begin(0x77)) { Serial.print(PREFIX); Serial.println(" BME680 introuvable (I2C)"); }
  bme.setTemperatureOversampling(BME680_OS_8X);
  bme.setGasHeater(0, 0);                       // chauffe gaz coupee
  connectWiFi();
}

void loop() {
  connectMQTT();

  float t = NAN;
  if (bme.performReading()) t = bme.temperature;

  Serial.print(PREFIX); Serial.print(" Publication temperature = ");
  Serial.print(t, 1); Serial.print(" C -> ");
  if (tempFeed.publish(t)) Serial.println("OK");
  else                     Serial.println("ECHEC");

  delay(10000);                                 // 1 point / 10 s (< limite 30/min)
}