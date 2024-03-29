#include "../inc/enigma.h"
#include <stdlib.h>

int main() {
    Enigma *enigma;
    enigma = enigmaInit("ab EC Dg OP QW NM");
    if(enigma != NULL) enigmaFree(enigma);

    return 0;    
}
