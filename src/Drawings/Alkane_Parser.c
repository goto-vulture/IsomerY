/**
 * @file Alkane_Drawing.c
 *
 * @date 10.03.2022
 * @author am1
 */

#include "Alkane_Parser.h"
#include <ctype.h>
#include "../str2int.h"
#include "../String_Tools.h"





/**
 * Funktionen fuer den Alkane_Lexer.
 */
static struct Alkane_Lexer Start_Lexer (const char* const iupac_name, const size_t length);
static void Next_Char (struct Alkane_Lexer* const lexer_data);
static void Split_Char_Found (struct Alkane_Lexer* const lexer_data);
// "Alkyl_End_Found" und "Number_Word_Found" sind vom Code identisch. Dennoch gibt es diese beiden Funktionen, um die
// Klassifizierungen der neuen Tokens per Funktion durchfuehren zu koennen
static void Alkyl_End_Found (struct Alkane_Lexer* const lexer_data);
static void Number_Word_Found (struct Alkane_Lexer* const lexer_data);
static void End_Char_Found (struct Alkane_Lexer* const lexer_data);

static enum Token_Type Type_Of_Token (const char* const token, const size_t length);
static void Copy_Token_To_Lexer_Data (struct Alkane_Lexer* const lexer_data);

//=====================================================================================================================

