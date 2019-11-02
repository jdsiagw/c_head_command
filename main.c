#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
#include "mpp2file.h"

#define BUFSIZE 512

void nHead(char *file, long numberOfRows) {
    int count = 0;
    if (numberOfRows == -1) {
        numberOfRows = 10;
    }
    char *line = (char *) malloc(BUFSIZE);
    off_t offset = 0;
    while (readline(line, BUFSIZE, file, &offset) != -1 && count < numberOfRows) {
        write(STDOUT_FILENO, line, strlen(line));
        write(STDOUT_FILENO, "\n", 1);
        count++;
    }
    free(line);
}

void cHead(char *file, long numberOfChars) {
    int count = 0;
    int fd;
    if ((fd = open(file, O_RDONLY)) >= 0) {
        char c;
        while (read(fd, &c, 1) == 1 && count < numberOfChars) {
            write(STDOUT_FILENO, &c, 1);
            count++;
        }
    }
}

void vHead(char *fileName) {
    write(STDOUT_FILENO, "==>", 3);
    write(STDOUT_FILENO, fileName, strlen(fileName));
    write(STDOUT_FILENO, "<==\n", 4);
}

int main(int argc, char *argv[]) {
    int option;
    long rows = 10;
    long chars = 0;
    char *endptr;
    bool qFlag = false;
    bool vFlag = false;
    while ((option = getopt(argc, argv, "n:c:q::v::")) != -1) {
        switch (option) {
            case 'n':
                chars = 0;
                rows = strtol(optarg, &endptr, 0);
                if (errno == ERANGE || *endptr != '\0' || optarg == endptr) {
                    write(STDOUT_FILENO, "head: invalid number of lines: ", 31);
                    write(STDOUT_FILENO, optarg, strlen(optarg));
                    write(STDOUT_FILENO, "\n", 1);
                    return 1;
                }
                break;
            case 'c':
                rows = 0;
                chars = strtol(optarg, &endptr, 0);
                if (errno == ERANGE || *endptr != '\0' || optarg == endptr) {
                    write(STDOUT_FILENO, "head: invalid number of bytes: ", 31);
                    write(STDOUT_FILENO, optarg, strlen(optarg));
                    write(STDOUT_FILENO, "\n", 1);
                    return 1;
                }
                break;
            case 'q':
                qFlag = true;
                vFlag = false;
                write(STDOUT_FILENO, "q\n", 2);
                break;
            case 'v':
                vFlag = true;
                qFlag = false;
                write(STDOUT_FILENO, "v\n", 2);
                break;
        }
    }
    for (; optind < argc; optind++) {
        char *fileName = argv[optind];

        if (argc - optind > 1) {
            vFlag = true;
            qFlag = false;
        }

        if (vFlag && !qFlag) {
            vHead(fileName);
        }
        if (rows > 0) {
            nHead(fileName, rows);
        }
        if (chars > 0) {
            cHead(fileName, chars);
        }
        write(STDOUT_FILENO, "\n", 1);
    }

    return 0;
}

