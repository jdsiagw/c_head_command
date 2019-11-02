#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

ssize_t readline(char *buf, size_t sz, char *fileName, off_t *offset) {
    int fileDescriptor = open(fileName, O_RDONLY);
    if (fileDescriptor == -1) {
        write(STDOUT_FILENO, "head: cannot open '", 19);
        write(STDOUT_FILENO, fileName, strlen(fileName));
        write(STDOUT_FILENO, "' for reading: No such file or directory\n", 41);
        return -1;
    }

    ssize_t nchr = 0;
    ssize_t i = 0;
    char *p = NULL;

    if ((nchr = lseek(fileDescriptor, *offset, SEEK_SET)) != -1)
        nchr = read(fileDescriptor, buf, sz);
    close(fileDescriptor);

    if (nchr == -1) {
        write(STDOUT_FILENO, "head: cannot read '", 19);
        write(STDOUT_FILENO, fileName, strlen(fileName));
        write(STDOUT_FILENO, "'\n", 2);
        return nchr;
    }

    if (nchr == 0) return -1; // end of file

    p = buf;
    while (i < nchr && *p != '\n') p++, i++;
    *p = 0;

    if (i == nchr) {  // if newline not found
        *offset += nchr;
        return 0;
    }

    *offset += ++i;

    return i;
}