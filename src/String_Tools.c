/**
 * @file String_Tools.h
 *
 * @brief Hier sind einige allgemeine Tools fuer die Verwendung von C-Strings.
 *
 * @date: 07.03.2022
 * @author: am1
 */

#include "String_Tools.h"
#include <string.h>
#include <ctype.h>
#include "Misc.h"



//---------------------------------------------------------------------------------------------------------------------

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
)
{
    strncpy (to_lower_string, orig_string, to_lower_string_size);

    for (size_t current_char = 0;
            (current_char < strlen (orig_string)) && (current_char < to_lower_string_size);
            ++ current_char)
    {
        // Alle Zeichen der Zeichenkette in Kleinbuchstaben umwandeln.
        // Eine Ueberpruefung, ob das aktuelle Zeichen ein Grossbuchstabe ist, ist nicht notwendig, da tolower() diesen
        // Test bereits durchfuehrt.
        // if (isupper (orig_string [current_char]) /* == true */)
            to_lower_string [current_char] = (char) tolower (orig_string [current_char]);
    }

    // Nullterminierung im Zielspeicher garantieren
    to_lower_string [strlen (orig_string)] = '\0';

    return;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    // Wenn die Laenge der Zeichenketten nicht identisch sind, dann koennen sie - auch ohne Beachtung der Gross- und
    // Keinschreibung - nicht gleich sein !
    if (strlen (string_1) != strlen (string_2))
    {
        return -1;
    }

    char string_1_lowercase [255];
    char string_2_lowercase [255];
    memset (string_1_lowercase, '\0', sizeof (string_1_lowercase));
    memset (string_2_lowercase, '\0', sizeof (string_2_lowercase));

    // Alle alphabetischen Zeichen in Kleinbuchstaben konvertieren, damit spaeter ein Vergleich unabhaengig von der
    // Gross- und Kleinschreibung stattfinden kann
    String_To_Lower(string_1, string_1_lowercase, COUNT_ARRAY_ELEMENTS(string_1_lowercase));
    String_To_Lower(string_2, string_2_lowercase, COUNT_ARRAY_ELEMENTS(string_2_lowercase));

    // Vergleich mit den angepassten Zeichenketten durchfuehren
    // Es gibt auch die Funktion "strncasecmp()" die vorzeichenlos Zeichenketten vergleicht. Diese ist aber leider eine
    // GNU-Extension und daher nicht auf allen Systemen verfuegbar !
    return strncmp (string_1_lowercase, string_2_lowercase, strlen (string_1_lowercase));
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Die Anzahl eines bestimmten Zeichens in einer Zeichenkette ermitteln und zurueckgeben.
 */
extern uint_fast8_t                             // Anzahl der gesuchten Zeichen in der Zeichenkette
Count_Char_In_String
(
        const char* const restrict c_string,    // Zeichenkette, die durchsucht wird
        const size_t string_length,             // Laenge der Zeichenkette
        const char searched_char                // Zu suchendes Zeichen
)
{
    uint_fast8_t result = 0;

    for (size_t i = 0; i < string_length; ++ i)
    {
        if (c_string [i] == searched_char)
        {
            ++ result;
        }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Uebergebenes Zeichen X-Mal an die uebergebene Zeichenkette anhaengen. Die Funktion geht davon aus, dass genug
 * Speicher in der Zeichenkette fuer die Operation vorhanden ist.
 *
 * @param[in] str Zeichenkette, an der die Zeichen angehaengt werden
 * @param[in] character Zeichen, welches angehaengt werden soll
 * @param[in] times Wie oft wird das Zeichen angehaengt
 */
extern void
Append_X_Times_Char
(
        char* const str,
        const char character,
        const size_t times
)
{
    const char one_size_string [2] = { character, '\0' };

    for (size_t i = 0; i < times; ++ i)
    {
        strncat (str, one_size_string, 1);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------
