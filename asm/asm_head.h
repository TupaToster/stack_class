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

struct Tag {

    char* name = NULL;
    int ip = -1;

    Tag (const char* _name, elem_t _ip) : ip (_ip) {

        assert (_name != NULL);
        name = strdup (_name);
        assert (name != NULL);
    }

    void DTOR () {

        for (char* i = name; *i != '\0'; i++) *i = '\0';
        free (name);
        name = NULL;
        ip = -1;
    }
};

char* handleComLine (int argc, char* argv[], char** outFileName);

void writeCode (Text* codeFile, char* outStr, Stack<Tag>* tags, size_t* Ip);

void handleArg (const Text* codeFile, const int line, char* outStr, const unsigned char cmdNum, Stack<Tag>* tags, size_t* Ip);

void printReg (size_t* Ip, const size_t commandIp, char* outStr, unsigned char* cmdNum, const char* arg);

void printImm (size_t *Ip, const size_t commandIp, char* outStr, unsigned char* cmdNum, const elem_t value);

void addTag (Stack<Tag>* tags, const char* name, size_t* Ip);

void printTag (Stack<Tag>* tags, char* name, char* outStr, size_t* Ip, size_t commandIp, char cmdNum);