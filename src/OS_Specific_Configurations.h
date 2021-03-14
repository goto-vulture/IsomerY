/**
 * OS_Specific_Configurations.h
 *
 *  Created on: 14.03.2021
 *      Author: x86 / Gyps
 */

#ifndef OS_SPECIFIC_CONFIGURATIONS_H
#define OS_SPECIFIC_CONFIGURATIONS_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stdint.h>
#include <limits.h>
#include <time.h>



/**
 * Die static_asserts funktionieren aktuell nur unter Linux
 */
#if defined(__linux__) || defined(unix) || defined(__unix__) || defined(__unix)
// Ist auf der Zielplattform ein Byte genau 8 Bit gross ?
// Wenn nicht dann wird die Kompilierung nicht ausgefuehrt, da alle verwendeten Mechanismen davon ausgehen das dies der
// Fall ist !
static_assert (CHAR_BIT == 8,               "A char needs to be exact eight bit !");

// Ein int / unsigned int / size_t muss mindestens 4 Byte gross sein ! Bei Systemen wo dies nicht der Fall ist, ist
// i.d.R. ohne groesseren Aufwand kaum zu arbeiten.
static_assert (sizeof (int) >= 4,           "The type \"int\" needs to be at least 4 byte !");
static_assert (sizeof (unsigned int) >= 4,  "The type \"unsigned int\" needs to be at least 4 byte !");
static_assert (sizeof (size_t) >= 4,        "The type \"size_t\" needs to be at least 4 byte !");

// Weitere Variable(n), die mindestens 4 Byte gross sein sollte(n)
static_assert (sizeof (time_t) >= 4,        "The type \"time_t\" needs to be at least 4 byte !");
static_assert (sizeof (clock_t) >= 4,       "The type \"clock_t\" needs to be at least 4 byte !");

// Der Standard schreibt vor, dass SIZE_MAX mindestens 65535 betragen muss. Fuer einige Operationen (insb. calloc (),
// malloc () und Co.) ist dies hier aber zu wenig ! Daher muss sichergestellt sein, dass SIZE_MAX eine akzeptable
// Groesse besitzt.
// I.d.R. ist SIZE_MAX (2^32) - 1 oder (2^64) - 1
static_assert (SIZE_MAX >= UINT32_MAX,      "SIZE_MAX needs to be at least 2^32 - 1 !");
#endif /* defined(__linux__) || defined(unix) || defined(__unix__) || defined(__unix) */



/**
 * Konfigurationen, wenn man sich auf einem Windows-System befindet
 */
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    // Eine Konstante definieren, dass man sich auf einem Windows-System befindet
    #ifndef WINDOWS
        #define WINDOWS
    #else
        #error "The macro \"WINDOWS\" is already defined !"
    #endif /* WINDOWS */

    // Damit es unter Windows bei dem Formatstring fuer size_t zu keiner Warnung kommt, wird diese Konstruktion
    // verwendet. PRI_SIZET wird mit einem MinGW spezifischen Formatstring ersetzt, der die passende Groesse fuer
    // ein size_t Objekt bereitstellt.
    #ifdef _WIN64
        #ifndef PRI_SIZET
            #define PRI_SIZET PRIu64
        #else
            #error "The macro \"PRI_SIZET\" is already defined !"
        #endif /* PRI_SIZET */
    #else
        #ifndef PRI_SIZET
            #define PRI_SIZET PRIu32
        #else
            #error "The macro \"PRI_SIZET\" is already defined !"
        #endif /* PRI_SIZET */
    #endif
#else
    // Unter anderen Betriebssystemen ist kein spezieller Formatstring fuer size_t notwendig, also wird der normale
    // (zu) verwendet.
    #ifndef PRI_SIZET
        #define PRI_SIZET "zu"
    #else
        #error "The macro \"PRI_SIZET\" is already defined !"
    #endif /* PRI_SIZET */
#endif /* defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) */



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OS_SPECIFIC_CONFIGURATIONS_H */
