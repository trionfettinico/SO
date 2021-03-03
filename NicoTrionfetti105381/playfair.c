
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <errno.h>
#include "playfair.h"

void encode(char *line, int x);

int decode(char *line, int x);

void toUpper(char *string);

void addSpace(char *string);

int removeSpaces(char *string);

void findInMatrix(char *line, int x1, int x2, int y1, int y2, int x, int value);

RealPlayFair *realPlayFair;

void toUpper(char *string) {
    unsigned int len = strlen(string);
    for (int i = 0; i < len; i++) {
        if (toupper(string[i]) == realPlayFair->keyFile->char_assente)
            string[i] = realPlayFair->keyFile->char_mancante;
        if (string[i] < 65 || string[i] > 90) {
            if (97 <= string[i] && string[i] <= 122) {
                string[i] -= 32;
            } else {
                memmove(&string[i], &string[i + 1], len - i);
                i--;
                len--;
            }
        }
    }
}

void addSpace(char *string) {
    char *app = (char *) malloc(BUFMAX);
    app[0] = '\0';
    strcpy(app, string);
    string[0] = '\0';
    for (int i = 0; i < strlen(app); i += 2) {
        if (app[i + 1] == '\0' || app[i + 1] == '\n' || app[i + 1] == '\r') {
            strncat(string, &app[i], 1);
            strncat(string, &realPlayFair->keyFile->char_mancante, 1);
        } else if (app[i] == app[i + 1]) {
            strncat(string, &app[i], 1);
            strncat(string, &realPlayFair->keyFile->char_speciale, 1);
            i--;
        } else {
            strncat(string, &app[i], 1);
            strncat(string, &app[i + 1], 1);
        }
        if (app[i + 2] != '\0') {
            strcat(string, " ");
        }
    }
    free(app);
}

int removeSpaces(char *string) {
    int i, count = 0;
    for (i = 0; i < strlen(string); i++)
        if (string[i] != ' ')
            string[count++] = string[i];
    string[count] = '\0';
    return count;
}

int encodeLine(char *line) {
    toUpper(line);
    addSpace(line);
    printf("mssaggio in chiaro : %s\n", line);
    for (int i = 0; i < strlen(line); i += 3) {
        encode(line, i);
    }
    printf("mssaggio criptato : %s\n", line);
    return 1;
}


void encode(char *line, int x) {
    int i, j;
    int x1 = 0;
    int y1 = 0;
    int x2 = 0;
    int y2 = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (line[x] == realPlayFair->block[i][j]) {
                x1 = i;
                y1 = j;
            } else if (line[x + 1] == realPlayFair->block[i][j]) {
                x2 = i;
                y2 = j;
            }
        }
    }
    findInMatrix(line, x1, x2, y1, y2, x, 1);
}

int decodeLine(char *line) {
    printf("mssaggio criptato : %s\n", line);
    removeSpaces(line);
    if (strlen(line) % 2 != 0) {
        errno = EIO;
        perror("errore : messaggio criptato non valido");
        return 0;
    }
    for (int i = 0; i < strlen(line); i += 2) {
        if (!decode(line, i))
            return 0;
    }
    addSpace(line);
    printf("mssaggio in chiaro : %s\n", line);
    return 1;
}

int decode(char *line, int x) {
    if (!(line[x] <= 90 && line[x] >= 65) || !(line[x + 1] <= 90 && line[x + 1] >= 65)) {
        errno = EIO;
        perror("errore : messaggio criptato non valido");
        return 0;
    }
    if (line[x] == realPlayFair->keyFile->char_assente || line[x + 1] == realPlayFair->keyFile->char_assente) {
        errno = EIO;
        perror("errore : messaggio criptato non valido");
        return 0;
    }
    int i, j;
    int x1, y1, x2, y2;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (line[x] == realPlayFair->block[i][j]) {
                x1 = i;
                y1 = j;
            } else if (line[x + 1] == realPlayFair->block[i][j]) {
                x2 = i;
                y2 = j;
            }
        }
    }
    findInMatrix(line, x1, x2, y1, y2, x, -1);
    return 1;
}

void findInMatrix(char *line, int x1, int x2, int y1, int y2, int x, int value) {
    if (x1 == x2) {
        y1 = (y1 + value) % 5;
        y2 = (y2 + value) % 5;
        if (y1 < 0) y1 = 4;
        if (y2 < 0) y2 = 4;
        line[x] = realPlayFair->block[x1][y1];
        line[x + 1] = realPlayFair->block[x2][y2];
    } else if (y1 == y2) {
        x1 = (x1 + value) % 5;
        x2 = (x2 + value) % 5;
        if (x1 < 0) x1 = 4;
        if (x2 < 0) x2 = 4;
        line[x] = realPlayFair->block[x1][y1];
        line[x + 1] = realPlayFair->block[x2][y2];
    } else {
        line[x] = realPlayFair->block[x1][y2];
        line[x + 1] = realPlayFair->block[x2][y1];
    }
}

int createMatrix(RealKeyFile *keyfile) {
    realPlayFair->keyFile = keyfile;
    int i, j, k, m = 0;
    char keyminus[25];
    removeSpaces(realPlayFair->keyFile->chiave);
    j = 0;
    for (i = 0; i < strlen(realPlayFair->keyFile->alfabeto); i++) {
        for (k = 0; k < strlen(realPlayFair->keyFile->chiave); k++) {
            if (realPlayFair->keyFile->chiave[k] == realPlayFair->keyFile->alfabeto[i] ||
                realPlayFair->keyFile->alfabeto[i] == realPlayFair->keyFile->char_assente)
                break;
        }
        if (k == strlen(realPlayFair->keyFile->chiave)) {
            keyminus[j] = realPlayFair->keyFile->alfabeto[i];
            j++;
        }
    }
    k = 0;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            if (k < strlen(realPlayFair->keyFile->chiave)) {
                realPlayFair->block[i][j] = realPlayFair->keyFile->chiave[k];
                k++;
            } else {
                realPlayFair->block[i][j] = keyminus[m];
                m++;
            }
        }
    }
    return 0;
}

void initPlayFair() {
    realPlayFair = malloc(sizeof(RealPlayFair));
}

void setType(bool isEncode) {
    realPlayFair->isEncode = isEncode;
}

bool getType() {
    return realPlayFair->isEncode;
}



