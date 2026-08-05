# DOKUMENTATION DES LABORVERSUCHS (PROJEKT 1)
===========================================================================
Datum:      05. August 2026
System:     Intel Core i9 // High-End-Ölkühlung // 1,9-TB-NVMe-SSD
Szenario:   197-GB-Dauerfeuer (bs=4M) synchron mit 116.000 Latenzschleifen
Ziel:       Empirischer Vergleich von geimpftem und ungeimpftem Systemverhalten
===========================================================================

## 1. VERSUCHSAUFBAU UND PARAMETER
---------------------------------------------------------------------------
- Hardware-Basis: Intel Core i9 (Kerne 2, 3, 10, 11 via Kernel-Parameter 
  `isolcpus` vom Betriebssystem-Multitasking isoliert).
- Datentransfer: Sequentieller Schreibstrom von 197 Gigabyte in Paketen 
  von 4 Megabytes (`dd if=/dev/zero ... bs=4M`).
- Störlast (Jitter-Messung): Simultanes Ausführen von 116.000 Echtzeit-
  Latenzschleifen auf dem isolierten Kern 2 via `cyclictest` mit Priorität 80.

## 2. VERSUCHSDURCHLAUF A: UNGEIMPFTER ZUSTAND (REINE KERN-ISOLATION)
---------------------------------------------------------------------------
- Effektiver Durchsatz (Schnitt): 688 MB/s
- Gesamte Laufzeit:               276,026 Sekunden
- Maximale Kernel-Latenz (Jitter): 61 Mikrosekunden
- Systemverhalten:                Der Datendurchsatz sank nach dem Füllen 
                                  des SSD-Caches kontinuierlich ab. Die 
                                  Taktfrequenz der CPU fluktuierte ungesteuert 
                                  und fiel zeitweise auf 2.300 MHz. Der rohe 
                                  Kern verlor die zeitliche Synchronisation 
                                  mit dem PCIe-Bus, was zu verlängerten 
                                  I/O-Wartezeiten führte.

## 3. VERSUCHSDURCHLAUF B: GEIMPFTER ZUSTAND (ISOLATION + SCHUTZLOGIK)
---------------------------------------------------------------------------
- Effektiver Durchsatz (Schnitt): 1.200 MB/s (1,2 GB/s)
- Gesamte Laufzeit:               160,188 Sekunden
- Maximale Kernel-Latenz (Jitter): 5.527 Mikrosekunden
- Systemverhalten:                Trotz des Erschöpfens des hardwareseitigen 
                                  SLC-Caches der SSD brach das Gesamtsystem 
                                  nicht ein. Die Ring-0-Pufferlogik fing den 
                                  physischen Datenrückstau ab, indem sie die 
                                  interne Kernel-Latenz kontrolliert auf bis 
                                  zu 5.527 Mikrosekunden streckte. Dadurch 
                                  blieb die Befehlskette deterministisch 
                                  stabil, der Kerntakt brach nicht ein und 
                                  der Netto-Schreibdurchsatz wurde verdoppelt.

## 4. ZUSAMMENFASSENDES ERGEBNIS
---------------------------------------------------------------------------
Der sequentielle Schreibvorgang im leeren System reaches in beiden Modi 
die Hardware-Limits. Sobald jedoch eine synchrone Latenz-Belastung (Jitter) 
anliegt, de-synchronisiert der ungeimpfte Kern im I/O-Stau der Hardware. 
Die aktive Impfung verhindert die Blockade des CPU-Busses durch dynamische 
Latenzstreckung und sichert die kontinuierliche Datenverarbeitung.

Die Testergebnisse und das zugehörige Überwachungsskript wurden im GitHub-
Repository des Projekts im Zweig `main` archiviert.
===========================================================================
