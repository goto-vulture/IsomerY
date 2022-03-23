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
#include "../Misc.h"
#include "../Print_Tools.h"



// Konstanten fuer die Terminale. Gleiche Bezeichnung wie im Kommentarblock
enum Terminalsymbol
{
    NO_TERMINALSYMBOL = -1,
    y = 0,
    o, n, z, m, k, c, a
};


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

static enum Terminalsymbol Token_Type_To_Terminalsymbol (const enum Token_Type token_type);

//=====================================================================================================================

extern _Bool Parse_Alkane (const char* const iupac_name, const size_t length)
{
    struct Alkane_Lexer lexer_data = Start_Lexer (iupac_name, length);

    // Den eigentlichen Parsing-Vorgang durchfuehren und das Wortproblem mittels der Lexer-Daten loesen
    // Verwendeter Algorithmus: Cocke-Younger-Kasami-Algorithmus (CYK-Algorithmus)
    /* Die Chomsky-Normalform:
        S   ->  B2 A
        S   ->  a
        B3  ->  y
        N1  ->  o
        N3  ->  y
        W   ->  n
        Z   ->  z
        M   ->  m
        K2  ->  k
        C   ->  c
        A   ->  a
        B1  ->  Z M
        B1  ->  Z X1
        X1  ->  K M
        B2  ->  B1 X2
        X2  ->  W B3
        B2  ->  B1 X3
        X3  ->  W X4
        X4  ->  N2 X5
        X5  ->  B3 C
        B2  ->  B1 B3
        N2  ->  N1 X6
        X6  ->  B1 N3
        N2  ->  N1 X7
        X7  ->  B1 X8
        X8  ->  N2 X12
        X12 ->  N3 C
        K   ->  K2 X9
        K   ->  K2 Z
        X9  ->  Z K
        B2  ->  B1 X10
        X10 ->  N2 X11
        X11 ->  B3 C
     */
    // Konstanten fuer die Nichtterminale. Gleiche Bezeichnung wie im Kommentarblock
    enum Nonterminalsymbol
    {
        NO_NONTERMINALSYMBOL = -1,
        S = 0,
        B3, N1, N3, W,
        Z, M, K2, C, A,

        B1, X1, B2, X2, X3, X4, X5, N2, X6, X7, X8, K, X9, X10, X11, X12
    };

    struct Production_Rule
    {
        enum Nonterminalsymbol source;                  // Linke Seite
        enum Terminalsymbol terminal_dest;              // Terminalsymbol auf der rechten Seite
                                                        // ODER
        enum Nonterminalsymbol non_terminal_dest_1;     // 1. Nichtterminalsymbol und
        enum Nonterminalsymbol non_terminal_dest_2;     // 2. Nichtterminalsymbol auf der rechten Seite
    };

    // Alle Produktionsregeln
    struct Production_Rule rules [] =
    {
        { S,    NO_TERMINALSYMBOL,  B2,                     A                    },
        { S,    a,                  NO_NONTERMINALSYMBOL,   NO_NONTERMINALSYMBOL },

        { B3,   y,                  NO_NONTERMINALSYMBOL,   NO_NONTERMINALSYMBOL },
        { N1,   o,                  NO_NONTERMINALSYMBOL,   NO_NONTERMINALSYMBOL },
        { N3,   y,                  NO_NONTERMINALSYMBOL,   NO_NONTERMINALSYMBOL },
        { W,    n,                  NO_NONTERMINALSYMBOL,   NO_NONTERMINALSYMBOL },

        { Z,    z,                  NO_NONTERMINALSYMBOL,   NO_NONTERMINALSYMBOL },
        { M,    m,                  NO_NONTERMINALSYMBOL,   NO_NONTERMINALSYMBOL },
        { K2,   k,                  NO_NONTERMINALSYMBOL,   NO_NONTERMINALSYMBOL },
        { C,    c,                  NO_NONTERMINALSYMBOL,   NO_NONTERMINALSYMBOL },
        { A,    a,                  NO_NONTERMINALSYMBOL,   NO_NONTERMINALSYMBOL },

        { B1,   NO_TERMINALSYMBOL,  Z,                      M                    },
        { B1,   NO_TERMINALSYMBOL,  Z,                      X1                   },
        { X1,   NO_TERMINALSYMBOL,  K,                      M                    },
        { B2,   NO_TERMINALSYMBOL,  B1,                     X2                   },
        { X2,   NO_TERMINALSYMBOL,  W,                      B3                   },
        { B2,   NO_TERMINALSYMBOL,  B1,                     X3                   },
        { X3,   NO_TERMINALSYMBOL,  W,                      X4                   },
        { X4,   NO_TERMINALSYMBOL,  N2,                     X5                   },
        { X5,   NO_TERMINALSYMBOL,  B3,                     C                    },
        { B2,   NO_TERMINALSYMBOL,  B1,                     B3                   },
        { N2,   NO_TERMINALSYMBOL,  N1,                     X6                   },
        { X6,   NO_TERMINALSYMBOL,  B1,                     N3                   },
        { N2,   NO_TERMINALSYMBOL,  N1,                     X7                   },
        { X7,   NO_TERMINALSYMBOL,  B1,                     X8                   },
        { X8,   NO_TERMINALSYMBOL,  N2,                     X12                  },
        { X12,  NO_TERMINALSYMBOL,  N3,                     C                    },
        { K,    NO_TERMINALSYMBOL,  K2,                     X9                   },
        { K,    NO_TERMINALSYMBOL,  K2,                     Z                    },
        { X9,   NO_TERMINALSYMBOL,  Z,                      K                    },
        { B2,   NO_TERMINALSYMBOL,  B1,                     X10                  },
        { X10,  NO_TERMINALSYMBOL,  N2,                     X11                  },
        { X11,  NO_TERMINALSYMBOL,  B3,                     C                    }
    };

    // !!! Alle drei Dimensonen: 1 Indexierung !!!
    // 1. Dimension:    Zeile
    // 2. Dimension:    Spalte
    // 3. Dimension:    Menge an Regeln, die fuer die Erzeugung des bzw. der Token verwendet werden koennen
    //                  Hier ist eine Menge erforderlich, da durchaus mehrere Regeln fuer die Erzeugung verwendet werden
    //                  koennen. Alle Regeln muessen fuer den weiteren Verlauf gesichert werden !
    _Bool P [MAX_NUMBER_OF_C_ATOMS + 1][MAX_NUMBER_OF_C_ATOMS + 1][COUNT_ARRAY_ELEMENTS(rules) + 1];
    memset (P, '\0', sizeof (P));
    size_t true_writes = 0;

    // Pseudocode des CYK-Algorithmus
    // Siehe: https://en.wikipedia.org/wiki/CYK_algorithm#As_pseudocode
    // Demo als Hilfe fuer die Implementierung: https://martinlaz.github.io/demos/cky.html
    /*
        let the input be a string I consisting of n characters: a1 ... an.
        let the grammar contain r nonterminal symbols R1 ... Rr, with start symbol R1.
        let P[n,n,r] be an array of booleans. Initialize all elements of P to false.

        for each s = 1 to n
            for each unit production Rv -> as
                set P[1,s,v] = true

        for each l = 2 to n -- Length of span
            for each s = 1 to n-l+1 -- Start of span
                for each p = 1 to l-1 -- Partition of span
                    for each production Ra    -> Rb Rc
                        if P[p,s,b] and P[l-p,s+p,c] then set P[l,s,a] = true

        if P[n,1,1] is true then
            I is member of language
        else
            I is not member of language
     */

    const size_t count_tokens = lexer_data.next_free_token; // Entspricht dem n im Pseudocode

    // Alle Produktionsregeln finden, die das richtige Termialsymbol erzeugen
    // Dies funktioniert nur fuer die 1. Zeile !
    for (size_t s = 0; s < count_tokens; ++ s)
    {
        for (size_t v = 0; v < COUNT_ARRAY_ELEMENTS(rules); ++ v)
        {
            if (rules [v].terminal_dest != NO_TERMINALSYMBOL &&
                    rules [v].terminal_dest == Token_Type_To_Terminalsymbol(lexer_data.token_type [s]))
            {
                P [1][s + 1][v + 1] = true;
                ++ true_writes;
            }
        }
    }

    // ===== BEGINN Die drei Schleifen, die direkt im Pseudocode stehen =====
    for (size_t l = 2; l <= count_tokens; ++ l)
    {
        for (size_t s = 1; s <= (count_tokens - l + 1); ++ s)
        {
            for (size_t p = 1; p <= (l - 1); ++ p)
            {
            // ===== ENDE Die drei Schleifen, die direkt im Pseudocode stehen =====

                // PRINTF_FFLUSH("(%zu, %zu) ", p, l - p);
                // ===== BEGINN Alle moeglichen Kombinationen zweier Produktionsregeln bilden =====
                for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(rules); ++ i)
                {
                    for (size_t i2 = 0; i2 < COUNT_ARRAY_ELEMENTS(rules); ++ i2)
                    {
                    // ===== ENDE Alle moeglichen Kombinationen zweier Produktionsregeln bilden =====

                        // Erinnerung 3. Dimension: Menge an Regeln, die fuer die Erzeugung des bzw. der Token verwendet
                        //                          werden koennen
                        // Gibt es bei der 3. Dimension eine Kombination, wo beide Tokens verwendet werden ?
                        if (P [p][s][i + 1] == true && P [l - p][s + p][i2 + 1] == true)
                        {
                            // Wenn ja: Alle Produktionsregeln durchgehen, um welche zu finden, wo die beiden
                            // Nichtterminalsymbole auf der rechten Seite der linken Nichtterminalsymbole von den beiden
                            // Produktionsregeln, die durch die Kombination gefunden wurden, entsprechen
                            for (size_t i3 = 0; i3 < COUNT_ARRAY_ELEMENTS(rules); ++ i3)
                            {
                                if (rules [i].source == rules [i3].non_terminal_dest_1 &&
                                        rules [i2].source == rules [i3].non_terminal_dest_2)
                                {
                                    P [l][s][i3 + 1] = true;
                                    // PRINTF_FFLUSH("\n>%zu, %zu, %zu<\n", l, s, i3 + 1);
                                    ++ true_writes;
                                }
                            }
                        }

                    }
                }

            }
        }
    }

    _Bool return_value = (P [count_tokens][1][1] /* == true */) ? true : false;

    // !!! Alle drei Dimensonen: 1 Indexierung !!!
    if (P [count_tokens][1][1] /* == true */)
    {
        PRINTF_FFLUSH("%-50s     is in the grammer. (%3zu)\n", lexer_data.alkane_name, true_writes);
    }
    else
    {
        PRINTF_FFLUSH("%-50s is NOT in the grammer. (%3zu)\n", lexer_data.alkane_name, true_writes);
    }

    return return_value;
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

