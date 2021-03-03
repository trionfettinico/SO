//
// Created by nico on 28/12/2020.
//

#ifndef PLAYFAIR_KEYFILE_H
#define PLAYFAIR_KEYFILE_H

#define DIMALPA 27
#define DIMCHARM 3
#define DIMCHARS 3
#define BUFKEY 256

typedef struct keyFile {
    char *alfabeto;
    char char_mancante;
    char char_speciale;
    char char_assente;
    char *chiave;
} RealKeyFile;

RealKeyFile *checkKeyFile(char *keyFile);

#endif //PLAYFAIR_KEYFILE_H
