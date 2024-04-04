#include "../inc/enigma.h"
#include "../inc/enigmaErrors.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int plugboardInit(Enigma *enigma, char *plugboardConnections);
int rotorInit(Enigma *enigma, int rotorIndex, char *rotorWiringConnections, char rotorP, char ringP);
int rotorsInit(Enigma *init, char *rotors, char *rotorsPositons, char *ringPositions);
int reflectorInit(Enigma *enigma, char *reflectorConnections);


char rotorI[ALPHABET_SIZE] = {'E', 'K', 'M', 'F', 'L', 'G', 'D', 'Q', 'V', 'Z', 'N', 'T', 'O', 'W', 'Y', 'H', 'X', 'U', 'S', 'P', 'A', 'I', 'B', 'R', 'C', 'J'};
char rotorII[ALPHABET_SIZE] = {'A', 'J', 'D', 'K', 'S', 'I', 'R', 'U', 'X', 'B', 'L', 'H', 'W', 'T', 'M', 'C', 'Q', 'G', 'Z', 'N', 'P', 'Y', 'F', 'V', 'O', 'E'};
char rotorIII[ALPHABET_SIZE] = {'B', 'D', 'F', 'H', 'J', 'L', 'C', 'P', 'R', 'T', 'X', 'V', 'Z', 'N', 'Y', 'E', 'I', 'W', 'G', 'A', 'K', 'M', 'U', 'S', 'Q', 'O'};
char rotorIV[ALPHABET_SIZE] = {'E', 'S', 'O', 'V', 'P', 'Z', 'J', 'A', 'Y', 'Q', 'U', 'I', 'R', 'H', 'X', 'L', 'N', 'F', 'T', 'G', 'K', 'D', 'C', 'M', 'W', 'B'};
char rotorV[ALPHABET_SIZE] = {'V', 'Z', 'B', 'R', 'G', 'I', 'T', 'Y', 'U', 'P', 'S', 'D', 'N', 'H', 'L', 'X', 'A', 'W', 'M', 'J', 'Q', 'O', 'F', 'E', 'C', 'K'};
char reflectorA[ALPHABET_SIZE] = {'E', 'J', 'M', 'Z', 'A', 'L', 'Y', 'X', 'V', 'B', 'W', 'F', 'C', 'R', 'Q', 'U', 'O', 'N', 'T', 'S', 'P', 'I', 'K', 'H', 'G', 'D'};
char reflectorB[ALPHABET_SIZE] = {'Y', 'R', 'U', 'H', 'Q', 'S', 'L', 'D', 'P', 'X', 'N', 'G', 'O', 'K', 'M', 'I', 'E', 'B', 'F', 'Z', 'C', 'W', 'V', 'J', 'A', 'T'};
char reflectorC[ALPHABET_SIZE] = {'F', 'V', 'P', 'J', 'I', 'A', 'O', 'Y', 'E', 'D', 'R', 'Z', 'X', 'W', 'G', 'C', 'T', 'K', 'U', 'Q', 'S', 'B', 'N', 'M', 'H', 'L'};
const char *rotorNames[] = {"I", "II", "III", "IV", "V"};
char *rotorAlphabets[] = {rotorI, rotorII, rotorIII, rotorIV, rotorV};

const char delim[] = " ";

static ENIGMA_ERROR lastEnigmaError = ENIGMA_SUCCESS;

const char *errorStrings[] =
{
    "ENIGMA_SUCCESS",
    "ENIGMA_PLUGBOARD_TOO_MANY_CONNECTIONS",
    "ENIGMA_PLUGBOARD_WRONG_CONNECTION_LENGTH",
    "ENIGMA_PLUGBOARD_CONNECTION_NOT_ALPHA",
    "ENIGMA_PLUGBOARD_CONNECTION_ALREADY_USED",
    "ENIGMA_PLUGBOARD_TOO_FEW_CONNECTIONS",
    "ENIGMA_ERROR_OUT_OF_RANGE"
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

Enigma *enigmaInit(char *plugboardConnections, char *rotors, char *rotorsPosition, char *ringsPosition) {
    Enigma *enigma;
    enigma = (Enigma *)malloc(sizeof(Enigma));
    if(enigma == NULL)
        return NULL;
    if(plugboardInit(enigma, plugboardConnections))
        return NULL;
    if(rotorsInit(enigma, rotors, rotorsPosition, ringsPosition))
        return NULL;

    // TODO: Initilize rest of the Enigma structure
    return enigma;
}

void enigmaFree(Enigma *enigma) {
    free(enigma);
}

ENIGMA_ERROR plugboardInit(Enigma *enigma, char *plugboardConnections) {
    char *connection;
    int connectionCount = 0;
    bool used[ALPHABET_SIZE] = {false};
    char tmpStr[strlen(plugboardConnections) + 1];
    strcpy(tmpStr, plugboardConnections);
    for(int i = 0; i < ALPHABET_SIZE; i++) enigma->plugboardSubstitute[i] = i;

    connection = strtok(tmpStr, delim);
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
        enigma->plugboardSubstitute[plug1] = plug2;
        enigma->plugboardSubstitute[plug2] = plug1;
        used[plug1] = true;
        used[plug2] = true;

        connection = strtok(NULL, delim);
    }

    if(connectionCount != CONNECTIONS_COUNT) {
        lastEnigmaError = ENIGMA_PLUGBOARD_TOO_FEW_CONNECTIONS;
        return lastEnigmaError;
    }

    lastEnigmaError = ENIGMA_SUCCESS;
    return lastEnigmaError;
}

