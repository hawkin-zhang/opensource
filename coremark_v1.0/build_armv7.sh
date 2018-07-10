#!/bin/bash
runarmv7hf
make PORT_DIR=armv7 PORT_CFLAGS="-O3 -march=armv7-a -mfloat-abi=hard -mfpu=neon -mtune=cortex-a7 -funroll-all-loops --param max-inline-insns-auto=300 -static -DMULTITHREAD=4 -DUSE_FORK=1"

