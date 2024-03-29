#include "../inc/enigma.h"
#include <stdlib.h>

int main() {
    Enigma *enigma;
    enigma = enigmaInit("AB CD EF GH IJ KL");
    if(enigma != NULL) enigmaFree(enigma);

    return 0;    
}
