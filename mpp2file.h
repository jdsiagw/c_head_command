//
// Created by admin on 02.11.2019.
//
#pragma once
#ifndef C_HEAD_COMMAND_MPP2FILE_H
#define C_HEAD_COMMAND_MPP2FILE_H
#define BUFSIZE 4096

ssize_t openFile(char *fileName);
int countLinesOfFile(char *filename);
long countCharsOfFile(char *filename);

#endif //C_HEAD_COMMAND_MPP2FILE_H

