

#include <stdio.h>
#include <errno.h>
#include <stdbool.h>
#include "file.h"

void printHelp();

int main(int argc, char **argv) {
    if (argc < 4) {
        errno = EINVAL;
        perror("paramatri errati");
        printHelp();
        return 0;
    }
    if (checkType(argv[1])) {
        MyFile *myFile = checkArgv(argv, argc);
        if (myFile->fp == NULL) {
            errno = ENOENT;
            perror("nessun file da criptare/decriptare passato");
            return 0;
        }
        while (myFile->fp != NULL) {
            playfair(myFile);
            printf("file %s creato con successo \n", myFile->outPath);
            myFile = myFile->nextFile;
        }
        return 0;
    }
    printHelp();
    return 0;
}

void printHelp() {
    printf("Different Features: \n");
    printf("  encode <keyfile> <outputdir> <file1> … <filen>  \n");
    printf("  encode <keyfile> <file1> … <filen>  \n");
    printf("  decode <keyfile> <outputdir> <file1> … <filen> \n");
    printf("  decode <keyfile> <file1> … <filen> \n");
}
