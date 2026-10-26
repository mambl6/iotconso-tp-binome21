// ============================================================================
// [1.5.3.2] ADC - LDR GL5528 : resistance reelle (Ohm) puis luminosite (lux)
// Binome 21 - Albert & Brayane
//
// Montage : MEME pont diviseur que 1.5.2 (on repasse la LDR + 10k sur GPIO36) :
//   3V3 ---[ LDR ]---+---[ R_fixe 10k ]--- GND        ( noeud -> GPIO36 )
//
// Etape 1 : R_LDR a partir du pont diviseur (LDR cote 3V3) :
//   Vnode = Vsupply * R_fixe / (R_LDR + R_fixe)
//   ==> R_LDR = R_fixe * (Vsupply - Vnode) / Vnode
//
// Etape 2 : luminosite en lux a partir du datasheet GL5528 (serie GL55, p.2) :
//   une LDR suit  R = R10 * (E/10)^(-gamma)   ==>   E(lux) = 10 * (R10 / R_LDR)^(1/gamma)
//   GL5528 : R10 (a 10 lux) = 10-20 kOhm  (on prend 15k nominal) ; gamma = 0.6
//
// NB : R10 est donne en PLAGE par le datasheet -> le lux obtenu est un ORDRE DE
//      GRANDEUR, pas une mesure de luxmetre. On lit avec analogReadMilliVolts()
//      (tension calibree) car on exploite la valeur absolue de Vnode.
// ============================================================================
#include <Arduino.h>
#include <math.h>

const char* PREFIX = "[Binome 21 - Albert & Brayane]";
const int   LDR        = 36;        // GPIO36 = VP = ADC1_CH0 (noeud du pont)
const float VSUPPLY_MV = 3300.0;    // rail 3V3 (nominal ; alim de la LDR = 3V3, PAS VIN)
const float R_FIXE     = 10000.0;   // resistance fixe du pont : 10 kOhm

// --- Parametres datasheet GL5528 (serie GL55) ---
const float R10   = 15000.0;        // resistance a 10 lux (~10-20k -> 15k nominal)
const float GAMMA = 0.6;            // pente gamma (10-100 lux)

const int N = 16;                   // moyenne pour lisser le bruit

void setup() {
  Serial.begin(115200);
}

void loop() {
  uint32_t somme = 0;
  for (int i = 0; i < N; i++) { somme += analogReadMilliVolts(LDR); delay(2); }
  float vnode_mV = somme / (float)N;                  // tension du noeud (mV, calibree)

  if (vnode_mV < 1) vnode_mV = 1;                     // garde-fou division par zero
  float r_ldr = R_FIXE * (VSUPPLY_MV - vnode_mV) / vnode_mV;   // Ohm
  if (r_ldr < 1) r_ldr = 1;

  float lux = 10.0 * pow(R10 / r_ldr, 1.0 / GAMMA);   // formule GL5528

  Serial.print(PREFIX);
  Serial.print(" Vnode = "); Serial.print(vnode_mV, 0); Serial.print(" mV");
  Serial.print(" | R_LDR = "); Serial.print(r_ldr, 0); Serial.print(" Ohm");
  Serial.print(" | Luminosite = "); Serial.print(lux, 0); Serial.println(" lux");

  delay(1000);
}