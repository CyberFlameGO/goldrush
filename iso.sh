#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/goldrush isodir/boot/goldrush
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "Goldrush" {
	multiboot /boot/goldrush
}
EOF
grub-mkrescue -o myos.iso isodir
