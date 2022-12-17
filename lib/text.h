#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <sys\stat.h>
#include <ctype.h>

/// @brief Struct to store Lines of Text in format [begin, end)
struct Line {

    char* begin = NULL; ///< Ptr to the beginning
    char* end   = NULL; ///< Ptr to the end
};

/// @brief Stores Text in format of string and array of Line structures
struct Text {

    char* TextString = NULL; ///< char* that points to the beginning of a Text
    size_t TextSize = 0;     ///< size_t that declares sizeof Text in chars
    Line* Lines = NULL;      ///< ptr to a Line array that stores Text in a format of Lines
    size_t stringCnt = 0;    ///< size_t that indicates amount of Lines in Text
};

void initText (Text* target);

void initLine (Line* target);

void killText (Text* target);

void killLine (Line* target);

size_t get_size (const char* filename);

void bufferize (const char* filename, Text* writeTo);

Text read_Text (const char* filename);

void cntLines (Text* writeTo);

void separateLines (Text* writeTo);