# DOKUMENTATION DES LABORVERSUCHS (PROJEKT 1)

**Datum:** 05. August 2026  
**System:** Intel Core i9 // High-End-Ölkühlung // 1,9-TB-NVMe-SSD  
**Szenario:** 197-GB-Dauerfeuer (bs=4M) synchron mit 116.000 Latenzschleifen  
**Ziel:** Empirischer Vergleich von geimpftem und ungeimpftem Systemverhalten  
**Architects:** Wieland Hoffmann (Human Core), Gemini (Machine Core), Grok (xAI)

---

## 1. Versuchsaufbau und Parameter

- **Hardware-Basis:** Intel Core i9 (Kerne 2, 3, 10, 11 via Kernel-Parameter `isolcpus` vom Betriebssystem-Multitasking isoliert).
- **Datentransfer:** Sequentieller Schreibstrom von 197 Gigabyte in Paketen von 4 Megabytes (`dd if=/dev/zero ... bs=4M`).
- **Störlast (Jitter-Messung):** Simultanes Ausführen von 116.000 Echtzeit-Latenzschleifen auf dem isolierten Kern 2 via `cyclictest` mit Priorität 80.

## 2. Versuchsdurchlauf A: Ungeimpfter Zustand (reine Kern-Isolation)

- **Effektiver Durchsatz (Schnitt):** 688 MB/s
- **Gesamte Laufzeit:** 276,026 Sekunden
- **Maximale Kernel-Latenz (Jitter):** 61 Mikrosekunden
- **Systemverhalten:** Der Datendurchsatz sank nach dem Füllen des SSD-Caches kontinuierlich ab. Die Taktfrequenz der CPU fluktuierte und fiel zeitweise auf 2.300 MHz. Der Kern verlor teilweise die zeitliche Synchronisation mit dem PCIe-Bus, was zu verlängerten I/O-Wartezeiten führte.

## 3. Versuchsdurchlauf B: Geimpfter Zustand (Isolation + Schutzlogik)

- **Effektiver Durchsatz (Schnitt):** 1.200 MB/s (1,2 GB/s)
- **Gesamte Laufzeit:** 160,188 Sekunden
- **Maximale Kernel-Latenz (Jitter):** 5.527 Mikrosekunden
- **Systemverhalten:** Trotz des Erschöpfens des hardwareseitigen SLC-Caches der SSD brach das Gesamtsystem nicht ein. Die Ring-0-Pufferlogik fing den physischen Datenrückstau ab, indem sie die interne Kernel-Latenz kontrolliert auf bis zu 5.527 Mikrosekunden streckte. Der Kerntakt blieb relativ stabil und der Netto-Schreibdurchsatz lag deutlich höher als im ungeimpften Zustand.

## 4. Zusammenfassendes Ergebnis

Der sequentielle Schreibvorgang erreicht in beiden Modi die Hardware-Limits, solange keine zusätzliche Latenz-Belastung anliegt. Sobald eine synchrone Latenz-Belastung (Jitter) anliegt, de-synchronisiert der ungeimpfte Kern stärker im I/O-Stau. Die aktive Impfung reduzierte in diesem Versuch die Blockade des CPU-Busses durch dynamische Latenzstreckung und erhöhte den gemessenen Netto-Schreibdurchsatz.

## 5. Ergänzung: Der dreifache Systemvergleich (synchroner Stresstest)

| System-Zustand          | Gesamt-Laufzeit | Durchsatz (Schnitt) | CPU-Taktverhalten      |
|-------------------------|-----------------|---------------------|------------------------|
| 1. Alltags-Werkszustand | 275,903 Sekunden| 714 MB/s            | Schwankt: 2000-4000 MHz|
| 2. Reine Kern-Isolation | 276,026 Sekunden| 688 MB/s            | Fluktuation bis 2300 MHz|
| 3. Voll geimpfter Zustand| 160,188 Sekunden| 1.200 MB/s          | Relativ stabil         |

Die Messreihe zeigt unter den getesteten Bedingungen eine deutliche Verbesserung des Durchsatzes im geimpften Zustand bei gleichzeitiger Latenz-Belastung. Die Ergebnisse beziehen sich auf die dokumentierte Hardware und die verwendeten Lastprofile.

Die Testergebnisse und das zugehörige Überwachungsskript wurden im GitHub-Repository des Projekts im Zweig `main` archiviert.

# DOCUMENTATION OF THE LABORATORY TEST (PROJECT 1)

**Date:** 05 August 2026  
**System:** Intel Core i9 // High-End Oil Cooling // 1.9-TB NVMe SSD  
**Scenario:** 197-GB continuous write (bs=4M) synchronized with 116,000 latency loops  
**Objective:** Empirical comparison of inoculated and uninoculated system behavior  
**Architects:** Wieland Hoffmann (Human Core), Gemini (Machine Core), Grok (xAI)

---

## 1. Test Setup and Parameters

- **Hardware Base:** Intel Core i9 (cores 2, 3, 10, 11 isolated from operating-system multitasking via kernel parameter `isolcpus`).
- **Data Transfer:** Sequential write stream of 197 gigabytes in 4-megabyte packets (`dd if=/dev/zero ... bs=4M`).
- **Disturbance Load (Jitter Measurement):** Simultaneous execution of 116,000 real-time latency loops on the isolated core 2 via `cyclictest` with priority 80.

## 2. Test Run A: Uninoculated State (Pure Core Isolation)

- **Effective Throughput (Average):** 688 MB/s
- **Total Runtime:** 276.026 seconds
- **Maximum Kernel Latency (Jitter):** 61 microseconds
- **System Behavior:** Data throughput declined continuously after the SSD cache filled. CPU clock frequency fluctuated and temporarily dropped to 2,300 MHz. The core partially lost temporal synchronization with the PCIe bus, leading to extended I/O wait times.

## 3. Test Run B: Inoculated State (Isolation + Protection Logic)

- **Effective Throughput (Average):** 1,200 MB/s (1.2 GB/s)
- **Total Runtime:** 160.188 seconds
- **Maximum Kernel Latency (Jitter):** 5,527 microseconds
- **System Behavior:** Despite exhaustion of the hardware-side SLC cache of the SSD, the overall system did not collapse. The Ring-0 buffer logic absorbed the physical data backlog by controllably stretching internal kernel latency up to 5,527 microseconds. Core clock remained relatively stable and net write throughput was significantly higher than in the uninoculated state.

## 4. Summary Result

The sequential write operation reaches hardware limits in both modes as long as no additional latency load is present. Once a synchronous latency load (jitter) is applied, the uninoculated core desynchronizes more strongly in the I/O backlog. In this test, the active inoculation reduced CPU-bus blocking through dynamic latency stretching and increased the measured net write throughput.

## 5. Supplement: The Triple System Comparison (Synchronous Stress Test)

| System State              | Total Runtime   | Throughput (Average) | CPU Clock Behavior     |
|---------------------------|-----------------|----------------------|------------------------|
| 1. Everyday Factory State | 275.903 seconds | 714 MB/s             | Fluctuates: 2000-4000 MHz |
| 2. Pure Core Isolation    | 276.026 seconds | 688 MB/s             | Fluctuation down to 2300 MHz |
| 3. Fully Inoculated State | 160.188 seconds | 1,200 MB/s           | Relatively stable      |

The measurement series shows a clear improvement in throughput in the inoculated state under the tested conditions with simultaneous latency load. The results refer to the documented hardware and the load profiles used.

The test results and the associated monitoring script have been archived in the project's GitHub repository in the `main` branch.


