#include "../inc/enigma.h"
#include <stdlib.h>
#include <stdio.h>


int main() {
    Enigma *enigma;
    enigma = enigmaInit("AB CD EF GH IJ KL", "I II III", "J V Z", "L X O");

    if(enigma == NULL) {
        printf(getEnigmaErrorStr(getLastEnigmaError()));
        return 1;
    }

    enigmaFree(enigma);
    return 0;
}
