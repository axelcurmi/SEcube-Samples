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
    uint8_t i;

    l0 = make_unique<L0>();

    memset(dataOut, 0, dataOutSize);
    for (i = 0; i < dataOutSize; i++) {
        printf("%02x ", dataOut[i]);
    }
    printf("\n");

	l0->L0Open();
    l0->L0GenerateRandom(dataOutSize, dataOut, dataOutSize);
	l0->L0Close();

    for (i = 0; i < dataOutSize; i++) {
        printf("%02x ", dataOut[i]);
    }
    printf("\n");

    return 0;
}
