#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <string.h>

#define BUFSIZE 512

ssize_t readline(char *buf, size_t sz, char *fn, off_t *offset) {
    int fd = open(fn, O_RDONLY);
    if (fd == -1) {
        fprintf(stderr, "%s() error: file open failed '%s'.\n",
                __func__, fn);
        return -1;
    }

    ssize_t nchr = 0;
    ssize_t idx = 0;
    char *p = NULL;

    /* position fd & read line */
    if ((nchr = lseek(fd, *offset, SEEK_SET)) != -1)
        nchr = read(fd, buf, sz);
    close(fd);

    if (nchr == -1) {   /* read error   */
        fprintf(stderr, "%s() error: read failure in '%s'.\n",
                __func__, fn);
        return nchr;
    }

    /* end of file - no chars read
    (not an error, but return -1 )*/
    if (nchr == 0) return -1;

    p = buf;    /* check each chacr */
    while (idx < nchr && *p != '\n') p++, idx++;
    *p = 0;

    if (idx == nchr) {  /* newline not found  */
        *offset += nchr;

        /* check file missing newline at end */
        return nchr < (ssize_t) sz ? nchr : 0;
    }

    *offset += idx + 1;

    return idx;
}

//gcc -Wall -Wextra -std=gnu99 -pedantic -Werror main.c
//const char msg[] = "Hello world";
//write( STDOUT_FILENO, msg, sizeof( msg ) - 1 );
void nHead(char *file, int numberOfRows) {
//    char line[512];
    int count = 0;
    if (numberOfRows == -1) {
        numberOfRows = 10;
    }
//    while (fgets(line, sizeof(line), file) != NULL && count < numberOfRows) {
//        printf("%s", line);
//        count++;
//    }
//    char line[BUFSIZE] = {0};
    char *line = (char *) malloc(BUFSIZE);
    off_t offset = 0;
//    ssize_t len = 0;
//    size_t i = 0;
    while (readline(line, BUFSIZE, file, &offset) != -1 && count < numberOfRows) {
//        printf("%s\n", line);
        write(STDOUT_FILENO, line, strlen(line));
        write(STDOUT_FILENO, "\n", 1);
        count++;
    }
    free(line);
}

void cHead(char *file, int numberOfChars) {
//    char line[2];
    int count = 0;
//    while (fgets(line, sizeof(line), file) != NULL && count < numberOfChars) {
//
//    }
    int fd;
    if ((fd = open(file, O_RDONLY)) >= 0) {
        char c;
        while (read(fd, &c, 1) == 1 && count < numberOfChars) {
//            printf("%c", c);
            write(STDOUT_FILENO, &c, 1);
            count++;
        }
//        free(c);

    }
//
//            printf (" line[%2zu] : %s (%zd chars)\n", i++, line, len);
}

void vHead(char *fileName) {
//    printf("==>%s<==\n", strrchr(fileName, '\\'));
    printf("==>%s<==\n", fileName);
}

int main(int argc, char *argv[]) {
    int option;
    int rows = 0;
    int chars = 0;
    char *endptr;
    bool qFlag = false;
    bool vFlag = false;
    while ((option = getopt(argc, argv, "n:c:q::v::")) != -1) { //get option from the getopt() method
        switch (option) {
            case 'n':
                chars = 0;
                rows = strtol(optarg, &endptr, 0);
                if (errno == ERANGE || *endptr != '\0' || optarg == endptr) {
//                    printf("head: invalid number of lines: '%s'\n", optarg);
                    write(STDOUT_FILENO, "head: invalid number of lines: ", 31);
                    write(STDOUT_FILENO, optarg, strlen(optarg));
                    return 1;
                }
                break;
            case 'c':
                rows = 0;
                chars = strtol(optarg, &endptr, 0);
                if (errno == ERANGE || *endptr != '\0' || optarg == endptr) {
//                    printf("head: invalid number of bytes: '%s'\n", optarg);
                    write(STDOUT_FILENO, "head: invalid number of bytes: ", 31);
                    write(STDOUT_FILENO, optarg, strlen(optarg));
                    return 1;
                }
//                printf("c: %s\n", optarg);
                break;
            case 'q':
                qFlag = true;
                vFlag = false;
//                printf("q\n");
                write(STDOUT_FILENO, "q\n", 2);
                break;
            case 'v':
                vFlag = true;
                qFlag = false;
//                printf("v\n");
                write(STDOUT_FILENO, "v\n", 2);
                break;
        }
    }
    for (; optind < argc; optind++) { //when some extra arguments are passed
        char *fileName = argv[optind]; /* should check that argc > 1 */
//        FILE *file = fopen(fileName, "r"); /* should check the result */

        if (vFlag && !qFlag) {
            vHead(fileName);
        }

        if (rows > 0) {
            nHead(fileName, rows);
        }
        if (chars > 0) {
            cHead(fileName, chars);
        }
//        fclose(file);
//        printf("\n");
        write(STDOUT_FILENO, "\n", 1);
    }

//    vHead(fileName);


    return 0;
}

