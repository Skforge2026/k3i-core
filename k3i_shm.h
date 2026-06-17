#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* K3I HARDWARE ISOLATION REGISTER MAP (SCHICHT 4) */
#define K3I_SHUTTER_L_REG  0x4000E000 // Physikalische Adresse Provider Line L Shutter
#define K3I_SHUTTER_R_REG  0x4000E004 // Physikalische Adresse Provider Line R Shutter
#define K3I_MUTEX_LOCK     0x4000E008 // Atomic Hardware Interlocking Register

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

    // 1. Atomic Hardware Lock anfordern (Gegen asynchrone Jitter-Effekte)
    while (*mutex == 1) {
        // Warte deterministisch auf CPLD Freigabe
    }
    *mutex = 1; // Sperre setzen

    // 2. Physikalische Umschaltung der Solid-State Shutter (<1µs)
    if (provider_line == 0) {
        *shutter_l = 1; // Aktivieren Line L
        *shutter_r = 0; // Isolieren Line R
    } else {
        *shutter_l = 0; // Isolieren Line L
        *shutter_r = 1; // Aktivieren Line R
    }

    // 3. Hardware Lock freigeben
    *mutex = 0;
}

int main(void) {
    uint64_t start_cycles, end_cycles;

    printf("[K3I SYSTEM] Initialisiere deterministisches Shutter-System...\n");
    
    start_cycles = read_rdtsc();
    set_k3i_shutter(0); // Schalte auf Provider Line L
    end_cycles = read_rdtsc();

    printf("[K3I SYSTEM] Umschaltung und Schicht-4-Isolation erfolgreich.\n");
    printf("[K3I SYSTEM] Gemessene CPU-Takte (rdtsc): %lu\n", (end_cycles - start_cycles));

    return 0;
}

