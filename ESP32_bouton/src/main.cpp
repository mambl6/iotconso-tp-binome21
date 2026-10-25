// [1.4.2 - Bouton, polling] Compte les appuis complets (presser puis relacher)
// Bouton sur GPIO4, pull-down externe 10k -> repos LOW, appui HIGH. Pas de LED.
#include <Arduino.h>

const char* PREFIX = "[Binome 21 - Albert & Brayane]";
const int BUTTON = 4;

int lastReading = LOW;                 // derniere lecture brute
int stableState = LOW;                 // etat stable apres anti-rebond
unsigned long lastChange = 0;
const unsigned long DEBOUNCE = 30;     // ms
unsigned long counter = 0;

void logCounter() {
  Serial.print(PREFIX);
  Serial.print(" Appuis complets : ");
  Serial.println(counter);
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON, INPUT);              // la resistance externe fait le pull-down
  logCounter();                        // affiche 0 au demarrage
}

void loop() {
  int reading = digitalRead(BUTTON);

  if (reading != lastReading) {        // le signal vient de bouger
    lastChange = millis();             // (re)demarre le chrono anti-rebond
    lastReading = reading;
  }

  if (millis() - lastChange > DEBOUNCE) {   // stable depuis assez longtemps ?
    if (reading != stableState) {
      stableState = reading;
      if (stableState == LOW) {        // front descendant = relache -> on compte
        counter++;
        logCounter();
      }
    }
  }
}