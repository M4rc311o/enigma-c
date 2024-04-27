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
ENIGMA_ERROR rotorInit(Rotor *rotor, const char *rotorWiringConnections, int rotorP, int ringP, int notchP);
ENIGMA_ERROR reflectorInit(Reflector *reflector, const char *reflectorConnections);
ENIGMA_ERROR rotorsRotate(Rotor rotors[]);
int plugboardEncChar(Plugboard plugboard, int pos);
int rotorsEncCharRightToLeft(Rotor rotors[], int pos);
int rotorsEncCharLeftToRight(Rotor rotors[], int pos);
int reflectorEncChar(Reflector reflector, int pos);

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

const char delim[] = " ";

const char *const errorStrings[] =
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
    "ENIGMA_ROTORS_TOO_FEW_RING_POSITIONS",
    "ENIGMA_ENC_CHAR_NOT_ALPHA",
    "ENIGMA_UNSUCCESSFUL_MEM_ALLOCATION",
    "ENIGMA_INIT_NULL_INPUT"
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
    char *saveConnection = NULL;
    if(plugboardConnections == NULL) {
        lastEnigmaError = ENIGMA_INIT_NULL_INPUT;
        return lastEnigmaError;
    }

    bool used[ALPHABET_SIZE] = {false};
    size_t plugBlength = strlen(plugboardConnections) + 1;
    char *tmpStr = (char *)malloc(plugBlength * sizeof(char));

    if(tmpStr == NULL) {
        lastEnigmaError = ENIGMA_UNSUCCESSFUL_MEM_ALLOCATION;
        return lastEnigmaError;

    }

    strcpy(tmpStr, plugboardConnections);
    for(int i = 0; i < ALPHABET_SIZE; i++) plugboard->plugboardSubstitute[i] = i;

    connection = strtok_r(tmpStr, delim, &saveConnection);
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

        connection = strtok_r(NULL, delim, &saveConnection);
    }

    if(connectionCount != CONNECTIONS_COUNT) {
        lastEnigmaError = ENIGMA_PLUGBOARD_TOO_FEW_CONNECTIONS;
        return lastEnigmaError;
    }

    lastEnigmaError = ENIGMA_SUCCESS;
    free(tmpStr);
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

    if(rotorsNames == NULL || rotorsPositons == NULL || ringPositions == NULL) {
        lastEnigmaError = ENIGMA_INIT_NULL_INPUT;
        return lastEnigmaError;
    }

    size_t rotorNlength = strlen(rotorsNames) + 1;
    size_t rotorPlength = strlen(rotorsPositons) + 1;
    size_t ringPlength = strlen(ringPositions) + 1;

    char *tmpRotor = (char *)malloc(rotorNlength * sizeof(char));
    char *tmpRotorP = (char *)malloc(rotorPlength * sizeof(char));
    char *tmpRingP = (char *)malloc(ringPlength * sizeof(char));

    if(tmpRotor == NULL || tmpRotorP == NULL || tmpRingP == NULL) {
        free(tmpRotor);
        free(tmpRotorP);
        free(tmpRingP);
        lastEnigmaError = ENIGMA_UNSUCCESSFUL_MEM_ALLOCATION;
        return lastEnigmaError;
    }

    strcpy(tmpRotor, rotorsNames);
    strcpy(tmpRotorP, rotorsPositons);
    strcpy(tmpRingP, ringPositions);

    rotor = strtok_r(tmpRotor, delim, &saveRotor);
    rotorP = strtok_r(tmpRotorP, delim, &saveRotorP);
    ringP = strtok_r(tmpRingP, delim, &saveRingP);

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

        rotor = strtok_r(NULL, delim, &saveRotor);
        rotorP = strtok_r(NULL, delim, &saveRotorP);
        ringP = strtok_r(NULL, delim, &saveRingP);
    }

    if(numOfRotor != ROTOR_COUNT) {
        lastEnigmaError = ENIGMA_ROTORS_TOO_FEW_ROTORS;
        return lastEnigmaError;
    }

    lastEnigmaError = ENIGMA_SUCCESS;

    free(tmpRotor);
    free(tmpRotorP);
    free(tmpRingP);
    return lastEnigmaError;
}

