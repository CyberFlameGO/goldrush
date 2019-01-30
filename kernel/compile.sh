#!/bin/sh

i686-elf-as boot.s -o boot.o
i686-elf-gcc -I../shared -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -I../shared -c ../shared/string.c -o ../shared/string.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot.o kernel.o  ../shared/string.o -lgcc