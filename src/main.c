#include "../inc/enigma.h"
#include <stdlib.h>
#include <stdio.h>


int main() {
    Enigma *enigma;
    enigma = enigmaInit("AB CD EF GH IJ KL", "I II III", "A A A", "A A A");
    if(enigma == NULL) {
        printf(getEnigmaErrorStr(getLastEnigmaError()));
        return -1;
    }
    
    for(int j = 0; j < ROTOR_COUNT; j++) {
        printf("\nRotor %d (%s) Substitution:\n", j, enigma->rotors[j].name);

        for(int i = 0; i < ALPHABET_SIZE; i++) {
            printf("%c -> %c\n", 'A' + i, 'A' + enigma->rotors[j].rotorSubstitute[i]);
        }

        printf("\nRotor %d (%s) Inverse Substitution:\n", j, enigma->rotors[j].name);
        for(int i = 0; i < ALPHABET_SIZE; i++) {
            printf("%c -> %c\n", 'A' + i, 'A' + enigma->rotors[j].rotorInverseSubstitute[i]);
        }
        printf("\n");
    }

    printf("Reflector (%s) substitution:\n", enigma->reflector.name);
    for(int i = 0; i < ALPHABET_SIZE; i++) {
        printf("%c -> %c\n", 'A' + i, 'A' + enigma->reflector.reflectorSubstitute[i]);
    }

    enigmaFree(enigma);
    return 0;
}
