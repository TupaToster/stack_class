/*! \mainpage Index page

    \section Intorduction

    This is a soft-cpu project with it's own asm language and hand written function to work with text and a stack to store data during it's work.

    \section ASM

    Language consists of commands: push, pop, add, sub, mult, div, out. dump, hlt, pop, jmp, ja, jae, jb, jbe, je, jne, call, ret, video, in. More info on those commands can
    be seen in cmd.h, i.e. how they work and what they do.
*/

#ifndef GENERAL_CMD
#define GENERAL_CMD

#define PUSH(val) stk.push (val)
#define POP stk.pop ()

#define signa "CP03" ///< signature of a fine file

#define TAGS_SIZE 512

#define VALUE_ARG 1
#define PTR_ARG   2
#define IP_ARG    3

#define TEMP_STR_LEN 100

#define DEF_CMD(name, num, arg, code)

typedef double elem_t;

#include <string.h>
#include <assert.h>

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

#else ///< Template for auto code gen starts here

#define DEF_JMP(name, num, arg, condition)\
                                          \
    DEF_CMD (name, num, arg, {            \
                                          \
        ELEM_T scd = POP;                 \
        ELEM_T fst = POP;                 \
        if (condition) {                  \
                                          \
            ip = (int) *cmdArg;      \
        }                                 \
    })

DEF_CMD (push, 1, VALUE_ARG, {

    PUSH (*cmdArg);
})

DEF_CMD (add, 2, 0, {

    PUSH (POP + POP);
})

DEF_CMD (sub, 3, 0, {

    ELEM_T temp = POP;
    PUSH (POP - temp);
})

DEF_CMD (mult, 4, 0, {

    PUSH (POP * POP);
})

DEF_CMD (div, 5, 0, {

    ELEM_T temp = POP;
    PUSH (POP / temp);
})

DEF_CMD (out, 6, 0, {

    printf (getFormat (ELEM_T), POP);
    printf ("\n");
})

DEF_CMD (dump, 7, 0, {

    dump (*this);
})

DEF_CMD (hlt, 0, 0, {

    exit (0);
})

DEF_CMD (pop, 8, PTR_ARG, {

    *cmdArg = POP;
})

DEF_JMP (jmp, 9, IP_ARG, true)

DEF_JMP (jb, 10, IP_ARG, fst < scd)

DEF_JMP (jbe, 11, IP_ARG, fst < scd or cmp (fst, scd))

DEF_JMP (ja, 12, IP_ARG, fst > scd)

DEF_JMP (jae, 13, IP_ARG, fst > scd or cmp (fst, scd))

DEF_JMP (je, 14, IP_ARG, cmp (fst, scd))

DEF_JMP (jne, 15, IP_ARG, !cmp (fst, scd))

DEF_CMD (call, 16, IP_ARG, {

    funcIp.push (ip);
    ip = (size_t) *cmdArg;
})

DEF_CMD (ret, 17, 0, {

    ip = funcIp.pop ();
})

DEF_CMD (video, 18, 0, {

    for (int i = 0; i < 10; i++) {

        for (int j = 0; j < 10; j++) {

            if (ram[10*i + j] == 0) printf (".");
            else printf ("*");
        }
        printf ("\n");
    }
})

DEF_CMD (in, 19, 0, {

    printf ("Insert number to push: ");
    ELEM_T val = 0;
    scanf (getFormat (ELEM_T), &val);
    PUSH (val);
})

DEF_CMD (sqrt, 20, 0, {

    PUSH (sqrt (POP));
})

#undef DEF_JMP

#endif