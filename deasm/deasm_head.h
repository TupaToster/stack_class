#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "D:\los_projectados\wtfProc\lib\globalization.h"
#include header(flog)
#include header(stack)
#include header(text)
#include header(cmd)

extern int tags[];

extern FILE* outFile;

/// @brief Macros for dumping processor
#define ProcDump(cpu)        \
    StackDump (cpu->stk);    \
    StackDump (cpu->funcIp); \
    ProcDumpInside (cpu);    \



/// @brief Struct to store data of a running soft cpu
struct Proc {

    char*   code            = NULL; ///< String of binary code
    size_t  codeSize        = 0;    ///< Size of code
    elem_t  regs[REG_SIZE]  = {0};  ///< Array of registers
    int     ip              = 0;    ///< Instruction pointer
    Stack   stk             = {};   ///< Stack of elem_t for work of process
    Stack   funcIp          = {};   ///< Stack of ip for function calls
    elem_t* ram             = NULL; ///< Array that represents RAM
};

/// @brief Handles command line arguments
/// @param argc Amount of args
/// @param argv Array of args
/// @return Name of file to read from
char* handleComLine (int argc, char* argv[]);

/// @brief Constructs a Proc var
/// @param cpu Ptr to var to construct
void ProcCtor (Proc* cpu);

/// @brief Destructs a Proc var
/// @param cpu Ptr to var to destruct
void ProcDtor (Proc* cpu);

/// @brief Dumps processor into logs_out
/// @param cpu Ptr to var to dump
void ProcDumpInside (Proc* cpu);

/// @brief Checks for file signature and proc version
/// @param cpu Ptr to var to dump
void checkFileSign (Proc* cpu);

/// @brief Disassembles code
/// @param cpu Ptr to var to run from
void ProcDeasmCode (Proc* cpu);

/// @brief Prints a elem_t argument
/// @param cpu Ptr to var to run from
void printValuePtrArg (Proc* cpu);

/// @brief Prints a tag
/// @param cpu Ptr to var to run from
void printIpArg (Proc* cpu);
