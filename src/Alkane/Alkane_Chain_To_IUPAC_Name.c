/**
 * Alkane_Chain_To_IUPAC_Name.c
 *
 *  Created on: 23.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane_Chain_To_IUPAC_Name.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdbool.h>
#include "Alkane_Info_Constitutional_Isomer.h"
#include "../str2int.h"
#include "../Print_Tools.h"
#include "../Misc.h"





/**
 * Vergleichsfunktion fuer die qsort-Funktion.
 * Alphabetisch aufsteigende Sortierung der Namensfragmente OHNE Beachtung ggf. vorhandener Zahlenpraefixe !
 */
static int Cmp_Name_Fragments (const void* a, const void* b);

/**
 * Vergleichsfunktion fuer die qsort-Funktion.
 * Einfache nummerisch aufsteigende Sortierung.
 */
static inline int Cmp_Position_Information (const void* a, const void* b);

/**
 * Statusinformationen fuer das Erzeugen des IUPAC-Namen aus den Chain-Objketen. In dieser Struktur sind alle
 * Informationen, die fuer diesen Prozess benoetigt werden.
 */
struct State_Information
{
    char* iupac_name;                       // Speicher fuer den IUPAC-String
    size_t iupac_name_length;               // Maximale Laenge, die der IUPAC-Name annehmen kann (Groesse des
                                            // reservierten Speichers)
    struct Alkane* alkane;                  // Alkan-Objekt (enthaelt die Chain-Objekte)

    uint_fast8_t current_index;             // Aktueller Chain-Index
    uint_fast8_t current_nesting_depth;     // Aktuelle Verschachtelungstiefe
    uint_fast8_t last_alkyl_word_inserted;  // Letzter Chain-Index, bei dem ein Alkyl-Wort angebracht wurde
};

// Zustandsfunktionen fuer die Erzeugung des IUPAC-Namen
static void Next_Chain (struct State_Information* const restrict state);
static void Try_To_Merge_Chains (struct State_Information* const restrict state);
static void Insert_Numbers (struct State_Information* const restrict state);
static void Insert_Alkylword (struct State_Information* const restrict state);
static void Down_In_Nesting (struct State_Information* const restrict state);
static void Up_In_Nesting (struct State_Information* const restrict state);

//=====================================================================================================================

/**
 * Diese Funktion konvertiert die zuvor erzeugten Chain-Objekte in den IUPAC-Namen.
 * Intern arbeitet dieser Prozess mit zustandsbasierten statischen Funktionen.
 */
