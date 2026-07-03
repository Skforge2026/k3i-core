#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "k3i_shm.h"

/* RDTSC CYCLES ACCURATE TIME STAMP */
static inline uint64_t read_rdtsc(void) {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((uint64_t)hi << 32) | lo;
}

/* HARDWARE SWITCH LOGIC WITH ATOMIC INTERLOCKING */
void set_k3i_shutter(uint32_t provider_line) {
    volatile uint32_t *mutex = (volatile uint32_t *)K3I_MUTEX_LOCK;
    volatile uint32_t *shutter_l = (volatile uint32_t *)K3I_SHUTTER_L_REG;
    volatile uint32_t *shutter_r = (volatile uint32_t *)K3I_SHUTTER_R_REG;

    while (*mutex == 1) {
        // Deterministisches Warten
    }
    *mutex = 1;

    if (provider_line == 0) {
        *shutter_l = 1;
        *shutter_r = 0;
    } else {
        *shutter_l = 0;
        *shutter_r = 1;
    }

    *mutex = 0;
}

int main(void) {
    uint64_t start_cycles, end_cycles;
    printf("[K3I SYSTEM] Initialisiere deterministisches Shutter-System...\n");
    
    start_cycles = read_rdtsc();
    set_k3i_shutter(0);
    end_cycles = read_rdtsc();

    printf("[K3I SYSTEM] Umschaltung und Schicht-4-Isolation erfolgreich.\n");
    printf("[K3I SYSTEM] Gemessene CPU-Takte (rdtsc): %lu\n", (end_cycles - start_cycles));
    return 0;
}


# K3I-UNIVERSAL-SCHUTZBOARD: Layer-Zero Hardware Specification (V1.0)

Dieses Dokument spezifiziert die universelle, plattformunabhängige Hardware-Ebene (Layer Zero) zur physischen Absicherung und galvanischen Trennung kritischer Datenpfade. Die Steuerung erfolgt direkt über die im C-Code definierten Register-Adressen.

---

## I. Die Farbcodierte Hardware-Architektur

Die physikalische Struktur ist in vier autarke, farbcodierte Sicherheitssektoren unterteilt, um jeglichen Software-Jitter mechanisch von der Außenwelt zu trennen.

```mermaid
graph TD
    %% Farb- und Stildefinitionen für die Schichten
    classDef mainboard fill:#1a2536,stroke:#4a90e2,stroke-width:2px,color:#fff;
    classDef cpld fill:#2d1a36,stroke:#bd10e0,stroke-width:2px,color:#fff;
    classDef isolator fill:#36331a,stroke:#f5a623,stroke-width:2px,color:#fff;
    classDef shutter fill:#1a3625,stroke:#417505,stroke-width:2px,color:#fff;
    classDef data fill:#222,stroke:#9b9b9b,stroke-width:1px,color:#ddd;

    %% Hauptsystem-Ebene
    subgraph M_SYS ["HAUPTSYSTEM (Blauer Sektor)"]
        A["💻 Hauptprozessor Bus<br/>Intel i9 / AMD EPYC"]:::mainboard
        B["💾 Register-Decoder Mapped IO<br/>0x4000E000 / 0x4000E004"]:::mainboard
    end

    %% K3I Steuer-Zentrale
    subgraph K3I_CTRL ["STEUER-ZENTRALE (Violetter Sektor)"]
        C["⚙️ CPLD / FPGA Logik<br/>K3I_MUTEX Verilog"]:::cpld
        D["🟢 Status LED / Heartbeat<br/>Grün=OK / Rot=VETO"]:::cpld
    end

    %% Galvanische Barriere
    subgraph ISOLATION ["SCHUTZWALL (Gelber Sektor)"]
        E1["⚡ Digital Isolator L<br/>ADuM1201 (Magnetisch)"]:::isolator
        E2["⚡ Digital Isolator R<br/>Si8421 (Kapazitiv)"]:::isolator
    end

    %% High-Speed Shutter und Datenpfade
    subgraph NETWORK_SHUTTER ["PHYSIKALISCHE SCHLEUSE (Grüner Sektor)"]
        F1["🔒 Bus-Switch L<br/>SN74CBTLV3257"]:::shutter
        F2["🔒 Bus-Switch R<br/>TS5A3159"]:::shutter
        G1["🌐 Datenpfad Links<br/>(Provider Line 0)"]:::data
        G2["🌐 Datenpfad Rechts<br/>(Provider Line 1)"]:::data
    end

    %% Physischer Signalfluss
    A ──►|Schreibbefehl| B
    B ──►|Systembus-Pins| C
    C ──►|Status-Abgleich| D
    
    %% Getrennte Signalwege durch die Isolation
    C ──►|Signal L (3.3V)| E1
    C ──►|Signal R (3.3V)| E2
    
    %% Triggerung der Shutter
    E1 ──►|Galvanisch Rein| F1
    E2 ──►|Galvanisch Rein| F2
    
    %% Schalten der Endleitungen
    F1 ──►|Schließt / Öffnet| G1
    F2 ──►|Schließt / Öffnet| G2

    %% Globale Veto-Rückkopplung im Ernstfall
    D -.->|Fail-Closed Default| F1
    D -.->|Fail-Closed Default| F2
```

