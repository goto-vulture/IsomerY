/**
 * Misc.h
 *
 *  Created on: 14.03.2021
 *      Author: x86 / Gyps
 */

#ifndef MISC_H
#define MISC_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/**
 * Dieses Makro zaehlt die Elemente in einem Array.
 * ! Dies funktioniert nur dann, wenn die Groesse des Arrays bereits zur Uebersetzungszeit bekannt ist !
 * !!! Wenn dies nicht der Fall ist (wie z.B. bei Speicherbereiche, die durch malloc() angelegt wurden, dann liefert
 *     dieses Makro FALSCHE ERGEBNISSE !!!
 */
#ifndef COUNT_ARRAY_ELEMENTS
#define COUNT_ARRAY_ELEMENTS(array) ((sizeof (array)) / (sizeof (array [0])))
#else
#error "The macro \"COUNT_ARRAY_ELEMENTS\" is already defined !"
#endif /* COUNT_ARRAY_ELEMENTS */



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MISC_H */
