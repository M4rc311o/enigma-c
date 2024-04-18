#include "../inc/enigma.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void ClearChar() {
    int c;
    while((c = getchar()) != '\n' && c != EOF);
}

char *allocateSettingChars(char *function) {
    char *setting = malloc(sizeof(char) * 30);
    fgets(setting, 30, stdin);
    setting[strcspn(setting, "\n")] = 0;
    function = malloc(strlen(setting) + 1);
    strcpy(function, setting);
    return function;
}

char *ReadInput(char *input) {
    if(fgets(input, 1002, stdin) == NULL) {
        fprintf(stderr, "Problem with input reading.");
        return NULL;
    }
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


int main() {
    Enigma *enigma;
    enigma = enigmaInit("AB CD EF GH IJ KL", "I II III", "J V Z", "L X O");

    if(enigma == NULL) {
        fprintf(stderr, getEnigmaErrorStr(getLastEnigmaError()));
        return 1;
    }

    char *enigmaPlugboardConnectionSetting;
    char *enigmaRotorsNameSetting;
    char *enigmaRotorPositionSetting;
    char *enigmaRingPositionSetting;
    char *input = malloc(sizeof(char) * 1002);

    for(;;) {
        puts("======== CURRENT ENIGMA SETTINGS ========");
        printf("Plugboard connection:\t ");
        printPlugboardConnections(enigma);
        printf("%s\n", enigma->plugboard.plugboardSubstitute);
        printf("Rotor names: \t\t %s %s %s\n", enigma->rotors[0].name, enigma->rotors[1].name, enigma->rotors[2].name);
        printf("Rotor positions:\t %c %c %c\n", 'A' + enigma->rotors[0].rotorPosition, 'A' + enigma->rotors[1].rotorPosition, 'A' + enigma->rotors[2].rotorPosition);
        printf("Rotor ring pos.:\t %c %c %c\n", 'A' + enigma->rotors[0].ringPosition, 'A' + enigma->rotors[1].ringPosition, 'A' + enigma->rotors[2].ringPosition);

        puts("\n============= ENIGMA OPTIONS =============");
        puts("(a)..........Enigma encryption\n(b)..........Enigma decryption\n(s)..........Enigma settings\n(x)..........Enigma exit\n");
        input = ReadInput(input);

        switch(input[0]) {
            char output[1000];
        case 'a':
            puts("\n=================\nEnigma encryption\n=================\n");
            puts("Input text to encrypt [max. 1000 symbols]:");
            enigmaEncStr(enigma, ReadInput(input), output);
            fprintf(stdout, "Encrypted text is: %s\n\n", output);
            break;
        case 'b':
            puts("\n=================\nEnigma decryption\n=================\n");
            puts("Input text to decrypt [max. 1000 symbols]:");
            enigmaEncStr(enigma, ReadInput(input), output);
            fprintf(stdout, "Decrypted text is: %s\n\n", output);
            break;
        case 's':
            puts("\n===============\nEnigma Settings\n===============\n");
            puts("Insert Plugboard Connections in form: 'AB CD EF GH IJ KL'.");
            enigmaPlugboardConnectionSetting = allocateSettingChars(enigmaPlugboardConnectionSetting);

            puts("Insert 3 Rotors. You can choose from I, II, III, IV, V. Form is: 'I IV V'.");
            enigmaRotorsNameSetting = allocateSettingChars(enigmaRotorsNameSetting);

            puts("Insert Position of 3 rotors. You can choose from A - Z. Form is: 'A B C'.");
            enigmaRotorPositionSetting = allocateSettingChars(enigmaRotorPositionSetting);

            puts("Insert Ring Position of 3 rotors. You can choose from A - Z. Form is: 'A B C'.");
            enigmaRingPositionSetting = allocateSettingChars(enigmaRingPositionSetting);

            enigma = enigmaInit(enigmaPlugboardConnectionSetting, enigmaRotorsNameSetting, enigmaRotorPositionSetting, enigmaRingPositionSetting);
            break;
        case 'x':
            puts("============= Goodbye. =============");
            return 0;
            break;
        default:
            puts("\n!#=========== Wrong option. ===========#!\n");
        }
    }

    free(enigmaPlugboardConnectionSetting);
    free(enigmaRotorsNameSetting);
    free(enigmaRotorPositionSetting);
    free(enigmaRingPositionSetting);
    enigmaFree(enigma);
    return 0;
}
