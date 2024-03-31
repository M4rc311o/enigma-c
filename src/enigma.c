#include "../inc/enigma.h"
#include "../inc/enigmaErrors.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int plugboardInit(Enigma *enigma, char *plugboardConnections);

static ENIGMA_ERROR lastEnigmaError = ENIGMA_SUCCESS;

ENIGMA_ERROR getLastEnigmaError(){
    return lastEnigmaError;
}

const char *getEnigmaErrorStr(ENIGMA_ERROR enigmaError) {
    if(enigmaError < 0 || enigmaError >= sizeof(errorStrings)/sizeof(errorStrings[0])) {
        lastEnigmaError = ENIGMA_ERROR_OUT_OF_RANGE;
        return NULL;
    }
    return errorStrings[enigmaError];
}

Enigma *enigmaInit(char *plugboardConnections) {
    Enigma *enigma;
    enigma = (Enigma*)malloc(sizeof(Enigma));
    if(enigma == NULL) return NULL;
    if(plugboardInit(enigma, plugboardConnections) != ENIGMA_SUCCESS) return NULL;
    //TODO: Initilize rest of the Enigma structure
    return enigma;
}

void enigmaFree(Enigma *enigma) {
    free(enigma);
}

ENIGMA_ERROR plugboardInit(Enigma *enigma, char *plugboardConnections) {
    char *connection;
    int connectionCount = 0;
    const char delim[] = " ";
    bool used[ALPHABET_SIZE] = { false };
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
