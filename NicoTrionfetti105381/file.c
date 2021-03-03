//
// Created by nico on 22/11/2020.
//
#include "playfair.h"
#include "file.h"
#include <stdbool.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>

int isDirectory(const char *path);

void createOutputPath(char *file, MyFile *myFile);

void initMyFile(MyFile *myFile);

MyFile *checkArgv(char **argv, int argc) {
    MyFile *head = malloc(sizeof(MyFile));
    MyFile *myFile = malloc(sizeof(MyFile));
    myFile->dirPath = malloc(DIRPATH_MAX * sizeof(char));
    initMyFile(myFile);
    head = myFile;
    createMatrix(checkKeyFile(argv[2]));
    int dir = isDirectory(argv[3]);
    int i = 3;
    while (i < argc) {
        if (dir != 0) {
            myFile->dirPath = malloc(strlen(argv[3]));
            myFile->dirPath = strcpy(myFile->dirPath, argv[3]);
            if (i == 3)
                i++;
        }
        myFile->fp = fopen(argv[i], "r");
        if (myFile->fp != NULL) {
            printf("file %s valido \n", argv[i]);
            createOutputPath(argv[i], myFile);
            myFile->nextFile = malloc(sizeof(MyFile));
            myFile = myFile->nextFile;
            initMyFile(myFile);
        } else {
            if (argv[i] != NULL)
                printf("file %s non esiste\n", argv[i]);
        }
        i++;
    }
    if (head->fp != NULL) {
        return head;
    }
    errno = ENOENT;
    perror("nessun file disponibile");
    exit(1);
}

int isDirectory(const char *path) {
    struct stat s;
    if (stat(path, &s) == 0) {
        if (s.st_mode & S_IFDIR) {
            printf("directory presente : %s\n", path);
            return 1;
        }
    }
    return 0;
}

bool checkType(char *type) {
    if (strcmp(type, "encode") == 0 || strcmp(type, "decode") == 0) {
        initPlayFair();
        if (strcmp(type, "encode") == 0)
            setType(true);
        else
            setType(false);
        return 1;
    }
    errno = EPERM;
    perror("azioni eseguibili : 'encode' o 'decode' ");
    return 0;
}

void initMyFile(MyFile *myFile) {
    myFile->fp = NULL;
    myFile->nextFile = NULL;
    myFile->dirPath = "";
}

void playfair(MyFile *mf) {
    char *line = malloc(BUFMAX);
    FILE *fileOutput = fopen(mf->outPath, "w");
    bool someError = false;
    while (!someError && ((fgets(line, BUF, mf->fp)) != NULL)) {
        if (getType()) {
            someError = encodeLine(line);
        } else {
            someError = decodeLine(line);
        }
        fprintf(fileOutput, "%s", line);
        if (!someError)
            if (remove(mf->outPath) == 0) {
                errno = ENOTSUP;
                perror("file rimosso a causa del contenuto errato");
            }
    }
    free(line);
    fclose(fileOutput);
    fclose(mf->fp);
}

void createOutputPath(char *file, MyFile *myFile) {
    char *filepath = (char *) malloc(DIRPATH_MAX * sizeof(char));
    if (strcmp(myFile->dirPath, "") != 0) {
        strcpy(filepath, myFile->dirPath);
        if (strcmp(filepath, "./") == 0)
            filepath[strlen(filepath) - 1] = 0;
        strcat(filepath, "/");
        strcat(filepath, file);
    } else {
        filepath[0] = '\0';
        strcat(filepath, file);
    }
    if (getType()) {
        filepath = strcat(filepath, ".pf");
    } else {
        if (file[strlen(file) - 3] == '.' && file[strlen(file) - 2] == 'p' &&
            file[strlen(file) - 1] == 'f') {
            filepath[strlen(filepath) - 1] = 0;
            filepath[strlen(filepath) - 1] = 0;
            filepath[strlen(filepath) - 1] = 0;
            filepath = strcat(filepath, ".dec");
        } else {
            filepath = strcat(filepath, ".dec");
        }
    }
    printf("path = : %s\n", filepath);
    myFile->outPath = filepath;
}
