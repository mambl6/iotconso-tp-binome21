// ============================================================================
// [1.4.2] Bouton - VERSION INTERRUPTION (Programme 2)
// Binome 21 - Albert & Brayane
// Bouton sur GPIO4, pull-down externe 10 kOhm -> repos LOW, appui HIGH. Pas de LED.
//
// Difference avec le polling (Programme 1) :
//   - Polling    : loop() lit la broche en permanence -> le CPU "scrute" sans arret.
//   - Interruption : le CPU ne lit plus rien ; le materiel le "reveille" seulement
//                    quand la broche change. Le processeur est libre entre deux appuis
//                    -> il pourrait dormir (light sleep). C'est la base de l'economie
//                    d'energie etudiee en Partie 3.
// ============================================================================
#include <Arduino.h>

const char* PREFIX = "[Binome 21 - Albert & Brayane]";
const int BUTTON = 4;

volatile unsigned long counter = 0;    // modifiee dans l'ISR -> doit etre volatile
volatile bool updated = false;         // drapeau : un appui vient d'etre compte
volatile unsigned long lastIsrMs = 0;  // horodatage pour l'anti-rebond
const unsigned long DEBOUNCE = 50;     // ms

// Routine d'interruption (ISR) : la plus courte possible, placee en RAM interne
// (IRAM_ATTR) pour ne pas dependre de la flash. On n'y met PAS de Serial.print.
void IRAM_ATTR onButtonRelease() {
  unsigned long now = millis();
  if (now - lastIsrMs > DEBOUNCE) {    // ignore les rebonds du contact
    counter++;
    updated = true;
    lastIsrMs = now;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON, INPUT);              // la resistance externe assure le pull-down

  // Front descendant = relachement (repos LOW) -> on compte l'appui complet,
  // exactement comme la version polling, pour une comparaison juste.
  attachInterrupt(digitalPinToInterrupt(BUTTON), onButtonRelease, FALLING);

  Serial.print(PREFIX);
  Serial.println(" Appuis (interruption) : 0");
}

void loop() {
  // L'affichage se fait ICI (hors ISR). Le CPU n'a rien d'autre a faire :
  // il pourrait etre endormi et reveille par l'interruption.
  if (updated) {
    updated = false;
    Serial.print(PREFIX);
    Serial.print(" Appuis (interruption) : ");
    Serial.println(counter);
  }
}