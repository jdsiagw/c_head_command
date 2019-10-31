#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdbool.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

//gcc -Wall -Wextra -std=gnu99 -pedantic -Werror main.c
void nHead(FILE *file, int numberOfRows) {
    char line[512];
    int count = 0;
    if (numberOfRows == -1) {
        numberOfRows = 10;
    }
    while (fgets(line, sizeof(line), file) != NULL && count < numberOfRows) {
        printf("%s", line);
        count++;
    }
}

void cHead(FILE *file, int numberOfChars) {
    char line[2];
    int count = 0;
    while (fgets(line, sizeof(line), file) != NULL && count < numberOfChars) {
        printf("%s", line);
        count++;
    }
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
//    bool qFlag = false;
//    bool vFlag = false;
    while((option = getopt(argc, argv, "n:c:q::v::")) != -1){ //get option from the getopt() method
        switch(option){
            case 'n':
                if (chars > 0) {
                    chars = 0;
                }
                rows = strtol(optarg, &endptr, 0);
                if (errno == ERANGE || *endptr != '\0' || optarg == endptr) {
                    printf("head: invalid number of lines: '%s'\n", optarg);
                    return 1;
                }
                break;
            case 'c':
                if (rows > 0) {
                    rows = 0;
                }
                chars = strtol(optarg, &endptr, 0);
                if (errno == ERANGE || *endptr != '\0' || optarg == endptr) {
                    printf("head: invalid number of lines: '%s'\n", optarg);
                    return 1;
                }
//                printf("c: %s\n", optarg);
                break;
            case 'q':
//                qFlag = true;
                printf("q\n");
                break;
            case 'v':
//                vFlag = true;
                printf("v\n");
                break;
        }
    }
    for(; optind < argc; optind++){ //when some extra arguments are passed
        char *fileName = argv[optind]; /* should check that argc > 1 */
        FILE *file = fopen(fileName, "r"); /* should check the result */


        if (rows > 0) {
            vHead(fileName);
            nHead(file, rows);
        }
        if (chars > 0) {
            vHead(fileName);
            cHead(file, chars);
        }
        fclose(file);
        printf("\n");
    }

//    vHead(fileName);


    return 0;
}

