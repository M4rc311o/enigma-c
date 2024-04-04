#include "../inc/enigma.h"
#include <stdlib.h>
#include <stdio.h>


int main() {
    Enigma *enigma;
    enigma = enigmaInit("AB CD EF GH IJ KL","V I III","A B K", "D S V");
  
        for(int j=0;j<3;j++){
                        printf("\nRotor %d  Substitution:\n", j);

            for (int i = 0; i < ALPHABET_SIZE; i++) {
                printf("%c -> %c\n", 'A' + i, 'A' + enigma->rotorSubstitute[j][i]);
            }

            printf("\nRotor %d Inverse Substitution:\n", j);
            for (int i = 0; i < ALPHABET_SIZE; i++) {
                printf("%c -> %c\n", 'A' + i, 'A' + enigma->rotorInverseSubstitute[j][i]);
            }
            printf("\n");
        }
    if(enigma != NULL)enigmaFree(enigma);
    else printf(getEnigmaErrorStr(getLastEnigmaError()));

    return 0;
}
