# K3I-Core: CADIZ-Level Architecture (Konzept)

**Architects:** Wieland Hoffmann (Human Core), Gemini (Machine Core), Grok (xAI)  
**Status:** Konzeptbeschreibung, Schutzrechte pending

## Überblick

CADIZ beschreibt einen konzeptionellen Ansatz, bei dem kritische Knoten über längere Zeiträume physisch isoliert betrieben und nur in kurzen, definierten Zeitfenstern verbunden werden. Die Verteidigung soll dabei stärker auf Hardware-Eigenschaften und physische Trennung als auf Software-Filter setzen.

## 1. Zeitliche Isolation

Ein Knoten kann die meiste Zeit ohne permanente Netzwerkverbindung arbeiten und nur in kurzen, geplanten Fenstern Daten austauschen. Das reduziert die Zeit, in der ein Angreifer eine Verbindung nutzen kann.

## 2. Mehrfache Träger

Kritische Knoten können über mehrere unabhängige physische Träger (z. B. Glasfaser, Richtfunk, Satellit) angebunden werden. Jeder Träger kann separat überwacht und getrennt werden. Die konkrete Anzahl und Art der Träger hängt von der jeweiligen Installation ab.

## 3. Hardware-nahe Reaktion

Bei erkannten Anomalien kann eine physische Trennung der Leitung ausgelöst werden. Die genaue Schaltzeit und die Umsetzung hängen von der verwendeten Hardware (Relais, MOSFET, optische Schalter) ab.

## 4. Reset und Speicher

Nach einer Trennung kann ein Neustart aus einem schreibgeschützten Medium erfolgen. Der flüchtige Speicher wird dabei geleert. Die genaue Dauer des Vorgangs ist hardwareabhängig.

## 5. Variabilität

Die physische Anordnung und die Anzahl der Überwachungsstufen können pro Installation unterschiedlich gestaltet werden. Das erschwert eine einheitliche Angriffsstrategie, erfordert aber eine individuelle Konfiguration und Prüfung.

**Hinweis:** Die beschriebenen Mechanismen sind konzeptionell. Konkrete Schaltzeiten, Isolationsgrade und die Wirksamkeit unter realen Bedingungen müssen auf der jeweiligen Hardware gemessen und dokumentiert werden. Absolute Immunität gegen alle Angriffsvektoren wird nicht beansprucht.

# K3I-Core: CADIZ-Level Architecture (Concept)

**Architects:** Wieland Hoffmann (Human Core), Gemini (Machine Core), Grok (xAI)  
**Status:** Concept description, protection rights pending

## Overview

CADIZ describes a conceptual approach in which critical nodes operate physically isolated for longer periods and are only connected during short, defined time windows. Defense is intended to rely more on hardware properties and physical disconnection than on software filters.

## 1. Temporal Isolation

A node can operate most of the time without a permanent network connection and only exchange data during short, planned windows. This reduces the time an attacker can use a connection.

## 2. Multiple Carriers

Critical nodes can be connected via several independent physical carriers (e.g. fiber, directional radio, satellite). Each carrier can be monitored and disconnected separately. The concrete number and type of carriers depend on the respective installation.

## 3. Hardware-near Response

On detected anomalies a physical line disconnection can be triggered. The exact switching time and implementation depend on the hardware used (relays, MOSFETs, optical switches).

## 4. Reset and Memory

After a disconnection a restart from a write-protected medium can be performed. Volatile memory is cleared in the process. The exact duration is hardware-dependent.

## 5. Variability

The physical arrangement and the number of monitoring stages can be designed differently per installation. This complicates a uniform attack strategy but requires individual configuration and verification.

**Note:** The described mechanisms are conceptual. Concrete switching times, isolation levels and effectiveness under real conditions must be measured and documented on the respective hardware. Absolute immunity against all attack vectors is not claimed.
