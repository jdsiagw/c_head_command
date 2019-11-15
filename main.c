#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdbool.h>
#include "mpp2file.h"
#include "mpp2print.h"
#include "head.h"

int main(int argc, char *argv[]) {
    int option;
    long rep = 10;
    long rows = 10;
    long chars = 0;
    char *endptr;
    bool qFlag = false;
    bool vFlag = false;

    if (argc == 1) {
        defaultWay(rep);
    }
    if (argc == 2) {
        rep = strtol(argv[1], &endptr, 0);
        if (errno != ERANGE && *endptr == '\0' && argv[1] != endptr && rep < 0) {
            defaultWay(0-rep);
            return 0;
        }
    }

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
                break;
            case 'v':
                vFlag = true;
                qFlag = false;
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