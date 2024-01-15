#include "SEcubeSources/L0/L0.h"

#include <memory>
#include <thread>
#include <iostream>
#include <cstring>

using namespace std;

int main() {
    unique_ptr<L0> l0;
    uint16_t dataOutSize = 16;
    uint8_t dataOut[dataOutSize];
    int i, status;

    l0 = make_unique<L0>();

    memset(dataOut, 0, dataOutSize);
    for (i = 0; i < dataOutSize; i++) {
        printf("%02x ", dataOut[i]);
    }
    printf("\n");

	l0->L0Open();
    status = l0->L0GenerateRandom(dataOutSize, dataOut, dataOutSize);
	l0->L0Close();

    for (i = 0; i < dataOutSize; i++) {
        printf("%02x ", dataOut[i]);
    }
    printf("\nStatus: %d\n", status);

    return 0;
}
