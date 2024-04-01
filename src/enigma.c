#include "../inc/enigma.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

int plugboardInit(Enigma *enigma, char *plugboardConnections);
int rotorInit(Enigma *enigma, int rotorIndex, char *rotorWiringConnections, char *rotorP, char *ringP);
int reflectorInit(Enigma *enigma, char *reflectorConnections);
char correctInput(char *input);


Enigma *enigmaInit(char *plugboardConnections, char *firstRotor, char *rotorP1, char *ringP1, char *secondRotor, char *rotorP2, char *ringP2, char *thirdRotor, char *rotorP3, char *ringP3, char *reflector)
{
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

