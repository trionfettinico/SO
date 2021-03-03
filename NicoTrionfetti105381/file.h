//
// Created by nico on 28/12/2020.
//

#ifndef PLAYFAIR_FILE_H
#define PLAYFAIR_FILE_H

#include <stdio.h>

#define DIRPATH_MAX 50

typedef struct file {
    FILE *fp;
    char *outPath;
    char *dirPath;
    struct file *nextFile;
} MyFile;

MyFile *checkArgv(char **argv, int argc);

bool checkType(char *type);

void playfair(MyFile *mf);

#endif //PLAYFAIR_FILE_H
