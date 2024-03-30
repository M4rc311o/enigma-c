#include "../inc/enigma.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int plugboardInit(Enigma *enigma, char *plugboardConnections);

Enigma *enigmaInit(char *plugboardConnections) {
    Enigma *enigma;
    enigma = (Enigma*)malloc(sizeof(Enigma));
    if(enigma == NULL) return NULL;
    if(plugboardInit(enigma, plugboardConnections)) return NULL;
    //TODO: Initilize rest of the Enigma structure
    return enigma;
}

void enigmaFree(Enigma *enigma) {
    free(enigma);
}

int plugboardInit(Enigma *enigma, char *plugboardConnections) {
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
            fputs("Error: Too many connections", stderr);
            return 1;
        }
        if(strlen(connection) != 2) {
            fputs("Error: Plugborad connection wrong length", stderr);
            return 1;
        }
        if(!isalpha(connection[0]) || !isalpha(connection[1])) {
            fputs("Error: Plugborad connection is not alpha", stderr);
            return 1;
        }
        if(used[connection[0] - 'A'] || used[connection[1] - 'A']) {
            fputs("Error: Connection already used", stderr);
            return 1;
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
        fputs("Error: Too few connections", stderr);
        return 1;
    }

    return 0;
}
