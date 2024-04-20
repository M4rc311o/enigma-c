#include "../inc/enigma.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

char *allocateSettingChars(char *function, char *setting) {
    free(function);
    setting[strcspn(setting, "\n")] = 0;
    function = malloc(strlen(setting) + 1);
    strcpy(function, setting);
    return function;
}

char *ReadInput(char *input) {
    if(fgets(input, 1005, stdin) == NULL) {
        fprintf(stderr, "Problem with input reading.");
        return NULL;
    }
    input[strcspn(input, "\n")] = 0;
    return input;
}

void printPlugboardConnections(Enigma *enigma) {
    int alreadyPrinted[ALPHABET_SIZE] = {0};
    for(int i = 0; i < ALPHABET_SIZE; i++) {
        int substitution = enigma->plugboard.plugboardSubstitute[i];

        if(!alreadyPrinted[i] && substitution != i) {
            printf("%c%c ", 'A' + (i), 'A' + (substitution));
            alreadyPrinted[i] = 1;
            alreadyPrinted[substitution] = 1;
        }
    }
}

bool enigmaGotError() {
    if(getLastEnigmaError() != 0) {
        fprintf(stderr, "\nError: %s\nReturning to main menu.\n", getEnigmaErrorStr(getLastEnigmaError()));
        return true;
    }
    return false;
}


