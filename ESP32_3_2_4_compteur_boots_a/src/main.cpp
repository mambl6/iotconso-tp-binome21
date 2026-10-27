// ============================================================================
// [3.2.4] Compteur de boots - VERSION 1 : variable globale ORDINAIRE
// Binome 21 - Albert & Brayane
// On ajoute un compteur au programme deep sleep du 3.2.3.
// But de cette 1re version (demandee par l'enonce) : MONTRER que la variable
// globale NE survit PAS au deep sleep -> elle repart a 0 a chaque reveil, donc
// le compteur affiche toujours "boot #1". C'est la preuve que la RAM est perdue.
// ============================================================================
#include <Arduino.h>
#include "esp_sleep.h"

int bootCount = 0;                            // RAM normale -> EFFACEE en deep sleep

const char*         PREFIX    = "[Binome 21 - Albert & Brayane]";
const int           LED       = 2;
const unsigned long TON       = 1050;         // blink 1050 ms (50 x N)
const uint64_t      TSLEEP_US = 6250000ULL;   // 6,25 s (1 + 0.25 x N)

void setup() {
  Serial.begin(115200);
  delay(300);
  pinMode(LED, OUTPUT);

  bootCount++;                                 // +1 a chaque demarrage
  Serial.print(PREFIX);
  Serial.print(" boot #"); Serial.print(bootCount);
  Serial.println(" - (variable globale ordinaire)");

  digitalWrite(LED, HIGH); delay(TON); digitalWrite(LED, LOW);   // le blink du 3.2.3

  esp_sleep_enable_timer_wakeup(TSLEEP_US);
  esp_deep_sleep_start();                       // au reveil -> setup() recommence
}

void loop() { }                                 // jamais atteint