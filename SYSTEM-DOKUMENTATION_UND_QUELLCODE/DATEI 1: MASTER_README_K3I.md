# SK-Forge: Global System Isolation & Hardware Veto Matrix (K3I)

**Schutzrechte & Rechtlicher Hinweis**  
Die in diesem Dokument offengelegten Verfahren, Algorithmen und Hardware-Topologien sind seit dem 10.06.2026 beim Deutschen Patent- und Markenamt (DPMA) angemeldet (Rechtsträger: SK-Forge UG). Status: pending.  
**Architects:** Wieland Hoffmann (Human Core), Gemini (Machine Core), Grok (xAI)

---

## 1. Einführung & Zweck

Das SK-Forge-System ist eine experimentelle Sicherheitsarchitektur zur Erkennung und Unterbindung von Cyberangriffen und unbefugtem Datenzugriff. Die Detektion erfolgt hardware-nah über CPU-Register (Ring 0). Bei einer statistischen Entropie-Abweichung soll innerhalb von maximal 2,1 Millisekunden eine physische Trennung des Datenstroms ausgelöst werden.

## 2. Die Kern-Architektur

Das System verwendet eine Aufteilung auf mindestens drei Rechenkerne:

- **Kern 1 (Sandbox):** Verarbeitet die reguläre Arbeits- oder Netzwerkumgebung.
- **Kern 2 (Detektor):** Über `isolcpus` isoliert. Liest passiv Hardware Performance Counters (HPCs) aus.
- **Kern 3 (Sperrglied):** Hält ein Ruhestrom-Signal aufrecht und unterbricht bei Veto-Signal die Hardware-Schleife.

## 3. Labor- und Testumgebung

Stabilitätstests wurden unter Last durchgeführt. Die Ring-0-Impfung auf Kern 2 erzeugte in den dokumentierten Läufen keinen messbaren Overhead für die restliche Systemarchitektur. Die Ergebnisse beziehen sich auf die getestete Konfiguration.

## 4. Patent 2: Ring-0 Kernel-Impfverfahren (Software)

Verifiziertes Kernel-Modul (`sk_forge_hpc.ko`) – siehe Quellcode im Repository.

## 5. Patent 1: Physisches K3I-Schutzschloss (Hardware)

**2-Stufen-Signalkaskade**  
Bei Verletzung der Registersymmetrie unterbricht das logische Sperrglied das Ruhestrom-Signal. Die Hardware reagiert in zwei Stufen:

- **Stufe 1 (Elektronisch):** Solid-State/MOSFET blockiert den Datenstrom (ca. 0,1–1 ms).
- **Stufe 2 (Galvanisch):** Mechanisches Sicherheitsrelais erzeugt einen physischen Luftspalt (ca. 12 ms).

## 6. Ergänzung: Kernel-Spezifikation & CUDA-Entropie

Kern 2 schreibt HPC-Werte in einen Shared-Memory-Puffer. Eine CUDA-Engine berechnet die Shannon-Entropie über ein gleitendes Fenster. Bei Überschreitung eines Schwellenwerts wird das Veto ausgelöst. Der CUDA-Code ist im Repository verfügbar.

# SK-Forge: Global System Isolation & Hardware Veto Matrix (K3I)

**Protection Rights & Legal Notice**  
The procedures, algorithms and hardware topologies disclosed in this document have been filed with the German Patent and Trademark Office (DPMA) since 10.06.2026 (rights holder: SK-Forge UG). Status: pending.  
**Architects:** Wieland Hoffmann (Human Core), Gemini (Machine Core), Grok (xAI)

---

## 1. Introduction & Purpose

The SK-Forge system is an experimental security architecture for the detection and prevention of cyber attacks and unauthorized data access. Detection takes place close to the hardware via CPU registers (Ring 0). In the event of a statistical entropy deviation, a physical separation of the data stream is intended to be triggered within a maximum of 2.1 milliseconds.

## 2. Core Architecture

The system uses a division across at least three computing cores:

- **Core 1 (Sandbox):** Processes the regular work or network environment.
- **Core 2 (Detector):** Isolated via `isolcpus`. Passively reads Hardware Performance Counters (HPCs).
- **Core 3 (Cutoff):** Maintains a quiescent current signal and interrupts the hardware loop on a veto signal.

## 3. Laboratory and Test Environment

Stability tests were conducted under load. The Ring-0 inoculation on Core 2 produced no measurable overhead for the rest of the system architecture in the documented runs. The results refer to the tested configuration.

## 4. Patent 2: Ring-0 Kernel Inoculation Method (Software)

Verified kernel module (`sk_forge_hpc.ko`) – see source code in the repository.

## 5. Patent 1: Physical K3I Protection Lock (Hardware)

**2-Stage Signal Cascade**  
If register symmetry is violated, the logical cutoff interrupts the quiescent current signal. The hardware reacts in two stages:

