#include "softCPU.h"

int main (int argc, char* argv[]) {

    CPU<elem_t> cpu;

    char* codeFileName = cpu.handleComLine (argc, argv);
    assert (codeFileName != NULL);

    Text codeFile = read_Text (codeFileName);

    cpu.code     = codeFile.TextString;
    cpu.codeSize = codeFile.TextSize;

    cpu.checkFileSign ();

    cpu.runCode ();

    cpu.DTOR ();
    killText (&codeFile);
    free (codeFileName);
}