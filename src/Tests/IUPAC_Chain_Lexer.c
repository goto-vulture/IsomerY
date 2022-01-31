/**
 * @file IUPAC_Chain_Lexer.h
 *
 * @brief Einfacher Lexer, um IUPAC-Namen an den Grenzen der Ketten (OHNE Beachtung von Verschachtelungen) zu trennen.
 *
 * Aufbau des Lexers:
 *
 *
 *
 *  Create_Chain_Tokens() -> Next_Char() -> End_Char
 *
 *
 *
 * @date 27.01.2022
 * @author am1
 */

#include "IUPAC_Chain_Lexer.h"
#include <ctype.h>
#include "../Print_Tools.h"





static void Next_Char (struct IUPAC_Chain_Lexer_Result* const lexer_data);
static void Safe_Split_Char_Found (struct IUPAC_Chain_Lexer_Result* const lexer_data);
static void Possible_Split_Char (struct IUPAC_Chain_Lexer_Result* const lexer_data);
static void Split_Char_Confirmed (struct IUPAC_Chain_Lexer_Result* const lexer_data);
static void End_Char_Found (struct IUPAC_Chain_Lexer_Result* const lexer_data);

//=====================================================================================================================

struct IUPAC_Chain_Lexer_Result Create_Chain_Tokens (const char* const iupac_name)
{
    struct IUPAC_Chain_Lexer_Result result;
    memset (&result, '\0', sizeof(result));

    result.orig_string = iupac_name;

    Next_Char(&result);

    return result;
}

//=====================================================================================================================

static void Next_Char (struct IUPAC_Chain_Lexer_Result* const lexer_data)
{
    const char current_char = lexer_data->orig_string [lexer_data->current_char];

    if (current_char == '\0')
    {
        lexer_data->next_free_token ++;
        End_Char_Found(lexer_data);
    }
    else if (current_char == ')')
    {
        Safe_Split_Char_Found(lexer_data);
    }
    else if (current_char == '-')
    {
        Possible_Split_Char(lexer_data);
    }
    else
    {
        lexer_data->result_tokens[lexer_data->next_free_token][lexer_data->next_free_char_in_token] = current_char;
        lexer_data->current_char ++;
        lexer_data->next_free_char_in_token ++;

        Next_Char(lexer_data);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Safe_Split_Char_Found (struct IUPAC_Chain_Lexer_Result* const lexer_data)
{
    const char current_char = lexer_data->orig_string [lexer_data->current_char];

    lexer_data->result_tokens[lexer_data->next_free_token][lexer_data->next_free_char_in_token] = current_char;
    lexer_data->next_free_char_in_token ++;
    lexer_data->result_tokens[lexer_data->next_free_token][lexer_data->next_free_char_in_token] = '\0';
    lexer_data->next_free_char_in_token = 0;
    lexer_data->current_char ++;
    lexer_data->next_free_token ++;

    Next_Char(lexer_data);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Possible_Split_Char (struct IUPAC_Chain_Lexer_Result* const lexer_data)
{
    const char current_char = lexer_data->orig_string [lexer_data->current_char];
    const char current_char_plus_1 = lexer_data->orig_string [lexer_data->current_char + 1];

    // Das '-' wird NICHT in die Tokens uebernommen !
    if (isdigit (current_char_plus_1))
    {
        Split_Char_Confirmed(lexer_data);
    }
    else
    {
        lexer_data->result_tokens[lexer_data->next_free_token][lexer_data->next_free_char_in_token] = current_char;
        lexer_data->current_char ++;
        lexer_data->next_free_char_in_token ++;

        Next_Char(lexer_data);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Split_Char_Confirmed (struct IUPAC_Chain_Lexer_Result* const lexer_data)
{
    lexer_data->result_tokens[lexer_data->next_free_token][lexer_data->next_free_char_in_token] = '\0';
    lexer_data->next_free_token ++;
    lexer_data->next_free_char_in_token = 0;
    lexer_data->current_char ++;

    Next_Char(lexer_data);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void End_Char_Found (struct IUPAC_Chain_Lexer_Result* const lexer_data)
{
    lexer_data->orig_string = NULL;

    for (uint_fast8_t i = 0; i < lexer_data->next_free_token; ++ i)
    {
        PRINTF_FFLUSH("%" PRIxFAST8 ": %s\n", i, lexer_data->result_tokens [i]);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------