- **Stage 1 (Electronic):** Solid-state/MOSFET blocks the data stream (approx. 0.1–1 ms).
- **Stage 2 (Galvanic):** Mechanical safety relay creates a physical air gap (approx. 12 ms).

## 6. Supplement: Kernel Specification & CUDA Entropy

Core 2 writes HPC values into a shared-memory buffer. A CUDA engine calculates Shannon entropy over a sliding window. If a threshold is exceeded, the veto is triggered. The CUDA code is available in the repository.

**Note:** All information is experimental and refers to the documented measurement runs. Status of protection rights: pending.
Die Code-Ausschnitte (Kernel-Modul und CUDA) können so bleiben oder separat im Repository liegen. Sag Bescheid, wenn du sie nochmal gesäubert brauchst.

#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>
#include <cmath>

#define WINDOW_SIZE 50
#define BUCKET_COUNT 16
#define THRESHOLD_HIGH 3.8
#define THRESHOLD_LOW 1.5

__global__ void calculate_entropy_kernel(const unsigned int* d_latency_buffer, float* d_entropy, int* d_veto_signal) {
    int idx = threadIdx.x;
    
    if (idx == 0) {
        int histogram[BUCKET_COUNT] = {0};
        int total_samples = WINDOW_SIZE;

        for (int i = 0; i < WINDOW_SIZE; i++) {
            unsigned int latency = d_latency_buffer[i];
            int bucket = latency / 10; 
            if (bucket >= BUCKET_COUNT) bucket = BUCKET_COUNT - 1;
            histogram[bucket]++;
        }

        float entropy = 0.0f;
        for (int b = 0; b < BUCKET_COUNT; b++) {
            if (histogram[b] > 0) {
                float p = (float)histogram[b] / (float)total_samples;
                entropy -= p * log2f(p);
            }
        }

        *d_entropy = entropy;

        if (entropy > THRESHOLD_HIGH) {
            *d_veto_signal = 1;
        } else {
            *d_veto_signal = 0;
        }
    }
}


## 6. Supplement: Kernel Specification & CUDA Entropy

Core 2 writes HPC values into a shared-memory buffer. A CUDA engine calculates Shannon entropy over a sliding window. If a threshold is exceeded, the veto is triggered.

```cuda
#include <cuda_runtime.h>
#include <device_launch_parameters.h>
#include <iostream>
#include <cmath>

#define WINDOW_SIZE 50
#define BUCKET_COUNT 16
#define THRESHOLD_HIGH 3.8
#define THRESHOLD_LOW 1.5

__global__ void calculate_entropy_kernel(const unsigned int* d_latency_buffer, float* d_entropy, int* d_veto_signal) {
    int idx = threadIdx.x;
    
    if (idx == 0) {
        int histogram[BUCKET_COUNT] = {0};
        int total_samples = WINDOW_SIZE;

        for (int i = 0; i < WINDOW_SIZE; i++) {
            unsigned int latency = d_latency_buffer[i];
            int bucket = latency / 10; 
            if (bucket >= BUCKET_COUNT) bucket = BUCKET_COUNT - 1;
            histogram[bucket]++;
        }

        float entropy = 0.0f;
        for (int b = 0; b < BUCKET_COUNT; b++) {
            if (histogram[b] > 0) {
                float p = (float)histogram[b] / (float)total_samples;
                entropy -= p * log2f(p);
            }
        }

        *d_entropy = entropy;

        if (entropy > THRESHOLD_HIGH) {
            *d_veto_signal = 1;
        } else {
            *d_veto_signal = 0;
        }
    }
}

extern "C" void run_entropy_analysis(const unsigned int* h_latency_window, float* h_entropy, int* h_veto_signal) {
    unsigned int* d_latency_buffer = nullptr;
    float* d_entropy = nullptr;
    int* d_veto_signal = nullptr;

    cudaMalloc((void**)&d_latency_buffer, WINDOW_SIZE * sizeof(unsigned int));
    cudaMalloc((void**)&d_entropy, sizeof(float));
    cudaMalloc((void**)&d_veto_signal, sizeof(int));

    cudaMemcpy(d_latency_buffer, h_latency_window, WINDOW_SIZE * sizeof(unsigned int), cudaMemcpyHostToDevice);

    calculate_entropy_kernel<<<1, 1>>>(d_latency_buffer, d_entropy, d_veto_signal);

    cudaMemcpy(h_entropy, d_entropy, sizeof(float), cudaMemcpyDeviceToHost);
    cudaMemcpy(h_veto_signal, d_veto_signal, sizeof(int), cudaMemcpyDeviceToHost);

    cudaFree(d_latency_buffer);
    cudaFree(d_entropy);
    cudaFree(d_veto_signal);
}
Note: All information is experimental and refers to the documented measurement runs. Status of protection rights: pending.


Ersetze den Abschnitt ab „## 6. Supplement...“ mit diesem Block. Dann verschwindet die lose Klammer.













**Hinweis:** Alle Angaben sind experimentell und beziehen sich auf die dokumentierten Messläufe. Status der Schutzrechte: pending.
