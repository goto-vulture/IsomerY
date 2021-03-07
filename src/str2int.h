/**
 * str2int.h
 *
 *  Created on: 07.03.2021
 *      Author: x86
 */

#ifndef STR2INT_H
#define STR2INT_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



enum str2int_errno
{
    STR2INT_SUCCESS = 0,
    STR2INT_OVERFLOW,
    STR2INT_UNDERFLOW,
    STR2INT_INCONVERTIBLE
};

/**
 * C-String zu long int konvertieren.
 */
extern enum str2int_errno str2int (long int* out, const char* input_string, const int base);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STR2INT_H */
