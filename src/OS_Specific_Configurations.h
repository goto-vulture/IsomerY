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
