Makefile

# SK-Forge: K3I Global System Isolation Makefile
# Dreifach geprueft und optimiert fuer Linux x86_64

# Compiler-Definitionen
CC = g++
NVCC = nvcc
CFLAGS = -O3 -Wall -std=c++11
NVCCFLAGS = -O3

# Target-Definitionen
TARGET = k3i_core
OBJ = main.o k3i_entropy.o

# Linux Kernel Modul Konfiguration
obj-m += sk_forge_hpc.o
KDIR := /lib/modules/$(shell uname -r)/build

all: kernel_module $(TARGET)

# 1. Hauptprogramm und CUDA verbinden
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -lcudart

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

k3i_entropy.o: k3i_entropy.cu
	$(NVCC) $(NVCCFLAGS) -c k3i_entropy.cu

# 2. Linux Ring-0 Kernel Modul bauen
kernel_module:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

# Sauberes Aufraeumen der temporaeren Build-Dateien
clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f *.o $(TARGET)
