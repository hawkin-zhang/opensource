#!/bin/bash
export ARCH=arm
export LOAD_ADDR=0x10008000
export CROSS_COMPILE=arm-linux-gnueabihf-
export PATH=/opt/hawkin/toolchain/arm-linux-guneabihf/gcc-linaro-6.3.1-2017.05-x86_64_arm-linux-gnueabihf/bin/:$PATH

make PORT_DIR=armv7 PORT_CFLAGS="-O3 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mtune=cortex-a7 -funroll-all-loops --param max-inline-insns-auto=300 -static -DMULTITHREAD=4 -DUSE_FORK=1"

