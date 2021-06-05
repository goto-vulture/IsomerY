/**
 * Alkane_Chain_To_IUPAC_Name.c
 *
 *  Created on: 23.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane_Chain_To_IUPAC_Name.h"
#include <string.h>
#include <stdbool.h>
#include "Alkane_Info_Constitutional_Isomer.h"
#include "../str2int.h"
#include "../Print_Tools.h"
#include "../Misc.h"
#include "../Error_Handling/Assert_Msg.h"





/**
 * Statusinformationen, die bei der Konvertierung der Chain-Objekte zu dem IUPAC-Namen verwendet werden.
 */
struct State_Information
{
    char* iupac_name;                       // Speicher fuer den IUPAC-String
    size_t iupac_name_length;               // Maximale Laenge, die der IUPAC-Name annehmen kann (Groesse des
                                            // reservierten Speichers)
    size_t iupac_name_space_left;           // Anzahl an Zeichen, die im Speicherbereich fuer den IUPAC-Namen noch
                                            // frei sind
    struct Alkane* alkane;                  // Alkan-Objekt (enthaelt die Chain-Objekte)

    uint_fast8_t current_chain;             // Aktuelles Chain-Objekt, welches betrachtet wird
    uint_fast8_t completed_chain;           // Das letzte Chain-Objekt, welches KOMPLETT verarbeitet wurde

    struct Chain* cmp_chain;                // Vergleichs Chain-Objekt; Dies dient dazu, um gleiche Alkylreste
                                            // zusammenfassen zu koennen

    // Liste an Chain-Objekte, die teilweise verarbeitet wurden
    // Dies ist dann der Fall, wenn es Verschachtelungen gibt
    uint_fast8_t partly_completed_chains [MAX_NUMBER_OF_C_ATOMS];
    uint_fast8_t next_free_partly_completed_chains;
};

// ===== ===== ===== Zustaende fuer die Chain-Objekt-Konvertierung ===== ===== =====
static void Next_Chain (struct State_Information* const restrict state);
static void New_Compare_Chain (struct State_Information* const restrict state);
static void Chains_Mergeable (struct State_Information* const restrict state);

static void Different_Type (struct State_Information* const restrict state);
static void Different_Length (struct State_Information* const restrict state);
static void Down (struct State_Information* const restrict state);
static void Up (struct State_Information* const restrict state);

static void Last_Chain_Found (struct State_Information* const restrict state);
static void Complete_Name (struct State_Information* const restrict state);
// ===== ===== ===== Zustaende fuer die Chain-Objekt-Konvertierung ===== ===== =====

// KEIN Zustand, sondern eine einfache statische Funktion, die bei der Konvertierung verwendet wird !
static void Write_Alkyl_Data_Into_IUPAC_Name (struct State_Information* const restrict state);



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

    struct State_Information state_information;
    memset (&state_information, '\0', sizeof (struct State_Information));

    state_information.iupac_name            = iupac_name;
    state_information.iupac_name_length     = iupac_name_length;
    state_information.iupac_name_space_left = iupac_name_length - 1;
    state_information.alkane                = alkane;
    state_information.cmp_chain             = NULL;

    // => Generierung des Namen starten <=
    Next_Chain (&state_information);

    // Nullterminierung garantieren
    iupac_name [iupac_name_length - 1] = '\0';

    return;
}

//=====================================================================================================================

