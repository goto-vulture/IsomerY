/**
 * str2int.h
 *
 * Hier sind zwei Funktionen definiert, die fuer die Konvertierung von Zeichenketten in Ganzzahen sowie der
 * Konvertierung von Ganzzahlen in Zeichenketten verantwortlich sind.
 *
 *  Created on: 07.03.2021
 *      Author: x86 / Gyps
 */

#ifndef STR2INT_H
#define STR2INT_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stddef.h>



enum str2int_errno
{
    STR2INT_SUCCESS = 0,
    STR2INT_OVERFLOW,
    STR2INT_UNDERFLOW,
    STR2INT_INCONVERTIBLE
};

/**
 * C-String zu long int konvertieren.
 * Fehlerhafte Eingaben werden ueber den Rueckgabewert angezeigt.
 *
 * Asserts:
 *      N/A
 */
extern enum str2int_errno str2int (long int* out, const char* input_string, const int base);

/**
 * long int zu C-String konvertieren.
 *
 * Asserts:
 *      N/A
 */
extern void int2str (char* const output_string, const size_t output_string_size, const long int input);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STR2INT_H */
