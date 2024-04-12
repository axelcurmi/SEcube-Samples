#!/bin/bash
rm -rf {obj,bin}
mkdir {obj,bin}

secube_objects="obj/aes256.o obj/crc16.o obj/L0.o obj/L1.o obj/pbkdf2.o obj/se3_common.o obj/se3comm.o obj/sha256.o"

find . -name "*.c" -exec bash -c 'name=$(basename $1); gcc -c -g -O0 -D_GNU_SOURCE -o "obj/${name%.c}.o" $1' - '{}' \;

gcc -o bin/hello $secube_objects obj/hello.o
gcc -o bin/echo $secube_objects obj/echo.o
gcc -o bin/random $secube_objects obj/random.o
gcc -o bin/random2file $secube_objects obj/random2file.o