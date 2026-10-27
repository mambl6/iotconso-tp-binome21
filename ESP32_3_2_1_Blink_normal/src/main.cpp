// ============================================================================
// [3.2.1] Blink NORMAL - reference de consommation
// Binome 21 - Albert & Brayane
// Ton = 1050 ms (50 x 21), periode 2000 ms -> eteinte 950 ms. LED sur GPIO2.
// Mode normal : le CPU reste ACTIF en permanence (delay = attente active).
// -> courant de reference, le plus eleve.
// ============================================================================
#include <Arduino.h>

const int LED = 2;                       // GPIO2 = LED integree (D2)
const unsigned long TON     = 1050;      // ms allumee  (50 x N)
const unsigned long PERIODE = 2000;      // ms periode (identique pour tous)

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);
  delay(TON);                            // 1050 ms : LED on, CPU actif
  digitalWrite(LED, LOW);
  delay(PERIODE - TON);                  // 950 ms : LED off, CPU TOUJOURS actif
}