void
Chain_To_IUPAC
(
        char* const restrict iupac_name,        // Speicher fuer den IUPAC-Namen
        const size_t iupac_name_length,         // Groesse des Speichers, welches fuer den IUPAC-Namen reserviert wurde
        struct Alkane* const restrict alkane    // Alkan-Objekt (enthaelt die Chain-Objekte)
)
{
    memset (iupac_name, '\0', iupac_name_length * sizeof (char));

    struct State_Information state_information =
    {
            .iupac_name         = iupac_name,
            .iupac_name_length  = iupac_name_length,
            .alkane             = alkane,

            .current_index              = 0,
            .current_nesting_depth      = 0,
            .last_alkyl_word_inserted   = 0
    };


    // Nur wenn es mehr als einen Ast gibt, gibt es Aeste, die nicht zur Hauptkette gehoeren
    // Der erste Ast ist immer die Hauptkette !
    if (alkane->next_free_chain > 1)
    {
        // Erzeugung des IUPAC-Namen fuer die Aeste - die nicht zur Hauptkette gehoeren - starten
        Next_Chain (&state_information);

        char temp_fragments [20] [iupac_name_length];
        memset (temp_fragments, '\0', sizeof (temp_fragments));

        uint_fast8_t next_fragment  = 0;
        size_t last_used_char       = 0;
        uint_fast8_t nested_depth   = 0;

        // Namensfragmente aufspalten
        for (size_t i = 0; i < strlen (iupac_name) - 1; ++ i)
        {
            if (iupac_name [i] == '(')
            {
                ++ nested_depth;
            }
            else if (iupac_name [i] == ')')
            {
                -- nested_depth;
            }

            // Nach dem Ende eines Namensfragments suchen
            if (islower (iupac_name [i]) && iupac_name [i + 1] == '-')
            {
                iupac_name [i + 1] = '\0';
                // PRINTF_FFLUSH ("(1) %s\n", iupac_name + last_used_char);

                strncpy (temp_fragments [next_fragment], iupac_name + last_used_char, strlen (iupac_name + last_used_char));

                ++ next_fragment;
                iupac_name [i + 1] = '-';

                last_used_char += (i - last_used_char + 2);
            }
            else if (islower (iupac_name [i]) && iupac_name [i + 1] == ')' && nested_depth > 1)
            {
                iupac_name [i + 1] = '\0';
                // PRINTF_FFLUSH ("(2) %s\n", iupac_name + last_used_char);

                strncpy (temp_fragments [next_fragment], iupac_name + last_used_char, strlen (iupac_name + last_used_char));

                ++ next_fragment;
                iupac_name [i + 1] = ')';

                last_used_char += (i - last_used_char + 1);

                ++ next_fragment;
            }
        }

        // Namensfragmente sortieren, falls notwendig (alphabetisch bzgl. des Verzweigungstyps)
        // Aeste des Alkans alphabetisch sortieren, damit beim IUPAC-Namen z.B. Ethyl-Abzweigungen vor Methyl-Abzweigungen
        // stehen
        if (next_fragment > 1)
        {
            // Sortierung der Namensfragmente durchfuehren
            qsort (temp_fragments, next_fragment, iupac_name_length * sizeof (char), Cmp_Name_Fragments);

            size_t next_free_char = 0;
            memset (iupac_name, '\0', iupac_name_length * sizeof (char));

            // Sortierte Namensfragmente wieder zusammensetzen
            for (uint_fast8_t current_fragment = 0; current_fragment < next_fragment; ++ current_fragment)
            {
                strncpy (iupac_name + next_free_char, temp_fragments [current_fragment],
                        strlen (temp_fragments [current_fragment]));
                next_free_char += strlen (temp_fragments [current_fragment]);

                if ((current_fragment + 1) < next_fragment)
                {
                    iupac_name [next_free_char] = '-';
                    ++ next_free_char;
                }
            }
        }

        // Abschliessendes "-" Zeichen entfernen, falls notwendig
        if (iupac_name [strlen (iupac_name) - 1] == '-')
        {
            iupac_name [strlen (iupac_name) - 1] = '\0';
        }
    }

    // Bei einem gradlinigen Isomer wird "n-" angebracht, um anzudeuten, dass die gradlinige Variante gemeint ist
    // Ein gradliniges Isomer besteht nur aus einem Ast - der Hauptkette
    if (alkane->next_free_chain == 1)
    {
        strncat (iupac_name + strlen (iupac_name), "n-", iupac_name_length - strlen (iupac_name) - 1);
    }

    // Alkanwort am Ende anbrigen
    strncat (iupac_name + strlen (iupac_name), ALKAN_WORDS [alkane->chains [0].length - 1],
            iupac_name_length - strlen (iupac_name) - 1);

    // Nullterminierung garantieren
    iupac_name [iupac_name_length - 1] = '\0';

    return;
}

//=====================================================================================================================

/**
 * Vergleichsfunktion fuer die qsort-Funktion.
 * Alphabetisch aufsteigende Sortierung der Namensfragmente OHNE Beachtung ggf. vorhandener Zahlenpraefixe !
 */
