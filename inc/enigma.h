#ifndef ENIGMA_H_
#define ENIGMA_H_

#define ROTOR_COUNT 3
#define ALPHABET_SIZE 26

typedef struct {
    unsigned char rotorSubstitute[ROTOR_COUNT][ALPHABET_SIZE];
    unsigned char rotorInverseSubstitute[ROTOR_COUNT][ALPHABET_SIZE];
    unsigned char reflectorSubstitute[ALPHABET_SIZE];
    unsigned char plugboardSubstitute[ALPHABET_SIZE];
    unsigned char ringPosition[ROTOR_COUNT];
    unsigned char rotorPosition[ROTOR_COUNT];
} Enigma;

Enigma *enigmaInit();
void enigmaFree(Enigma *enigma);

#endif /* ENIGMA_H_ */
