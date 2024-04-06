#include "../inc/enigma.h"
#include "../inc/enigmaErrors.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#define AVAILABLE_ROTORS (sizeof(rotorNames)/sizeof(rotorNames[0]))
#define AVAILABLE_REFLECTORS (sizeof(reflectorNames)/sizeof(reflectorNames[0]))

ENIGMA_ERROR plugboardInit(Plugboard *plugboard, char *plugboardConnections);
ENIGMA_ERROR rotorsInit(Rotor rotors[], char *rotorsNames, char *rotorsPositons, char *ringPositions);
ENIGMA_ERROR rotorInit(Rotor *rotor, const char *rotorWiringConnections, char rotorP, char ringP, char notchP);
ENIGMA_ERROR reflectorInit(Reflector *reflector, const char *reflectorConnections);

const char rotorI[ALPHABET_SIZE] = {'E', 'K', 'M', 'F', 'L', 'G', 'D', 'Q', 'V', 'Z', 'N', 'T', 'O', 'W', 'Y', 'H', 'X', 'U', 'S', 'P', 'A', 'I', 'B', 'R', 'C', 'J'};
const char rotorII[ALPHABET_SIZE] = {'A', 'J', 'D', 'K', 'S', 'I', 'R', 'U', 'X', 'B', 'L', 'H', 'W', 'T', 'M', 'C', 'Q', 'G', 'Z', 'N', 'P', 'Y', 'F', 'V', 'O', 'E'};
const char rotorIII[ALPHABET_SIZE] = {'B', 'D', 'F', 'H', 'J', 'L', 'C', 'P', 'R', 'T', 'X', 'V', 'Z', 'N', 'Y', 'E', 'I', 'W', 'G', 'A', 'K', 'M', 'U', 'S', 'Q', 'O'};
const char rotorIV[ALPHABET_SIZE] = {'E', 'S', 'O', 'V', 'P', 'Z', 'J', 'A', 'Y', 'Q', 'U', 'I', 'R', 'H', 'X', 'L', 'N', 'F', 'T', 'G', 'K', 'D', 'C', 'M', 'W', 'B'};
const char rotorV[ALPHABET_SIZE] = {'V', 'Z', 'B', 'R', 'G', 'I', 'T', 'Y', 'U', 'P', 'S', 'D', 'N', 'H', 'L', 'X', 'A', 'W', 'M', 'J', 'Q', 'O', 'F', 'E', 'C', 'K'};
const char *const rotorAlphabets[] = {rotorI, rotorII, rotorIII, rotorIV, rotorV};
const char *const rotorNames[] = {"I", "II", "III", "IV", "V"};
const char rotorNotches[] = {'Q', 'E', 'V', 'J', 'Z'};

const char reflectorA[ALPHABET_SIZE] = {'E', 'J', 'M', 'Z', 'A', 'L', 'Y', 'X', 'V', 'B', 'W', 'F', 'C', 'R', 'Q', 'U', 'O', 'N', 'T', 'S', 'P', 'I', 'K', 'H', 'G', 'D'};
const char reflectorB[ALPHABET_SIZE] = {'Y', 'R', 'U', 'H', 'Q', 'S', 'L', 'D', 'P', 'X', 'N', 'G', 'O', 'K', 'M', 'I', 'E', 'B', 'F', 'Z', 'C', 'W', 'V', 'J', 'A', 'T'};
const char reflectorC[ALPHABET_SIZE] = {'F', 'V', 'P', 'J', 'I', 'A', 'O', 'Y', 'E', 'D', 'R', 'Z', 'X', 'W', 'G', 'C', 'T', 'K', 'U', 'Q', 'S', 'B', 'N', 'M', 'H', 'L'};
const char *const reflectorAlphabets[] = {reflectorA, reflectorB, reflectorC};
const char *const reflectorNames[] = {"Reflector A", "Reflector B", "Reflector C"};

static ENIGMA_ERROR lastEnigmaError = ENIGMA_SUCCESS;

