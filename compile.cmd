mkdir obj
gcc -c -g -O0 -D_GNU_SOURCE -o obj/aes256.o secube/aes256.c
gcc -c -g -O0 -D_GNU_SOURCE -o obj/crc16.o secube/crc16.c
gcc -c -g -O0 -D_GNU_SOURCE -o obj/L0.o secube/L0.c
gcc -c -g -O0 -D_GNU_SOURCE -o obj/L1.o secube/L1.c
gcc -c -g -O0 -D_GNU_SOURCE -o obj/pbkdf2.o secube/pbkdf2.c
gcc -c -g -O0 -D_GNU_SOURCE -o obj/se3_common.o secube/se3_common.c
gcc -c -g -O0 -D_GNU_SOURCE -o obj/se3comm.o secube/se3comm.c
gcc -c -g -O0 -D_GNU_SOURCE -o obj/sha256.o secube/sha256.c

gcc -c -g -O0 -D_GNU_SOURCE -o obj/echo.o echo.c
gcc -c -g -O0 -D_GNU_SOURCE -o obj/random.o random.c


mkdir bin
gcc -o bin/echo.exe obj/echo.o obj/aes256.o obj/crc16.o obj/L0.o obj/L1.o obj/pbkdf2.o obj/se3_common.o obj/se3comm.o obj/sha256.o
gcc -o bin/random.exe obj/random.o obj/aes256.o obj/crc16.o obj/L0.o obj/L1.o obj/pbkdf2.o obj/se3_common.o obj/se3comm.o obj/sha256.o

