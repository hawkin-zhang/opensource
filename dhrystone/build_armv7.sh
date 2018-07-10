#!/bin/bash
export ARCH=arm
export LOAD_ADDR=0x10008000
export CROSS_COMPILE=arm-linux-gnueabihf-
export PATH=/opt/hawkin/toolchain/arm-linux-guneabihf/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/bin/:$PATH

arm-linux-gnueabihf-gcc -O3 -funroll-all-loops  -static dhry21a.c dhry21b.c timers.c -o dhrystone

