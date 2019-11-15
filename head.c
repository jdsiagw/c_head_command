#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "mpp2file.h"
#include "mpp2print.h"

void nHead(char *filename, long numberOfRows) {
    char *buffer = (char *) malloc(BUFSIZE);
    char buf[1] = {0};
    int bytesRead;
    int lines = 0;

    int fd = openFile(filename);
    if (fd == ENOENT) {
        printError(strerror(ENOENT));
        return;
    }
    bytesRead = read(fd, buffer, sizeof(buffer));
    while (bytesRead && lines < numberOfRows) {
        for (int i = 0; i < bytesRead; ++i) {
            buf[0] = buffer[i];
            print(buf);
            if (buffer[i] == '\n' || buffer[i] == 0x0) {
                lines++;
                if (lines == numberOfRows) {
                    break;
                }
            }
        }
        bytesRead = read(fd, buffer, sizeof(buffer));
    }
    free(buffer);
}

void cHead(char *filename, long numberOfChars) {
    char *buffer = (char *) malloc(BUFSIZE);
    int bytesRead;
    int count = 0;
    char buf[1] = {0};
    int fd = openFile(filename);
    if (fd == ENOENT) {
        printError(strerror(ENOENT));
        printError("\n");
        return;
    }
    bytesRead = read(fd, buffer, sizeof(buffer));
    while (bytesRead && count < numberOfChars) {
        for (int i = 0; i < bytesRead; ++i) {
            buf[0] = buffer[i];
            print(buf);
            count++;
            if (count == numberOfChars) {
                break;
            }
        }
        bytesRead = read(fd, buffer, sizeof(buffer));
    }
}

void vHead(char *fileName) {
    print("==>");
    print(fileName);
    print("<==\n");
}

void defaultWay(long rep) {
    char *str = (char *) malloc(BUFSIZE);
    int count = 0;
    while (count < rep) {
        read(STDIN_FILENO, str, BUFSIZE);
        print(str);
        count++;
    }
    free(str);
}