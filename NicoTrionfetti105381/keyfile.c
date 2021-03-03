//
// Created by nico on 18/12/2020.
//

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include "keyfile.h"

bool checkCharDuplicate(char *str);

bool checkWordUpperCase(char *word);

bool checkAlpabeto(char *alpa);

bool checkCharMancante(char *charMancante);

bool checkCharSpeciale(char *charSpeciale);

void checkCharAssente();

bool checkChiave(char *chiave);

void cleanKey();

void removeAll(char *str, char toRemove, int index);


RealKeyFile *realKeyFile;


bool checkAlpabeto(char *alpa) {
    if (strlen(alpa) == DIMALPA && alpa[strlen(alpa) - 1] == '\n' && alpa[strlen(alpa) - 2] == '\r') {
        if (checkCharDuplicate(alpa)) {
            if (!checkWordUpperCase(alpa)) {
                errno = EIO;
                perror("alfabeto non valido\n");
                exit(1);
            }
            realKeyFile->alfabeto = malloc(strlen(alpa));
            strcpy(realKeyFile->alfabeto, alpa);
            checkCharAssente();
            return true;
        } else {
            errno = EIO;
            perror("lettere duplicate nell'alfabeto alfabeto\n");
            exit(1);
        }

    } else {
        errno = EIO;
        perror("alfabeto non valido\n");
        exit(1);
    }
}

bool checkCharMancante(char *charMancante) {
    if (strlen(charMancante) == DIMCHARM && charMancante[strlen(charMancante) - 1] == '\n' &&
        charMancante[strlen(charMancante) - 2] == '\r') {
        if (!checkWordUpperCase(charMancante)) {
            errno = EIO;
            perror("char_mancante non valido\n");
            exit(1);
        }
        if (charMancante[0] == realKeyFile->char_assente) {
            if (charMancante[0] == 'J')
                charMancante[0] = 'I';
            if (charMancante[0] == 'W')
                charMancante[0] = 'V';
        }
        realKeyFile->char_mancante = charMancante[0];
        return true;
    } else {
        errno = EIO;
        perror("char_mancante non valido\ndeve essere un solo carattere MAIUSCOLO\n");
        exit(1);
    }
}

bool checkCharSpeciale(char *charSpeciale) {
    if (strlen(charSpeciale) == DIMCHARS && charSpeciale[strlen(charSpeciale) - 1] == '\n' &&
        charSpeciale[strlen(charSpeciale) - 2] == '\r') {
        if (!checkWordUpperCase(charSpeciale)) {
            errno = EIO;
            perror("char_speciale non valido\n");
            exit(1);
        }
        if (charSpeciale[0] == realKeyFile->char_assente) {
            if (charSpeciale[0] == 'J')
                charSpeciale[0] = 'I';
            if (charSpeciale[0] == 'W')
                charSpeciale[0] = 'V';
        }
        realKeyFile->char_speciale = charSpeciale[0];
        return true;
    } else {
        errno = EIO;
        perror("char_speciale non valido \ndeve essere un solo carattere MAIUSCOLO\n");
        exit(1);
    }
}

bool checkChiave(char *chiave) {
    if (!checkWordUpperCase(chiave)) {
        errno = EIO;
        perror("chiave non valida\n");
        exit(1);
    }
    if (chiave[strlen(chiave) - 1] != '\n' || chiave[strlen(chiave) - 2] != '\r') {
        errno = EIO;
        perror("formato chiave non valido");
        exit(1);
    }
    realKeyFile->chiave = malloc(strlen(chiave));
    strcpy(realKeyFile->chiave, chiave);
    realKeyFile->chiave[strlen(realKeyFile->chiave) - 1] = 0;
    realKeyFile->chiave[strlen(realKeyFile->chiave) - 1] = 0;
    return true;
}

RealKeyFile *checkKeyFile(char *keyFile) {
    FILE *fp = fopen(keyFile, "r");
    if (fp == NULL) {
        errno = ENOENT;
        perror("key file non esistente");
        exit(1);
    }
    realKeyFile = malloc(sizeof(RealKeyFile));
    char *line = malloc(BUFKEY*sizeof (char));
    size_t len = 0;
    getline(&line, &len, fp);
    checkAlpabeto(line);
    line[0] = '\0';
    getline(&line, &len, fp);
    checkCharMancante(line);
    line[0] = '\0';
    getline(&line, &len, fp);
    checkCharSpeciale(line);
    line[0] = '\0';
    getline(&line, &len, fp);
    checkChiave(line);
    cleanKey();
    return realKeyFile;
}

bool checkCharDuplicate(char *str) {
    for (int i = 0; i < strlen(str) - 1; i++) {
        for (int j = i + 1; j < strlen(str); j++) {
            if (str[i] == str[j]) {
                return false;
            }
        }
    }
    return true;
}

bool checkWordUpperCase(char *word) {
    for (int i = 0; i < strlen(word); i++) {
        if ((word[i] < 65 || word[i] > 90)) {
            if (word[i] != '\x0D' && word[i] != '\x0A' && word[i] != ' ')
                return false;
        }
    }
    return true;
}

void checkCharAssente() {
    bool find = false;
    char a;
    int b;
    for (a = 65; a <= 90; a++) {
        find = false;
        for (b = 0; b < strlen(realKeyFile->alfabeto) && !find; b++) {
            if (a == realKeyFile->alfabeto[b]) {
                find = true;
            }
        }
        if (!find) {
            realKeyFile->char_assente = a;
            return;
        }
    }
}

void cleanKey() {
    int i = 0;
    while (realKeyFile->chiave[i] != '\0') {
        removeAll(realKeyFile->chiave, realKeyFile->chiave[i], i + 1);
        i++;
    }
    unsigned int len = strlen(realKeyFile->chiave);
    for (i = 0; i < len; i++) {
        if (realKeyFile->chiave[i] == realKeyFile->char_assente) {
            memmove(&realKeyFile->chiave[i], &realKeyFile->chiave[i + 1], len - i);
            i--;
            len--;
        }
    }
}

void removeAll(char *str, char toRemove, int index) {
    int i;
    while (str[index] != '\0') {
        if (str[index] == toRemove) {
            i = index;
            while (str[i] != '\0') {
                str[i] = str[i + 1];
                i++;
            }
        } else {
            index++;
        }
    }
}