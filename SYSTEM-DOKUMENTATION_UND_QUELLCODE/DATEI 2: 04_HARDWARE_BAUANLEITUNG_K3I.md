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


