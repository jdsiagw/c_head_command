#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <asm/errno.h>
#include <stdlib.h>
#include "mpp2file.h"
#include "mpp2print.h"

ssize_t openFile(char *fileName) {
    int fd = open(fileName, O_RDONLY);
    if (fd == -1) {
        return ENOENT;
    }
    return fd;
}

int countLinesOfFile(char *filename) {
    char *buffer = (char *) malloc(BUFSIZE);
    int bytesRead;
    int lines = 1;
    int fd = openFile(filename);
    if (fd == ENOENT) {
        printError(strerror(ENOENT));
        return ENOENT;
    }
    bytesRead = read(fd, buffer, sizeof(buffer));
    while (bytesRead) {
        for (int i = 0; i < bytesRead; ++i) {
            if (buffer[i] == '\n' || buffer[i] == 0x0) {
                lines++;
            }
        }
        bytesRead = read(fd, buffer, sizeof(buffer));
    }
    free(buffer);
    return lines;
}

long countCharsOfFile(char *filename) {
    char *buffer = (char *) malloc(BUFSIZE);
    int bytesRead;
    int count = 0;
    int fd = openFile(filename);
    if (fd == ENOENT) {
        printError(strerror(ENOENT));
        return ENOENT;
    }
    bytesRead = read(fd, buffer, sizeof(buffer));
    while (bytesRead) {
        for (int i = 0; i < bytesRead; ++i) {
            count++;
        }
        bytesRead = read(fd, buffer, sizeof(buffer));
    }
    return count;
}