int main() {
    Enigma *enigma;
    char *input = malloc(sizeof(char) * 1005);
    char *enigmaPlugboardConnectionSetting = malloc(strlen("AB CD EF GH IJ KL") + 1);
    char *enigmaRotorsNameSetting = malloc(strlen("I II III") + 1);
    char *enigmaRotorPositionSetting = malloc(strlen("J V Z") + 1);
    char *enigmaRingPositionSetting = malloc(strlen("L X O") + 1);
    char output[1000];

    strcpy(enigmaPlugboardConnectionSetting, "AB CD EF GH IJ KL");
    strcpy(enigmaRotorsNameSetting, "I II III");
    strcpy(enigmaRotorPositionSetting, "J V Z");
    strcpy(enigmaRingPositionSetting, "L X O");

    for(;;) {
        Enigma *enigmaTmp;
        enigma = enigmaInit(enigmaPlugboardConnectionSetting, enigmaRotorsNameSetting, enigmaRotorPositionSetting, enigmaRingPositionSetting);
        if(enigma == NULL) {
            fprintf(stderr, getEnigmaErrorStr(getLastEnigmaError()));
            return 1;
        }

        puts("\n======== CURRENT ENIGMA SETTINGS ========");
        printf("Plugboard connection:\t ");
        printPlugboardConnections(enigma);
        printf("\nRotor names: \t\t %s %s %s\n", enigma->rotors[0].name, enigma->rotors[1].name, enigma->rotors[2].name);
        printf("Rotor positions:\t %c %c %c\n", 'A' + enigma->rotors[0].rotorPosition, 'A' + enigma->rotors[1].rotorPosition, 'A' + enigma->rotors[2].rotorPosition);
        printf("Rotor ring pos.:\t %c %c %c\n", 'A' + enigma->rotors[0].ringPosition, 'A' + enigma->rotors[1].ringPosition, 'A' + enigma->rotors[2].ringPosition);

        puts("\n============= ENIGMA OPTIONS =============");
        puts("(a)..........Enigma encryption\n(b)..........Enigma decryption\n(s)..........Enigma settings\n(x)..........Enigma exit\n");
        input = ReadInput(input);

        switch(input[0]) {
        case 'a':
            puts("\n=================\nEnigma encryption\n=================\n");
            puts("Input text to encrypt [max. 1000 symbols]:");
            if(enigmaEncStr(enigma, ReadInput(input), output) != 0) {
                fprintf(stderr, "Error: %s", getEnigmaErrorStr(getLastEnigmaError()));
                fprintf(stdout, "\nReturning to main menu.\n");
                break;
            }
            fprintf(stdout, "Encrypted text is: %s\n\n", output);
            break;
        case 'b':
            puts("\n=================\nEnigma decryption\n=================\n");
            puts("Input text to decrypt [max. 1000 symbols]:");
            if(enigmaEncStr(enigma, ReadInput(input), output) != 0) {
                fprintf(stderr, "Error: %s", getEnigmaErrorStr(getLastEnigmaError()));
                fprintf(stdout, "\nReturning to main menu.\n");
                break;
            }
            fprintf(stdout, "Decrypted text is: %s\n\n", output);
            break;
        case 's':
            puts("\n===============\nEnigma Settings\n===============\n");
            puts("Insert Plugboard Connections in form: 'AB CD EF GH IJ KL'.");
            input = ReadInput(input);
            if(input[0] != '\0') {
                enigmaTmp = enigmaInit(input, enigmaRotorsNameSetting, enigmaRotorPositionSetting, enigmaRingPositionSetting);
                if(enigmaGotError()) {
                    enigmaFree(enigmaTmp);
                    break;
                }
                enigmaFree(enigma);
                enigmaPlugboardConnectionSetting = allocateSettingChars(enigmaPlugboardConnectionSetting, input);
            }
            else fprintf(stdout, "Skipping plugboard settings.\n\n");
            puts("Insert 3 Rotors. You can choose from I, II, III, IV, V. Form is: 'I IV V'.");
            input = ReadInput(input);
            if(input[0] != '\0') {
                enigmaTmp = enigmaInit(enigmaPlugboardConnectionSetting, input, enigmaRotorPositionSetting, enigmaRingPositionSetting);
                if(enigmaGotError()) {
                    enigmaFree(enigmaTmp);
                    break;
                }
                enigmaFree(enigmaTmp);
                enigmaRotorsNameSetting = allocateSettingChars(enigmaRotorsNameSetting, input);
            }
            else fprintf(stdout, "Skipping rotor name settings.\n\n");

            puts("Insert Position of 3 rotors. You can choose from A - Z. Form is: 'A B C'.");
            input = ReadInput(input);
            if(input[0] != '\0') {
                enigmaTmp = enigmaInit(enigmaPlugboardConnectionSetting, enigmaRotorsNameSetting, input, enigmaRingPositionSetting);
                if(enigmaGotError()) {
                    enigmaFree(enigmaTmp);
                    break;
                }
                enigmaRotorPositionSetting = allocateSettingChars(enigmaRotorPositionSetting, input);
                enigmaFree(enigmaTmp);
            }
            else fprintf(stdout, "Skipping rotor position settings.\n\n");

            puts("Insert Ring Position of 3 rotors. You can choose from A - Z. Form is: 'A B C'.");
            input = ReadInput(input);
            if(input[0] != '\0') {
                enigmaInit(enigmaPlugboardConnectionSetting, enigmaRotorsNameSetting, enigmaRotorPositionSetting, input);
                if(enigmaGotError()) {
                    enigmaFree(enigmaTmp);
                    break;
                }
                enigmaRingPositionSetting = allocateSettingChars(enigmaRingPositionSetting, input);
                enigmaFree(enigmaTmp);
            }
            else fprintf(stdout, "Skipping ring position settings.\n\n");

            enigma = enigmaInit(enigmaPlugboardConnectionSetting, enigmaRotorsNameSetting, enigmaRotorPositionSetting, enigmaRingPositionSetting);
            break;
        case 'x':
            puts("============= Goodbye. =============");
            goto freeMemory;
        default:
            puts("\n!#=========== Wrong option. ===========#!\n");
        }
    }

freeMemory:
    free(input);
    free(enigmaPlugboardConnectionSetting);
    free(enigmaRotorsNameSetting);
    free(enigmaRotorPositionSetting);
    free(enigmaRingPositionSetting);
    enigmaFree(enigma);

    return 0;
}
