#include "../inc/enigma.h"
#include "../inc/enigmaErrors.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int plugboardInit(Enigma *enigma, char *plugboardConnections);
int rotorInit(Enigma *enigma, int rotorIndex, char *rotorWiringConnections, char *rotorP, char *ringP);
int reflectorInit(Enigma *enigma, char *reflectorConnections);
char correctInput(char *input);

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

Enigma *enigmaInit(char *plugboardConnections, char *firstRotor, char *rotorP1, char *ringP1, char *secondRotor, char *rotorP2, char *ringP2, char *thirdRotor, char *rotorP3, char *ringP3, char *reflector) {
    Enigma *enigma;
    enigma = (Enigma *)malloc(sizeof(Enigma));
    if (enigma == NULL)
        return NULL;
    if (plugboardInit(enigma, plugboardConnections))
        return NULL;

    if (rotorInit(enigma, 0, firstRotor, rotorP1, ringP1))
        return NULL;
    if (rotorInit(enigma, 1, secondRotor, rotorP2, ringP2))
        return NULL;
    if (rotorInit(enigma, 2, thirdRotor, rotorP3, ringP3))
        return NULL;
    if (reflectorInit(enigma, reflector))
        return NULL;
    // TODO: Initilize rest of the Enigma structure
    return enigma;
}

void enigmaFree(Enigma *enigma)
{
    free(enigma);
}

ENIGMA_ERROR plugboardInit(Enigma *enigma, char *plugboardConnections) {
    char *connection;
    int connectionCount = 0;
    const char delim[] = " ";
    bool used[ALPHABET_SIZE] = {false};
    char tmpStr[strlen(plugboardConnections) + 1];
    strcpy(tmpStr, plugboardConnections);
    for(int i = 0; i < ALPHABET_SIZE; i++) enigma->plugboardSubstitute[i] = i;

    connection = strtok(tmpStr, delim);
    while (connection)
    {
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

int rotorInit(Enigma *enigma, int rotorIndex, char *rotorWiringConnections, char *rotorP, char *ringP)

{
    if (rotorIndex < 0 || rotorIndex >= ROTOR_COUNT)
    {
        fputs("Error: Invalid rotor number", stderr);
        return 1;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (rotorWiringConnections[i] < 'A' || rotorWiringConnections[i] > 'Z')
        {
            fputs("Error: Invalid rotor wiring connections", stderr);
            return 1;
        }
    }

    // mapping characters (way right->left)
    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        enigma->rotorSubstitute[rotorIndex][i] = rotorWiringConnections[i] - 'A';
    }

    // mapping characters (way left->right), if rotor has letter i in position j then the inverse rotor will have letter j in position i

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        enigma->rotorInverseSubstitute[rotorIndex][rotorWiringConnections[i] - 'A'] = i;
    }

    // ring and rotor position
    char cRotorP = correctInput(rotorP);
    char cRingP = correctInput(ringP);
    if(cRotorP !=1 && cRingP != 1){
    enigma->rotorPosition[rotorIndex] = cRotorP - 'A';
    enigma->ringPosition[rotorIndex] = cRingP - 'A';
    }else{
        fputs("Error: Invalid starting position", stderr);
            return 1;
    }


    
    return 0;
}

int reflectorInit(Enigma *enigma, char *reflectorConnections ){

 for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (reflectorConnections[i] < 'A' || reflectorConnections[i] > 'Z')
        {
            fputs("Error: Invalid reflector wiring connections", stderr);
            return 1;
        }
    }

    for(int i = 0; i < ALPHABET_SIZE; i++){
        enigma->reflectorSubstitute[i] = i;
    }
    return 0;

}

char correctInput(char *input) {
 int length = strlen(input);
 //capital letter
  if (length != 1 || !isalpha(input[0]) || !isupper(input[0])) {
    return 1;
  }        

  return *input;
}

