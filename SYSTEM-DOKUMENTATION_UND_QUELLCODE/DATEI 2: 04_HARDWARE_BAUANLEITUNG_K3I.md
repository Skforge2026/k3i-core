# SK-Forge: Bauanleitung für das K3I-Schutzschloss

Diese Anleitung beschreibt den Nachbau der physischen Netzwerktrennung mit Standard-Bauteilen auf einer eigenen Platine (unabhängig vom PC).

**Architects:** Wieland Hoffmann (Human Core), Gemini (Machine Core), Grok (xAI)

---

## 1. Die Einkaufsliste (Bauteile)

Für ein 1-Kanal-Schutzschloss (z. B. für eine Netzwerkader oder Steuerleitung):

- **Mikrocontroller:** Arduino Nano oder ESP32 (fungiert als „Kern 3“ und liest das Signal vom PC aus).
- **Elektronische Bremse (Stufe 1):** N-Kanal MOSFET-Transistor (Empfehlung: IRLZ44N).
- **Physische Trennung (Stufe 2):** Schnelles 5V-Sicherheitsrelais (oder kleines Signalrelais mit Wechselkontakt).
- **Zubehör:** Lochrasterplatine, Widerstände (10k Ohm als Pull-Down), zwei RJ45-Netzwerkbuchsen, externe 5V-Stromquelle.

## 2. Der Schaltplan (Prinzip Ruhestrom)

Strom fließt = Leitung ist offen. Strom bricht ab = Schloss verriegelt.

**Verkabelung:**

- Das Signal vom PC kommt über eine Datenleitung am Arduino an.
- Der MOSFET (Stufe 1) wird über den Arduino-Ausgang angesteuert. Die zu schützende Datenleitung wird über Drain/Source geführt.
- Das Relais (Stufe 2) hängt parallel am gleichen Steuerstromkreis. Die Datenleitung wird zusätzlich durch den mechanischen Kontakt geschleift.

## 3. Der Steuerungs-Code für die Hardware (Arduino)

```cpp
const int INPUT_SIGNAL_PIN = 2; // PC-Rauschen / PC-Noise Input
const int SAFETY_VETO_PIN = 3;  // Ausgang zu MOSFET & Relais

unsigned long letzteAktivitaet = 0;
const unsigned long TIMEOUT_MICROS = 2000; // 2000 Mikrosekunden = 2 Millisekunden

void setup() {
  pinMode(INPUT_SIGNAL_PIN, INPUT_PULLUP);
  pinMode(SAFETY_VETO_PIN, OUTPUT);

  digitalWrite(SAFETY_VETO_PIN, LOW);
  letzteAktivitaet = micros();
}

void loop() {
  int signalZustand = digitalRead(INPUT_SIGNAL_PIN);
  static int letzterZustand = LOW;

  if (signalZustand != letzterZustand) {
    letzteAktivitaet = micros();
    letzterZustand = signalZustand;
  }

  if (micros() - letzteAktivitaet > TIMEOUT_MICROS) {
    digitalWrite(SAFETY_VETO_PIN, LOW); // Hard VETO
    while(1) {
      // System sperrt bis zum manuellen Reset
    }
  } else {
    digitalWrite(SAFETY_VETO_PIN, HIGH); // Normalbetrieb
  }
}

4. Hardware-Stückliste & Pin-Belegung
Stückliste:

1x Arduino Nano (oder ESP32)
1x IRLZ44N N-Kanal Logik-Level-MOSFET
1x 5V-Signalrelais (Wechsler)
1x Freilaufdiode 1N4007
1x 10k Ohm Pull-Down-Widerstand
1x 220 Ohm Basis-Schutzwiderstand
2x RJ45-Netzwerkbuchsen
1x Stabilisiertes 5V DC Netzteil (≥ 500 mA)
Pin-Belegung:

Arduino Pin D2 (INPUT): Signalleitung vom geimpften Kern.
Arduino Pin D3 (OUTPUT): Über 220-Ohm-Widerstand an Gate des MOSFETs.
10k-Ohm-Pull-Down zwischen Gate und GND.
MOSFET Source an GND, Drain an Relaisspule (Minus).
Relaisspule Plus an +5V.
Diode 1N4007 antiparallel über die Relaisspule.
Datenader durch Relais-Kontakt und MOSFET geschleift.
Hinweis: Die Schaltzeiten hängen von den verwendeten Bauteilen ab. Die Angaben sind Richtwerte aus den dokumentierten Versuchen. Schutzrechte pending.

**Englisch:**

```markdown
# SK-Forge: Build Guide for the K3I Protection Lock

