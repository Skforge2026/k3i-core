# SK-Forge: Idiotensichere Bauanleitung für das K3I-Schutzschloss

Diese Anleitung beschreibt den Nachbau der physischen Netzwerktrennung. Sie ist so geschrieben, dass Bastler und Ingenieure weltweit das System mit Standard-Bauteilen auf einer eigenen Platine (unabhängig vom PC) aufbauen können.

---

## 1. Die Einkaufsliste (Bauteile für den Elektronikladen)

Für ein 1-Kanal-Schutzschloss (z. B. für eine Netzwerkader oder Steuerleitung) brauchst du:

1. Das Gehirn: Einen kleinen, billigen Mikrocontroller (z. B. Arduino Nano oder ESP32). Er fungiert als "Kern 3" und liest das Signal vom PC aus.
2. Die elektronische Bremse (Stufe 1 - 1 Millisekunde): 
   * Einen N-Kanal MOSFET-Transistor (Typen-Empfehlung: IRLZ44N – der schaltet extrem schnell und lässt sich direkt mit 5V ansteuern).
3. Die physische Trennung (Stufe 2 - 12 Millisekunden):
   * Ein schnelles 5V-Sicherheitsrelais (oder ein kleines Signalrelais mit Wechselkontakt).
4. Zubehör: Eine Standard-Lochrasterplatine, ein paar Widerstände (10k Ohm als Pull-Down), zwei RJ45-Netzwerkbuchsen (zum Durchschleifen des Kabels) und eine externe Stromquelle (z. B. 5V-Netzteil).

---

## 2. Der Schaltplan (Wie wird verdrahtet?)

Das Prinzip basiert auf Ruhestrom. Das bedeutet: Strom fließt = Leitung ist offen (Sicher). Strom bricht ab = Schloss verriegelt sofort (Alarm).

### Schritt-für-Schritt-Verkabelung:

1. Das Signal vom PC: Das Signal (Keep-Alive) kommt aus dem geimpften Kern über eine Datenleitung am Arduino an.
2. Der MOSFET (Stufe 1):
   * Verbinde den Steuerpin (Gate) des MOSFETs mit dem Ausgang des Arduinos.
   * Die zu schützende Datenleitung wird an den Schalter (Drain/Source) des MOSFETs gehängt.
   * Effekt: Solange der Arduino Saft gibt, leitet der MOSFET. Fällt das Signal, macht der MOSFET in 1 Millisekunde dicht und blockiert die Daten digital.
3. Der Relais (Stufe 2):
   * Hänge die Spule des mechanischen Relais parallel an den gleichen Steuerstromkreis.
   * Die Datenleitung wird zusätzlich durch den mechanischen Schaltkontakt (Wechsler) des Relais geschleift.
   * Effekt: Sobald der Strom wegbricht, fällt der Anker des Relais ab. Nach spätestens 12 Millisekunden entsteht ein echter Luftspalt (Air Gap).

---

## 3. Der Steuerungs-Code für die Hardware (English)

// Der Steuerungs-Code für die Hardware (Englisch)
const int INPUT_SIGNAL_PIN = 2; // PC-Rauschen / PC-Noise Input
const int SAFETY_VETO_PIN = 3;  // Ausgang zu MOSFET & Relais / Output to MOSFET & Relay

unsigned long letzteAktivitaet = 0;
const unsigned long TIMEOUT_MICROS = 2000; // 2000 Mikrosekunden = 2 Millisekunden

void setup() {
  pinMode(INPUT_SIGNAL_PIN, INPUT_PULLUP);
  pinMode(SAFETY_VETO_PIN, OUTPUT);

  // Start-Zustand: Erst sperren, um undefinierte Zustände beim Booten zu verhindern
  digitalWrite(SAFETY_VETO_PIN, LOW);

  // Initialisierung des Zeitstempels beim Start
  letzteAktivitaet = micros();
}