const char *errorStrings[] =
{
    "ENIGMA_SUCCESS",
    "ENIGMA_PLUGBOARD_TOO_MANY_CONNECTIONS",
    "ENIGMA_PLUGBOARD_WRONG_CONNECTION_LENGTH",
    "ENIGMA_PLUGBOARD_CONNECTION_NOT_ALPHA",
    "ENIGMA_PLUGBOARD_CONNECTION_ALREADY_USED",
    "ENIGMA_PLUGBOARD_TOO_FEW_CONNECTIONS",
    "ENIGMA_ERROR_OUT_OF_RANGE",
    "ENIGMA_ROTORS_TOO_MANY_ROTORS",
    "ENIGMA_ROTORS_ROTOR_NAME_NOT_FOUND",
    "ENIGMA_ROTORS_ROTOR_ALREADY_USED",
    "ENIGMA_ROTORS_INVALID_ROTOR_POSITION",
    "ENIGMA_ROTORS_INVALID_RING_POSITION",
    "ENIGMA_ROTORS_TOO_FEW_ROTORS",
    "ENIGMA_ROTORS_TOO_FEW_ROTOR_POSITIONS",
    "ENIGMA_ROTORS_TOO_FEW_RING_POSITIONS"
};

ENIGMA_ERROR getLastEnigmaError() {
    ENIGMA_ERROR err = lastEnigmaError;
    lastEnigmaError = ENIGMA_SUCCESS;
    return err;
}

const char *getEnigmaErrorStr(ENIGMA_ERROR enigmaError) {
    if(enigmaError < 0 || enigmaError >= sizeof(errorStrings) / sizeof(errorStrings[0])) {
        lastEnigmaError = ENIGMA_ERROR_OUT_OF_RANGE;
        return NULL;
    }
    return errorStrings[enigmaError];
}

Enigma *enigmaInit(char *plugboardConnections, char *rotorsNames, char *rotorsPosition, char *ringsPosition) {
    Enigma *enigma;
    enigma = (Enigma *)malloc(sizeof(Enigma));
    if(enigma == NULL)
        return NULL;
    if(plugboardInit(&enigma->plugboard, plugboardConnections)) {
        enigmaFree(enigma);
        return NULL;
    }
    if(rotorsInit(enigma->rotors, rotorsNames, rotorsPosition, ringsPosition)) {
        enigmaFree(enigma);
        return NULL;
    }

    // Reflector B
    enigma->reflector.name = reflectorNames[1];
    if(reflectorInit(&enigma->reflector, reflectorAlphabets[1])) {
        enigmaFree(enigma);
        return NULL;
    }

    return enigma;
}

void enigmaFree(Enigma *enigma) {
    free(enigma);
}

ENIGMA_ERROR plugboardInit(Plugboard *plugboard, char *plugboardConnections) {
    char *connection;
    int connectionCount = 0;
    bool used[ALPHABET_SIZE] = {false};
    char tmpStr[strlen(plugboardConnections) + 1];
    strcpy(tmpStr, plugboardConnections);
    for(int i = 0; i < ALPHABET_SIZE; i++) plugboard->plugboardSubstitute[i] = i;

    connection = strtok(tmpStr, " ");
    while(connection) {
        connectionCount++;
        strupr(connection);
        if(connectionCount > CONNECTIONS_COUNT) {
            lastEnigmaError = ENIGMA_PLUGBOARD_TOO_MANY_CONNECTIONS;
            return lastEnigmaError;
        }
        if(strlen(connection) != 2) {
            lastEnigmaError = ENIGMA_PLUGBOARD_WRONG_CONNECTION_LENGTH;
            return lastEnigmaError;
        }
        if(!isalpha(connection[0]) || !isalpha(connection[1])) {
            lastEnigmaError = ENIGMA_PLUGBOARD_CONNECTION_NOT_ALPHA;
            return lastEnigmaError;
        }
        if(used[connection[0] - 'A'] || used[connection[1] - 'A'] || (connection[0] - 'A') == (connection[1] - 'A')) {
            lastEnigmaError = ENIGMA_PLUGBOARD_CONNECTION_ALREADY_USED;
            return lastEnigmaError;
        }

        int plug1 = connection[0] - 'A';
        int plug2 = connection[1] - 'A';
        plugboard->plugboardSubstitute[plug1] = plug2;
        plugboard->plugboardSubstitute[plug2] = plug1;
        used[plug1] = true;
        used[plug2] = true;

        connection = strtok(NULL, " ");
    }

    if(connectionCount != CONNECTIONS_COUNT) {
        lastEnigmaError = ENIGMA_PLUGBOARD_TOO_FEW_CONNECTIONS;
        return lastEnigmaError;
    }

    lastEnigmaError = ENIGMA_SUCCESS;
    return lastEnigmaError;
}

