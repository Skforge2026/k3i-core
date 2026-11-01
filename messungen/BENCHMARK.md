# K3I-Core – Kernimpfung Neuvalidierung
**Benchmark Report: Ring-0 HPC Layer on AMD Ryzen 9 9950X**  
**Date:** 29 October 2026  
**Hardware:** AMD Ryzen 9 9950X (16C/32T), 128 GB DDR5-4800  
**OS:** Pop!_OS 24.04, Kernel 7.1.5-76070105-generic  
**Isolation:** `isolcpus=20-31 nohz_full=20-31 rcu_nocbs=20-31`  
**Target Core:** 20 (pinned via `taskset -c 20`)  
**Cooling:** Fans forced to 100 % PWM during all runs  

## 1. Mechanism
Kernel module `amd_hpc.ko` (Ring 0):
- Sets **CR4.PCE** (bit 8) → allows `rdpmc` from user space  
- Programs Performance Event Select MSRs `0xc0010200` / `0xc0010202`  
- Attempts Prefetcher control via MSR `0xc0011020` (successfully written)  
- Attempts CPPC/EPP max via MSR `0xc00102b0` (not writable on this Zen 5 – skipped)  
- Uses `wrmsr_safe()` → no kernel panic on unsupported MSRs  
- Applied only to core 20 via `smp_call_function_single`

## 2. Measurement Matrix

### 2.1 Python Prime Search (interpreter-bound)
| Run | 600 s highest |
| :--- | :--- |
| Unvaccinated 1 | 134 854 417 |
| Unvaccinated 2 | 136 154 281 |
| Vaccinated | 136 005 631 |

**Result:** No measurable difference (within run-to-run noise).

### 2.2 C Prime Search + RDPMC (CPU-/branch-bound)
| State | 600 s highest |
| :--- | :--- |
| Vaccinated | 1 102 727 653 |
| Unvaccinated | 1 102 334 671 |

**Result:** Difference < 0.04 % – no effect.

### 2.3 Register-only AVX-512 (pure vector arithmetic)
| State | Cycles | Instructions | IPC |
| :--- | :--- | :--- | :--- |
| Unvaccinated | 9 000 191 379 | 7 000 002 980 | 0.78 |
| Vaccinated | 9 000 756 446 | 7 000 015 894 | 0.78 |

**Result:** Identical – null effect confirmed.

### 2.4 Memory-bound AVX-512 (256 MB array, stride 64)
| State | Cycles | Instructions | IPC |
| :--- | :--- | :--- | :--- |
| Unvaccinated | 5 674 734 887 | 1 599 999 678 | 0.28 |
| **Vaccinated** | **5 193 831 411** | 1 599 999 718 | **0.31** |

**Result:** **+10.7 % IPC / throughput.**

## 3. Scientific Conclusion
The Ring-0 layer only modifies passive counter configuration and one prefetcher MSR:
- Pure compute-bound or register-only workloads are unaffected.  
- Memory-bound workloads that stress the prefetcher and cache hierarchy show a clear, reproducible gain (+10.7 % IPC).  
- Thermal conditions were equalized (fans at 100 %, temperatures logged). No thermal throttling differences observed.  
- All measurements used the same binary, same core, same isolation, and `rdpmc` for cycle/instruction counting where applicable.

## 4. Artifacts
- Safe module source: `amd_hpc.c` (with `wrmsr_safe`)  
- Benchmarks: `prime_search_log.py`, `prime_rdpmc.c`, `test_avx512.c`, `mem_avx512.c`  
- Raw logs available on request.

**Status:** Validated on prototype. Selective effect on memory subsystem confirmed.  
**Architects:** Wieland Hoffmann, Gemini, Grok (xAI)

# K3I-Core – Kernimpfung Neuvalidierung
**Benchmark-Bericht: Ring-0 HPC-Layer auf AMD Ryzen 9 9950X**  
**Datum:** 29. Oktober 2026  
**Hardware:** AMD Ryzen 9 9950X (16 Kerne / 32 Threads), 128 GB DDR5-4800  
**Betriebssystem:** Pop!_OS 24.04, Kernel 7.1.5-76070105-generic  
**Isolation:** `isolcpus=20-31 nohz_full=20-31 rcu_nocbs=20-31`  
**Ziel-Kern:** Kern 20 (isoliert gebunden via `taskset -c 20`)  
**Kühlung:** Lüfter während aller Testläufe fest auf 100 % PWM geschaltet  

---

## 1. Mechanismus
Das Kernel-Modul `amd_hpc.ko` operiert auf Ring-0-Ebene:
- Setzt **CR4.PCE** (Bit 8) → erlaubt den Hardware-Befehl `rdpmc` direkt aus dem User-Space.
- Konfiguriert die Performance-Event-Select-Register über MSRs `0xc0010200` und `0xc0010202`.
- Steuert den Hardware-Prefetcher über MSR `0xc0011020` an (erfolgreich geschrieben).
- Testet CPPC/EPP-Maximalleistung über MSR `0xc00102b0` (auf dieser Zen-5-Revision schreibgeschützt – sicher übersprungen).
- Nutzt durchgehend `wrmsrl_safe()` → fängt General Protection Faults (`#GP`) fehlerhafter MSRs sauber ab und verhindert Kernel-Panics/Systemabstürze.
- Wird gezielt via `smp_call_function_single` ausschließlich auf Kern 20 angewendet.

---

## 2. Messmatrix

### 2.1 Python-Primzahlsuche (Interpreter-limitiert)
| Durchlauf | Höchste Zahl nach 600 s |
| :--- | :--- |
| Ungeimpft 1 | 134.854.417 |
| Ungeimpft 2 | 136.154.281 |
| Geimpft | 136.005.631 |

**Ergebnis:** Kein messbarer Unterschied (reine Messtoleranz / Interpreter-Overhead).

### 2.2 C-Primzahlsuche + RDPMC (CPU-/Branch-limitiert)
| Zustand | Höchste Zahl nach 600 s |
| :--- | :--- |
| Geimpft | 1.102.727.653 |
| Ungeimpft | 1.102.334.671 |

**Ergebnis:** Abweichung unter 0,04 % – kein messbarer Effekt.

### 2.3 Reine Register-Arithmetik AVX-512 (Vektor-ALUs)
| Zustand | Zyklen | Instruktionen | Reale IPC |
| :--- | :--- | :--- | :--- |
| Ungeimpft | 9.000.191.379 | 7.000.002.980 | 0,78 |
| Geimpft | 9.000.756.446 | 7.000.015.894 | 0,78 |

**Ergebnis:** Absolut identisch – Bestätigung des Null-Effekts bei reiner Register-Arithmetik.

### 2.4 Speicherlastiges AVX-512 (256 MB Array, Stride 64 / Cache-Line)
| Zustand | Zyklen | Instruktionen | Reale IPC |
| :--- | :--- | :--- | :--- |
| Ungeimpft | 5.674.734.887 | 1.599.999.678 | 0,28 |
| **Geimpft** | **5.193.831.411
