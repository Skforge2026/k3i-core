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
```mermaid
graph TD
    subgraph Provider ["LAYER ZERO: PROVIDER INFRASCHLEIFE"]
        LAN_In([Physisches Ethernet-Signal / RJ45])
    end

    subgraph Veto_Schloss ["K3I VETO-SCHLOSS / ROUTER MATRIX"]
        Gatter{ODER-Logik 74LVC32A}
        
        subgraph Stufe_1 ["STUFE 1: ELEKTRONISCH (<10ns)"]
            HF_Schalter[Analog Devices ADG904 HF-Schalter]
        end
        
        subgraph Stufe_2 ["STUFE 2: GALVANISCH (5-10ms)"]
            Relais[Panasonic DS4E-M-DC5V Relais]
        end
    end

    subgraph Host_System ["i9 HAUPTRECHNER / KERNEL CONTROLLER"]
        Wächter(((Isolierter Wächter-Core / rdtsc)))
        PHY[(Netzwerk-Controller / enp4s0)]
    end

    %% Signalwege
    LAN_In -->|Differenzielle Paare DA/DB/DC/DD| HF_Schalter
    
    Wächter -->|Symmetrie-Überwachung 1+1=1| Gatter
    Gatter -->|Sofort-Veto-Signal| HF_Schalter
    
    HF_Schalter -->|Verzögerungsfreie Blockade| Relais
    Relais -->|Mechanische Luftbrücke| PHY

    %% Nativer GitHub-Mermaid Design-Stil
    style Provider fill:#e1f5fe,stroke:#01579b,stroke-width:2px
    style Veto_Schloss fill:#fce4ec,stroke:#880e4f,stroke-width:2px
    style Stufe_1 fill:#ffebee,stroke:#b71c1c,stroke-width:1px
    style Stufe_2 fill:#fff8e1,stroke:#f57f17,stroke-width:1px
    style Host_System fill:#f5f5f5,stroke:#424242,stroke-width:2px
    style Gatter fill:#fff9c4,stroke:#f57f17,stroke-width:2px
    style HF_Schalter fill:#ffffff,stroke:#b71c1c,stroke-width:2px
    style Relais fill:#ffffff,stroke:#f57f17,stroke-width:2px
    style Wächter fill:#ffecb3,stroke:#ff8f00,stroke-width:2px
    style PHY fill:#e8f5e9,stroke:#2e7d32,stroke-width:2px
    style LAN_In fill:#ffffff,stroke:#01579b,stroke-width:2px


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