static void Next_Chain (struct State_Information* const restrict state)
{
    state->current_chain ++;

    if (state->current_chain >= state->alkane->next_free_chain - 1 &&
            state->completed_chain >= state->alkane->next_free_chain - 1)
    {
        Last_Chain_Found (state);
    }
    else if (state->cmp_chain == NULL)
    {
        New_Compare_Chain (state);
    }
    else if (state->cmp_chain->length == state->alkane->chains [state->current_chain].length &&
            state->cmp_chain->nesting_depth == state->alkane->chains [state->current_chain].nesting_depth)
    {
        Chains_Mergeable (state);
    }
    else
    {
        Different_Type (state);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void New_Compare_Chain (struct State_Information* const restrict state)
{
    state->cmp_chain = &(state->alkane->chains [state->current_chain]);

    Next_Chain (state);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Chains_Mergeable (struct State_Information* const restrict state)
{
    Next_Chain (state);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Different_Type (struct State_Information* const restrict state)
{
    if ((state->alkane->chains [state->current_chain].nesting_depth >
            state->alkane->chains [state->current_chain - 1].nesting_depth) &&
            state->current_chain < state->alkane->next_free_chain)
    {
        // Dies ist ein Sonderfall !
        // Wenn es Objekte gibt, die zusammengefasst werden koennen, dann wird dies nicht durchgefuehrt, wenn direkt
        // darauf eine tiefere Verschachtelung gefunden wurde
        if ((state->current_chain - (state->completed_chain + 1)) > 1)
        {
            // Fuer die Erstellung des Alkylfragments muss das aktuelle Chain-Objekt ignoriert werden, da dies erst
            // beim Eintritt in eine tiefere Verschachtelungstiefe betrachtet werden darf !
            state->current_chain --;
            Write_Alkyl_Data_Into_IUPAC_Name (state);
            state->current_chain ++;
        }

        Down (state);
    }
    else if ((state->alkane->chains [state->current_chain].nesting_depth <
            state->alkane->chains [state->current_chain - 1].nesting_depth) &&
            state->current_chain < state->alkane->next_free_chain)
    {
        if ((state->current_chain - (state->completed_chain + 1)) > 1)
        {
            Write_Alkyl_Data_Into_IUPAC_Name (state);
        }

        Up (state);
    }
    else if (state->alkane->chains [state->current_chain].length !=
            state->alkane->chains [state->current_chain - 1].length)
    {
        Different_Length (state);
    }
    // Dieser Part darf nicht aufgerufen werden ! Wenn dies der Fall ist, dann ist dies auf jeden Fall ein Fehler !
    else
    {
        ASSERT_MSG (false, "This else part may not executed !");
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Different_Length (struct State_Information* const restrict state)
{
    Write_Alkyl_Data_Into_IUPAC_Name (state);

    // current_chain um 1 zuruecksetzen, da mit den gerade durchgefuehrten Operationen die Chain-Objekte bis vor dem
    // current_chain abgearbeitet wurden
    state->current_chain --;
    state->completed_chain  = state->current_chain;
    state->cmp_chain        = NULL;

    Next_Chain (state);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Down (struct State_Information* const restrict state)
{
    if (strlen (state->iupac_name) > 0)
    {
        strncat (state->iupac_name, "-", state->iupac_name_space_left);
        state->iupac_name_space_left -= strlen ("-");
    }

    // Position des letzten Objektes ermitteln und anbringen
    char int_to_str [10] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
    int2str (int_to_str, COUNT_ARRAY_ELEMENTS(int_to_str) - 1, state->alkane->chains [state->current_chain - 1].position);

    strncat (state->iupac_name, int_to_str, state->iupac_name_space_left);
    state->iupac_name_space_left -= strlen (int_to_str);
    strncat (state->iupac_name, "-(", state->iupac_name_space_left);
    state->iupac_name_space_left -= strlen ("-(");

    // Partialinformation aufnehmen
    state->partly_completed_chains [state->next_free_partly_completed_chains] = (uint_fast8_t) (state->current_chain - 1);
    state->next_free_partly_completed_chains ++;

    state->current_chain --;
    state->completed_chain  = state->current_chain;
    state->cmp_chain        = NULL;

    Next_Chain (state);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Up (struct State_Information* const restrict state)
{
    strncat (state->iupac_name,
            ALKYL_WORDS [state->alkane->chains [state->partly_completed_chains [state->next_free_partly_completed_chains - 1]].length - 1],
            state->iupac_name_space_left);
    state->iupac_name_space_left -=
            strlen (ALKYL_WORDS [state->alkane->chains [state->partly_completed_chains [state->next_free_partly_completed_chains - 1]].length - 1]);

    strncat (state->iupac_name, ")-", state->iupac_name_space_left);
    state->iupac_name_space_left -= strlen (")-");

    state->partly_completed_chains [state->next_free_partly_completed_chains - 1] = 0;
    state->next_free_partly_completed_chains --;

    Next_Chain (state);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Last_Chain_Found (struct State_Information* const restrict state)
{
    if (state->next_free_partly_completed_chains > 0)
    {
        for (int i = state->next_free_partly_completed_chains - 1; i >= 0; -- i)
        {
            strncat (state->iupac_name,
                    ALKYL_WORDS [state->alkane->chains [state->partly_completed_chains [i]].length - 1],
                    state->iupac_name_space_left);
            state->iupac_name_space_left -=
                    strlen (ALKYL_WORDS [state->alkane->chains [state->partly_completed_chains [i]].length - 1]);

            strncat (state->iupac_name, ")-", state->iupac_name_space_left);
            state->iupac_name_space_left -= strlen (")-");
        }
    }

    Complete_Name (state);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Complete_Name (struct State_Information* const restrict state)
{
    if (strlen (state->iupac_name) == 0)
    {
        strncat (state->iupac_name, "n-", state->iupac_name_space_left);
        state->iupac_name_space_left -= strlen ("n-");
    }

    strncat (state->iupac_name, ALKAN_WORDS [state->alkane->chains [0].length - 1], state->iupac_name_space_left);
    state->iupac_name_space_left -= strlen (ALKAN_WORDS [state->alkane->chains [0].length - 1]);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

// KEIN Zustand, sondern eine einfache statische Funktion, die bei der Konvertierung verwendet wird !
static void Write_Alkyl_Data_Into_IUPAC_Name (struct State_Information* const restrict state)
{
    if (strlen (state->iupac_name) > 0 && state->iupac_name [strlen (state->iupac_name) - 1] != '(')
    {
        strncat (state->iupac_name, "-", state->iupac_name_space_left);
        state->iupac_name_space_left -= strlen ("-");
    }

    for (int i = state->completed_chain + 1; i < state->current_chain; ++ i)
    {
        char int_to_str [10] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
        int2str (int_to_str, COUNT_ARRAY_ELEMENTS(int_to_str) - 1, state->alkane->chains [i].position);

        strncat (state->iupac_name, int_to_str, state->iupac_name_space_left);
        state->iupac_name_space_left -= strlen (int_to_str);

        if ((i + 1) < state->current_chain)
        {
            strncat (state->iupac_name, ",", state->iupac_name_space_left);
            state->iupac_name_space_left -= strlen (",");
        }
    }
    strncat (state->iupac_name, "-", state->iupac_name_space_left);
    state->iupac_name_space_left -= strlen ("-");

    const int chains_wrote = state->current_chain - (state->completed_chain + 1);

    if (chains_wrote > 1)
    {
        strncat (state->iupac_name, NUMBER_WORDS [chains_wrote - 1], state->iupac_name_space_left);
        state->iupac_name_space_left -= strlen (NUMBER_WORDS [chains_wrote - 1]);
    }

    // Wort fuer Kettentyp anbringen
    strncat (state->iupac_name, ALKYL_WORDS [state->alkane->chains [state->current_chain - 1].length - 1],
            state->iupac_name_space_left);
    state->iupac_name_space_left -= strlen (ALKYL_WORDS [state->alkane->chains [state->current_chain - 1].length - 1]);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
