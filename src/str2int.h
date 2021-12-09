/**
 * @file str2int.h
 *
 * @brief Konvertierungsfunktion C-String -> int.
 *
 *  Created on: 07.03.2021
 *      Author: x86 / Gyps
 */

#ifndef STR2INT_H
#define INT2STR_H

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
 * @brief C-String zu long int konvertieren.
 *
 * Fehlerhafte Eingaben werden ueber den Rueckgabewert angezeigt.
 *
 * @param[out] out Pointer auf das Ausgabe-Objekt.
 * @param[in] input_string C-String Eingabe.
 * @param[in] base Basis der Zahl (Angabe des Zahlensystems).
 *
 * @return Fehlercode. 0, wenn alles in Ordnung war; ansonsten != 0
 */
extern enum str2int_errno str2int (long int* out, const char* input_string, const int base);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STR2INT_H */
