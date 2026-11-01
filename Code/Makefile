# SK-Forge: K3I Global System Isolation Makefile
# Für Linux x86_64

CC = g++
NVCC = nvcc
CFLAGS = -O3 -Wall -std=c++11
NVCCFLAGS = -O3

TARGET = k3i_core
OBJ = main.o k3i_entropy.o

obj-m += sk_forge_hpc.o
KDIR := /lib/modules/$(shell uname -r)/build

all: kernel_module $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ) -lcudart

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

k3i_entropy.o: k3i_entropy.cu
	$(NVCC) $(NVCCFLAGS) -c k3i_entropy.cu

kernel_module:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
	rm -f *.o $(TARGET)
Das Makefile baut das Kernel-Modul und das Hauptprogramm mit CUDA..


