// ============================================================================
// [2.2.1] Weatherstack via API : ville, heure locale, temperature, vent
// Binome 21 - Albert & Brayane
// Appel HTTP (http obligatoire en offre gratuite) + lecture JSON (ArduinoJson)
// Librairie : bblanchon/ArduinoJson | WiFi + HTTPClient inclus au framework ESP32
// ============================================================================
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "secrets.h"                              // WIFI_SSID, WIFI_PASS, WS_KEY (NON commite)

const char* PREFIX = "[Binome 21 - Albert & Brayane]";
const char* CITY   = "Paris";                     // changer si besoin (New York, Libreville, ...)

void logln(const String& m) { Serial.print(PREFIX); Serial.print(' '); Serial.println(m); }

void setup() {
    Serial.begin(115200);
    delay(1000);

    // --- 1. Connexion WiFi ---
    logln("Connexion WiFi...");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println();
    logln("WiFi connecte, IP = " + WiFi.localIP().toString());

    // --- 2. Appel API (UNE SEULE FOIS : le quota gratuit est tres limite) ---
    HTTPClient http;
    String url = String("http://api.weatherstack.com/current?access_key=") + WS_KEY + "&query=" + CITY;
    http.begin(url);
    int code = http.GET();

    if (code == 200) {
        String payload = http.getString();
        JsonDocument doc;                          // ArduinoJson v7
        DeserializationError err = deserializeJson(doc, payload);

        if (err) {
            logln(String("JSON invalide : ") + err.c_str());
        } else if (doc["success"] == false) {       // Weatherstack renvoie un HTTP 200 meme en cas d'erreur
            logln(String("Erreur API : ") + doc["error"]["info"].as<const char*>());
        } else {
            // --- 3. Lecture des champs dans l'arbre JSON (chemin depuis la racine) ---
            logln("----- Meteo " + String(doc["location"]["name"].as<const char*>()) + " -----");
            logln("Heure locale : " + String(doc["location"]["localtime"].as<const char*>()));
            logln("Temperature  : " + String(doc["current"]["temperature"].as<int>()) + " C");
            logln("Vent         : " + String(doc["current"]["wind_speed"].as<int>()) + " km/h");
        }
    } else {
        logln("Erreur HTTP : " + String(code));
    }
    http.end();

    logln("Termine. Appuyer sur EN (RESET) pour un nouvel appel.");
}

void loop() {
    // Rien : un seul appel par demarrage, pour ne pas epuiser le quota gratuit.
}