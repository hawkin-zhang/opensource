#!/bin/bash
export ARCH=arm64
export LOADADDR=0x80008000
export CROSS_COMPILE=aarch64-linux-gnu-
export PATH=/opt/hawkin/toolchain/aarch64-linux-gnu/gcc-linaro-6.3.1-2017.05-x86_64_aarch64-linux-gnu/bin/:$PATH

aarch64-linux-gnu-gcc -O3 -funroll-all-loops  -static dhry21a.c dhry21b.c timers.c -o dhrystone
