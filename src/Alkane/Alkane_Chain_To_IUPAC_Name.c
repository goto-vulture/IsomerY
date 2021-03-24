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





struct State_Information
{
    char* iupac_name;
    size_t iupac_name_length;
    struct Alkane* alkane;

    uint_fast8_t current_index;
    uint_fast8_t current_nesting_depth;
    uint_fast8_t last_alkyl_word_inserted;
};

static void Next_Chain  (struct State_Information* const restrict state);
static void Try_To_Merge_Chains (struct State_Information* const restrict state);
static void Insert_Numbers (struct State_Information* const restrict state);
static void Insert_Alkylword (struct State_Information* const restrict state);
static void Down_In_Nesting (struct State_Information* const restrict state);
static void Up_In_Nesting (struct State_Information* const restrict state);

//=====================================================================================================================

void
Chain_To_IUPAC
(
        char* const restrict iupac_name,
        const size_t iupac_name_length,
        struct Alkane* const restrict alkane
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

    Next_Chain (&state_information);

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

    if ((state->current_index >= state->alkane->next_free_chain && state->current_nesting_depth > 0) ||
            (state->alkane->chains [state->current_index].nesting_depth <
                    state->alkane->chains [state->current_index - 1].nesting_depth))
    {
        Up_In_Nesting (state);
    }
    else
    {
        Try_To_Merge_Chains (state);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Try_To_Merge_Chains (struct State_Information* const restrict state)
{
    if (state->alkane->chains [state->current_index].length == state->alkane->chains [state->current_index + 1].length &&
            state->alkane->chains [state->current_index].nesting_depth == state->alkane->chains [state->current_index + 1].nesting_depth)
    {
        Next_Chain (state);
    }
    else if ((state->current_index > 1) && (state->alkane->chains [state->current_index].nesting_depth >
                state->alkane->chains [state->last_alkyl_word_inserted].nesting_depth))
    {
        Down_In_Nesting (state);
    }
    else
    {
        Insert_Numbers (state);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Insert_Numbers (struct State_Information* const restrict state)
{
    for (uint_fast8_t i = (uint_fast8_t) (state->last_alkyl_word_inserted + 1); i <= state->current_index; ++ i)
    {
        char temp_string [10];
        memset (temp_string, '\0', sizeof (temp_string));

        int2str (temp_string, sizeof (temp_string) - 1, state->alkane->chains [i].position);

        strncat (state->iupac_name + strlen (state->iupac_name), temp_string,
                state->iupac_name_length - strlen (state->iupac_name) - 1);

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

    // Zusatz anbringen, falls notwendig
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

    if (strlen (state->iupac_name) > 0)
    {
        state->iupac_name [strlen (state->iupac_name) - 1] = '\0';
    }

    // U.a. schliessende Klammer anbringen
    strncat (state->iupac_name + strlen (state->iupac_name), ")-",
            state->iupac_name_length - strlen (state->iupac_name) - 1);

    Next_Chain (state);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
