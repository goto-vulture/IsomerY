/**
 * @file IUPAC_Chain_Lexer.h
 *
 * @brief Einfacher Lexer, um IUPAC-Namen an den Grenzen der Ketten (OHNE Beachtung von Verschachtelungen) zu trennen.
 *
 * Funktional ist der Aufbau sehr einfach. Zustandsfunktionen, die direkt oder indirekt rekursiv aufgerufen werden.
 *
 * Aufbau des Lexers:
 *                                    +---> return
 *                                    |
 *                            +----------------+
 *                            | End_Char_Found |
 *                            +----------------+
 *                                    ^
 *                              +--+  |  +-------------------------------+
 *                              |  |  |  |                               |
 *                              v  |  |  v                               |
 * +---------------------+    +-----------+    +----------------------+  |
 * | Create_chain_Tokens | -> | Next_Char | -> | Safe_Splt_Char_Found | -+
 * +---------------------+    +-----------+    +----------------------+
 *                               ^  |    ^
 *                               |  |    |
 *                               |  |    +-----------------------------------------+
 *                               |  v                                              |
 *                            +---------------------+    +----------------------+  |
 *                            | Possible_Split_Char | -> | Spilt_Char_Confirmed | -+
 *                            +---------------------+    +----------------------+
 *
 * @date 27.01.2022
 * @author am1
 */

#include "IUPAC_Chain_Lexer.h"
#include <ctype.h>
#include "../Print_Tools.h"
#include "../Alkane/Alkane_Info_Constitutional_Isomer.h"




/**
 * Rekursiv verwendete Zustaende des Lexers.
 */
static void Next_Char (struct IUPAC_Chain_Lexer_Result* const lexer_data);
static void Safe_Split_Char_Found (struct IUPAC_Chain_Lexer_Result* const lexer_data);
static void Possible_Split_Char (struct IUPAC_Chain_Lexer_Result* const lexer_data);
static void Split_Char_Confirmed (struct IUPAC_Chain_Lexer_Result* const lexer_data);
static void End_Char_Found (struct IUPAC_Chain_Lexer_Result* const lexer_data);

//=====================================================================================================================

/**
 * Lexter mit der Originalzeichenkette starten.
 */
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
    // Nur wenn alle Verschachtelungen beendet wurden, kann eine schliessende Klammer als Split-Zeichen verwendet
    // werden !
    else if (current_char == ')' && lexer_data->nesting_depth == 1)
    {
        lexer_data->nesting_depth --;
        Safe_Split_Char_Found(lexer_data);
    }
    else if (current_char == '-')
    {
        // In einigen Faellen erscheint ein Minuszeichen obwohl das aktuelle Ergebnis keine Zeichen enthaelt
        // Wenn dies der Fall ist, dann muss das Zeichen uebersprungen werden
        if (lexer_data->next_free_char_in_token > 0)
        {
            Possible_Split_Char(lexer_data);
        }
        else
        {
            lexer_data->current_char ++;
            Next_Char(lexer_data);
        }
    }
    else
    {
        if (current_char == '(')
        {
            lexer_data->nesting_depth ++;
        }

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
    const char current_char         = lexer_data->orig_string [lexer_data->current_char];
    const char current_char_plus_1  = lexer_data->orig_string [lexer_data->current_char + 1];

    _Bool last_token_is_an_alkane_token = false;
    for (size_t i = 0; i < NUMBER_OF_ALKAN_WORDS; ++ i)
    {
        const char* current_start_address = &(lexer_data->orig_string [lexer_data->current_char + 1]);

        if (strncmp (current_start_address, ALKAN_WORDS_DE [i], strlen (current_start_address)) == 0)
        {
            last_token_is_an_alkane_token = true;
            break;
        }
        if (strncmp (current_start_address, ALKAN_WORDS_EN [i], strlen (current_start_address)) == 0)
        {
            last_token_is_an_alkane_token = true;
            break;
        }
    }

    // Das '-' wird NICHT in die Tokens uebernommen !
    if (isdigit (current_char_plus_1))
    {
        Split_Char_Confirmed(lexer_data);
    }
    // Wenn der restliche Originalstring ein Alkanname ist, dann wurde ebenfalls das Ende eines Tokens gefunden
    else if (last_token_is_an_alkane_token /* == true */)
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

    // Debug-Ausgaben
    // for (uint_fast8_t i = 0; i < lexer_data->next_free_token; ++ i)
    // {
    //     PRINTF_FFLUSH("%" PRIxFAST8 ": %s\n", i, lexer_data->result_tokens [i]);
    // }

    return;
}

//---------------------------------------------------------------------------------------------------------------------
