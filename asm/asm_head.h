#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../lib/globalization.h"
#include "../lib/text.h"
#include "../lib/stack.h"
#include "../lib/flog.h"
#include "../lib/cmd.h"

#define CONST_STR_LEN 121

extern int Errors;

char* handleComLine (int argc, char* argv[], char** outFileName);

void writeCode (Text* codeFile, char* outStr, Stack<Tag>* tags, size_t* Ip);

void handleArg (const Text* codeFile, const int line, char* outStr, const unsigned char cmdNum, Stack<Tag>* tags, size_t* Ip);

void printReg (size_t* Ip, const size_t commandIp, char* outStr, unsigned char* cmdNum, const char* arg);

void printImm (size_t *Ip, const size_t commandIp, char* outStr, unsigned char* cmdNum, const elem_t value);

void addTag (Stack<Tag>* tags, const char* name, size_t* Ip);

void printTag (Stack<Tag>* tags, char* name, char* outStr, size_t* Ip, size_t commandIp, char cmdNum);