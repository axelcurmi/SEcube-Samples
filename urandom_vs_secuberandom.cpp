#include "SEcubeSources/L0/L0.h"

#include <sys/random.h>

#include <memory>
#include <thread>
#include <iostream>
#include <cstring>
#include <chrono>

using namespace std;

const uint16_t max_chunk_size = 5 * 1024;

int compute_secube(uint64_t n)
{
    unique_ptr<L0> l0;
    uint64_t total_generated, total_microseconds = 0;
    uint16_t chunk_size = 0;
    uint8_t buff[max_chunk_size];
    int status;

    printf("[+] Computing SEcube random for %ld\n", n);
	
    l0 = make_unique<L0>();
    l0->L0Open();
    while (total_generated < n) {
        if (n - total_generated < max_chunk_size) {
            chunk_size = n - total_generated;
        } else {
            chunk_size = max_chunk_size;
        }
	
        auto start = std::chrono::high_resolution_clock::now();
        status = l0->L0GenerateRandom(chunk_size, buff, max_chunk_size);
        auto finish = std::chrono::high_resolution_clock::now();

        if (status != 0) {
            fprintf(stderr, "Failed to generate SEcube random\n");
            return 1;
        }

        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
        total_microseconds += microseconds.count();
        total_generated += chunk_size;
    }
    l0->L0Close();

    printf("[+] Finished SEcube random for %ld in %ld microseconds\n", n, total_microseconds);
    return 0;
}

int compute_osrandom(uint64_t n)
{
    uint64_t total_generated, total_microseconds = 0;
    uint16_t chunk_size = 0;
    uint8_t buff[max_chunk_size];
    int fd, status;

    printf("[+] Computing urandom for %ld\n", n);
    fd = open("/dev/urandom", O_RDONLY);
	if(fd < 0) {
        fprintf(stderr, "Failed to open /dev/urandom\n");
        return 1;
    }	

    while (total_generated < n) {
        if (n - total_generated < max_chunk_size) {
            chunk_size = n - total_generated;
        } else {
            chunk_size = max_chunk_size;
        }

        auto start = std::chrono::high_resolution_clock::now();
        status = read(fd, buff, chunk_size);
        auto finish = std::chrono::high_resolution_clock::now();

        if (status < 0 || status == 0) {
            fprintf(stderr, "Failed to generate random from /dev/urandom\n");
            return 1;
        }

        auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(finish-start);
        total_microseconds += microseconds.count();
        total_generated += chunk_size;
    }

    printf("[+] Finished urandom for %ld in %ld microseconds\n", n, total_microseconds);
    return 0;
}

int main() {
    uint64_t N[5] = {
        10000,
        100000,
        1000000,
        10000000,
        100000000
    };
    uint i;

    for (i = 0; i < sizeof(N) / sizeof(uint64_t); i++)
    {
        uint64_t n = N[i];
        

        if (compute_osrandom(n) != 0)
            return 1;
        if (compute_secube(n) != 0)
            return 1;
    }

    return 0;
}
