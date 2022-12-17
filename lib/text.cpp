#include "Text.h"

/*!
    \brief Initializes a Line type var
    \param target Line* to var to init
*/
void initLine(Line* target) {

    target->begin = NULL;
    target->end = NULL;
}

/*!
    \brief Deinitializes a Line type var
    \param target Line* to var to kill
*/
void killLine(Line* target) {

    target->begin = NULL;
    target->end   = NULL;
}


/*!
    \brief Initializes a Text type var
    \param target Text* to var to init
*/
void initText(Text* target) {

    target->TextString  = NULL;
    target->TextSize    = 0;
    target->stringCnt   = 0;
    target->Lines       = NULL;
}

/*!
    \brief Deinitializes a Text type var
    \param target Text* to var to kill
*/
void killText (Text* target) {

    free (target->TextString);
    free (target->Lines);
    target->TextSize  = 0;
    target->stringCnt = 0;
    return;
}

/// @brief counts amount of Lines in a Text
/// @param src Text* to wite to and read from
void cntLines (Text* src) {

    assert (src != NULL);

    for (size_t i = 0; i < src->TextSize; i++) {

        if (src->TextString[i] == '\n') src->stringCnt++;
    }
    src->stringCnt++;
}


/*!
    \brief Returns size of file in chars
    \param filename name of file to measure
    \return sizeof file in chars
*/
size_t get_size (const char* filename) {

    assert (filename != NULL);

    struct stat temp = {};

    stat (filename, &temp);

    return temp.st_size;
}

/// @brief Buffirezes file
/// @param filename name of file to bufferize
/// @param writeTo ptr to struct Text to write buffer and buffer length to
void bufferize (const char* filename, Text* writeTo) {

    writeTo->TextSize = get_size (filename);

    writeTo->TextString = (char*) calloc (writeTo->TextSize + 2, sizeof (char));
    assert (writeTo->TextString != NULL);

    writeTo->TextString++;

    FILE* fIn = fopen (filename, "rb");
    assert (fIn != NULL);

    assert (fread (writeTo->TextString, sizeof (char), writeTo->TextSize, fIn) != 0);

    writeTo->TextString[writeTo->TextSize] = '\0';

    fclose (fIn);

}

/*!
    \brief Fills Text type var with input from file
    \param filename name of file to fill from
    \return Text struct full of stuff

    Fills Text struct with:
    TextString - whole Text of a file
    Lines - array of Lines of file of type [begin, end)
    stringCnt - amount of strings in file
    TextSize - sizeof Text (by get_size)
*/
Text read_Text (const char* filename) {

    assert (filename != NULL);

    Text retVal = {};
    initText (&retVal);

    bufferize (filename, &retVal);

    cntLines (&retVal);

    retVal.Lines = (Line*) calloc (retVal.stringCnt + 1, sizeof (Line));
    assert (retVal.Lines != NULL);


    separateLines (&retVal);

    return retVal;
}

/// @brief separates Lines in a Text
/// @param writeTo Text* to write to and read from
void separateLines (Text* writeTo) {

    assert (writeTo != NULL);
    assert (writeTo->Lines != NULL);

    size_t Line_iter = 0;

    for (size_t i = 0; i < writeTo->TextSize; i++) {

        if (writeTo->Lines[Line_iter].begin == NULL) writeTo->Lines[Line_iter].begin = writeTo->TextString + i;

        if (writeTo->TextString[i] == '\n') {

            writeTo->Lines[Line_iter].end = writeTo->TextString + i + 1;
            Line_iter++;
        }
    }

    writeTo->Lines[Line_iter].end = writeTo->TextString + writeTo->TextSize;
}