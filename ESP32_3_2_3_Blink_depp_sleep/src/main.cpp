// ============================================================================
// [3.2.3] Blink avec DEEP SLEEP - reveil toutes les 6,25 s (Tsleep = 1 + 0.25 x 21)
// Binome 21 - Albert & Brayane
// En DEEP SLEEP : presque tout est coupe, la RAM est PERDUE. Au reveil, la puce
// REDEMARRE a zero (un BOOT complet) -> setup() recommence depuis le debut.
//   -> c'est pour ca que le code du blink est dans setup(), pas dans loop().
//   -> chaque reveil = un boot COUTEUX (pic de courant), d'ou un courant moyen
//      qui peut depasser le light sleep si on se reveille souvent.
// ============================================================================
#include <Arduino.h>
#include "esp_sleep.h"

const int LED = 2;
const unsigned long TON       = 1050;              // ms allumee
const uint64_t      TSLEEP_US = 6250000ULL;        // 6,25 s en microsecondes

void setup() {
  pinMode(LED, OUTPUT);

  // A chaque reveil (= chaque boot) : on fait le blink, puis on repart dormir.
  digitalWrite(LED, HIGH);
  delay(TON);                                      // 1050 ms : LED on
  digitalWrite(LED, LOW);

  esp_sleep_enable_timer_wakeup(TSLEEP_US);        // reveil dans 6,25 s
  esp_deep_sleep_start();                          // deep sleep -> au reveil, REBOOT
}

void loop() {
  // Jamais atteint : le deep sleep redemarre toujours par setup().
}