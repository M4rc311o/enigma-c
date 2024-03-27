#include "../inc/enigma.h"
#include <stdio.h>

Enigma *enigmaInit() {
    Enigma *enigma;
    enigma = (Enigma*)malloc(sizeof(Enigma));
    if(enigma == NULL) return NULL;
    //TODO: Initialize other Enigma structure parts
    return enigma;
}

void enigmaFree(Enigma *enigma) {
    free(enigma);
}
