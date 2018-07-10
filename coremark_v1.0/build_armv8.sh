#!/bin/bash
runarmv864

make PORT_DIR=armv8 PORT_CFLAGS="-O3 -funroll-all-loops --param max-inline-insns-auto=550 -DMULTITHREAD=4 -DUSE_FORK=1" 
