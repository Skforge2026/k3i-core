# K3I-SPACE: Layer-Zero Orbital Safeguard & Satellite Isolation (V1.0-Beta)

---

## DEUTSCH

**Ziel:** Physischer Schutz von On-Board-Computern (OBC) in CubeSats und Kommunikationssatelliten vor Uplink-Exploits, DoS-Angriffen und Single-Event-Upsets durch kosmische Strahlung.

### 🛠️ Technische Umsetzung: Hardware-Veto im Vakuum
Statt Software-Filtern (Ring-3) überwacht ein strahlungsgehärteter Hardware-Wächter direkt hinter dem RF-Frontend (Antennen-Schnittstelle) die Signalflanken und den Systemtakt.

* **Physische HF-Trennung:** Zwischen Transceiver und Haupt-OBC sitzt eine symmetrische Anordnung strahlungsresistenter GaAs-HF-Schalter. Diese können die Verbindungsleitung im Nanosekunden-Bereich physikalisch unterbrechen.
* **Jitter- und Anomalie-Detektion:** Der isolierte Wächter detektiert sowohl gezielte Uplink-Angriffe (Command Flooding) als auch strahlungsinduzierte Bit-Flips (Single Event Upsets) im Taktbereich.
* **Hardware-Veto (Fail-Closed):** Bei erkannter Anomalie kappt der Wächter die Steuerspannung der HF-Schalter. Die Verbindung zum Hauptprozessor wird physikalisch getrennt. Der Angreifer läuft ins Leere.

### 🔄 Auto-Recovery & Orbital Ping-Pong-Architektur
* **Isolierter Reset:** Nach der Trennung löst der Wächter über festverdrahtete Logik einen harten Hardware-Reset des betroffenen Prozessors aus und entlädt den Arbeitsspeicher vollständig.
* **Unterbrechungsfreier Betrieb:** Das System arbeitet mit zwei physikalisch getrennten, redundanten On-Board-Computern (A + B). Wird Prozessor A isoliert, übernimmt Prozessor B nahtlos Lageregelung, Telemetrie und Nutzlastbetrieb.

*Dieses Design gewährleistet maximale Autonomie und Resilienz im Orbit, wo ein manueller Eingriff unmöglich ist.*

---

## ENGLISH

**Objective:** Physical hardware protection of On-Board Computers (OBC) in CubeSats and communication satellites against uplink exploits, DoS attacks, and radiation-induced Single Event Upsets.

### 🛠️ Technical Implementation: Hardware Veto in Vacuum
Instead of relying on software-level filtering (Ring-3), a radiation-hardened hardware watchdog monitors signal edges and system clock states directly behind the RF frontend (antenna interface).

* **Physical RF Decoupling:** A symmetrical configuration of radiation-resistant GaAs RF switches is integrated between the transceiver and the main OBC, capable of physically severing the line within nanoseconds.
* **Jitter and Anomaly Detection:** The isolated watchdog detects targeted uplink attacks (command flooding) as well as radiation-induced bit-flips (Single Event Upsets) within the clock domain.
* **Hardware Veto (Fail-Closed):** Upon detecting an anomaly, the watchdog cuts the control voltage of the RF switches. The connection to the main processor is physically disconnected, leaving the attacker neutralized.

### 🔄 Auto-Recovery & Orbital Ping-Pong Architecture
* **Isolated Reset:** Following physical isolation, the watchdog triggers a hard hardware reset of the affected processor via hardwired logic, fully purging the volatile memory.
* **Uninterrupted Operation:** The architecture utilizes two physically isolated, redundant On-Board Computers (A + B). If Processor A is isolated, Processor B seamlessly assumes attitude control, telemetry, and payload operations.

*This design ensures maximum autonomy and resilience in orbit, where manual intervention is impossible.*