ENIGMA_ERROR rotorsInit(Enigma *enigma, char *rotors, char *rotorsPositons, char *ringPositions) {
    char *selectedAlphabet;

    char *rotor;
    char *rotorP;
    char *ringP;

    char *saveRotor = NULL;
    char *saveRotorP = NULL;
    char *saveRingP = NULL;

    char tmpRotor[strlen(rotors) + 1];
    char tmpRotorP[strlen(rotorsPositons) + 1];
    char tmpRingP[strlen(ringPositions) + 1];

    strcpy(tmpRotor, rotors);
    strcpy(tmpRotorP, rotorsPositons);
    strcpy(tmpRingP, ringPositions);

    rotor = strtok_r(tmpRotor, delim, &saveRotor);
    rotorP = strtok_r(tmpRotorP, delim, &saveRotorP);
    ringP = strtok_r(tmpRingP, delim, &saveRingP);

    strupr(rotor);
    strupr(rotorP);
    strupr(ringP);

    bool usedRotor[ROTOR_COUNT] = {false};


    int rotorIndex = 0;
    int round = 0;
    int i = 0;
    while(rotor && rotorP && ringP) {
        round++;

        if(rotorIndex >= ROTOR_COUNT) {
            lastEnigmaError = ENIGMA_ROTORS_TOO_MANY_ROTORS;
            return lastEnigmaError;
        }

      for(int i = 0; i <= ROTOR_COUNT; i++) {
            if(strcmp(rotor, rotorNames[i]) == 0) {
                if(usedRotor[i]) {
                    return ENIGMA_ROTORS_DUPLICATE_ROTOR;
                }
                else {
                    usedRotor[i] = true;
                    break;
                }
            }
        }

        if(!isalpha(rotorP[0]) || !isalpha(ringP[0])) {
            lastEnigmaError = ENIGMA_ROTORS_INPUT_NOT_ALPHA;
            return lastEnigmaError;
        }

        if(strlen(rotorP) != 1 || strlen(ringP) != 1) {
            lastEnigmaError = ENIGMA_ROTORS_WRONG_INPUT_LENGTH;
            return lastEnigmaError;
        }

        //char choiceOfRotor[strlen(rotor) + strlen("rotor") + 1];
        //strcpy(choiceOfRotor, "rotor");
        //strcat(choiceOfRotor, rotor);

        for(i = 0; rotorNames[i] != NULL; i++) {
            if(strcmp(rotor, rotorNames[i]) == 0) {
                selectedAlphabet = rotorAlphabets[i];
                break;
            }
        }

        if(selectedAlphabet == NULL) {
            lastEnigmaError = ENIGMA_ROTORS_INVALID_ROTOR;
            return lastEnigmaError;
        }

        rotorInit(enigma, rotorIndex, selectedAlphabet, rotorP[0], ringP[0]);

        rotor = strtok_r(NULL, delim, &saveRotor);
        rotorP = strtok_r(NULL, delim, &saveRotorP);
        ringP = strtok_r(NULL, delim, &saveRingP);
        rotorIndex++;
    }

    if(round != ROTOR_COUNT) {
        lastEnigmaError = ENIGMA_ROTORS_TOO_FEW_ROTORS;
        return lastEnigmaError;
    }

    lastEnigmaError = ENIGMA_SUCCESS;
    return lastEnigmaError;
}

int rotorInit(Enigma *enigma, int rotorIndex, char *rotorWiringConnections, char rotorP, char ringP)

{
    // mapping characters (way right->left)
    for(int i = 0; i < ALPHABET_SIZE; i++) {
        enigma->rotorSubstitute[rotorIndex][i] = rotorWiringConnections[i] - 'A';
        // mapping characters (way left->right), if rotor has letter i in position j then the inverse rotor will have letter j in position i
        enigma->rotorInverseSubstitute[rotorIndex][rotorWiringConnections[i] - 'A'] = i;
    }

    enigma->rotorPosition[rotorIndex] = rotorP - 'A';
    enigma->ringPosition[rotorIndex] = ringP - 'A';
    return 0;
}

int reflectorInit(Enigma *enigma, char *reflectorConnections) {

    for(int i = 0; i < ALPHABET_SIZE; i++) {
        enigma->reflectorSubstitute[i] = i;
    }
    return 0;

}



