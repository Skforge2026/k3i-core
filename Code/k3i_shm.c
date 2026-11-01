# K3I-Universal-Schutzboard: Layer-Zero Hardware Specification (Konzept)

**Architects:** Wieland Hoffmann (Human Core), Gemini (Machine Core), Grok (xAI)  
**Status:** Konzeptbeschreibung, Schutzrechte pending

## Architektur

Das Board besteht aus vier Sektoren:

- **Hauptsystem:** Register-Schreibbefehle vom Prozessor
- **CPLD/FPGA:** Mutex und Register-Decoder
- **Galvanische Isolation:** Digital-Isolatoren (z. B. ADuM1201 / Si8421)
- **Bus-Switches:** High-Speed-Schalter (z. B. SN74CBTLV3257 / TS5A3159)

## Stückliste (Beispiel)

| Komponente          | Beispiel              | Anzahl |
|---------------------|-----------------------|--------|
| CPLD / FPGA         | Lattice iCE40HX       | 1      |
| Digital-Isolator    | ADuM1201 / Si8421     | 2      |
| Bus-Switch          | SN74CBTLV3257         | 2      |
| TVS-Schutzdioden    | P6KE6.8CA             | 4      |

## Signalfluss

1. C-Programm schreibt in die Register.
2. CPLD wandelt den Befehl in ein Steuersignal um.
3. Signal passiert den Digital-Isolator.
4. Bus-Switch trennt oder schließt den Datenpfad.

Bei Ausfall des Heartbeat-Signals gehen beide Pfade in den getrennten Zustand.

**Hinweis:** Die Schaltzeiten und die Immunität gegen Software-Eingriffe hängen von der konkreten Hardware ab. Absolute Werte werden nicht beansprucht

# K3I Universal Protection Board: Layer-Zero Hardware Specification (Concept)

**Architects:** Wieland Hoffmann (Human Core), Gemini (Machine Core), Grok (xAI)  
**Status:** Conceptual Specification, Intellectual Property Rights Pending

## Architecture

The board consists of four sectors:

- **Main System:** Register write commands from the processor

- **CPLD/FPGA:** Mutex and register decoder

- **Galvanic Isolation:** Digital isolators (e.g., ADuM1201 / Si8421)

- **Bus Switches:** High-speed switches (e.g., SN74CBTLV3257 / TS5A3159)


## Bill of Materials (Example)

| Component          | Example               | Quantity |
|--------------------|-----------------------|----------|
| CPLD / FPGA        | Lattice iCE40HX       | 1        |
| Digital Isolator   | ADuM1201 / Si8421     | 2        |
| Bus Switch         | SN74CBTLV3257         | 2        |
| TVS Diodes         | P6KE6.8CA             | 4        |

## Signal Flow

1. C program writes to the registers.

2. CPLD converts the command into a control signal.
3. Signal passes through the digital isolator.
4. Bus switch connects or disconnects the data path.

In the event of a heartbeat signal loss, both paths enter the disconnected state.

**Note:** Switching times and immunity to software tampering depend on the specific hardware implementation. Absolute values are not claimed.