extern void Parse_Alkane (const char* const iupac_name, const size_t length)
{
    const struct Alkane_Lexer lexer_data = Start_Lexer (iupac_name, length);

    // Erstellte Lexer-Daten fuer den Parser vorbereiten ...

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern struct Alkane_Lexer Create_Alkane_Tokens (const char* const iupac_name, const size_t length)
{
    return Start_Lexer (iupac_name, length);
}

//=====================================================================================================================

static struct Alkane_Lexer Start_Lexer (const char* const iupac_name, const size_t length)
{
    struct Alkane_Lexer lexer_data;
    memset (&lexer_data, '\0', sizeof (lexer_data));

    lexer_data.alkane_name = iupac_name;
    lexer_data.name_length = length;

    // Lexer starten
    Next_Char (&lexer_data);

    return lexer_data;
}

//---------------------------------------------------------------------------------------------------------------------

static void Next_Char (struct Alkane_Lexer* const lexer_data)
{
    const char current_char = lexer_data->alkane_name [lexer_data->current_char];
    const size_t current_token_size = (size_t) (lexer_data->current_char - lexer_data->last_char_used);

    // Alle Zeichen verarbeitet
    if (current_char == '\0')
    {
        End_Char_Found (lexer_data);
    }
    // Aktuelles Zeichen ein Trennzeichen ?
    else if (current_char == ',' || current_char == '-' || current_char == '(' || current_char == ')')
    {
        Split_Char_Found (lexer_data);
    }
    else if (Type_Of_Token (&(lexer_data->alkane_name [lexer_data->last_char_used]),
            current_token_size) == TOKEN_TYPE_ALKYL_WORD)
    {
        Alkyl_End_Found (lexer_data);
    }
    // In manchen Faellen kommt es vor, dass ein Teil eines Alkanwortes als ein Zahlenwort interpretiert werden
    // kann. Z.B. "heptane" -> Zahlenwort "hepta" enthalten
    // Damit dies nicht passiert, wird fuer ein Zahlenwort vorausgesetzt, dass noch mind. 5 Zeichen uebrig sind
    // 5, da das kuerzeste Alkanwort "Butan" 5 Zeichen lang ist
    else if (Type_Of_Token (&(lexer_data->alkane_name [lexer_data->last_char_used]),
            current_token_size) == TOKEN_TYPE_NUMBER_WORD &&
            (lexer_data->name_length - lexer_data->current_char >= 5))
    {
        Number_Word_Found (lexer_data);
    }
    else
    {
        lexer_data->current_char ++;
        Next_Char (lexer_data);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Split_Char_Found (struct Alkane_Lexer* const lexer_data)
{
    const size_t token_length = (size_t) (lexer_data->current_char - lexer_data->last_char_used);
    const enum Token_Type token_type = Type_Of_Token (&(lexer_data->alkane_name [lexer_data->last_char_used]),
            token_length);
    const enum Token_Type split_token_type = Type_Of_Token(&(lexer_data->alkane_name [lexer_data->current_char]),
            1);

    // Fuer Ziffern bzw. Zahlen
    if (token_type != TOKEN_TYPE_OPEN_BRACKET && token_type != TOKEN_TYPE_CLOSE_BRACKET &&
            token_type != TOKEN_TYPE_SUB_CHAR && token_type != TOKEN_TYPE_COMMA_CHAR && token_type != TOKEN_TYPE_N_A)
    {
        // Copy_Token_To_Lexer_Data (lexer_data);
        // Kann NICHT verwendet werden, da diese Funktion ebenfalls "current_char" und "last_char_used" anpasst. Diese
        // Anpassungen sind normalerweise auch notwendig; nur hier nicht ! -> Keine Verwendung dieser Funktion.
        strncpy ((char*) &(lexer_data->result_tokens [lexer_data->next_free_token]),
                &(lexer_data->alkane_name [lexer_data->last_char_used]), token_length);
        lexer_data->token_type [lexer_data->next_free_token] = token_type;
        lexer_data->next_free_token ++;
    }

    lexer_data->result_tokens [lexer_data->next_free_token][0] = lexer_data->alkane_name [lexer_data->current_char];
    lexer_data->token_type [lexer_data->next_free_token] = split_token_type;
    lexer_data->next_free_token ++;

    lexer_data->current_char ++;
    lexer_data->last_char_used = lexer_data->current_char;

    Next_Char (lexer_data);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Alkyl_End_Found (struct Alkane_Lexer* const lexer_data)
{
    Copy_Token_To_Lexer_Data (lexer_data);
    Next_Char (lexer_data);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Number_Word_Found (struct Alkane_Lexer* const lexer_data)
{
    Copy_Token_To_Lexer_Data (lexer_data);
    Next_Char (lexer_data);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void End_Char_Found (struct Alkane_Lexer* const lexer_data)
{
    Copy_Token_To_Lexer_Data (lexer_data);
    lexer_data->alkane_name = NULL;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static enum Token_Type Type_Of_Token (const char* const token, const size_t length)
{
    enum Token_Type result = TOKEN_TYPE_N_A;

    if (length == 1 && ! isalpha (*token) /* == false */)
    {
        switch (token [0])
        {
        case ',':
            result = TOKEN_TYPE_COMMA_CHAR;
            break;
        case '-':
            result = TOKEN_TYPE_SUB_CHAR;
            break;
        case '(':
            result = TOKEN_TYPE_OPEN_BRACKET;
            break;
        case ')':
            result = TOKEN_TYPE_CLOSE_BRACKET;
            break;
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            result = TOKEN_TYPE_NUMBER;
            break;
        default:
            // Die Ausfuehrung des Default-Paths ist hier immer ein Fehler !
            ASSERT_MSG(false, "Switch-Case default path executed !")
        }
    }
    else
    {
        // Da der Token nicht zwingend nullterminiert ist, muessen fuer die weiteren Operationen eine Zeichenkette mit
        // genau dieser Eigenschaft gebildet werden
        char token_with_null [30];
        memset (token_with_null, '\0', sizeof (token_with_null));
        strncpy (token_with_null, token, length);

        // Ist der Token eine Zahl ? Ausprobieren durch eine str -> int Konvertierung
        long int str2int_out = 0;
        if (str2int (&str2int_out, token_with_null, 10) == STR2INT_SUCCESS)
        {
            result = TOKEN_TYPE_NUMBER;
        }
        if (result == TOKEN_TYPE_N_A)
        {
            // Ist es ein Alkan-Wort ? (Getestet wird sowohl in Deutsch als auch in Englisch)
            for (uint_fast8_t i = 0; i < NUMBER_OF_ALKAN_WORDS; ++ i)
            {
                if (Compare_Strings_Case_Insensitive (token_with_null, ALKAN_WORDS_DE [i]) == 0 ||
                        Compare_Strings_Case_Insensitive (token_with_null, ALKAN_WORDS_EN [i]) == 0)
                {
                    result = TOKEN_TYPE_ALKANE_WORD;
                    break;
                }
            }
        }
        if (result == TOKEN_TYPE_N_A)
        {
            // Ist das Token ein Alkyl-Wort ?
            for (uint_fast8_t i = 0; i < NUMBER_OF_ALKYL_WORDS; ++ i)
            {
                if (Compare_Strings_Case_Insensitive (token_with_null, ALKYL_WORDS [i]) == 0)
                {
                    result = TOKEN_TYPE_ALKYL_WORD;
                    break;
                }
            }
        }
        if (result == TOKEN_TYPE_N_A)
        {
            // Ist das Token ein Zahlen-Wort ?
            for (uint_fast8_t i = 0; i < NUMBER_OF_NUMBER_WORDS; ++ i)
            {
                if (Compare_Strings_Case_Insensitive (token_with_null, NUMBER_WORDS [i]) == 0)
                {
                    result = TOKEN_TYPE_NUMBER_WORD;
                    break;
                }
            }
        }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

static void Copy_Token_To_Lexer_Data (struct Alkane_Lexer* const lexer_data)
{
    const size_t token_length = (size_t) (lexer_data->current_char - lexer_data->last_char_used);
    const enum Token_Type token_type = Type_Of_Token (&(lexer_data->alkane_name [lexer_data->last_char_used]),
            token_length);

    strncpy ((char*) &(lexer_data->result_tokens [lexer_data->next_free_token]),
            &(lexer_data->alkane_name [lexer_data->last_char_used]), token_length);
    lexer_data->token_type [lexer_data->next_free_token] = token_type;
    lexer_data->next_free_token ++;

    lexer_data->last_char_used = lexer_data->current_char;
    lexer_data->current_char ++;

    return;
}

//---------------------------------------------------------------------------------------------------------------------
