/**
 * @file String_Tools.h
 *
 * @brief Hier sind einige allgemeine Tools fuer die Verwendung von C-Strings.
 *
 * @date: 07.03.2022
 * @author: am1
 */

#ifndef STRING_TOOLS_H ///< Include-Guard
#define STRING_TOOLS_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stddef.h>
#include <inttypes.h>



/**
 * @brief Eine Zeichenkette kopieren, in der nur Kleinbuchstaben aus der Original-Zeichenkette vorkommen.
 *
 * Asserts: N/A
 *
 * @param[in] orig_string Original-String
 * @param[out] to_lower_string Zielstring
 * @param[in] to_lower_string_size Laenge des Zielstrings
 */
extern void
String_To_Lower
(
        const char* const restrict orig_string,     // Originale Zeichenkette, die konvertiert werden soll
        char* const restrict to_lower_string,       // Zielspeicher fuer die konvertierte Zeichenkette
        const size_t to_lower_string_size           // Groesse des Zielspeichers
);

/**
 * @brief Vergleichen zweier C-Strings OHNE Beachtung der Gross- und Kleinschreibung.
 *
 * Asserts:
 *      strlen (string_1) == strlen (string_2)
 *
 * @param[in] string_1 1. Zeichenkette
 * @param[in] string_2 2. Zeichenkette
 */
extern int                                              // 0, falls die Zeichenketten uebereinstimmen, ansonsten != 0
Compare_Strings_Case_Insensitive
(
        const char* const restrict string_1,            // 1. C-String
        const char* const restrict string_2             // 2. C-String
);

/**
 * Die Anzahl eines bestimmten Zeichens in einer Zeichenkette ermitteln und zurueckgeben.
 */
extern uint_fast8_t                             // Anzahl der gesuchten Zeichen in der Zeichenkette
Count_Char_In_String
(
        const char* const restrict c_string,    // Zeichenkette, die durchsucht wird
        const size_t string_length,             // Laenge der Zeichenkette
        const char searched_char                // Zu suchendes Zeichen
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* STRING_TOOLS_H */
