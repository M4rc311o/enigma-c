#ifndef ENIGMA_H_
#define ENIGMA_H_

#define ROTOR_COUNT 3
#define ALPHABET_SIZE 26
#define MAX_PLUGS 6

typedef struct {
    char rotorSubstitute[ROTOR_COUNT][ALPHABET_SIZE];
    char rotorInverseSubstitute[ROTOR_COUNT][ALPHABET_SIZE];
    char reflectorSubstitute[ALPHABET_SIZE];
    char plugboardSubstitute[ALPHABET_SIZE];
    char ringPosition[ROTOR_COUNT];
    char rotorPosition[ROTOR_COUNT];
} Enigma;

Enigma *enigmaInit(char *plugboardConnections);
void enigmaFree(Enigma *enigma);

#endif /* ENIGMA_H_ */
