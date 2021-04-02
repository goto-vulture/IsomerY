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

/**
 * Dieses Makro soll die Erstellung einer Zeichenkettendarstellung von Objekten vereinfachen.
 * Bei der Konvertierung ist das Procedere immer das gleiche. Nur das Stringliteral unterscheidet sich. Daher bietet
 * sich dafuer ein Makro an.
 * Natuerlich wird dadurch vorausgesetzt, dass die Variablen sowie das Goto-Label vorher korrekt erzeugt wurden. Dieses
 * Makro nimmt nur eine Zeichenkette entgegen - KEINEN Formatstring mit weiteren Parametern.
 *
 * => Fuer die Verwendung eines Formatstrings: Siehe das Makro "TO_STRING_HELPER_VA_ARGS"
 */
#ifndef TO_STRING_HELPER
#define TO_STRING_HELPER(new_string)                                                                    \
        if (remaining_memory == 0) { goto no_remaining_memory; }                                        \
        used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, new_string);   \
        next_free_byte += used_char;                                                                    \
        remaining_memory -= used_char;
#else
#error "The macro \"TO_STRING_HELPER\" is already defined !"
#endif /* TO_STRING_HELPER */

/**
 * Dieses Makro soll die Erstellung einer Zeichenkettendarstellung von Objekten vereinfachen.
 * Diesmal mit einem Formatstring.
 *
 * Diese Unterteilung ist - leider - notwendig, da bei einem Makro, welches __VA_ARGS__ verwendet, ein Kommata zu viel
 * erzeugt wird, wenn __VA_ARGS__ leer ist.
 * Es gibt zwar compilerspezifische Erweiterungen, die dies unterbinden. Eine standardkonforme Loesung existiert aber
 * nicht !
 */
#ifndef TO_STRING_HELPER_VA_ARGS
#define TO_STRING_HELPER_VA_ARGS(format_string, ...)                                                                    \
        if (remaining_memory == 0) { goto no_remaining_memory; }                                                        \
        used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, format_string, __VA_ARGS__);   \
        next_free_byte += used_char;                                                                                    \
        remaining_memory -= used_char;
#else
#error "The macro \"TO_STRING_HELPER_VA_ARGS\" is already defined !"
#endif /* TO_STRING_HELPER_VA_ARGS */



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
