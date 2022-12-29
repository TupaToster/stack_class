#pragma once
#pragma GCC diagnostic ignored "-Wmultichar"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "flog.h"

#define Pow2After_size (1 << ((size_t) ceil (log2 (_size))))


#ifndef NDEBUG
#define dump(clas) (clas).dumpInside (#clas, __FILE__, __FUNCTION__, __LINE__) ///< NEW_STRUCT dump macros
#else
#define dump(clas) ;
#endif

template<typename ELEM_T>
class Vector {

    // constants
    static constexpr unsigned int       CANL      = 0xDEADBEEF; ///< Left cannary of a structure
    static constexpr unsigned int       CANR      = 0xD34DB33F; ///< Right cannary of a structure
    static constexpr unsigned char      POISON1   = 0xBD; ///< 1 byte Poison
    static constexpr unsigned short     POISON2   = 0xBDCF; ///< 2 byte Poison
    static constexpr unsigned int       POISON4   = 0xBADC0FEE; ///< 4 byte Poison
    static constexpr unsigned long long POISON8   = 0xBADC0FEEF04DED32; ///< 8 byte Poison
    static constexpr unsigned int       HASH_MULT = 37u; ///< Multiplier for hash
    static constexpr unsigned int       MAX_RANKS = 100;

    enum errorCodes {
        ok                   = 0,    ///< All ok
        POISON_ACCESS        = 1<<0, ///< One or more struct elements is poison
        BAD_CAN_L            = 1<<1, ///< Dead left cannary of structure
        BAD_CAN_R            = 1<<2, ///< Dead right cannary of structure
        BAD_data_CAN_L       = 1<<3, ///< Dead left cannary of  data
        BAD_data_CAN_R       = 1<<4, ///< Dead right cannary of data
        NULL_data_PTR        = 1<<5, ///< NULL ptr for data
        NULL_data_CAN_L_PTR  = 1<<6, ///< NULL ptr for left  data cannary
        NULL_data_CAN_R_PTR  = 1<<7, ///< NULL ptr for right data cannary
        WRONG_SIZE           = 1<<8, ///< Something wrong with size var
        POISONED_ERRCOD      = 1<<9, ///< Errcod variable is poisoned; Ususally means that struct has been destructed
        WRONG_HASH           = 1<<10 ///< Hash was changed without any changes from specified function
    };

    unsigned int   canL      = CANL; ///< left cannary of struct
    unsigned int   hash      = 0;    ///< hash value
    size_t         errCode   = ok;   ///< error code
    size_t         cap       = 0;
    size_t         size      = 0;
    ELEM_T*        data      = 0;
    unsigned int*  dataCanL  = NULL; ///< left cannary of data
    unsigned int*  dataCanR  = NULL; ///< right cannary of data
    unsigned int   canR      = CANR; ///< right cannary of struct

    void countHashSeg (void* left, void* right, unsigned int* multiplier) {

        assert (left != NULL);
        assert (right != NULL);
        assert (left < right);
        assert (multiplier != NULL);

        for (; left < right; left++) {

            hash += ((unsigned int) * (char*) left) * (*multiplier);
            *multiplier *= HASH_MULT;
        }
    }

    void logPrintErrors () {

        errCheck ();

        char names [11][40] = {};
        int iter = 0;
        if (errCode & POISON_ACCESS        ) strcpy (names[iter++], "\t\t[POISON_ACCESS       ]<br>");
        if (errCode & BAD_CAN_L            ) strcpy (names[iter++], "\t\t[BAD_CAN_L           ]<br>");
        if (errCode & BAD_CAN_R            ) strcpy (names[iter++], "\t\t[BAD_CAN_R           ]<br>");
        if (errCode & BAD_data_CAN_L       ) strcpy (names[iter++], "\t\t[BAD_data_CAN_L      ]<br>");
        if (errCode & BAD_data_CAN_R       ) strcpy (names[iter++], "\t\t[BAD_data_CAN_R      ]<br>");
        if (errCode & NULL_data_PTR        ) strcpy (names[iter++], "\t\t[NULL_data_PTR       ]<br>");
        if (errCode & NULL_data_CAN_L_PTR  ) strcpy (names[iter++], "\t\t[NULL_data_CAN_L_PTR ]<br>");
        if (errCode & NULL_data_CAN_R_PTR  ) strcpy (names[iter++], "\t\t[NULL_data_CAN_R_PTR ]<br>");
        if (errCode & POISONED_ERRCOD      ) strcpy (names[iter++], "\t\t[POISONED_ERRCOD     ]<br>");
        if (errCode & WRONG_HASH           ) strcpy (names[iter++], "\t\t[WRONG_HASH          ]<br>");
        if (iter == 0) flogprintf ( "\t\t[ok]<br>")
        else
            for (int i = 0; i < iter; i++) flogprintf ( "%s", names[i])

    }