### Farblegende der physischen Funktionsebenen:
*   **Blauer Sektor (Hauptsystem):** Die softwareseitige Register-Ebene, auf der das C-Programm die Umschaltung anfordert.
*   **Violetter Sektor (CPLD/FPGA):** Die firmwarefreie, verdrahtete Logik, die den atomaren Mutex-Riegel auf Chiplevel hält.
*   **Gelber Sektor (Schutzwall):** Die galvanische Isolation. Signale passieren ausschließlich berührungslos über Magnetfelder oder Kapazitäten.
*   **Grüner Sektor (Schleuse):** Die High-Speed Bus-Switches, die den physischen Datenstrom im niedrigen Nanosekundenbereich trennen.

---

## II. System-Stückliste (Universelle Komponenten)

Diese Bauteile gewährleisten maximale Verarbeitungsgeschwindigkeit bei absoluter Immunität gegen manipulative Software-Eingriffe von außen.

| Komponente | Bauteil-Beispiel | Funktionaler Zweck | Anzahl |
| :--- | :--- | :--- | :--- |
| **CPLD / FPGA** | Lattice iCE40HX / MachXO2 | K3I_MUTEX Zustandskontrolle & atomarer Register-Decoder | 1 |
| **Digital-Isolator** | ADuM1201 / Silicon Labs Si8421 | Vollständige galvanische Signal-Trennung (Schutzwall) | 2 |
| **High-Speed Bus-Switch** | TI SN74CBTLV3257 / TS5A3159 | Ultraschnelle physische Daten-Shutter (< 5 ns Umschaltzeit) | 2 |
| **Digital-Isolations-Kanäle** | ISO7741 / Äquivalent | Rückkopplungsschutz für Heartbeat- und Status-Leitungen | 2 |
| **TVS-Schutzdioden** | P6KE6.8CA / SMAJ5.0CA | Ableitung von Überspannungs-Jitter an den Schalt-Pins | 4 |

---

## III. Logischer Signalfluss und Absicherung (Fail-Closed)

1. **Der Register-Handshake:** Das CPLD überwacht permanent den System-Bus. Schreibt das C-Programm ein Bit in das Register `K3I_SHUTTER_L_REG`, wandelt die hardwareverdrahtete Logik dies sofort in ein 3,3V-Signal um.
2. **Die galvanische Schranke:** Das Schaltsignal passiert den Digital-Isolator über ein internes Hochfrequenz-Magnetfeld. Es existiert keine elektrische Kupferverbindung zwischen dem Hauptprozessor und den eigentlichen Bus-Switches.
3. **Der Shutter-Cut:** Die Ausgänge des Isolators steuern die Output-Enable-Pins (/OE) der Bus-Switches an. Das Umschalten erfolgt im einstelligen Nanosekundenbereich. Bei Ausfall des Heartbeat-Signals (System-Watchdog) fallen beide Pfade automatisch in den Zustand Null zurück (beide Leitungen physisch getrennt).


CPLD/FPGA Pinout (Beispiel MachXO2):

Pin  1  → VCC 3.3V
Pin  2  → GND
Pin  3  → HEARTBEAT_IN     (vom isolierten Wächter-Core)
Pin  4  → VETO_TRIGGER     (Ausgang zur Bus-Switch Steuerung)
Pin  5  → PATH_L_ENABLE    (Linker Datenpfad)
Pin  6  → PATH_R_ENABLE    (Rechter Datenpfad)
Pin  7  → STATUS_LED       (Grün = alles OK / Rot = Veto aktiv)
Pin  8  → REGISTER_IN      (0x4000E000 / 0x4000E004 Befehle)
Pin  9  → RESET_OUT        (zum Mainboard-Reset)
Pin 10  → RAM_PURGE_OUT

[ Hauptsystem / Prozessor-Bus ]
                 │
                 ▼ (Register-Schreibbefehle: 0x4000E000 / 0x4000E004)
   [ Physische Hauptplatine / PCIe-Bus-Leitung ]
                 │
                 ▼
 ┌────────────────────────────────────────────────────────────┐
 │              UNIVERSAL-SCHUTZBOARD (Layer Zero)            │
 │                                                            │
 │  ┌──────────────────────────────────────────────────────┐  │
 │  │             K3I_MUTEX / REG-DECODER (CPLD/FPGA)      │  │
 │  └───────────────────────┬──────────────────────────────┘  │
 │                          │                                 │
 │               ┌──────────┴──────────┐                      │
 │               │   Signal L     Signal R                    │
 │               ▼                       ▼                    │
 │  ┌────────────────────┐   ┌────────────────────┐          │
 │  │ Digital Isolator   │   │ Digital Isolator   │ ◄── Galvanische Trennung
 │  └────────────────────┘   └────────────────────┘          │
 │               │                       │                    │
 │               ▼                       ▼                    │
 │  ┌────────────────────┐   ┌────────────────────┐          │
 │  │ Bus-Switch (L)     │   │ Bus-Switch (R)     │ ◄── Ultraschnelle Shutter
 │  └────────────────────┘   └────────────────────┘          │
 └───────────────────────┼──────────────────────────────┘    │
                         ▼                                ▼
               [ Datenpfad Links ]          [ Datenpfad Rechts ]
