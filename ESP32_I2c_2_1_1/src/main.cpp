// ============================================================================
// [2.1.1] SEN-BME680 via I2C : temperature (C et F) + humidite sur le moniteur serie
// Binome 21 - Albert & Brayane
// Bus I2C : SDA = GPIO21, SCL = GPIO22 | adresse BME680 = 0x77 (confirmee au scan)
// Librairies : Adafruit BME680 Library (+ Adafruit Unified Sensor)
// ============================================================================
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>

const char* PREFIX = "[Binome 21 - Albert & Brayane]";
Adafruit_BME680 bme;                               // par defaut en I2C (bus Wire)

void setup() {
    Serial.begin(115200);
    delay(1000);
    Wire.begin(21, 22);                            // SDA = GPIO21, SCL = GPIO22

    if (!bme.begin(0x77)) {                         // 0x77 = adresse confirmee au scan I2C
        Serial.print(PREFIX);
        Serial.println(" BME680 introuvable ! Verifier cablage / adresse (0x76 ?)");
        while (true) delay(1000);                   // on bloque tant que le capteur n'est pas vu
    }

    // --- Configuration du capteur (voir le detail de chaque ligne dans le rapport) ---
    bme.setTemperatureOversampling(BME680_OS_8X);   // moyenne de 8 mesures -> moins de bruit
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);     // filtre interne -> lisse les a-coups (temp/pression)
    bme.setGasHeater(320, 150);                     // chauffe la plaque gaz : POSTE LE PLUS COUTEUX
    // Remarque conso : ici on ne lit PAS le gaz -> setGasHeater(0, 0) economiserait de l'energie.

    Serial.print(PREFIX); Serial.println(" BME680 pret.");
}

void loop() {
    if (!bme.performReading()) {                    // declenche UNE mesure (mode force), puis le capteur se rendort
        Serial.print(PREFIX); Serial.println(" Echec de lecture");
        delay(2000);
        return;
    }

    float tC = bme.temperature;
    float tF = tC * 9.0 / 5.0 + 32.0;               // Celsius -> Fahrenheit
    float h  = bme.humidity;

    Serial.print(PREFIX);
    Serial.printf(" T = %.2f C | %.2f F | Humidite = %.1f %%\n", tC, tF, h);

    delay(2000);
}