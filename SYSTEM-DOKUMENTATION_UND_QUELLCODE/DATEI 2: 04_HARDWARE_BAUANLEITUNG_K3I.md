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

const int INPUT_SIGNAL_PIN = 2;  // Signal input from geimpftem PC
const int SAFETY_VETO_PIN = 3;   // Outputs to MOSFET and Relay

void setup() {
  pinMode(INPUT_SIGNAL_PIN, INPUT_PULLUP);
  pinMode(SAFETY_VETO_PIN, OUTPUT);
  digitalWrite(SAFETY_VETO_PIN, HIGH);
}

void loop() {
  int sentryStatus = digitalRead(INPUT_SIGNAL_PIN);
  if (sentryStatus == LOW) {
    digitalWrite(SAFETY_VETO_PIN, LOW);
    while(true) {}
  }
}

