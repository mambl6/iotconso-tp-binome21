// [1.3.2] Hello Arduino World! toutes les 2 s, avec prefixe de binome
// Binome 21 - Albert & Brayane
// Aucun montage : seul le cable USB (alimentation + liaison serie).
#include <Arduino.h>

const char* PREFIX = "[Binome 21 - Albert & Brayane]";

void logln(const String& msg) {   // ajoute le prefixe sur chaque ligne
  Serial.print(PREFIX);
  Serial.print(' ');
  Serial.println(msg);
}

void setup() {
  Serial.begin(115200);
  delay(1000);                    // laisse le moniteur serie s'ouvrir
}

void loop() {
  logln("Hello Arduino World!");
  delay(2000);                    // toutes les 2 secondes
}