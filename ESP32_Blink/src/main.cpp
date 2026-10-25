// [1.4.2 - Blink] Parametres du binome N = 21
// Ton = 1050 ms, periode = 2000 ms (eteinte 950 ms), rapport cyclique 52,5 %
// LED integree sur GPIO2 : aucun montage externe.
#include <Arduino.h>

const char* PREFIX = "[Binome 21 - Albert & Brayane]";
const int LED = 2;
const unsigned long T_ON   = 1050;              // 50 x N ms
const unsigned long PERIOD = 2000;              // fixe pour tous
const unsigned long T_OFF  = PERIOD - T_ON;     // 950 ms

void setup() {
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  delay(1000);
  Serial.print(PREFIX);
  Serial.println(" Blink : Ton=1050 ms, periode=2000 ms, rapport cyclique 52.5%");
}

void loop() {
  digitalWrite(LED, HIGH);
  delay(T_ON);
  digitalWrite(LED, LOW);
  delay(T_OFF);
}