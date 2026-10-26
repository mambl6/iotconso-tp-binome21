// ============================================================================
// [2.1.2 - prog 2] LCD I2C : temperature + humidite du BME680 affichees sur l'ecran
// Binome 21 - Albert & Brayane
// Bus I2C : SDA = GPIO21, SCL = GPIO22 | BME680 = 0x77, LCD = 0x27 (confirmes au scan)
// Librairies : Adafruit BME680 (+ Unified Sensor) + LiquidCrystal_I2C
// ============================================================================
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME680.h>
#include <LiquidCrystal_I2C.h>

const char* PREFIX = "[Binome 21 - Albert & Brayane]";
Adafruit_BME680 bme;                               // BME680 en I2C
LiquidCrystal_I2C lcd(0x27, 16, 2);                // LCD 16x2

// Ecrit une ligne et la complete a 16 caracteres (efface les restes), tronque si trop long.
void printLine(uint8_t row, const String& text) {
    String padded = text;
    while (padded.length() < 16) padded += ' ';    // bourrage d'espaces -> efface l'affichage precedent
    lcd.setCursor(0, row);
    lcd.print(padded.substring(0, 16));            // jamais plus de 16 colonnes
}

void setup() {
    Serial.begin(115200);
    Wire.begin(21, 22);                            // SDA = GPIO21, SCL = GPIO22

    lcd.init();
    lcd.backlight();

    if (!bme.begin(0x77)) {                         // 0x77 = adresse confirmee au scan
        Serial.print(PREFIX); Serial.println(" BME680 introuvable !");
        printLine(0, "BME680 KO");
        while (true) delay(1000);
    }

    bme.setTemperatureOversampling(BME680_OS_8X);
    bme.setHumidityOversampling(BME680_OS_2X);
    bme.setPressureOversampling(BME680_OS_4X);
    bme.setIIRFilterSize(BME680_FILTER_SIZE_3);
    bme.setGasHeater(0, 0);                         // gaz non lu ici -> chauffe DESACTIVEE (economie + moins d'auto-echauffement)

    Serial.print(PREFIX); Serial.println(" BME680 + LCD prets.");
}

void loop() {
    if (bme.performReading()) {
        float tC = bme.temperature;
        float h  = bme.humidity;

        printLine(0, "Temp: " + String(tC, 1) + " C");   // ligne 1
        printLine(1, "Hum : " + String(h, 1) + " %");    // ligne 2

        Serial.print(PREFIX);
        Serial.printf(" LCD <- T=%.1f C | H=%.1f %%\n", tC, h);   // trace de debogage
    } else {
        Serial.print(PREFIX); Serial.println(" Echec de lecture BME680");
    }

    delay(2000);
}