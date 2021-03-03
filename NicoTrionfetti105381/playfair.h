//
// Created by nico on 28/12/2020.
//

#ifndef PLAYFAIR_PLAYFAIR_H
#define PLAYFAIR_PLAYFAIR_H

#include "keyfile.h"
#include <stdbool.h>

#define BUF 4095
#define BUFMAX 8190


typedef struct playFair {
    char block[5][5];
    bool isEncode;
    struct keyFile *keyFile;
} RealPlayFair;

int createMatrix(RealKeyFile *realKeyFile);

int encodeLine(char *line);

int decodeLine(char *line);

void initPlayFair();

void setType(bool isEncode);

bool getType();


#endif //PLAYFAIR_PLAYFAIR_H
