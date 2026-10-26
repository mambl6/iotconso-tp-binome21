// [1.5.3.1] LM35 : température réelle (°C) — Binôme 21 - Albert & Brayane
// Montage : +Vs -> VIN (~5V, PAS 3V3, le LM35 exige >= 4V) ; Vout -> GPIO36 ; GND -> GND
// T(°C) = Vout_mV / 10   (datasheet TI SNIS159, p.13 §7.3.1)
#include <Arduino.h>

const char* PREFIX = "[Binome 21 - Albert & Brayane]";
const int LM35 = 36;          // GPIO36 = VP = ADC1_CH0 ; relié à Vout du LM35
const int N    = 16;          // moyenne pour lisser le bruit de l'ADC

void setup() {
  Serial.begin(115200);
}

void loop() {
  uint32_t somme = 0;
  for (int i = 0; i < N; i++) {           // moyenne de N lectures calibrées
    somme += analogReadMilliVolts(LM35);
    delay(2);
  }
  float vout_mV = somme / (float)N;       // tension calibrée moyenne (mV)
  float tempC   = vout_mV / 10.0;         // LM35 : 10 mV/°C, pas d'offset

  Serial.print(PREFIX);
  Serial.print(" LM35 : Vout = "); Serial.print(vout_mV, 1); Serial.print(" mV");
  Serial.print("  ->  T = ");      Serial.print(tempC, 1);   Serial.println(" C");

  delay(1000);
}