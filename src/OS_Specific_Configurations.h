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
#endif /* defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) */



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* OS_SPECIFIC_CONFIGURATIONS_H */
