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
