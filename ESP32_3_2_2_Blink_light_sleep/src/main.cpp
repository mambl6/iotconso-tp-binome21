// ============================================================================
// [3.2.2] Blink avec LIGHT SLEEP - meme parametres
// Binome 21 - Albert & Brayane
// Pendant la phase ETEINTE (950 ms), le CPU passe en LIGHT SLEEP :
//   - horloges CPU coupees, mais RAM + peripheriques conserves ;
//   - reveil par timer -> le programme REPREND ou il s'etait arrete (loop continue).
// -> courant moyen nettement plus bas que le mode normal.
// ============================================================================
#include <Arduino.h>
#include "esp_sleep.h"

const int LED = 2;
const unsigned long TON     = 1050;
const unsigned long PERIODE = 2000;

void setup() {
  pinMode(LED, OUTPUT);
}

void loop() {
  digitalWrite(LED, HIGH);
  delay(TON);                                        // 1050 ms : LED on, CPU actif

  digitalWrite(LED, LOW);
  // Light sleep pendant la phase eteinte (reveil apres 950 ms)
  esp_sleep_enable_timer_wakeup((uint64_t)(PERIODE - TON) * 1000ULL);  // en microsecondes
  esp_light_sleep_start();                           // dort ~950 ms, etat conserve
}