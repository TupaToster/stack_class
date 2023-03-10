
#include <stdio.h>
#include <stdlib.h>
#include <typeinfo>
#include <time.h>
#include <string.h>

// DEFINE NDEBUG TO DISABLE LOGS

extern FILE* logOutf;

void flogIntern (const void* val, const char* varType, const char* varName, size_t varSize, const char* fileName, const char* funcName, size_t line);

const char* getFormatIntern (const char* varType);

#define getFormat(var) getFormatIntern (typeid (var).name ())


#define flog(a)                                                                                   \
            if (logOutf == NULL){                                                                 \
                logOutf = fopen ("logs_out.html", "a");                                           \
                setvbuf (logOutf, NULL, _IONBF, 0);                                               \
                fprintf (logOutf, "----------------------------------------\n"                    \
                "<style> body {padding: 25px; background-color: #252525; color: white; font-size: 25px;} </style>"\
                "Logging session at compiled time : %s %s\n\n", __TIME__, __DATE__);              \
            }                                                                                     \
            flogIntern (&a, typeid (a).name (), #a, sizeof (a), __FILE__, __FUNCTION__, __LINE__)

#define flogprintf(...)                                                                           \
    {if (logOutf == NULL){                                                                        \
                logOutf = fopen ("logs_out.html", "a");                                           \
                setvbuf (logOutf, NULL, _IONBF, 0);                                               \
                fprintf (logOutf, "----------------------------------------\n"                    \
                "<style> body {padding: 25px; background-color: #252525; color: white; font-size: 25px;} </style>"\
                "Logging session at compiled time : %s %s\n\n", __TIME__, __DATE__);              \
    }                                                                                             \
    fprintf (logOutf, __VA_ARGS__);}

#ifdef NDEBUG
#define flog(a) ;
#define flogFileInit ;
#endif