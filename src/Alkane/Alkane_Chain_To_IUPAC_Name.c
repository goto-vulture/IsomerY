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

    uint_fast8_t current_diff_chain;        // Aktuelles Chain-Objekt, welches betrachtet wird
};

struct Chain_Diff
{
    int_fast8_t length;
    int_fast8_t position;
    int_fast8_t nesting_depth;
};


static struct Chain_Diff Parse_Chain_Diff (struct State_Information* const restrict state, struct Chain_Diff diffs []);
static void Parse_Chain_Diff_Combined (struct State_Information* const restrict state, struct Chain_Diff diffs []);
static void Parse_Chain_List (struct State_Information* const restrict state);

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

    // => Generierung des Namen starten <=
    // Next_Chain (&state_information);
    Parse_Chain_List (&state_information);

    // Nullterminierung garantieren
    iupac_name [iupac_name_length - 1] = '\0';

    return;
}

//=====================================================================================================================

static void Parse_Chain_List (struct State_Information* const restrict state)
{
    struct Chain_Diff diffs [MAX_NUMBER_OF_C_ATOMS];
    memset (diffs, INT_FAST8_MIN, sizeof (diffs));

    for (uint_fast8_t i = 1; (i + 1) < state->alkane->next_free_chain; ++ i)
    {
        diffs [i - 1].length        =
                (int_fast8_t) (state->alkane->chains [i + 1].length - state->alkane->chains [i].length);
        diffs [i - 1].position      =
                (int_fast8_t) (state->alkane->chains [i + 1].position - state->alkane->chains [i].position);
        diffs [i - 1].nesting_depth =
                (int_fast8_t) (state->alkane->chains [i + 1].nesting_depth - state->alkane->chains [i].nesting_depth);
    }

    while (state->current_diff_chain < (state->alkane->next_free_chain - 1))
    {
        Parse_Chain_Diff_Combined(state, diffs);

        if ((state->current_diff_chain) < (state->alkane->next_free_chain - 1))
        {
            strncat (state->iupac_name, "-", state->iupac_name_space_left);
            state->iupac_name_space_left -= strlen ("-");
            //FPRINTF_FFLUSH_NO_VA_ARGS (stdout, "-");
        }
    }

    if (state->alkane->next_free_chain == 1)
    {
        strncat (state->iupac_name, "n-", state->iupac_name_space_left);
        state->iupac_name_space_left -= strlen ("n-");
        // FPRINTF_FFLUSH_NO_VA_ARGS (stdout, "n-");
    }

    strncat (state->iupac_name, ALKAN_WORDS [state->alkane->chains [0].length - 1], state->iupac_name_space_left);
    state->iupac_name_space_left -= strlen (ALKAN_WORDS [state->alkane->chains [0].length - 1]);
    // FPRINTF_FFLUSH (stdout, "%s\n", ALKAN_WORDS [state->alkane->chains [0].length - 1]);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static struct Chain_Diff Parse_Chain_Diff (struct State_Information* const restrict state, struct Chain_Diff diffs [])
{
    struct Chain_Diff ret_value = { .length = 0, .position = 0, .nesting_depth = 0 };

    if (state->current_diff_chain < (state->alkane->next_free_chain - 1))
    {
//        if (diffs [state->current_diff_chain].nesting_depth != 0 &&
//            diffs [state->current_diff_chain].nesting_depth != INT_FAST8_MIN)
//        {
            if (diffs [state->current_diff_chain].nesting_depth > 0)
            {
                state->current_diff_chain ++;
                uint_fast8_t current_diff_chain_backup = state->current_diff_chain;

                char int_to_str [10] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
                int2str (int_to_str, COUNT_ARRAY_ELEMENTS(int_to_str) - 1,
                        state->alkane->chains [state->current_diff_chain].position);

                strncat (state->iupac_name, int_to_str, state->iupac_name_space_left);
                state->iupac_name_space_left -= strlen (int_to_str);
                strncat (state->iupac_name, "-(", state->iupac_name_space_left);
                state->iupac_name_space_left -= strlen ("-(");
                // FPRINTF_FFLUSH(stdout, "%d-(", state->alkane->chains [state->current_diff_chain].position);
                Parse_Chain_Diff_Combined(state, diffs);

                strncat (state->iupac_name, ALKYL_WORDS [state->alkane->chains [current_diff_chain_backup].length - 1],
                        state->iupac_name_space_left);
                state->iupac_name_space_left -= strlen (ALKYL_WORDS [state->alkane->chains [current_diff_chain_backup].length - 1]);
                strncat (state->iupac_name, ")", state->iupac_name_space_left);
                state->iupac_name_space_left -= strlen (")");

//                if ((state->current_diff_chain + 1) < (state->alkane->next_free_chain - 1))
//                {
//                    strncat (state->iupac_name, ")", state->iupac_name_space_left);
//                    state->iupac_name_space_left -= strlen (")");
//                }
                // FPRINTF_FFLUSH(stdout, "%s", ALKYL_WORDS [state->alkane->chains [current_diff_chain_backup].length - 1]);
            }
//        }

        ret_value = diffs [state->current_diff_chain];
        state->current_diff_chain ++;
    }
    else
    {
        FPRINTF_FFLUSH_NO_VA_ARGS(stderr, "Parse Chain Error !\n");
    }

    return ret_value;
}

//---------------------------------------------------------------------------------------------------------------------

static void Parse_Chain_Diff_Combined (struct State_Information* const restrict state, struct Chain_Diff diffs [])
{
    char int_to_str [10] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };

    struct Chain_Diff current_diff = Parse_Chain_Diff (state, diffs);
    static uint_fast8_t count = 0;

    if (state->alkane->chains [state->current_diff_chain].position == 0)
    {
        return;
    }

    memset (int_to_str, '\0', sizeof (int_to_str));
    int2str (int_to_str, COUNT_ARRAY_ELEMENTS(int_to_str) - 1,
            state->alkane->chains [state->current_diff_chain].position);

    strncat (state->iupac_name, int_to_str, state->iupac_name_space_left);
    state->iupac_name_space_left -= strlen (int_to_str);

    // FPRINTF_FFLUSH (stdout, "%d", state->alkane->chains [state->current_diff_chain].position);
    count ++;

    while (current_diff.length == 0 && current_diff.nesting_depth == 0)
    {
        memset (int_to_str, '\0', sizeof (int_to_str));
        int2str (int_to_str, COUNT_ARRAY_ELEMENTS(int_to_str) - 1,
                state->alkane->chains [state->current_diff_chain + 1].position);

        strncat (state->iupac_name, ",", state->iupac_name_space_left);
        state->iupac_name_space_left -= strlen (",");
        strncat (state->iupac_name, int_to_str, state->iupac_name_space_left);
        state->iupac_name_space_left -= strlen (int_to_str);

        // FPRINTF_FFLUSH (stdout, ",%d", state->alkane->chains [state->current_diff_chain + 1].position);
        count ++;
        current_diff = Parse_Chain_Diff (state, diffs);
    }

    strncat (state->iupac_name, "-", state->iupac_name_space_left);
    state->iupac_name_space_left -= strlen ("-");
    strncat (state->iupac_name, ((count - 1) > 0) ? NUMBER_WORDS [count - 1] : "", state->iupac_name_space_left);
    state->iupac_name_space_left -= strlen (((count - 1) > 0) ? NUMBER_WORDS [count - 1] : "");
    strncat (state->iupac_name, ALKYL_WORDS [state->alkane->chains [state->current_diff_chain].length - 1],
            state->iupac_name_space_left);
    state->iupac_name_space_left -= strlen (ALKYL_WORDS [state->alkane->chains [state->current_diff_chain].length - 1]);
//    FPRINTF_FFLUSH (stdout, "-%s%s", ((count - 1) > 0) ? NUMBER_WORDS [count - 1] : "",
//            ALKYL_WORDS [state->alkane->chains [state->current_diff_chain].length - 1]);
    count = 0;

    return;
}

//---------------------------------------------------------------------------------------------------------------------
