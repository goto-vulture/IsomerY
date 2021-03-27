/**
 * Alkane_Chain_To_IUPAC_Name.c
 *
 *  Created on: 23.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane_Chain_To_IUPAC_Name.h"
#include <string.h>
#include "Alkane_Info_Constitutional_Isomer.h"
#include "../str2int.h"





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

static void Next_Chain  (struct State_Information* const restrict state);
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

    // Erzeugung des IUPAC-Namen starten
    Next_Chain (&state_information);

    // Bei einem gradlinigen Isomer wird "n-" angebracht, um anzudeuten, dass die gradlinige Variante gemeint ist
    // Ein gradliniges Isomer besteht nur aus einem Ast - der Hauptkette
    if (alkane->next_free_chain == 1)
    {
        strncat (iupac_name + strlen (iupac_name), "n-", iupac_name_length - strlen (iupac_name) - 1);
    }

    // Alkanwort am Ende anbrigen
    strncat (iupac_name + strlen (iupac_name), ALKAN_WORDS [alkane->chains [0].length - 1],
            iupac_name_length - strlen (iupac_name) - 1);

    return;
}

//=====================================================================================================================

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
    else if ((state->current_index > 1) && (state->alkane->chains [state->current_index].nesting_depth >
                state->alkane->chains [state->last_alkyl_word_inserted].nesting_depth))
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
    // Positionsnummern in den Ergebnisnamen einfuegen
    for (uint_fast8_t i = (uint_fast8_t) (state->last_alkyl_word_inserted + 1); i <= state->current_index; ++ i)
    {
        // Aktuelle Positionsnummer als Zeichenkette darstellen
        char temp_string [10] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
        int2str (temp_string, sizeof (temp_string) - 1, state->alkane->chains [i].position);

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
