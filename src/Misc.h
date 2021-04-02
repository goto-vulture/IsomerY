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



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MISC_H */