ENIGMA_ERROR rotorsInit(Rotor rotors[], char *rotorsNames, char *rotorsPositons, char *ringPositions) {
    const char *selectedAlphabet;

    char *rotor;
    char *rotorP;
    char *ringP;

    char *saveRotor = NULL;
    char *saveRotorP = NULL;
    char *saveRingP = NULL;

    char tmpRotor[strlen(rotorsNames) + 1];
    char tmpRotorP[strlen(rotorsPositons) + 1];
    char tmpRingP[strlen(ringPositions) + 1];

    strcpy(tmpRotor, rotorsNames);
    strcpy(tmpRotorP, rotorsPositons);
    strcpy(tmpRingP, ringPositions);

    rotor = strtok_r(tmpRotor, " ", &saveRotor);
    rotorP = strtok_r(tmpRotorP, " ", &saveRotorP);
    ringP = strtok_r(tmpRingP, " ", &saveRingP);

    bool usedRotor[AVAILABLE_ROTORS] = {false};
    char notchP;
    int numOfRotor = 0;

    while(rotor) {
        numOfRotor++;
        selectedAlphabet = NULL;

        if(numOfRotor > ROTOR_COUNT) {
            lastEnigmaError = ENIGMA_ROTORS_TOO_MANY_ROTORS;
            return lastEnigmaError;
        }

        if(rotorP == NULL) {
            lastEnigmaError = ENIGMA_ROTORS_TOO_FEW_ROTOR_POSITIONS;
            return lastEnigmaError;
        }

        if(ringP == NULL) {
            lastEnigmaError = ENIGMA_ROTORS_TOO_FEW_RING_POSITIONS;
            return lastEnigmaError;
        }

        strupr(rotor);
        strupr(rotorP);
        strupr(ringP);

        for(int i = 0; i < AVAILABLE_ROTORS; i++) {
            if(strcmp(rotor, rotorNames[i]) == 0) {
                if(usedRotor[i]) {
                    lastEnigmaError = ENIGMA_ROTORS_ROTOR_ALREADY_USED;
                    return lastEnigmaError;
                }
                rotors[numOfRotor - 1].name = rotorNames[i];
                selectedAlphabet = rotorAlphabets[i];
                notchP = rotorNotches[i];
                usedRotor[i] = true;
                break;
            }
        }

        if(selectedAlphabet == NULL) {
            lastEnigmaError = ENIGMA_ROTORS_ROTOR_NAME_NOT_FOUND;
            return lastEnigmaError;
        }

        if(!isalpha(rotorP[0]) || strlen(rotorP) != 1) {
            lastEnigmaError = ENIGMA_ROTORS_INVALID_ROTOR_POSITION;
            return lastEnigmaError;
        }

        if(!isalpha(ringP[0]) || strlen(ringP) != 1) {
            lastEnigmaError = ENIGMA_ROTORS_INVALID_RING_POSITION;
            return lastEnigmaError;
        }

        if(rotorInit(&rotors[numOfRotor - 1], selectedAlphabet, rotorP[0], ringP[0], notchP)) return lastEnigmaError;

        rotor = strtok_r(NULL, " ", &saveRotor);
        rotorP = strtok_r(NULL, " ", &saveRotorP);
        ringP = strtok_r(NULL, " ", &saveRingP);
    }

    if(numOfRotor != ROTOR_COUNT) {
        lastEnigmaError = ENIGMA_ROTORS_TOO_FEW_ROTORS;
        return lastEnigmaError;
    }

    lastEnigmaError = ENIGMA_SUCCESS;
    return lastEnigmaError;
}

ENIGMA_ERROR rotorInit(Rotor *rotor, const char *rotorWiringConnections, char rotorP, char ringP, char notchP) {
    for(int i = 0; i < ALPHABET_SIZE; i++) {
        // mapping characters (way right->left)
        rotor->rotorSubstitute[i] = rotorWiringConnections[i] - 'A';
        // mapping characters (way left->right), if rotor has letter i in position j then the inverse rotor will have letter j in position i
        rotor->rotorInverseSubstitute[rotorWiringConnections[i] - 'A'] = i;
    }

    rotor->rotorPosition = rotorP - 'A';
    rotor->ringPosition = ringP - 'A';

    rotor->notchPosition = notchP - 'A';

    lastEnigmaError = ENIGMA_SUCCESS;
    return lastEnigmaError;
}

ENIGMA_ERROR reflectorInit(Reflector *reflector, const char *reflectorConnections) {
    for(int i = 0; i < ALPHABET_SIZE; i++) {
        reflector->reflectorSubstitute[i] = reflectorConnections[i] - 'A';
    }

    lastEnigmaError = ENIGMA_SUCCESS;
    return lastEnigmaError;
}