#ifndef K3I_SHM_H
#define K3I_SHM_H

#include <stdint.h>

#define K3I_SHM_NAME "/k3i_buffer_shm"
#define K3I_MSG_SIZE 512

/* K3I HARDWARE ISOLATION REGISTER MAP (SCHICHT 4) */
#define K3I_SHUTTER_L_REG  0x4000E000 // Provider Line L Isolation
#define K3I_SHUTTER_R_REG  0x4000E004 // Provider Line R Isolation
#define K3I_MUTEX_LOCK     0x4000E008 // Atomic Hardware Interlocking

/* Shared Memory Struktur für den Hardware-Wächter */
typedef struct {
    uint64_t timestamp;     /* Echtzeit-Takt-Anker */
    uint32_t message_id;    /* Bau-ID zur Validierung */
    uint32_t status_flags;  /* System-Zustand (Ring 0/1) */
    char input_data[K3I_MSG_SIZE];  /* Befehl aus dem Forum */
    char output_data[K3I_MSG_SIZE]; /* Antwort der Live-Kerne */
    uint32_t crc32_checksum;/* Manipulationsschutz-Schild */
} k3i_shm_t;

#endif /* K3I_SHM_H */
