#ifndef ENIGMA_H_
#define ENIGMA_H_

#define ROTOR_COUNT 3
#define ALPHABET_SIZE 26
#define CONNECTIONS_COUNT 6

typedef struct {
    const char *name;
    int rotorSubstitute[ALPHABET_SIZE];
    int rotorInverseSubstitute[ALPHABET_SIZE];
    int ringPosition;
    int rotorPosition;
    int notchPosition;
} Rotor;

typedef struct {
    const char *name;
    int reflectorSubstitute[ALPHABET_SIZE];
} Reflector;

typedef struct {
    int plugboardSubstitute[ALPHABET_SIZE];
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

ENIGMA_ERROR enigmaEncStr(Enigma *enigma, char message[], char encrypted[]);

Enigma *enigmaInit(char *plugboardConnections, char *rotors, char *rotorsPosition, char *ringsPosition);
ENIGMA_ERROR enigmaFree(Enigma *enigma);

#endif /* ENIGMA_H_ */
