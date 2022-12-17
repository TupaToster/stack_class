#include "asm_head.h"

int Errors = 0;

int main (int argc, char* argv[]) {

    char* outFileName = NULL;
    char* inFileName = handleComLine (argc, argv, &outFileName);
    assert (inFileName != NULL);
    assert (outFileName != NULL);

    Text codeFile = read_Text (inFileName);
    size_t Ip = 0;
    Stack<Tag> tags;

    char* outStr = NULL;

    do {

        writeCode (&codeFile, outStr, &tags, &Ip);
        if (Errors) {

            printf ("Found %d errors. Stopping assembly pre write", Errors);
            break;
        }

        outStr = (char*) calloc (Ip, sizeof (char));
        assert (outStr != NULL);

        Ip = 0;

        writeCode (&codeFile, outStr, &tags, &Ip);
        if (Errors) {

            printf ("Found %d errors. Stopping assembly post write", Errors);
            break;
        }

        FILE* outFile = fopen (outFileName, "wb");
        assert (outFile != NULL);

        fwrite (outStr, Ip, sizeof (char), outFile);
        fclose (outFile);

    } while (false);

    killText (&codeFile);
    tags.DTOR ();
    free (outStr);
    free (inFileName);
    free (outFileName);
}