    unsigned int countHash () {

        unsigned int multiplier = 1;
        hash = 0;

        countHashSeg (&canL, &canR, &multiplier);

        //if (dataCanL != NULL) countHashSeg (dataCanL, dataCanR, &multiplier);

        return hash;
    }

    bool verifyHash () {

        if (hash != countHash()) {

            errCode |= WRONG_HASH;
            return false;
        }
        return true;
    }

    size_t errCheck () {

        //checking for poison
        if (isPoison (&errCode  )   ) {

            errCode = POISONED_ERRCOD;
            return errCode;
        }

        verifyHash ();

        if (isPoison (&canL     ) or
            isPoison (&canR     ) or
            isPoison (&data     ) or
            isPoison (&dataCanL ) or
            isPoison ( dataCanL ) or
            isPoison (&dataCanR ) or
            isPoison ( dataCanR ) or
            isPoison (&hash     )   ) errCode |= POISON_ACCESS;

        //end of check

        if ( canL     != CANL      ) errCode |= BAD_CAN_L;
        if ( canR     != CANR      ) errCode |= BAD_CAN_R;
        if ( dataCanL == NULL      ) errCode |= NULL_data_CAN_L_PTR;
        else if (*dataCanL != CANL ) errCode |= BAD_data_CAN_L;
        if ( dataCanR == NULL      ) errCode |= NULL_data_CAN_R_PTR;
        else if (*dataCanR != CANR ) errCode |= BAD_data_CAN_R;
        if ( data     == NULL      ) errCode |= NULL_data_PTR;

        countHash ();

        return errCode;
    }

    public:

    Vector (unsigned int _size = 0) :
        canL (CANL), canR (CANR), hash(0), errCode (ok), cap (__max (4, ceil (log2 (_size)))), size (_size) {

        dataCanL = (unsigned int*) calloc (sizeof (ELEM_T) * cap + 2 * sizeof (unsigned int), 1);
        assert (dataCanL != NULL);
        data = (ELEM_T*) (dataCanL + 1);
        assert (data != NULL);
        dataCanR = (unsigned int*) (data + cap);
        assert (dataCanR != NULL);

       *dataCanL = CANL;
       *dataCanR = CANR;

        countHash();
    }

    unsigned int geterrCode () {

        return errCode;
    }

    size_t getSize () {

        return size;
    }

    size_t getCap () {

        return cap;
    }

    ELEM_T* getData () {

        return data;
    }

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

    void DTOR () {

        setPoison (&canL);
        setPoison (&canR);
        setPoison (&errCode);
        setPoison (&hash);
        setPoison (&size);
        setPoison (&cap);

        if (dataCanL != NULL) {

            setPoison (dataCanL);
            setPoison (dataCanR);
            dataCanL++;
            for (; (void*) data < (void*) dataCanR; data++) setPoison (data);
            free (dataCanL);
            setPoison (&dataCanL);
            setPoison (&data);
            setPoison (&dataCanR);
        }
    }

