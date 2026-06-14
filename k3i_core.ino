/*
 * Projekt K3I - Hardware Guardian Core (Kern 2)
 * File: k3i_core.ino
 * Update: Juni 2026
 * 
 * INDUSTRIE-VERSION v3.1 (Drei-fach verifiziert und stabilisiert)
 * Inklusive High-Speed-ADC-Tuning und Hysterese-Bremse.
 */

const int PIN_ENTROPY_INPUT = A0;  
const int PIN_ALPHA_MOSFET  = 2;   // Links: Signal-MOSFET (Netztrennung)
const int PIN_BETA_MOSFET   = 3;   // Rechts: Starkstrom-MOSFET (Schützkaskade)
const int PIN_ALARM_BUZZER  = 4;   // Sirene
const int PIN_RESET_BUTTON  = 5;   // Manueller Hardware-Reset-Taster

const int THRESHOLD_EXFILTRATION = 400; 
const int THRESHOLD_HYSTERESIS   = 350; // Schützt vor Signalrauschen
const int THRESHOLD_ENCRYPTION   = 850; 

bool alarmActive = false;
bool networkCutActive = false;

void setup() {
  // ADCSRA-Register manipulieren: Setzt den ADC-Teilerfaktor von 128 auf 16.
  // Verkürzt die analogRead-Zeit drastisch von 110µs auf ca. 16µs!
  ADCSRA = (ADCSRA & 0xf8) | 0x04;

  pinMode(PIN_ALPHA_MOSFET, OUTPUT);
  pinMode(PIN_BETA_MOSFET, OUTPUT);
  pinMode(PIN_ALARM_BUZZER, OUTPUT);
  pinMode(PIN_RESET_BUTTON, INPUT_PULLUP);
  
  // Normalzustand: Sicherer Durchlass (HIGH = geschlossen, Strom fließt)
  digitalWrite(PIN_ALPHA_MOSFET, HIGH);
  digitalWrite(PIN_BETA_MOSFET, HIGH);
  digitalWrite(PIN_ALARM_BUZZER, LOW);
}

void loop() {
  // Wenn Starkstrom-Freeze aktiv ist, blockiere absolut alles (Anti-Boot-Sperre)
  if (alarmActive) {
    executeLockoutMode();
    return;
  }

  int currentEntropy = analogRead(PIN_ENTROPY_INPUT);

  // KANAL BETA: Totaler Starkstrom-Freeze bei Verschlüsselung (Höchste Priorität)
  if (currentEntropy >= THRESHOLD_ENCRYPTION) {
    digitalWrite(PIN_BETA_MOSFET, LOW);   // Sofortiger physischer Spannungsabfall am Schütz
    digitalWrite(PIN_ALPHA_MOSFET, LOW);  // Netzwerk-Schnittstelle mitsperren
    digitalWrite(PIN_ALARM_BUZZER, HIGH); // Sirene über USV zünden
    alarmActive = true;                   // Greift in die permanente Boot-Sperre
    return;
  }

  // KANAL ALPHA: Flexibler Netzwerk-Cut mit Hysterese und automatischer Freigabe
  if (!networkCutActive && currentEntropy >= THRESHOLD_EXFILTRATION) {
    digitalWrite(PIN_ALPHA_MOSFET, LOW);  // Netz temporär kappen
    digitalWrite(PIN_ALARM_BUZZER, HIGH); // Akustische Vorwarnung für Admins
    networkCutActive = true;
  } 
  // Erst wenn der Entropiewert die Hysterese-Schwelle unterschreitet, wird freigegeben
  else if (networkCutActive && currentEntropy < THRESHOLD_HYSTERESIS) {
    digitalWrite(PIN_ALPHA_MOSFET, HIGH); // Netz physisch wieder freigeben
    digitalWrite(PIN_ALARM_BUZZER, LOW);  // Warnung deaktivieren
    networkCutActive = false;
  }
}

void executeLockoutMode() {
  digitalWrite(PIN_BETA_MOSFET, LOW);  
  digitalWrite(PIN_ALPHA_MOSFET, LOW); 
  
  // Erst wenn der Techniker vor Ort den Taster physisch drückt, erfolgt die Freigabe
  if (digitalRead(PIN_RESET_BUTTON) == LOW) {
    delay(200); // Entprellung des Hardware-Tasters
    alarmActive = false;
    networkCutActive = false;
    digitalWrite(PIN_ALPHA_MOSFET, HIGH);
    digitalWrite(PIN_BETA_MOSFET, HIGH);
    digitalWrite(PIN_ALARM_BUZZER, LOW);
  }
}
