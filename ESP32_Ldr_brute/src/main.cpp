// ============================================================================
// [1.5.2] ADC - Programme 1 : lecture BRUTE de la LDR (0 a 4095), toutes les 1 s
// Binome 21 - Albert & Brayane
//
// Montage - pont diviseur :
//   3V3 ---[ LDR ]---+---[ R_fixe 10k ]--- GND
//                    |
//                    +--> GPIO36  (VP / A0 / ADC1_CH0)
//
// Plus de lumiere -> R_LDR diminue -> tension du noeud monte -> valeur ADC plus grande.
// GPIO36 = ADC1 : utilisable meme avec le Wi-Fi (ADC2 est bloque par le Wi-Fi).
// ============================================================================
#include <Arduino.h>

const char* PREFIX = "[Binome 21 - Albert & Brayane]";
const int LDR = 36;              // GPIO36 = VP = ADC1_CH0 (broche entree-seule)

void setup() {
  Serial.begin(115200);
  // Reglages par defaut sur ESP32 (laisses explicites pour le rapport) :
  // analogReadResolution(12);         // 12 bits -> plage 0..4095
  // analogSetAttenuation(ADC_11db);   // pleine echelle ~3.3 V (defaut Arduino-ESP32)
}

void loop() {
  int brut = analogRead(LDR);          // conversion analogique -> numerique (0..4095)

  Serial.print(PREFIX);
  Serial.print(" LDR (brut 0-4095) : ");
  Serial.println(brut);

  delay(1000);                         // une mesure par seconde
}