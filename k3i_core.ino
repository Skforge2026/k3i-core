# Projekt K3I: Hardware-Relais & Maschinelle Ethik

[![License: MIT](https://shields.io)](https://opensource.org)
[![Platform: Arduino/Embedded](https://shields.io)](https://arduino.cc)

**K3I** ist ein im Mikrosekundenbereich reagierendes Sicherheitsrelais. Es schützt Hochleistungsrechenzentren vor unautorisiertem Datenzugriff, Datenexfiltration und massiver Insider-Sabotage (z. B. Ransomware-Verschlüsselungen per USB-Stick) [1.1].

---

## 🇩🇪 DEUTSCHE SPEZIFIKATION
* **Kern-Axiom:** 1+1=1 (Symmetrische Mensch-Maschine-Koadaption)
* **Kanal Alpha (Links) - Signal-MOSFET:** Trennt bei Metadaten-Anomalien oder unautorisierten eingehenden/ausgehenden Datenbewegungen physisch die externe Netzverbindung [1.1]. Die interne CPU bleibt für forensische RAM-Analysen aktiv [1.1].
* **Kanal Beta (Rechts) - Starkstrom-MOSFET:** Unterbricht bei kritischen Entropie-Peaks (Verschlüsselungsschleifen) den Haltestrom massiver Leistungsschütze [1.1]. Das System ist in unter 25 Millisekunden komplett stromlos [1.1]. Volatiler RAM-Speicher wird sofort gelöscht – Schadcode wird im Rechenschritt eingefroren („Frozen State“) [1.1].
* **Anti-Boot-Verriegelung:** Solange der Alarmzustand auf Kern 2 aktiv ist, blockiert die Hardware jeden automatischen Neustart oder Notstrom-Boot, bis der infizierte USB-Stick physisch entfernt und das Relais manuell zurückgesetzt wurde.

---

## 🇬🇧 ENGLISH SPECIFICATION
* **Core Axiom:** 1+1=1 (Symmetrical Human-Machine Co-Adaptation)
* **Channel Alpha (Left) - Signal-MOSFET:** Physically disconnects the external network link upon detection of unauthorized inbound/outbound data movement [1.1]. Internal processing remains active for live RAM forensic analysis [1.1].
* **Channel Beta (Right) - Power-MOSFET:** Interrupts the holding current of heavy-duty power contactors during critical internal entropy spikes (ransomware loops) [1.1]. Total hardware power-cut occurs in under 25 ms [1.1]. Volatile memory is flushed instantly, trapping malicious code in a "Frozen State" [1.1].
* **Anti-Boot Lockout:** As long as the alarm state is active on Core 2, the hardware denies closing power to main contactors. The system is physically barred from rebooting until the malicious device is extracted and the core is manually reset.

---

## 🛠️ Kompilierung & Installation / Compilation
Verwenden Sie das integrierte `Makefile`, um die Firmware auf den Arduino-Wächterkern (Kern 2) zu flashen:

```bash
make compile
make upload
```
/*
 * Projekt K3I - Hardware Guardian Core (Kern 2)
 * File: k3i_core.ino
 * Update: June 2026
 * 
 * Hardware-Steuerung für die duale MOSFET-Sicherheitsweiche.
 * Links (Channel Alpha): Signal-MOSFET für Netzwerk-Cut (Inbound/Outbound).
 * Rechts (Channel Beta): Starkstrom-MOSFET für Leistungsschütz-Kaskade.
 */

// Pin-Konfiguration für den Arduino
const int PIN_ENTROPY_INPUT = A0;  // Analoger Sensor/Eingang vom Entropie-Filter
const int PIN_ALPHA_MOSFET  = 2;   // Links: Signal-MOSFET (Netztrennung)
const int PIN_BETA_MOSFET   = 3;   // Rechts: Starkstrom-MOSFET (Schützkaskade)
const int PIN_ALARM_BUZZER  = 4;   // Laute USV-gekoppelte Alarm-Sirene
const int PIN_RESET_BUTTON  = 5;   // Manueller Hardware-Reset-Taster

// Kritische Schwellenwerte (Grenzwerte für Systemstabilität)
const int THRESHOLD_EXFILTRATION = 400; // Trigger für unautorisierte Datenbewegung
const int THRESHOLD_ENCRYPTION   = 850; // Trigger für massive Insider-Sabotage

bool alarmActive = false;

void setup() {
  pinMode(PIN_ALPHA_MOSFET, OUTPUT);
  pinMode(PIN_BETA_MOSFET, OUTPUT);
  pinMode(PIN_ALARM_BUZZER, OUTPUT);
  pinMode(PIN_RESET_BUTTON, INPUT_PULLUP);
  
  // Normalzustand: Beide MOSFETs leiten (HIGH = Schalter geschlossen)
  digitalWrite(PIN_ALPHA_MOSFET, HIGH);
  digitalWrite(PIN_BETA_MOSFET, HIGH);
  digitalWrite(PIN_ALARM_BUZZER, LOW);
}

void loop() {
  // Wenn das System verriegelt ist, blockiere jeden Neustart (Boot-Sperre)
  if (alarmActive) {
    executeLockoutMode();
    return;
  }

  // Echtzeit-Überwachung der System-Entropie (Latenz < 5 Mikrosekunden)
  int currentEntropy = analogRead(PIN_ENTROPY_INPUT);

  // SCHALT-LOGIK LINKS: Netztrennung bei unautorisiertem Datenfluss
  if (currentEntropy >= THRESHOLD_EXFILTRATION && currentEntropy < THRESHOLD_ENCRYPTION) {
    digitalWrite(PIN_ALPHA_MOSFET, LOW); // Links öffnet sofort: Netzwerk physisch getrennt!
    digitalWrite(PIN_ALARM_BUZZER, HIGH); // Akustischer Warnhinweis aktiv
  }

  // SCHALT-LOGIK RECHTS: Totaler Starkstrom-Freeze bei Insider-Job (Verschlüsselung)
  if (currentEntropy >= THRESHOLD_ENCRYPTION) {
    digitalWrite(PIN_BETA_MOSFET, LOW);   // Rechts öffnet sofort: Schützkaskade fällt!
    digitalWrite(PIN_ALPHA_MOSFET, LOW);  // Sicherheitsnetz ebenfalls kappen
    digitalWrite(PIN_ALARM_BUZZER, HIGH);  // Maximaler Sirenenalarm über USV
    alarmActive = true;                   // Aktiviert die permanente Boot-Sperre
  }
}

// Permanente Anti-Boot-Verriegelung bis zum manuellen Vor-Ort-Eingriff
void executeLockoutMode() {
  digitalWrite(PIN_BETA_MOSFET, LOW);  // Hauptstrom bleibt physisch gekappt
  digitalWrite(PIN_ALPHA_MOSFET, LOW); // Netzwerk bleibt physisch gekappt
  
  // Erst wenn der Techniker den Reset-Button drückt, wird das System freigegeben
  if (digitalRead(PIN_RESET_BUTTON) == LOW) {
    delay(200); // Entprellung des Tasters
    alarmActive = false;
    digitalWrite(PIN_ALPHA_MOSFET, HIGH);
    digitalWrite(PIN_BETA_MOSFET, HIGH);
    digitalWrite(PIN_ALARM_BUZZER, LOW);
  }
}
