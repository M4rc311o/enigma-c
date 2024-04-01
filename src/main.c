#include "../inc/enigma.h"
#include <stdlib.h>
#include <stdio.h>

int main()
{

    Enigma *enigma;
    //probably better to add rotorPosition and ringPosition separately
    enigma = enigmaInit("AB CD EF GH IJ KL","D","A");
    if (enigma != NULL)
        enigmaFree(enigma);
    return 0;
}
