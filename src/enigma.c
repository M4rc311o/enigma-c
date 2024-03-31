#include "../inc/enigma.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int plugboardInit(Enigma *enigma, char *plugboardConnections);
int rotorInit(Enigma *enigma, int rotorIndex, char *rotorWiringConnections, char *rotorP, char *ringP);

char rotorI[ALPHABET_SIZE] = {'E', 'K', 'M', 'F', 'L', 'G', 'D', 'Q', 'V', 'Z', 'N', 'T', 'O', 'W', 'Y', 'H', 'X', 'U', 'S', 'P', 'A', 'I', 'B', 'R', 'C', 'J'};
char rotorII[ALPHABET_SIZE] = {'A', 'J', 'D', 'K', 'S', 'I', 'R', 'U', 'X', 'B', 'L', 'H', 'W', 'T', 'M', 'C', 'Q', 'G', 'Z', 'N', 'P', 'Y', 'F', 'V', 'O', 'E'};
char rotorIII[ALPHABET_SIZE] = {'B', 'D', 'F', 'H', 'J', 'L', 'C', 'P', 'R', 'T', 'X', 'V', 'Z', 'N', 'Y', 'E', 'I', 'W', 'G', 'A', 'K', 'M', 'U', 'S', 'Q', 'O'};
char rotorIV[ALPHABET_SIZE] = {'E', 'S', 'O', 'V', 'P', 'Z', 'J', 'A', 'Y', 'Q', 'U', 'I', 'R', 'H', 'X', 'L', 'N', 'F', 'T', 'G', 'K', 'D', 'C', 'M', 'W', 'B'};
char rotorV[ALPHABET_SIZE] = {'V', 'Z', 'B', 'R', 'G', 'I', 'T', 'Y', 'U', 'P', 'S', 'D', 'N', 'H', 'L', 'X', 'A', 'W', 'M', 'J', 'Q', 'O', 'F', 'E', 'C', 'K'};

Enigma *enigmaInit(char *plugboardConnections)
{
    Enigma *enigma;
    enigma = (Enigma *)malloc(sizeof(Enigma));
    if (enigma == NULL)
        return NULL;
    if (plugboardInit(enigma, plugboardConnections))
        return NULL;

    if (rotorInit(enigma, 1, rotorI, 'D', 'A'))
        return NULL;
    if (rotorInit(enigma, 2, rotorII, 'A', 'A'))
        return NULL;
    if (rotorInit(enigma, 3, rotorIII, 'A', 'A'))
        return NULL;
    // TODO: Initilize rest of the Enigma structure
    return enigma;
}

void enigmaFree(Enigma *enigma)
{
    free(enigma);
}

int plugboardInit(Enigma *enigma, char *plugboardConnections)
{
    char *connection;
    int connectionCount = 0;
    const char delim[] = " ";
    bool used[ALPHABET_SIZE] = {false};
    char tmpStr[strlen(plugboardConnections) + 1];
    strcpy(tmpStr, plugboardConnections);
    for (int i = 0; i < ALPHABET_SIZE; i++)
        enigma->plugboardSubstitute[i] = i;

    connection = strtok(tmpStr, delim);
    while (connection)
    {
        connectionCount++;
        strupr(connection);
        if (connectionCount > CONNECTIONS_COUNT)
        {
            fputs("Error: Too many connections", stderr);
            return 1;
        }
        if (strlen(connection) != 2)
        {
            fputs("Error: Plugborad connection wrong length", stderr);
            return 1;
        }
        if (!isalpha(connection[0]) || !isalpha(connection[1]))
        {
            fputs("Error: Plugborad connection is not alpha", stderr);
            return 1;
        }
        if (used[connection[0] - 'A'] || used[connection[1] - 'A'])
        {
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

    if (connectionCount != CONNECTIONS_COUNT)
    {
        fputs("Error: Too few connections", stderr);
        return 1;
    }

    return 0;
}

int rotorInit(Enigma *enigma, int rotorIndex, char *rotorWiringConnections, char *rotorP, char *ringP)

{
    if (rotorIndex < 0 || rotorIndex > ROTOR_COUNT)
    {
        fputs("Error: Invalid rotor number", stderr);
        return 1;
    }

    for (int i = 0; i < ALPHABET_SIZE; i++)
    {
        if (rotorWiringConnections[i] < 'A' || rotorWiringConnections[i] > 'Z')
        {
            fputs("Error: Invalid rotor wiring connections\n", stderr);
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

    // ring and rotor position - might be randomized
    enigma->rotorPosition[rotorIndex] = rotorP - 'A';
    enigma->ringPosition[rotorIndex] = ringP - 'A';

    //reflector
    return 0;
}
