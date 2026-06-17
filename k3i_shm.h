#ifndef K3I_SHM_H
#define K3I_SHM_H

#include <stdint.h>

#define K3I_SHM_NAME "/k3i_buffer_shm"
#define K3I_MSG_SIZE 512

/* Shared Memory Struktur fuer den Hardware-Waechter */
typedef struct {
    uint64_t timestamp;        /* Echtzeit-Takt-Anker */
    uint32_t message_id;       /* Bus-ID zur Validierung */
    uint32_t status_flags;     /* System-Zustand (Ring 0/3) */
    char input_data[K3I_MSG_SIZE];   /* Befehl aus dem Forum */
    char output_data[K3I_MSG_SIZE];  /* Antwort der Live-Kerne */
    uint32_t crc32_checksum;   /* Manipulationsschutz-Schild */
} k3i_shm_t;

#endif /* K3I_SHM_H */
