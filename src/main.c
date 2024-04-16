#include "../inc/enigma.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


int main() {
    Enigma *enigma;
    enigma = enigmaInit("AB CD EF GH IJ KL", "I II III", "J V Z", "L X O");

    if(enigma == NULL) {
        printf(getEnigmaErrorStr(getLastEnigmaError()));
        return 1;
    }

    char encrypted[1000]; 
    enigmaEncStr(enigma, "teSt SifRovacEj FunkcIe", encrypted);
    printf("%s",encrypted);    

    enigmaFree(enigma);
    return 0;
}
