#include "../inc/enigma.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void printPlugboardConnections(Enigma *enigma);

int main() {
    Enigma *enigma = NULL;
    enigma = enigmaInit("AB CD EF GH IJ KL", "I II III", "J V Z", "L X O");
    if(enigma == NULL) {
        fprintf(stderr, "\nError: %s", getEnigmaErrorStr(getLastEnigmaError()));
        return -1;
    }

    bool run = true;
    while(run) {
        puts("\n======== CURRENT ENIGMA SETTINGS ========");
        printf("Plugboard connection:\t ");
        printPlugboardConnections(enigma);
        printf("\nRotor names: \t\t %s %s %s\n", enigma->rotors[0].name, enigma->rotors[1].name, enigma->rotors[2].name);
        printf("Rotor positions:\t %c %c %c\n", 'A' + enigma->rotors[0].rotorPosition, 'A' + enigma->rotors[1].rotorPosition, 'A' + enigma->rotors[2].rotorPosition);
        printf("Rotor ring pos.:\t %c %c %c\n", 'A' + enigma->rotors[0].ringPosition, 'A' + enigma->rotors[1].ringPosition, 'A' + enigma->rotors[2].ringPosition);

        puts("\n============= ENIGMA OPTIONS =============");
        puts("(a)..........Enigma encryption\n(b)..........Enigma decryption\n(c)..........Enigma create\n(x)..........Enigma exit\n");

        char choice = getchar();
        int c;
        while((c = getchar()) != '\n' && c != EOF);

        switch(choice) {
        case 'a':
            puts("\n=================\nEnigma encryption\n=================\n");
            puts("Input text to encrypt:");
            while((c = getchar()) != '\n' && c != EOF) {
                char ec = enigmaEncChar(enigma, c);
                if(ec == 0) {
                    fprintf(stderr, "\nError: %s\nReturning to main menu.\n", getEnigmaErrorStr(getLastEnigmaError()));
                    while((c = getchar()) != '\n' && c != EOF);
                    break;
                }
                printf("%c", ec);
            }
            puts("");
            break;
        case 'b':
            puts("\n=================\nEnigma decryption\n=================\n");
            puts("Input text to decrypt:");
            while((c = getchar()) != '\n' && c != EOF) {
                char dc = enigmaEncChar(enigma, c);
                if(dc == 0) {
                    while((c = getchar()) != '\n' && c != EOF);
                    fprintf(stderr, "\nError: %s\nReturning to main menu.\n", getEnigmaErrorStr(getLastEnigmaError()));
                    break;
                }
                printf("%c", dc);
            }
            puts("");
            break;
        case 'c':
            Enigma * tmpEnigma;
            size_t len;
            size_t rLen;

            puts("\n===============\nEnigma Settings\n===============\n");
            puts("Insert Plugboard Connections in form: 'AB CD EF GH IJ KL'.");
            len = (CONNECTIONS_COUNT * 2 + CONNECTIONS_COUNT) * sizeof(char) * 2;
            char *conn = (char *)malloc(len);
            if(conn == NULL) {
                free(conn);
                fputs("Error with reading plugboard connections\nReturning to main menu.", stderr);
                break;
            }
            if(fgets(conn, len, stdin) == NULL) {
                free(conn);
                fputs("Error with reading plugboard connections\nReturning to main menu.", stderr);
                break;
            }
            rLen = strcspn(conn, "\n");
            conn[rLen] = 0;
            if(rLen == len - 1) while((c = getchar()) != '\n' && c != EOF);

            puts("Insert 3 Rotors. You can choose from I, II, III, IV, V. Form is: 'I IV V'.");
            len = (ROTOR_COUNT * 3 + ROTOR_COUNT) * sizeof(char) * 2;
            char *rot = (char *)malloc(len);
            if(rot == NULL) {
                free(conn);
                free(rot);
                fputs("Error with reading rotors\nReturning to main menu.", stderr);
                break;
            }
            if(fgets(rot, len, stdin) == NULL) {
                free(conn);
                free(rot);
                fputs("Error with reading rotors\nReturning to main menu.", stderr);
                break;
            }
            rLen = strcspn(rot, "\n");
            rot[rLen] = 0;
            if(rLen == len - 1) while((c = getchar()) != '\n' && c != EOF);

            puts("Insert Position of 3 rotors. You can choose from A - Z. Form is: 'A B C'.");
            len = (ROTOR_COUNT + ROTOR_COUNT) * sizeof(char) * 2;
            char *rotPos = (char *)malloc(len);
            if(rotPos == NULL) {
                free(conn);
                free(rot);
                free(rotPos);
                fputs("Error with reading rotor positions\nReturning to main menu.", stderr);
                break;
            }
            if(fgets(rotPos, len, stdin) == NULL) {
                free(conn);
                free(rot);
                free(rotPos);
                fputs("Error with reading rotor positions\nReturning to main menu.", stderr);
                break;
            }
            rLen = strcspn(rotPos, "\n");
            rotPos[rLen] = 0;
            if(rLen == len - 1) while((c = getchar()) != '\n' && c != EOF);

            puts("Insert Ring Position of 3 rotors. You can choose from A - Z. Form is: 'A B C'.");
            len = (ROTOR_COUNT * 3 + ROTOR_COUNT) * sizeof(char) * 2;
            char *rotRingPos = (char *)malloc(len);
            if(rotRingPos == NULL) {
                free(conn);
                free(rot);
                free(rotPos);
                free(rotRingPos);
                fputs("Error with reading ring positions\nReturning to main menu.", stderr);
                break;
            }
            if(fgets(rotRingPos, len, stdin) == NULL) {
                free(conn);
                free(rot);
                free(rotPos);
                free(rotRingPos);
                fputs("Error with reading ring positions\nReturning to main menu.", stderr);
                break;
            }
            rLen = strcspn(rotRingPos, "\n");
            rotRingPos[rLen] = 0;
            if(rLen == len - 1) while((c = getchar()) != '\n' && c != EOF);

            tmpEnigma = enigmaInit(conn, rot, rotPos, rotRingPos);
            free(conn);
            free(rot);
            free(rotPos);
            free(rotRingPos);
            if(tmpEnigma == NULL) {
                fprintf(stderr, "\nError: %s\nReturning to main menu.", getEnigmaErrorStr(getLastEnigmaError()));
                break;
            }
            enigmaFree(enigma);
            enigma = tmpEnigma;
            break;
        case 'x':
            puts("============= Goodbye. =============");
            run = false;
            break;
        default:
            puts("\n!#=========== Wrong option. ===========#!\n");
        }
    }

    enigmaFree(enigma);

    return 0;
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
