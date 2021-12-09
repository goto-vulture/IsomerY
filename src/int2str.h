/**
 * @file int2str.h
 *
 * @brief Konvertierungsfunktion C-String -> int.
 *
 *  Created on: 09.12.2021
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



enum int2str_errno
{
    INT2STR_SUCCESS = 0,
    INT2STR_INCONVERTIBLE
};




/**
 * @brief long int zu C-String konvertieren.
 *
 * Fehlerhafte Eingaben werden ueber den Rueckgabewert angezeigt.
 *
 * @param[out] output_string C-String fuer die Ausgabe
 * @param[in] output_string_size Maximale Groesse des Ausgabestrings.
 * @param[in] input Integer, der konvertiert wird.
 *
 * @return Fehlercode. 0, wenn alles in Ordnung war; ansonsten != 0
 */
extern enum int2str_errno int2str (char* const output_string, const size_t output_string_size, const long int input);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STR2INT_H */