/**
 * @brief Token-Typ (enum Variable) zum passenden Terminalsymbol-Typ konvertieren (ebenfalls eine enum Variable).
 *
 * Damit eine Ueberpruefung des vom Lexers erzeugte Liste an Token-Typen, muessen die enum Token_Type-Objekte nach
 * enum Terminalsymbol konvertiert werden !
 * Dafuer gibt es die Konvertierungsfunktion "Token_Type_To_Terminalsymbol"
 */
static enum Terminalsymbol Token_Type_To_Terminalsymbol (const enum Token_Type token_type)
{
    enum Terminalsymbol result = NO_TERMINALSYMBOL;

    switch (token_type)
    {
    case TOKEN_TYPE_N_A:
        result = NO_TERMINALSYMBOL;
        break;
    case TOKEN_TYPE_NUMBER:
        result = z;
        break;
    case TOKEN_TYPE_ALKYL_WORD:
        result = y;
        break;
    case TOKEN_TYPE_ALKANE_WORD:
        result = a;
        break;
    case TOKEN_TYPE_NUMBER_WORD:
        result = n;
        break;
    case TOKEN_TYPE_COMMA_CHAR:
        result = k;
        break;
    case TOKEN_TYPE_SUB_CHAR:
        result = m;
        break;
    case TOKEN_TYPE_OPEN_BRACKET:
        result = o;
        break;
    case TOKEN_TYPE_CLOSE_BRACKET:
        result = c;
        break;
    default:
        // Die Ausfuehrung des Default-Paths ist hier immer ein Fehler !
        ASSERT_MSG(false, "Switch-Case default path executed !")
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