static int Cmp_Name_Fragments (const void* a, const void* b)
{
    const char* const casted_strings [2] = { (const char* const) a, (const char* const) b };

    char first_base_char [2] = { '\0', '\0' };

    for (size_t current_string = 0; current_string < COUNT_ARRAY_ELEMENTS(casted_strings); ++ current_string)
    {
        for (size_t current_char = 0; current_char < strlen (casted_strings [current_string]); ++ current_char)
        {
            if (! isalpha (casted_strings [current_string][current_char]) /* == false */) { continue; }

            // Wenn es ein Buchstabe ist: Ist es Teil von einem Zahlenpraefix ?
            _Bool prefix_found  = false;
            size_t prefix_id    = 0;
            for (size_t i = 0; i < NUMBER_OF_NUMBER_WORDS; ++ i)
            {
                // Prefix vorhanden ?
                if (strncmp (&(casted_strings [current_string][current_char]),
                        NUMBER_WORDS [i], strlen (NUMBER_WORDS [i])) == 0)
                {
                    prefix_id       = i;
                    prefix_found    = true;
                    break;
                }
            }

            // Falls ein Zahlenpraefix vorhanden ist, dann wird dieser uebersprungen
            if (prefix_found /* == true */)
            {
                first_base_char [current_string] =
                        casted_strings [current_string][current_char + strlen (NUMBER_WORDS [prefix_id])];
            }
            else
            {
                first_base_char [current_string] = casted_strings [current_string][current_char];
            }
            break;
        }
    }

    return (first_base_char [0] - first_base_char [1]);
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Vergleichsfunktion fuer die qsort-Funktion.
 * Einfache nummerisch aufsteigende Sortierung.
 */
static inline int Cmp_Position_Information (const void* a, const void* b)
{
    return (*(int*) a - *(int*) b);
}

//---------------------------------------------------------------------------------------------------------------------

static void Next_Chain (struct State_Information* const restrict state)
{
    ++ state->current_index;

    // Wurde alles abgearbeitet ?
    if (state->current_index >= state->alkane->next_free_chain && state->current_nesting_depth == 0)
    {
        return;
    }
    // Beendigung einer Verschachtelungsebene ?
    if ((state->current_index >= state->alkane->next_free_chain && state->current_nesting_depth > 0) ||
            (state->alkane->chains [state->current_index].nesting_depth <
                    state->alkane->chains [state->current_index - 1].nesting_depth))
    {
        Up_In_Nesting (state);
    }
    // Versuch die aktuellen Chain-Objekte zu kombinieren
    // Z.B. zwei Methyl-Aeste werden dann als DiMethyl im IUPAC-Namen angegeben
    else
    {
        Try_To_Merge_Chains (state);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Try_To_Merge_Chains (struct State_Information* const restrict state)
{
    // Koennen die aktuellen Chain-Objekte zusammengefasst werden ?
    if (state->alkane->chains [state->current_index].length == state->alkane->chains [state->current_index + 1].length &&
            state->alkane->chains [state->current_index].nesting_depth == state->alkane->chains [state->current_index + 1].nesting_depth)
    {
        Next_Chain (state);
    }
    // Beginn eine Verschachtelung ?
    else if ((state->current_index > 1) &&              // Ein Vergleich zwischen zwei Aesten macht nur Sinn, wenn man
                                                        // mind. beim 3. Ast ist. (1. Ast ist der Hauptast; die zwei
                                                        // naechsten Aeste sind fuer den Vergleich)
            (state->last_alkyl_word_inserted > 0) &&    // Eine Verschachtelung ist nur moeglich, wenn bereits mind.
                                                        // ein Alkylwort geschrieben wurde
            (state->alkane->chains [state->current_index].nesting_depth >               // Ist die Verschachtelungs-
                state->alkane->chains [state->last_alkyl_word_inserted].nesting_depth)) // tiefe des aktuellen Asts
                                                                                        // groesser als beim letzten
                                                                                        // geschriebenen Alkylwort ?
    {
        Down_In_Nesting (state);
    }
    // Positionsnummern des / der aktuellen Aeste in den Ergebnisnamen einfuegen
    else
    {
        Insert_Numbers (state);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Insert_Numbers (struct State_Information* const restrict state)
{
    // Aktuelle Positionsinformationen
    int position_information [NUMBER_OF_NUMBER_WORDS]; // ! int !
    uint_fast8_t next_free_position_information = 0;
    memset (position_information, '\0', sizeof (position_information));
    for (uint_fast8_t i = (uint_fast8_t) (state->last_alkyl_word_inserted + 1); i <= state->current_index; ++ i)
    {
        position_information [next_free_position_information] = state->alkane->chains [i].position;
        ++ next_free_position_information;
    }

    // Positionsinformationen aufsteigend sortieren - falls notwendig - damit diese auch im IUPAC-Namen aufsteigend
    // eingetragen werden
    if (next_free_position_information > 1)
    {
        qsort (position_information, next_free_position_information, sizeof (int), Cmp_Position_Information);
    }

    // Positionsnummern in den Ergebnisnamen einfuegen
    for (uint_fast8_t i = (uint_fast8_t) (state->last_alkyl_word_inserted + 1); i <= state->current_index; ++ i)
    {
        // Aktuelle Positionsnummer als Zeichenkette darstellen
        char temp_string [10] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
        int2str (temp_string, sizeof (temp_string) - 1, position_information [i - (state->last_alkyl_word_inserted + 1)]);

        strncat (state->iupac_name + strlen (state->iupac_name), temp_string,
                state->iupac_name_length - strlen (state->iupac_name) - 1);

        // Kommata als Trennung zwischen verschiedenen Positionsnummern einfuegen, falls noch Positionsnummer folgen
        if ((i + 1) <= state->current_index)
        {
            strncat (state->iupac_name + strlen (state->iupac_name), ",",
                    state->iupac_name_length - strlen (state->iupac_name) - 1);
        }
    }

    Insert_Alkylword (state);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Insert_Alkylword (struct State_Information* const restrict state)
{
    strncat (state->iupac_name + strlen (state->iupac_name), "-",
            state->iupac_name_length - strlen (state->iupac_name) - 1);

    // Zusatz anbringen, falls notwendig (Z.B.: "Di", wenn ein Ast mehrfach vorkommt und so zusammengefasst werden
    // kann)
    if (state->current_index - state->last_alkyl_word_inserted > 1)
    {
        strncat (state->iupac_name + strlen (state->iupac_name),
                NUMBER_WORDS [state->current_index - state->last_alkyl_word_inserted - 1],
                state->iupac_name_length - strlen (state->iupac_name) - 1);
    }

    // Alkylword anbringen
    strncat (state->iupac_name + strlen (state->iupac_name), ALKYL_WORDS [state->alkane->chains [state->current_index].length - 1],
            state->iupac_name_length - strlen (state->iupac_name) - 1);

    strncat (state->iupac_name + strlen (state->iupac_name), "-",
            state->iupac_name_length - strlen (state->iupac_name) - 1);

    state->last_alkyl_word_inserted = state->current_index;

    Next_Chain (state);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Down_In_Nesting (struct State_Information* const restrict state)
{
    ++ state->current_nesting_depth;

    // Oeffnende Klammer anbringen
    strncat (state->iupac_name + strlen (state->iupac_name), "(",
            state->iupac_name_length - strlen (state->iupac_name) - 1);

    Insert_Numbers (state);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Up_In_Nesting (struct State_Information* const restrict state)
{
    -- state->current_nesting_depth;

    // Letztes Zeichen im IUPAC-Namen ueberschreiben
    // Dies wird immer ein "-" sein. Wenn eine Verschachtelung beendet wird, dann muss allerdings erst die schliessende
    // folgen !
    state->iupac_name [strlen (state->iupac_name) - 1] = '\0';

    // Schliessende Klammer und das gerade ueberschriebene Minuszeichen anbringen
    strncat (state->iupac_name + strlen (state->iupac_name), ")-",
            state->iupac_name_length - strlen (state->iupac_name) - 1);

    Next_Chain (state);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
