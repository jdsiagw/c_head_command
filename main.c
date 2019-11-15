#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>
//#include "mpp2file.h"

#define BUFSIZE 4096

void print(char *str) {
    size_t length = strlen(str);
    size_t size = 0;
    while (size < length) {
        size_t s = write(STDOUT_FILENO, str + size, length - size);
        if (s == -1) {
            return;
        }
        size += s;
    }
}

void printError(char *str) {
    size_t length = strlen(str);
    size_t size = 0;
    while (size < length) {
        size_t s = write(STDERR_FILENO, str + size, length - size);
        if (s == -1) {
            return;
        }
        size += s;
    }
}

ssize_t openFile(char *fileName) {
    int fd = open(fileName, O_RDONLY);
    if (fd == -1) {
        return ENOENT;
    }
    return fd;
}

int countLinesOfFile(char *filename) {
    char *buffer = (char *) malloc(BUFSIZE);
    int bytes_read;
    int lines = 0;
    int fd = openFile(filename);
    if (fd == ENOENT) {
        printError(strerror(ENOENT));
        return ENOENT;
    }
    bytes_read = read(fd, buffer, sizeof(buffer));
    while (bytes_read) {
        for (int i = 0; i < bytes_read; ++i) {
            if (buffer[i] == '\n' || buffer[i] == 0x0) {
                lines++;
            }
        }
        bytes_read = read(fd, buffer, sizeof(buffer));
    }
    free(buffer);
    return lines;
}

long countCharsOfFile(char *filename) {
    char *buffer = (char *) malloc(BUFSIZE);
    int bytes_read;
    int count = 0;
    int fd = openFile(filename);
    if (fd == ENOENT) {
        printError(strerror(ENOENT));
        return ENOENT;
    }
    bytes_read = read(fd, buffer, sizeof(buffer));
    while (bytes_read) {
        for (int i = 0; i < bytes_read; ++i) {
            count++;
        }
        bytes_read = read(fd, buffer, sizeof(buffer));
    }
    return count;
}

void nHead(char *filename, long numberOfRows) {
    char *buffer = (char *) malloc(BUFSIZE);
    char buf[1] = {0};
    int bytes_read;
    int lines = 0;

    int fd = openFile(filename);
    if (fd == ENOENT) {
        printError(strerror(ENOENT));
        return;
    }
    bytes_read = read(fd, buffer, sizeof(buffer));
    while (bytes_read && lines < numberOfRows) {
        for (int i = 0; i < bytes_read; ++i) {
            buf[0] = buffer[i];
            print(buf);
            if (buffer[i] == '\n' || buffer[i] == 0x0) {
                lines++;
                if (lines == numberOfRows) {
                    break;
                }
            }
        }
        bytes_read = read(fd, buffer, sizeof(buffer));
    }
    free(buffer);
}

void cHead(char *filename, long numberOfChars) {
    char *buffer = (char *) malloc(BUFSIZE);
    int bytes_read;
    int count = 0;
    char buf[1] = {0};
    int fd = openFile(filename);
    if (fd == ENOENT) {
        printError(strerror(ENOENT));
        printError("\n");
        return;
    }
    bytes_read = read(fd, buffer, sizeof(buffer));
    while (bytes_read && count < numberOfChars) {
        for (int i = 0; i < bytes_read; ++i) {
            buf[0] = buffer[i];
            print(buf);
            count++;
            if (count == numberOfChars) {
                break;
            }
        }
        bytes_read = read(fd, buffer, sizeof(buffer));
    }
}

void vHead(char *fileName) {
    print("==>");
    print(fileName);
    print("<==");
}

int main(int argc, char *argv[]) {
    if (argc == 0) {

    }
//    printError("123\n321");
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
                    printError("head: invalid number of lines: ");
                    printError(optarg);
                    printError("\n");
                    return 1;
                }
                break;
            case 'c':
                rows = 0;
                chars = strtol(optarg, &endptr, 0);
                if (errno == ERANGE || *endptr != '\0' || optarg == endptr) {
                    printError("head: invalid number of bytes: ");
                    printError(optarg);
                    printError("\n");
                    return 1;
                }
                break;
            case 'q':
                qFlag = true;
                vFlag = false;
//                write(STDOUT_FILENO, "q\n", 2);
                break;
            case 'v':
                vFlag = true;
                qFlag = false;
//                write(STDOUT_FILENO, "v\n", 2);
                break;
            default:
                return 0;
        }
    }
    for (; optind < argc; optind++) {
        char *fileName = argv[optind];

        if (argc - optind > 1 && qFlag == false) {
            vFlag = true;
        }
        if (vFlag && !qFlag) {
            vHead(fileName);
        }
        if (rows >= 0) {
            nHead(fileName, rows);
        }
        if (rows < 0) {
            long totalCount = countLinesOfFile(fileName);
            nHead(fileName, totalCount + rows);
        }
        if (chars >= 0) {
            long totalCount = 0;
            cHead(fileName, totalCount + chars);
        }
        if (chars < 0) {
            long totalCount = countCharsOfFile(fileName);
            cHead(fileName, totalCount + rows);
        }
        print("\n");
    }

    return 0;
}