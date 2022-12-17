#include "asm_head.h"

char* handleComLine (int argc, char* argv[], char** outFileName) {

    assert (argv != NULL);
    assert (outFileName != NULL);

    char* fileName = NULL;

    switch (argc) {

        case 1:

            printf ("Usage hint: ./asm.exe fileName [-o outFileName]\n"
                    "fileName - name of file to compile (commonly .codeFile)\n"
                    "-o - optional key to set name of output .wtf file\n"
                    "outFileName - name of output file with -o (set to a.wtf by default)\n");
            return NULL;
        break;

        case 2:

            fileName = strdup (argv[1]);
            assert (fileName != NULL);
        break;

        case 4:

            fileName = strdup (argv[1]);
            assert (fileName != NULL);


            if (strcmp (argv[2], "-o")) {

                printf ("Wrong args");
                return NULL;
            }

            *outFileName = strdup (argv[3]);
            assert (*outFileName != NULL);
        break;

        default:

            printf ("Wrong command line args");
            return NULL;
        break;
    }

    if (*outFileName == NULL) *outFileName = strdup ("a.wtf");
    assert (*outFileName != NULL);

    return fileName;
}

void writeCode (Text* codeFile, char* outStr, Stack<Tag>* tags, size_t* Ip) {

    assert (codeFile != NULL);
    assert (tags != NULL);
    assert (Ip != NULL);

    if (outStr != NULL) strcpy (outStr + *Ip, signa);

    *Ip += strlen (signa);

    for (int i = 0; i < codeFile->stringCnt; i++) {

        char cmd[CONST_STR_LEN] = "";

        sscanf (codeFile->Lines[i].begin, "%s", cmd);

        if (cmd[0] == '#') continue;

        #define DEF_CMD(name, num, arg, code)\
            if (strcmp (cmd, #name) == 0) {\
                if (arg == 0) {\
                    if (outStr != NULL) outStr[*Ip] = num;\
                    ++*Ip;\
                }\
                else {\
                    handleArg (codeFile, i, outStr, num, tags, Ip);\
                }\
            }\
            else

        #include "../lib/cmd.h"

        #undef DEF_CMD

        if (cmd [strlen (cmd) - 1] == ':') {

            addTag (tags, cmd, Ip);
        }
        else {

            printf ("Wrong command at line %d : <%s>" "\n", i+1, codeFile->Lines[i].begin);
            Errors++;
        }
    }
}

void handleArg (const Text* code, const int line, char* outStr, unsigned char cmdNum, Stack<Tag>* tags, size_t* Ip) {

    assert (code != NULL);
    assert (tags != NULL);
    assert (Ip != NULL);

    char arg1[CONST_STR_LEN] = "";
    char arg2[CONST_STR_LEN] = "";

    int it1 = 0;

    size_t commandIp = *Ip;
    *Ip += 1;

    elem_t value = 0;

    if (sscanf (code->Lines[line].begin, "%s %[^+\n\r ] + %[^]\n\r ]", arg1, arg1, arg2) < 2) {

        printf ("Wrong arguments in line %d : %s" "\n", line + 1, code->Lines[line].begin);
        Errors++;
        return;
    }

    if (arg1[it1] == ':') {

        printTag (tags, arg1 + it1 + 1, outStr, Ip, commandIp, cmdNum);
        return;
    }

    if (arg1[it1] == '[') {

        cmdNum |= MASK_RAM;
        it1++;
    }

    if (arg1[it1] == 'r' and arg1[it1 + 2] == 'x')
        printReg (Ip, commandIp, outStr, &cmdNum, arg1 + it1);

    if (sscanf (arg1 + it1, getFormat (elem_t), &value) == 1)
        printImm (Ip, commandIp, outStr, &cmdNum, value);

    if (arg2[0] == 'r' and arg2[2] == 'x') {

        if (cmdNum & MASK_REG) {

            printf ("Wrong arguments in line %d : %s" "\n", line + 1, code->Lines[line].begin);
            Errors++;
            return;
        }

        printReg (Ip, commandIp, outStr, &cmdNum, arg2 + 1);
    }

    if (sscanf (arg2, getFormat (elem_t), &value) == 1) {

        if (cmdNum & MASK_IMM) {

            printf ("Wrong arguments in line %d : %s" "\n", line + 1, code->Lines[line].begin);
            Errors++;
            return;
        }

        printImm (Ip, commandIp, outStr, &cmdNum, value);
    }

}

void printReg (size_t* Ip, const size_t commandIp, char* outStr, unsigned char* cmdNum, const char* arg) {

    *cmdNum |= MASK_REG;

    if (outStr != NULL) {

        outStr[commandIp] = *cmdNum;
        outStr[*Ip] = arg[1] - 'a' + 1;
    }

    *Ip += 1;
}

void printImm (size_t *Ip, const size_t commandIp, char* outStr, unsigned char* cmdNum, const elem_t value){

    *cmdNum |= MASK_IMM;

    if (outStr != NULL) {

        outStr[commandIp] = *cmdNum;
        if (*cmdNum & MASK_RAM) *(int*) (outStr + *Ip) = (int) value;
        else  *(elem_t*) (outStr + *Ip) = value;
    }

    *Ip += sizeof (elem_t);
}

void printTag (Stack<Tag>* tags, char* name, char* outStr, size_t* Ip, size_t commandIp, char cmdNum) {

    if (outStr != NULL) outStr[commandIp] = cmdNum;

    for (int i = 0; i < tags->size; i++) {

        if (strcmp (tags->data[i].name, name) == 0) {

            if (outStr != NULL) *(int*)(outStr + *Ip) = tags->data[i].ip;
            *Ip += sizeof (elem_t);
            return;
        }
    }

    if (outStr != NULL) *(int*)(outStr + *Ip) = -1;
    *Ip += sizeof (elem_t);
}

void addTag (Stack<Tag>* tags, const char* name, size_t* Ip) {

    for (int i = 0; i < tags->size; i++)
        if (tags->data[i].ip == (int) *Ip)
            return;

    tags->push (Tag (name, (int)*Ip));
    tags->data[tags->size - 1].name[strlen (tags->data[tags->size - 1].name) - 1] = '\0';
    dump (*tags);
}