    void dumpInside (const char* name, const char* fileName, const char* funcName, size_t line) {

        errCheck ();

        flogprintf ("<pre>" "In file %s, function %s, line %llu, Vector named %s was dumped : <br>",
            fileName, funcName, line, name);

        flogprintf ("\t" "Errors : <br>");

        logPrintErrors ();

                                         flogprintf ( "\t" "hash     = %u (", hash);
        if      ( isPoison (&hash)     ) flogprintf ( "POISONED)<br>")
        else                             flogprintf ( "ok)<br>")

                                         flogprintf ( "\t" "canL     = 0x%X (", canL);
        if      ( isPoison (&canL)     ) flogprintf ( "POISONED)<br>")
        else if ( canL      == CANL    ) flogprintf ( "ok)<br>")
        else                            flogprintf ( "NOT_OK)<br>")

                                         flogprintf ( "\t" "canR     = 0x%X (", canR);
        if      ( isPoison (&canR)     ) flogprintf ( "POISONED)<br>")
        else if ( canR      == CANR    ) flogprintf ( "ok)<br>")
        else                             flogprintf ( "NOT_OK)<br>")

                                         flogprintf ( "\t" "size     = %d (", size);
        if      ( isPoison (&size)     ) flogprintf ( "POISONED)<br>")
        else if (size > cap            ) flogprintf ( ">CAP)<br>")
        else                             flogprintf ( "ok)<br>")

                                         flogprintf ( "\t" "cap      = %d (", cap);
        if      ( isPoison (&cap)      ) flogprintf ( "POISONED)<br>")
        else if (size > cap            ) flogprintf ( "&ltSIZE)<br>")
        else                             flogprintf ( "ok)<br>")

                                         flogprintf ( "\t" "dataCanL = 0x%X (", *dataCanL);
        if      (isPoison (dataCanL)   ) flogprintf ( "POISONED)<br>")
        else if (*dataCanL == CANL     ) flogprintf ( "ok)<br>")
        else                             flogprintf ( "NOT_OK)<br>")

                                         flogprintf ( "\t" "dataCanR = 0x%X (", *dataCanR);
        if      (isPoison (dataCanR)   ) flogprintf ( "POISONED)<br>")
        else if (*dataCanR == CANR     ) flogprintf ( "ok)<br>")
        else                             flogprintf ( "NOT_OK)<br>")

        if (dataCanL != NULL) {

            flogprintf ( "\t" "data[] :<br>");
            for (int i = 0; i < size; i++ ) {

                flogprintf ( "\t\t" "%*d : ", ceil (log10 (size)), i);
                flogprintf ( getFormat (ELEM_T), data[i]);
                flogprintf ( "<br>");
            }
        }

        flogprintf ("</pre><hr>\n");
        countHash ();
    }

    //vector part

    ELEM_T& operator[](int index) {

        assert (index < size);
        return data[index];
    }

    void push_back (ELEM_T val) {

        errCheck ();

        resize (size + 1);

        data[size - 1] = val;

        countHash ();
    }

    ELEM_T pop_back () {

        errCheck ();

        ELEM_T retVal = data[size - 1];

        resize (size - 1);

        countHash ();

        return retVal;
    }

    void resize (size_t _size) {

        errCheck ();

        if (_size > cap * 3 / 8 and _size <= cap) {

            size = _size;
            return;
        }

        printf ("lol");

        data = (ELEM_T*) calloc ( __max (4, Pow2After_size) * sizeof (ELEM_T) + 2 * sizeof (unsigned int), 1);
        assert (data != NULL);
        memcpy (data, dataCanL, __min (cap, __max ( Pow2After_size, 4)) * sizeof (ELEM_T) + sizeof (unsigned int));

        cap = __max (4, Pow2After_size);
        size = _size;

        free (dataCanL);

        dataCanL = (unsigned int*) data;
        data = (ELEM_T*) (dataCanL + 1);

        dataCanR = (unsigned int*) (data + cap);
        *dataCanR = CANR;

        countHash ();
    }

    void insert (size_t index, ELEM_T val) { //insert ON index

        if (index >= size) {

            push_back (val);
            return;
        }

        errCheck ();

        resize (size + 1);

        for (int i = size - 2; i > index; i--) {

            data[i + 1] = data[i];
        }

        data[index] = val;

        countHash ();
    }
};