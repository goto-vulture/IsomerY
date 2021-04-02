/**
 * Alkane_Info_Constitutional_Isomer.h
 *
 *  Created on: 07.03.2021
 *      Author: x86 / Gyps
 */

#ifndef ALKANE_INFO_CONSTITUTIONAL_ISOMER_H
#define ALKANE_INFO_CONSTITUTIONAL_ISOMER_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/**
 * Maximale Anzahl an C-Atomen, die das Programm unterstuetzt.
 * Die Algorithmen besitzen an sich kein Limit. Es wurde aber eine obere Grenze gewaehlt, damit die Speicherverwaltung
 * einfacher wird.
 */
#ifndef MAX_NUMBER_OF_C_ATOMS
    #define MAX_NUMBER_OF_C_ATOMS 40u
#else
    #error "The macro \"MAX_NUMBER_OF_C_ATOMS\" is already defined !"
#endif /* MAX_NUMBER_OF_C_ATOMS */

/**
 * Anzahl an Zahlenwoerter (Zahlenpraefixe), die im Programm hinterlegt sind.
 */
#ifndef NUMBER_OF_NUMBER_WORDS
    #define NUMBER_OF_NUMBER_WORDS 10u
#else
    #error "The macro \"NUMBER_OF_NUMBER_WORDS\" is already defined !"
#endif /* NUMBER_OF_NUMBER_WORDS */

/**
 * Anzahl an Alkanwoerter, die im Programm hinterlegt sind.
 */
#ifndef NUMBER_OF_ALKAN_WORDS
    #define NUMBER_OF_ALKAN_WORDS 40u
#else
    #error "The macro \"NUMBER_OF_ALKAN_WORDS\" is already defined !"
#endif /* NUMBER_OF_ALKAN_WORDS */

/**
 * Anzahl an Substituenten, die im Programm hinterlegt sind.
 * Die Substituenten wurden erzeugt indem die letzten beiden Zeichen der Alkan-Woerter durch "yl" ersetzt werden !
 */
#ifndef NUMBER_OF_ALKYL_WORDS
    #define NUMBER_OF_ALKYL_WORDS 40u
#else
    #error "The macro \"NUMBER_OF_ALKYL_WORDS\" is already defined !"
#endif /* NUMBER_OF_ALKYL_WORDS */



#include <inttypes.h>



// Anzahl der Konsitutionsisomere (Strukturisomere) bei gegebener Anzahl an C-Atomen
extern const uint_fast64_t NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [MAX_NUMBER_OF_C_ATOMS];

// Begriffe, die fuer die Erstellung des IUPAC-Namens erforderlich sind
extern const char* NUMBER_WORDS [NUMBER_OF_NUMBER_WORDS];
extern const char* ALKAN_WORDS [NUMBER_OF_ALKAN_WORDS];
extern const char* ALKYL_WORDS [NUMBER_OF_ALKYL_WORDS];



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_INFO_CONSTITUTIONAL_ISOMER_H */
