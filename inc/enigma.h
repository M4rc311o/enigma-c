#ifndef ENIGMA_H_
#define ENIGMA_H_

#define ROTOR_COUNT 3
#define ALPHABET_SIZE 26
#define CONNECTIONS_COUNT 6

typedef struct {
    const char *name;
    char rotorSubstitute[ALPHABET_SIZE];
    char rotorInverseSubstitute[ALPHABET_SIZE];
    char ringPosition;
    char rotorPosition;
    char notchPosition;
} Rotor;

typedef struct {
    const char *name;
    char reflectorSubstitute[ALPHABET_SIZE];
} Reflector;

typedef struct {
    char plugboardSubstitute[ALPHABET_SIZE];
} Plugboard;

typedef struct {
    Rotor rotors[ROTOR_COUNT];
    Reflector reflector;
    Plugboard plugboard;
} Enigma;

typedef int ENIGMA_ERROR;

ENIGMA_ERROR getLastEnigmaError();
const char *getEnigmaErrorStr(ENIGMA_ERROR enigmaError);

char enigmaEncChar(Enigma *enigma, char ch);

Enigma *enigmaInit(char *plugboardConnections, char *rotors, char *rotorsPosition, char *ringsPosition);
ENIGMA_ERROR enigmaFree(Enigma *enigma);

#endif /* ENIGMA_H_ */
