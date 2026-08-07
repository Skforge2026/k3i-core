# K3I-Router-Reference-Design: Hardware-Enforced Layer-Zero Protection (V1.0)

**Ziel:** Ein kostengünstiges, nachrüstbares Sicherheits-Design für Consumer-Router (z. B. FRITZ!Box-ähnliche Geräte), das echte physikalische Isolation im Ring-0 ermöglicht.

---

### 🧮 1. Architektur-Übersicht
Moderne Heimrouter verfügen meist über Multi-Core-Prozessoren (z. B. Quad-Core ARM/MIPS). K3I bricht die übliche dynamische Lastverteilung auf:

* **Kerne 0, 1 & 3 (Normalbetrieb):** Verarbeiten den regulären Netzwerkverkehr, das Routing, das Web-Interface, WLAN, DHCP und die Telefonie (Ring-3/4).
* **Kern 2 (Isolierter K3I-Wächter):** Wird über den Kernel-Parameter `isolcpus=2` vollständig vom restlichen System isoliert. Hier läuft ausschließlich die deterministische `rdtsc`-Überwachungsschleife im Ring-0, unbeeinflusst von der Software-Last.

---

### ⚙️ 2. Physischer Hardware-Veto-Mechanismus
Der entscheidende Unterschied zu reiner Software-Sicherheit liegt in der direkten Koppelung an das Substrat:



* **Die Schaltung:** Die Hauptstromversorgung des WAN-Ports oder des integrierten Modems wird über einen ultraschnellen elektronischen Schalter (einen Power-MOSFET) auf der Platine geführt.
* **Die Steuerung:** Dieser MOSFET wird ausschließlich vom isolierten Kern 2 über einen dedizierten GPIO-Pin kontrolliert.
* **Der Signalabriss (Veto):** Im Normalzustand hält Kern 2 den Pin auf `HIGH` (Verbindung aktiv). Bei einer erkannten Anomalie oder einer `rdtsc`-Zyklen-Explosion fällt der Pin in Nanosekunden auf `LOW`. 
* **Der Effekt:** Es erfolgt ein sofortiger physischer Stromabriss. Der Router ist augenblicklich und mechanisch zu 100 % vom Internet getrennt, noch bevor Schadcode Daten abgreifen oder das lokale Netzwerk infizieren kann.

---

### 🌐 3. Vorteile und Nutzen
* **Sehr geringe Mehrkosten:** Zusätzliche Bauteile (MOSFET und Leiterbahnen-Anpassung) liegen in der Massenproduktion unter 1,00 Euro.
* **Volle Kompatibilität:** Das bestehende Betriebssystem des Routers bleibt weitgehend unverändert. Alle Komfortfunktionen für den Endanwender bleiben im Userspace erhalten.
* **Echter Schutz:** Schützt alle Geräte im Heimnetzwerk (PCs, NAS-Speicher, Smart-Home) vor externen Angriffen und Ransomware, selbst wenn die Software-Firewall bereits vollständig kompromittiert ist.
* **Einfache Nachrüstung:** Für Entwickler und die OpenWrt-Community auf Entwicklerboards mit direktem GPIO-Zugriff sofort experimentell umsetzbar.

%% Das zweistufige K3I Veto-Schloss (Hardware-Ebene)
subgraph Veto_Schloss [K3I VETO-SCHLOSS / ROUTER MATRIX]
    Gatter[Hardware ODER-Logik: 74LVC32A]
    
    %% Stufe 1: Ultraschnelle elektronische Abriegelung
    subgraph Stufe_1 [STUFE 1: ELEKTRONISCH <10ns]
        HF_Schalter[Analog Devices ADG904 / HF-Halbleiter-Matrix]
    end
    
    %% Stufe 2: Physische, galvanische Trennung
    subgraph Stufe_2 [STUFE 2: GALVANISCH 5-10ms]
        Relais[Panasonic DS4E-M-DC5V / Bistabiler Endschalter]
    end
end

%% Host-System und Netzwerk-Controller
subgraph Host_System [i9 HAUPTRECHNER / KERNEL CONTROLLER]
    Wächter[Isolierter Wächter-Core / Taktprüfung rdtsc]
    PHY[Netzwerk-Controller / enp4s0 Schnittstelle]
end

%% Signalfluss und logische Verriegelung
LAN_In -->|Differenzielle Paare DA/DB/DC/DD| HF_Schalter

Wächter -->|Symmetrie-Überwachung 1+1=1| Gatter
Gatter -->|Sofort-Veto-Signal| HF_Schalter

HF_Schalter -->|Verzögerungsfreie Blockade| Relais
Relais -->|Mechanische Luftbrücke| PHY

%% Design-Stile für die Grafik
style Veto_Schloss fill:#f9f,stroke:#333,stroke-width:2px
style Stufe_1 fill:#ffe6e6,stroke:#cc0000,stroke-width:1px
style Stufe_2 fill:#fff2cc,stroke:#d6b656,stroke-width:1px
style Wächter fill:#ff9,stroke:#333,stroke-width:2px