ENIGMA_ERROR rotorInit(Rotor *rotor, const char *rotorWiringConnections, int rotorP, int ringP, int notchP) {
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

ENIGMA_ERROR rotorsRotate(Rotor rotors[]) {
    for(int r = 0; r < ROTOR_COUNT; r++) {
        // Left most rotor
        if(r == 0) {
            if(rotors[r + 1].rotorPosition == rotors[r + 1].notchPosition) {
                rotors[r].rotorPosition = (rotors[r].rotorPosition + 1) % ALPHABET_SIZE;
            }
        }
        // Right most rotor
        else if(r == ROTOR_COUNT - 1) {
            rotors[r].rotorPosition = (rotors[r].rotorPosition + 1) % ALPHABET_SIZE;
        }
        // Middle rotors
        else {
            if(rotors[r].rotorPosition == rotors[r].notchPosition || rotors[r + 1].rotorPosition == rotors[r + 1].notchPosition) {
                rotors[r].rotorPosition = (rotors[r].rotorPosition + 1) % ALPHABET_SIZE;
            }
        }
    }

    lastEnigmaError = ENIGMA_SUCCESS;
    return lastEnigmaError;
}

int plugboardEncChar(Plugboard plugboard, int pos) {
    return plugboard.plugboardSubstitute[pos];
}

int rotorsEncCharRightToLeft(Rotor rotors[], int pos) {
    for(int r = ROTOR_COUNT - 1; r >= 0; r--) {
        Rotor rotor = rotors[r];
        int rPos = rotor.rotorPosition - rotor.ringPosition;
        if(rPos < 0) rPos += ALPHABET_SIZE;

        pos = rotor.rotorSubstitute[(pos + rPos) % ALPHABET_SIZE] - rPos;
        if(pos < 0) pos += ALPHABET_SIZE;
    }

    return pos;
}

int rotorsEncCharLeftToRight(Rotor rotors[], int pos) {
    for(int r = 0; r < ROTOR_COUNT; r++) {
        Rotor rotor = rotors[r];
        int rPos = rotor.rotorPosition - rotor.ringPosition;
        if(rPos < 0) rPos += ALPHABET_SIZE;

        pos = rotor.rotorInverseSubstitute[(pos + rPos) % ALPHABET_SIZE] - rPos;
        if(pos < 0) pos += ALPHABET_SIZE;
    }

    return pos;
}

int reflectorEncChar(Reflector reflector, int pos) {
    return reflector.reflectorSubstitute[pos];
}

char enigmaEncChar(Enigma *enigma, char ch) {
    // Add convertign char to upper case and check if it is valid character (A-Z)
    if(isalpha(ch) == 0) {
        lastEnigmaError = ENIGMA_ENC_CHAR_NOT_ALPHA;
        return 0;
    }
    ch = toupper(ch);
    int encCh = ch - 'A';

    rotorsRotate(enigma->rotors);

    encCh = plugboardEncChar(enigma->plugboard, encCh);
    encCh = rotorsEncCharRightToLeft(enigma->rotors, encCh);
    encCh = reflectorEncChar(enigma->reflector, encCh);
    encCh = rotorsEncCharLeftToRight(enigma->rotors, encCh);
    encCh = plugboardEncChar(enigma->plugboard, encCh);

    return encCh + 'A';
}

ENIGMA_ERROR enigmaEncStr(Enigma *enigma, char input[], char encrypted[]) {
    int j = 0;

    for(int i = 0; i <= strlen(input); i++) {
        if(input[i] != ' ') {
            encrypted[j] = enigmaEncChar(enigma, input[i]);
            if(encrypted[j] == 0) {
                return lastEnigmaError;
            }
            j++;
        }
    }
    encrypted[j] = '\0';
    lastEnigmaError = ENIGMA_SUCCESS;
    return ENIGMA_SUCCESS;
}