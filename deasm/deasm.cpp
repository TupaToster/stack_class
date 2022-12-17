#include "deasm_head.h"

FILE* outFile = NULL;
int   tags[TAGS_SIZE] = {0};

int main (int argc, char* argv[]) {

    char* codeFileName = handleComLine (argc, argv);
    assert (codeFileName != NULL);

    Text codeFile = read_Text (codeFileName);

    memset (tags, -1, TAGS_SIZE * sizeof (int));

    Proc cpu = {};
    ProcCtor (&cpu);

    cpu.code     = codeFile.TextString;
    cpu.codeSize = codeFile.TextSize;

    checkFileSign (&cpu);

    ProcDeasmCode (&cpu);

    codeFileName = (char*) realloc (codeFileName, strlen(codeFileName) + strlen("_deasm.code") + 1);
    assert (codeFileName != NULL);

    codeFileName = strcat (codeFileName, "_deasm.code");
    assert (codeFileName != NULL);

    outFile = fopen (codeFileName , "w");
    assert (outFile != NULL);

    cpu.ip = 0;

    checkFileSign (&cpu);

    ProcDeasmCode (&cpu);

    ProcDtor (&cpu);
    killText (&codeFile);
    free (codeFileName);
}