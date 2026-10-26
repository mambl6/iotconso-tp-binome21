// ============================================================================
// [ORIGINAL - exemple ADC fourni par le prof] -- conserve en commentaire
// Un projet ne peut avoir qu'un seul setup() et un seul loop() :
// l'ancien code est donc commente en bloc, pas ligne par ligne.
// ============================================================================
/*
// See https://docs.espressif.com/projects/arduino-esp32/en/latest/api/adc.html
// Example of using ADC on ESP32 with Arduino framework
#include <Arduino.h>
#include <driver/adc.h>

void setup() {
    Serial.begin(115200);
    // Configure ADC on GPIO 36 (ADC1_CH0)
    analogSetPinAttenuation(36, (adc_attenuation_t)ADC_ATTEN_DB_12);  // Set attenuation to 12dB (max ~3.1V)
    analogSetWidth(12);                                               // Set ADC resolution to 12 bits (0-4095)
}

void loop() {
    // Read raw ADC value (0-4095)
    int value = analogRead(36);
    Serial.println("Value: " + String(value));

    // Convert raw value to millivolts (150-3100mV with 11dB or 12dB attenuation with ESP32)
    float voltage = (float)value * (3100.0 - 150.0) / 4095.0 + 150.0;

    // Directly read voltage in millivolts
    float direct_voltage = analogReadMilliVolts(36);

    // Print calculated and direct millivolt values
    Serial.println("Millivolts: " + String(voltage));
    Serial.println("Direct Millivolts: " + String(direct_voltage));

    delay(1000);  // Wait 1 second
}
*/

// ============================================================================
// [NOUVEAU - 2.1] Scanner du bus I2C (BME680 + LCD sur SDA=21 / SCL=22)
// Binome 21 - Albert & Brayane
// ============================================================================
#include <Arduino.h>
#include <Wire.h>

const char* PREFIX = "[Binome 21 - Albert & Brayane]";

void setup() {
    Serial.begin(115200);
    delay(1000);                              // laisse le moniteur serie s'ouvrir
    Wire.begin(21, 22);                       // SDA = GPIO21, SCL = GPIO22
    Serial.print(PREFIX); Serial.println(" Scan du bus I2C...");
}

void loop() {
    int found = 0;
    for (uint8_t addr = 1; addr < 127; addr++) {
        Wire.beginTransmission(addr);
        if (Wire.endTransmission() == 0) {    // 0 = un composant a acquitte (ACK)
            Serial.print(PREFIX);
            Serial.printf(" Composant trouve a l'adresse 0x%02X\n", addr);
            found++;
        }
    }
    Serial.print(PREFIX);
    Serial.printf(" Scan termine : %d composant(s)\n", found);
    delay(5000);                              // nouveau scan toutes les 5 s
}