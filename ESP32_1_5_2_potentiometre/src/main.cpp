// ============================================================================
// [1.5.2] ADC - Programme 3 : POTENTIOMETRE (balayage 0 a 4095) -- lecture mediane
// Binome 21 - Albert & Brayane
//
// Montage : un potentiometre EST un pont diviseur reglable (3 bornes) :
//   borne 1 -> 3V3 ; borne 3 -> GND ; curseur (broche du MILIEU) -> GPIO36.
//
// Probleme observe : les pattes rondes du potentiometre tiennent mal dans la
// breadboard, le contact du curseur "glitche" parfois vers 4095. Solution :
// on lit la MEDIANE de plusieurs echantillons -> un glitch isole est rejete
// (filtrage de bruit). Le potentiometre couvrant deja toute l'echelle, aucune
// calibration n'est necessaire : un simple map(0..4095 -> 0..255) suffit.
// ============================================================================
#include <Arduino.h>

const char* PREFIX = "[Binome 21 - Albert & Brayane]";
const int POT = 36;                 // GPIO36 = VP = ADC1_CH0 (meme broche que la LDR)
const int N   = 9;                  // nb d'echantillons pour la mediane (impair)

// Lit N mesures et renvoie la valeur du milieu (mediane) : robuste aux glitches.
int lireMediane() {
  int v[N];
  for (int i = 0; i < N; i++) {
    v[i] = analogRead(POT);
    delayMicroseconds(500);
  }
  for (int i = 0; i < N - 1; i++)           // petit tri (N est petit)
    for (int j = i + 1; j < N; j++)
      if (v[j] < v[i]) { int t = v[i]; v[i] = v[j]; v[j] = t; }
  return v[N / 2];                           // mediane
}

void setup() {
  Serial.begin(115200);
}

void loop() {
  // int brut = analogRead(POT);            // lecture simple (sensible aux glitches)
  int brut   = lireMediane();               // lecture mediane (robuste au mauvais contact)
  int niveau = map(brut, 0, 4095, 0, 255);  // mise a l'echelle directe (pas de calibration)

  Serial.print(PREFIX);
  Serial.print(" POT (brut 0-4095) : "); Serial.print(brut);
  Serial.print("   | (0-255) : ");        Serial.println(niveau);

  delay(300);
}