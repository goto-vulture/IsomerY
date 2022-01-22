/**
 * @file CLI_Parameter.h
 *
 * @brief CLI-Parameter des Programms.
 *
 * Implementiert als globale Variablen, damit von ueberall ein Zugriff moeglich ist.
 *
 * @date 22.01.2022
 * @author: am1
 */

#ifndef CLI_PARAMETER_H
#define CLI_PARAMETER_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



extern const char* const GLOBAL_USAGES [];

extern const char* const GLOBAL_PROGRAM_DESCRIPTION;
extern const char* const GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION;



// Variablen fuer die geparsten CLI-Parameter
extern int GLOBAL_MAX_C_ATOMS_FOR_TESTS;



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* CLI_PARAMETER_H */
