#include <stdio.h>
#include <unistd.h>
#include <string.h>

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