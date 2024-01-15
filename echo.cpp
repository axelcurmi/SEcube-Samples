#include "SEcubeSources/L0/L0.h"

#include <memory>
#include <thread>
#include <iostream>
#include <cstring>

using namespace std;

int main() {
    unique_ptr<L0> l0;
    const char *dataIn = "You live in my dream state";
    uint16_t dataInLength = strlen(dataIn);
    uint8_t dataOut[dataInLength];

    memset(dataOut, 0, dataInLength);

    l0 = make_unique<L0>();

	l0->L0Open();
    l0->L0Echo((const uint8_t *)dataIn, dataInLength, dataOut);
	l0->L0Close();

    cout << dataOut << endl;

    return 0;
}
