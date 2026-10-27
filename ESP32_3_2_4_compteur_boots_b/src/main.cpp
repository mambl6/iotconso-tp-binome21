// ============================================================================
// [3.2.4] Compteur de boots - VERSION 2 : memoire RTC + cause du demarrage
// Binome 21 - Albert & Brayane
// 2 changements par rapport a la V1 :
//   (1) la variable passe en RTC_DATA_ATTR -> conservee pendant le deep sleep,
//       donc le compteur SURVIT et grimpe reellement (#1, #2, #3...).
//   (2) on imprime esp_reset_reason() : POWERON au 1er demarrage, DEEPSLEEP ensuite.
// ============================================================================
#include <Arduino.h>
#include "esp_sleep.h"

// int bootCount = 0;                         // V1 : RAM normale (perdue au reveil)
RTC_DATA_ATTR int bootCount = 0;              // V2 : memoire RTC -> SURVIT au deep sleep

const char*         PREFIX    = "[Binome 21 - Albert & Brayane]";
const int           LED       = 2;
const unsigned long TON       = 1050;
const uint64_t      TSLEEP_US = 6250000ULL;   // 6,25 s

void setup() {
  Serial.begin(115200);
  delay(300);
  pinMode(LED, OUTPUT);

  bootCount++;
  esp_reset_reason_t cause = esp_reset_reason();            // AJOUT V2 : pourquoi on a demarre

  Serial.print(PREFIX);
  Serial.print(" boot #"); Serial.print(bootCount);
  Serial.print(" - cause = ");
  Serial.println(cause == ESP_RST_DEEPSLEEP ? "ESP_RST_DEEPSLEEP (reveil deep sleep)"
               : cause == ESP_RST_POWERON   ? "ESP_RST_POWERON (demarrage a froid)"
                                            : "autre");

  digitalWrite(LED, HIGH); delay(TON); digitalWrite(LED, LOW);

  esp_sleep_enable_timer_wakeup(TSLEEP_US);
  esp_deep_sleep_start();
}

void loop() { }