#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "../lib/globalization.h"
#include "../lib/cmd.h"
#include "../lib/stack.h"
#include "../lib/text.h"
#include "../lib/cmd.h"

#ifndef NDEBUG
#define dump(clas) (clas).dumpInside (#clas, __FILE__, __FUNCTION__, __LINE__) ///< NEW_STRUCT dump macros
#else
#define dump(clas) ;
#endif


template<typename ELEM_T>
struct CPU {

    static constexpr unsigned char      POISON1   = 0xBD; ///< 1 byte Poison
    static constexpr unsigned short     POISON2   = 0xBDCF; ///< 2 byte Poison
    static constexpr unsigned int       POISON4   = 0xBADC0FEE; ///< 4 byte Poison
    static constexpr unsigned long long POISON8   = 0xBADC0FEEF04DED32; ///< 8 byte Poison
    static constexpr double             EPS = 0.001;
    static constexpr int                REG_SIZE = 5;
    static constexpr int                RAM_SIZE = 121;
    //end of constants

    CPU () : code (NULL), ip (0), codeSize (0) {

        memset (regs, 0, REG_SIZE * sizeof (char));
        memset (ram, 0, RAM_SIZE * sizeof (char));
    }

    char*         code     = NULL; ///< String of binary code
    size_t        codeSize = 0;    ///< Size of code
    ELEM_T        regs[REG_SIZE]  = {0};  ///< Array of registers
    int           ip       = 0;    ///< Instruction pointer
    Stack<ELEM_T> stk;             ///< Stack of ELEM_T for work of process
    Stack<int>    funcIp;          ///< Stack of ip for function calls
    ELEM_T        ram[RAM_SIZE] = {0}; ///< Array that represents RAM

    template<typename varType>
    void setPoison (varType* var) {

        if (var == NULL) return;

        switch(sizeof (varType)) {
            case 1 : *((unsigned char*     ) var)      = POISON1; break;
            case 2 : *((unsigned short*    ) var)      = POISON2; break;
            case 4 : *((unsigned int*      ) var)      = POISON4; break;
            case 8 : *((unsigned long long*) var)      = POISON8; break;

            default:
                memset (var, POISON1, sizeof (varType));
            break;
        }
    }

    void dumpInside (const char* name, const char* filename, const char* funcName, size_t line) {

        flogprintf ("<pre>" "In file %s, function %s, line %llu, CPU named %s was dumped : <br>",
            filename, funcName, line, name);

        flogprintf ("\t" "codeSize = %llu (", codeSize);
        if (isPoison (&codeSize)) flogprintf ("POISONED)<br>")
        else flogprintf ("ok)<br>")

        flogprintf ("\t" "ip = %llu (", ip);
        if (isPoison (&ip)) flogprintf ("POISONED)<br>")
        else flogprintf ("ok)<br>")

        flogprintf ("regs[%d] = { ", REG_SIZE);
        for (int i = 0; i < REG_SIZE; i++) {
            flogprintf ( getFormat (regs[i]), regs[i]);
            if (i != REG_SIZE - 1) flogprintf (", ");
        }
        flogprintf (" }<br>");

        flogprintf ("ram[%d] = { ", RAM_SIZE);
        for (int i = 0; i < RAM_SIZE; i++) {

            flogprintf ( getFormat (ram[i]), ram[i]);
            if (i != RAM_SIZE - 1) flogprintf (", ");
        }
        flogprintf (" }<br>");

        dump (stk);
        dump (funcIp);

        flogprintf ("</pre>\n");

    }

    template<typename varType>
    bool isPoison (varType* var) {

        if (var == NULL) return 1;

        switch(sizeof (varType)) {

            case 1:
                if ( *( (unsigned char*     ) &var) == POISON1) return 1;
                else                                            return 0;
            break;

            case 2:
                if ( *( (unsigned short*    ) &var) == POISON2) return 1;
                else                                            return 0;
            break;

            case 4:
                if ( *( (unsigned int*      ) &var) == POISON4) return 1;
                else                                            return 0;
            break;

            case 8:
                if ( *( (unsigned long long*) &var) == POISON8) return 1;
                else                                            return 0;
            break;

            default:
                for (int i = 0; i < sizeof (varType); i++) {

                    if (*( ( (unsigned char*) &var) + i) != POISON1) return 0;
                }
            return 1;
            break;
        }
    }

    char* handleComLine (int argc, char* argv[]) {

        assert (argv != NULL);

        char* codeFileName = NULL;

        switch (argc) {

            case 1:

                printf ("Good usage of wtfproc.exe:\n"
                        "./wtfproc.exe fileName\n"
                        "fileName - name of file to run (usually a .wtf)");
                exit (-1);
            break;

            case 2:

                codeFileName = (char*) calloc (strlen (argv[1]) + 1, sizeof (char));

                assert (codeFileName != NULL);

                strcpy (codeFileName, argv[1]);
            break;

            default:

                printf ("Wrong arg");
                exit(-1);
            break;
        }

        return codeFileName;
    }

    void checkFileSign () {

        assert (code != NULL);

        assert (code[0] == signa[0] and code[1] == signa[1]);
        assert (code[2] == signa[2] and code[3] == signa[3]);

        ip = 4;
    }

    bool cmp (const ELEM_T a, const ELEM_T b) {

        return fabs (a - b) < EPS;
    }

    void runCode () {

        // printf ("lol");
        ELEM_T* cmdArg = NULL;

        while (ip < codeSize) {

            // printf ("%d ", ip);
            switch (code[ip++] & MASK_CMD) {

                #define DEF_CMD(name, num, arg, code)\
                    case CMD_##name:\
                        if (arg != 0) cmdArg = getArg (arg);\
                        assert (cmdArg != NULL);\
                        code\
                    break;

                #include "../lib/cmd.h"

                #undef DEF_CMD

                default:

                    printf ("Wrong command");
                    dump (*this);
                    assert (0);
                break;
            }
        }
    }


    ELEM_T* getArg (int argType) {

        assert (code != NULL);
        assert (ram != NULL);
        assert (regs != NULL);

        char command = code[ip - 1];

        ELEM_T* retVal   = &regs[r0x];
        ELEM_T  immConst = 0;

        *retVal = 0;

        if (command & MASK_REG) {

            if (argType != PTR_ARG) *retVal += regs[code[ip]];
            else retVal = regs + code[ip];
            ip += sizeof (char);
        }

        if ((command & MASK_IMM) or (argType == IP_ARG)) {

            *retVal += *(ELEM_T*)(code + ip);
            immConst = *(ELEM_T*)(code + ip);
            ip += sizeof (ELEM_T);
        }

        if (command & MASK_RAM) {

            int ind = *retVal;
            *retVal -= immConst;
            retVal = ram + ind;
        }

        return retVal;
    }

    void DTOR () {

        for (int i = 0; i < codeSize; i++) setPoison (code + i);
        for (int i = 0; i < RAM_SIZE; i++) setPoison (ram + i);
        setPoison (&codeSize);
        setPoison (&ip);
        stk.DTOR ();
        funcIp.DTOR ();
        for (int i = 0; i < REG_SIZE; i++) setPoison (regs + i);
        free (code);
    }
};