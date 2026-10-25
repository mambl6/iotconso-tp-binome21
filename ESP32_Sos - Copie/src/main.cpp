// [1.4.2 - SOS morse] LED integree sur GPIO2, aucun montage.
// point=1, trait=3, silence symboles=1, lettres=3, mots=7 (en unites)
#include <Arduino.h>

const char* PREFIX = "[Binome 21 - Albert & Brayane]";
const int LED = 2;
const unsigned long UNIT = 200;          // duree d'une unite (ms)

// Allume N unites, puis 1 unite de silence (gap intra-lettre)
void symbol(int units) {
  digitalWrite(LED, HIGH);
  delay(units * UNIT);
  digitalWrite(LED, LOW);
  delay(UNIT);
}
void dot()  { symbol(1); }
void dash() { symbol(3); }
void letterGap() { delay(2 * UNIT); }    // 1 deja emise + 2 = 3 unites
void wordGap()   { delay(6 * UNIT); }    // 1 deja emise + 6 = 7 unites

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
}

void loop() {
  Serial.print(PREFIX); Serial.println(" Emission SOS");
  dot(); dot(); dot();      // S
  letterGap();
  dash(); dash(); dash();   // O
  letterGap();
  dot(); dot(); dot();      // S
  wordGap();
}