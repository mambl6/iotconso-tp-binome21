// ============================================================================
// [1.5.2] ADC - Programme 2 : LDR CALIBREE (0 a 255)
// Binome 21 - Albert & Brayane
//
// Meme montage que le Programme 1 (pont diviseur, noeud -> GPIO36).
// Methode de l'exemple Arduino "Calibration" :
//   - pendant une fenetre au demarrage, on releve le MIN et le MAX reels
//     en CACHANT la LDR (main -> min) PUIS en l'ECLAIRANT (flash colle -> max) ;
//   - ensuite, map() ramene la plage [min..max] mesuree sur [0..255],
//     et constrain() borne le resultat par securite.
//
// IMPORTANT : il faut atteindre les DEUX extremes pendant la fenetre, sinon la
// plage reste trop etroite et presque tout se retrouve ecrase a 0 ou a 255.
// Amelioration : la plage min/max s'affiche EN DIRECT pour voir qu'elle s'elargit.
// ============================================================================
#include <Arduino.h>

const char* PREFIX = "[Binome 21 - Albert & Brayane]";
const int LDR = 36;                      // GPIO36 = VP = ADC1_CH0

int capteurMin = 4095;                   // abaisse pendant la calibration
int capteurMax = 0;                      // releve pendant la calibration
// const unsigned long CALIB_MS = 5000;  // ancienne fenetre (trop courte pour 2 gestes)
const unsigned long CALIB_MS = 8000;     // 8 s : le temps de cacher PUIS d'eclairer

void setup() {
  Serial.begin(115200);
  delay(300);

  Serial.print(PREFIX);
  Serial.println(" Calibration 8 s : CACHE (main) PUIS ECLAIRE (flash) la LDR...");

  unsigned long t0 = millis();
  unsigned long lastPrint = 0;
  while (millis() - t0 < CALIB_MS) {
    int v = analogRead(LDR);
    if (v > capteurMax) capteurMax = v;  // retient le plus clair
    if (v < capteurMin) capteurMin = v;  // retient le plus sombre

    if (millis() - lastPrint > 500) {    // feedback en direct toutes les 0,5 s
      lastPrint = millis();
      Serial.print(PREFIX);
      Serial.print(" ...plage en cours : min = "); Serial.print(capteurMin);
      Serial.print(" | max = "); Serial.println(capteurMax);
    }
  }

  Serial.print(PREFIX);
  Serial.print(" Calibration finie -> min = "); Serial.print(capteurMin);
  Serial.print(" | max = "); Serial.println(capteurMax);
}

void loop() {
  int brut   = analogRead(LDR);
  int niveau = map(brut, capteurMin, capteurMax, 0, 255);  // mise a l'echelle
  niveau     = constrain(niveau, 0, 255);                  // borne 0..255

  Serial.print(PREFIX);
  Serial.print(" LDR calibree (0-255) : "); Serial.print(niveau);
  Serial.print("   [brut = "); Serial.print(brut); Serial.println("]");

  delay(1000);                                             // une mesure par seconde
}