void loop() {
  int signalZustand = digitalRead(INPUT_SIGNAL_PIN);
  static int letzterZustand = LOW;

  // Erkennung von Signalflanken (Wechsel im Rauschen)
  if (signalZustand != letzterZustand) {
    letzteAktivitaet = micros();
    letzterZustand = signalZustand;
  }

  // FAIL-SAFE: Überprüfung des Zeitfensters in Mikrosekunden
  if (micros() - letzteAktivitaet > TIMEOUT_MICROS) {
    digitalWrite(SAFETY_VETO_PIN, LOW); // Hard VETO / Sofortige physische Trennung
    while(1) {
      // Endlosschleife: System sperrt dauerhaft bis zum manuellen Hardware-Reset vor Ort
    }
  } else {
    digitalWrite(SAFETY_VETO_PIN, HIGH); // Normalbetrieb: Leitung bleibt offen (geschaltet)
  }
}

---

## 7. Hardware-Stückliste & Pin-Belegung (K3I-Schutzschloss)

### 7.1 Die Stückliste (Komponenten für den Nachbau)
Für den Aufbau der physischen Veto-Schaltung auf einer Lochrasterplatine werden folgende Standard-Bauteile benötigt:

* **1x Mikrocontroller:** Arduino Nano (oder ESP32) als Logik-Schnittstelle.
* **1x Halbleiter-Sperrglied (Stufe 1):** IRLZ44N N-Kanal Logik-Level-MOSFET (schaltet voll durch bei 5V Gate-Spannung).
* **1x Galvanisches Relais (Stufe 2):** 5V-Signalrelais (Wechsler / Monostabil, z. B. Finder 30.22.7.005.0010).
* **1x Freilaufdiode:** 1N4007 (Unerlässlich! Schützt den Arduino vor der induktiven Abschalt-Spannungsspitze der Relaisspule).
* **1x Pull-Down-Widerstand:** 10k Ohm (Hält das Gate des MOSFETs beim Booten sicher auf Masse).
* **1x Basis-Schutzwiderstand:** 220 Ohm (Begrenzt den Schaltstrom vom Arduino-Pin zum Gate des MOSFETs).
* **2x RJ45-Netzwerkbuchsen:** Zur physischen Integration (Durchschleifen) in die zu trennende Datenleitung.
* **1x Externe Spannungsversorgung:** Stabilisiertes 5V DC Netzteil (mindestens 500mA).

---

### 7.2 Die Pin-Belegung (Verdrahtungsplan)
Die physische Verkabelung der Komponenten auf der Platine erfolgt nach folgender Festlegung:

#### 1. Signal-Eingang (Vom PC kommend)
* **Arduino Pin D2 (INPUT):** Verbunden mit der Signalleitung (Keep-Alive / PC-Rauschen) des geimpften Kerns.
* **Arduino GND:** Verbunden mit der Masse (GND) des Signalgebers/PCs.

#### 2. Veto-Ausgang (Steuerstromkreis)
* **Arduino Pin D3 (OUTPUT):** Geht über den **220-Ohm-Widerstand** direkt an das **Gate (Pin 1)** des IRLZ44N MOSFETs.
* **Zwischen Gate und GND:** Der **10k-Ohm-Pull-Down-Widerstand** wird fest eingelötet.

#### 3. Leistungskreis & Relais-Kaskade
* **MOSFET Source (Pin 3):** Direkt an die gemeinsame Masse (**GND**).
* **MOSFET Drain (Pin 2):** Verbunden mit dem **Minuspol der Relaisspule** (A2).
* **Relaisspule Pluspol (A1):** Direkt an die externe **+5V Stromversorgung**.
* **Parallel zur Relaisspule (Wichtig):** Die Diode **1N4007** wird antiparallel über die Spulenanschlüsse gelötet (Kathode mit dem Ring an +5V/A1, Anode an Drain/A2).

#### 4. Datenleitungs-Trennung (RJ45-Schnittstelle)
* **RJ45 Buchse 1 (Eingang):** Die zu schützende Datenader geht an den **Mittelkontakt (Common)** des Relais-Wechslers.
* **Relais-Schließerkontakt (Normally Open / NO):** Wird weitergeschleift zum **Drain-Anschluss** des MOSFETs.
* **Relais-Öffnerkontakt (Normally Closed / NC):** Bleibt frei (isoliert).
* **RJ45 Buchse 2 (Ausgang):** Der **Source-Anschluss** des MOSFETs führt die geschützte Ader wieder nach draußen.


