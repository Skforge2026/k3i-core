#ifndef K3I_SHM_H
#define K3I_SHM_H

#include <stdint.h>

#define K3I_SHM_NAME "/k3i_buffer_shm"
#define K3I_MSG_SIZE 512

/* K3I HARDWARE ISOLATION REGISTER MAP (SCHICHT 4) */
#define K3I_SHUTTER_L_REG  0x4000E000 
#define K3I_SHUTTER_R_REG  0x4000E004 
#define K3I_MUTEX_LOCK     0x4000E008 

#endif /* K3I_SHM_H */