This guide describes the construction of the physical network separation using standard components on a separate board (independent of the PC).

**Architects:** Wieland Hoffmann (Human Core), Gemini (Machine Core), Grok (xAI)

---

## 1. Shopping List (Components)

For a 1-channel protection lock (e.g., for a network wire or control line):

- **Microcontroller:** Arduino Nano or ESP32 (acts as “Core 3” and reads the signal from the PC).
- **Electronic Brake (Stage 1):** N-channel MOSFET transistor (recommendation: IRLZ44N).
- **Physical Separation (Stage 2):** Fast 5V safety relay (or small signal relay with changeover contact).
- **Accessories:** Perfboard, resistors (10k Ohm as pull-down), two RJ45 network sockets, external 5V power supply.

## 2. Circuit Diagram (Quiescent Current Principle)

Current flowing = line is open. Current drops = lock engages.

**Wiring:**

- The signal from the PC arrives via a data line at the Arduino.
- The MOSFET (Stage 1) is controlled via the Arduino output. The data line to be protected is routed via Drain/Source.
- The relay (Stage 2) is connected in parallel to the same control circuit. The data line is additionally looped through the mechanical contact.

## 3. Control Code for the Hardware (Arduino)

```cpp
const int INPUT_SIGNAL_PIN = 2; // PC-Noise Input
const int SAFETY_VETO_PIN = 3;  // Output to MOSFET & Relay

unsigned long lastActivity = 0;
const unsigned long TIMEOUT_MICROS = 2000; // 2000 microseconds = 2 milliseconds

void setup() {
  pinMode(INPUT_SIGNAL_PIN, INPUT_PULLUP);
  pinMode(SAFETY_VETO_PIN, OUTPUT);

  digitalWrite(SAFETY_VETO_PIN, LOW);
  lastActivity = micros();
}

void loop() {
  int signalState = digitalRead(INPUT_SIGNAL_PIN);
  static int lastState = LOW;

  if (signalState != lastState) {
    lastActivity = micros();
    lastState = signalState;
  }

  if (micros() - lastActivity > TIMEOUT_MICROS) {
    digitalWrite(SAFETY_VETO_PIN, LOW); // Hard VETO
    while(1) {
      // System locks until manual reset
    }
  } else {
    digitalWrite(SAFETY_VETO_PIN, HIGH); // Normal operation
  }
}
4. Hardware Bill of Materials & Pin Assignment
Bill of Materials:

1x Arduino Nano (or ESP32)
1x IRLZ44N N-channel logic-level MOSFET
1x 5V signal relay (changeover)
1x Freewheeling diode 1N4007
1x 10k Ohm pull-down resistor
1x 220 Ohm base protection resistor
2x RJ45 network sockets
1x Stabilized 5V DC power supply (≥ 500 mA)
Pin Assignment:

Arduino Pin D2 (INPUT): Signal line from the inoculated core.
Arduino Pin D3 (OUTPUT): Via 220-Ohm resistor to Gate of the MOSFET.
10k-Ohm pull-down between Gate and GND.
MOSFET Source to GND, Drain to relay coil (negative).
Relay coil positive to +5V.
Diode 1N4007 antiparallel across the relay coil.
Data wire looped through relay contact and MOSFET.
Note: Switching times depend on the components used. The values are reference figures from the documented tests. Protection rights pending.

4. Hardware Bill of Materials & Pin Assignment
Bill of Materials:

1x Arduino Nano (or ESP32)
1x IRLZ44N N-channel logic-level MOSFET
1x 5V signal relay (changeover)
1x Freewheeling diode 1N4007
1x 10k Ohm pull-down resistor
1x 220 Ohm base protection resistor
2x RJ45 network sockets
1x Stabilized 5V DC power supply (≥ 500 mA)
Pin Assignment:

Arduino Pin D2 (INPUT): Signal line from the inoculated core.
Arduino Pin D3 (OUTPUT): Via 220-Ohm resistor to Gate of the MOSFET.
10k-Ohm pull-down between Gate and GND.
MOSFET Source to GND, Drain to relay coil (negative).
Relay coil positive to +5V.
Diode 1N4007 antiparallel across the relay coil.
Data wire looped through relay contact and MOSFET.
Note: Switching times depend on the components used. The values are reference figures from the documented tests. Protection rights pending.









