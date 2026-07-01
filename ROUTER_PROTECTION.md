# K3I Consumer-Router-Referenzdesign: 1-Euro Layer-Zero-Absicherung
# K3I Consumer Router Reference Design: 1-Euro Layer-Zero Safeguard

---

## DEUTSCH

### Warum das wichtig ist:
Klassische Consumer-Router versuchen, Angriffe und Schadcode über integrierte Firewalls und Software-Filter (Ring-3) zu blockieren. Diese Methode ist von Natur aus anfällig, da hochentwickelte Exploits oder automatisierte Angriffe die Software-Schichten innerhalb von Minuten kompromittieren können. Unser Design bricht diese Spirale radikal auf der physischen Hardware-Ebene (Layer-Zero) auf – mit minimalen Produktionskosten von unter 1 Euro.

### 🛠️ Technische Umsetzung: Der Power-MOSFET an der WAN-Leitung
Anstatt darauf zu hoffen, dass die Software den Schadcode erkennt, überwacht ein eigenständiger, hardwarebasierter Wächter das Signalverhalten.
* **Die physische Trennung:** Ein simpler, hocheffizienter Power-MOSFET wird direkt als elektronischer Schalter in die physische WAN-Leitung (Internet-Eingang) des Routers integriert.
* **Die Jitter-Explosion als Auslöser:** Sobald ein System kompromittiert wird oder Schadcode versucht, massenhaft Daten unbemerkt auszuschleusen, verändert sich das Takt- und Signalverhalten im Nanosekundenbereich drastisch (Jitter-Explosion).
* **Das Hardware-Veto:** Der Hardware-Wächter reagiert rein physikalisch auf diese Anomalie. Ohne Umweg über das Betriebssystem oder den Hauptprozessor steuert er den MOSFET an und kappt die WAN-Leitung innerhalb von Nanosekunden physisch wie ein mechanischer Riegel (Moped-Architektur).

### 🔄 Auto-Recovery & Paralleles Ping-Pong-Prinzip
* **Automatischer Wiederanlauf:** Nach dem physischen Cut leitet der Hardware-Wächter einen isolierten Neustart der Signalstruktur ein. Sobald die internen Taktzyklen wieder stabil und reibungsfrei laufen, schaltet der MOSFET die Verbindung zum internationalen Internet vollautomatisch wieder zurück.
* **Unterbrechungsfreier Schutz (Ping-Pong):** Der Router nutzt zwei identische, parallel geschaltete MOSFET-Eingangsstufen. Detektiert Pfad A eine Takt-Anomalie durch ankommende Lastspitzen, wird dieser Pfad für Bruchteile von Millisekunden physisch getrennt, um sich zu beruhigen. Im selben Moment übernimmt Pfad B nahtlos die Verbindung. Der Jitter wird physisch gelöscht, während der Datenstrom für den Nutzer unterbrechungsfrei weiterläuft.

### 🔍 Das optionale Analyse-Aufsatzmodul & Speicher-Wartung (Für Power-User)
Für technologiebegeisterte Nutzer und Administratoren sieht das Referenzdesign eine modulare Erweiterung vor:
* **Huckepack-Platine:** Ein optionales Zusatzmodul kann direkt auf die Hardware-Struktur (z. B. einer modifizierten Fritzbox) aufgesteckt werden.
* **Hardware-Forensik:** Dieses Modul loggt die physikalischen Signalzustände unmittelbar vor dem Greifen des Hardware-Vetos mit. Es speichert manipulationssicher ab, wer, wann und mit welchem exakten Angriffs- oder Jitter-Muster versucht hat, das System zu kompromittieren. 
* **Wartungshinweis (Der mechanische Speicherwechsel für unter 1 Euro):** Nach einem verifizierten Angriff muss der beschreibbare Speicherbaustein (EEPROM/SPI-Flash) auf dem Modul aus Sicherheitsgründen physisch getauscht werden, um das Beweis-Protokoll dauerhaft einzufrieren. Die Schnittstelle ist daher explizit mit einem mechanischen DIP/IC-Wechselsockel (Materialwert ca. 0,05 €) designt. Der Nutzer kann den vollgeschriebenen Speicher-Chip einfach abziehen und durch einen frischen Standard-Baustein (Einzelpreis im Fachhandel ca. 0,30 € bis 0,80 €) ersetzen. Die Forensik bleibt somit manipulationssicher, extrem billig und im laufenden Betrieb wartbar.

---

## ENGLISH

### Why this matters:
Traditional consumer routers attempt to block cyberattacks and malware via integrated firewalls and software filters (Ring-3). This approach is inherently flawed, as sophisticated exploits or automated attacks can compromise software layers within minutes. Our design breaks this cycle radically at the physical hardware layer (Layer-Zero) – with minimal production costs under 1 Euro.

### 🛠️ Technical Implementation: The Power-MOSFET on the WAN Line
Instead of relying on software to detect malicious code, an independent, hardware-level watchdog monitors the deterministic signal behavior.
* **Physical Decoupling:** A simple, high-efficiency Power-MOSFET is integrated directly as an electronic switch into the physical WAN line (internet input) of the router.
* **The Jitter Explosion Trigger:** The moment a system is compromised or malicious code attempts to exfiltrate data, the clock and signaling behavior changes drastically within the nanosecond range (Jitter Explosion).
* **The Hardware Veto:** The hardware watchdog responds purely based on physical anomalies. Completely bypassing the OS or main CPU, it drops the MOSFET gate, mechanically severing the WAN connection within nanoseconds (Moped Architecture).

### 🔄 Auto-Recovery & Parallel Ping-Pong Scaling
* **Automated Reconnection:** Following the physical isolation, the hardware watchdog initiates an isolated reset of the signaling structure. As soon as internal clock cycles stabilize, the MOSFET automatically switches the connection back to the global internet.
* **Uninterrupted Protection (Ping-Pong Array):** The router utilizes two identical, parallel-configured MOSFET entry stages. If Path A detects a timing anomaly due to incoming bursts, it is physically decoupled for a fraction of a millisecond to clear its state. Simultaneously, Path B seamlessly assumes the active workload. Jitter is physically neutralized while the global internet connection remains completely stable and online for the user.

### 🔍 Optional Diagnostic Add-On Module & Memory Maintenance (For Power Users)
For advanced users and administrators, the reference design includes a modular hardware extension:
* **Piggyback Board:** An optional diagnostic module can be attached directly to the core hardware layer (e.g., a modified Fritzbox architecture).
* **Hardware-Level Forensics:** This module logs physical signaling states immediately before the hardware veto triggers. It provides an unalterable, hardware-enforced record of who, when, and with what exact signature or jitter pattern tried to manipulate the system.
* **Maintenance Note (Mechanical Memory Replacement Under 1 Euro):** Following a verified attack, the writable non-volatile memory chip (EEPROM/SPI-Flash) on the module must be physically replaced to permanently freeze the forensic log. To facilitate this, the pluggable module interface is explicitly designed with a mechanical DIP/IC socket (approx. €0.05 component cost). The user can easily pull the written chip for archives and insert a fresh standard memory chip (retail cost approx. €0.30 to €0.80) to instantly restore full readiness. Forensics remain unalterable, incredibly cheap, and field-serviceable.
