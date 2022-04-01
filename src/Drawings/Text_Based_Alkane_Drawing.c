/**
 * @file Terminal_Alkane_Drawing.c
 *
 * @brief Aus einem IUPAC-Alkannamen eine einfache textbasierte Monofont-Zeichnung erstellen.
 *
 * @date 28.02.2022
 * @author: N450
 */

#include "Text_Based_Alkane_Drawing.h"
#include "../Tests/IUPAC_Chain_Lexer.h"
#include "../Print_Tools.h"
#include "../String_Tools.h"
#include "../Drawings/Alkane_Parser.h"
#include "../str2int.h"
#include "../Misc.h"



enum Direction
{
    N, E, S, W
};



static uint_fast8_t Get_Length_From_Alkane_Token
(
        const char* const token
);
static uint_fast8_t Get_Length_From_Alkyl_Token
(
        const char* const token
);
static uint_fast8_t Count_Branch_Tokens_In_Partition
(
        const enum Token_Type* const token_types,
        const uint_fast8_t start,
        const uint_fast8_t end
);
static void Draw_Branch
(
        struct Text_Based_Alkane_Drawing* const drawing,
        int_fast32_t pos_x,
        int_fast32_t pos_y,
        const enum Direction direction,
        const uint_fast8_t num_c_atoms,
        const uint_fast8_t distance
);
static void Make_Line_Drawable
(
        struct Text_Based_Alkane_Drawing* const drawing,
        const int_fast32_t line
);
static void Adjust_Drawing_For_Terminal
(
        struct Text_Based_Alkane_Drawing* const drawing
);



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Eine text-basierte Zeichnung eines Alkans erstellen.
 *
 * Grundsaetziches Vorgehen:
 * - Aufspaltung des Namens an bestimmten Minuszeichnen mithilfe des Chain-Lexers, der urspruenglich fuer die
 *   Testfunktionen geschrieben wurde
 *
 * - Ermittlung der Laenge der Hauptkette
 *
 * - Ermittlung der tiefsten Verschachtelung
 *
 * - Zeichnung der Hauptkette in der mittleren Zeile (Anhand der tiefsten Verschachtelung wird entschieden wie gross
 *   der Abstand zwischen den C-Atomen sein wird)
 *
 * - Zeichnung aller Aeste, die sich direkt an der Hauptkette befinden und keine Verschachtelung besitzen
 *
 * - Zeichnung aller Aeste, die eine Verschachtelung besitzen. Dabei wird "von Oben nach Unten" die Zeichnung erzeugt.
 *   Stufenweise wird in die Tiefe der Verschachtelung gegangen.
 *
 * Asserts:
 *      iupac_name != NULL
 *      name_length > 0
 *      name_length <= IUPAC_NAME_LENGTH
 *
 * @param[in] iupac_name IUPAC-Name eines Alkans
 * @param[in] name_length Laenge des IUPAC-Namen
 */
extern struct Text_Based_Alkane_Drawing*        // Das Ergebnis (die textbasierte-Zeichnung)
Create_Text_Based_Alkane_Drawing
(
        const char* const restrict iupac_name,  // IUPAC-Name, der gezeichnet werden soll
        const size_t name_length                // Laenge des IUPAC-Namen
)
{
    ASSERT_MSG(iupac_name != NULL,                  "IUPAC name is NULL !");
    ASSERT_MSG(name_length > 0,                     "name_length is 0 !");
    ASSERT_FMSG(name_length <= IUPAC_NAME_LENGTH,   "name_lenght is invalid ! Valid range: %u - %u.",
            1u, IUPAC_NAME_LENGTH)
    ASSERT_FMSG(Parse_Alkane(iupac_name, name_length) /* == true */,
                                                     "\"%s\" is not a valid alkane name !", iupac_name)

    struct Text_Based_Alkane_Drawing* drawing = CALLOC(1, sizeof (struct Text_Based_Alkane_Drawing));
    ASSERT_ALLOC(drawing, "Cannot allocate an Text_Based_Alkane_Drawing object !",
            sizeof (struct Text_Based_Alkane_Drawing));

    memcpy (drawing->iupac_name, iupac_name, name_length);

    drawing->state = TEXT_BASED_ALKANE_DRAWING_INITIALIZED_WITH_DATA;

    // => Schritt 1: Name zerlegen
    const struct Alkane_Lexer lexer_data = Create_Alkane_Tokens (iupac_name, name_length);

    // => Schritt 2: Laenge der Hauptkette bestimmen
    uint_fast8_t main_chain_length = 0;

    for (uint_fast8_t i = 0; i < lexer_data.next_free_token; ++ i)
    {
        if (lexer_data.token_type [i] == TOKEN_TYPE_ALKANE_WORD)
        {
            main_chain_length = Get_Length_From_Alkane_Token (lexer_data.result_tokens [i]);
            break;
        }
    }

    // => Schritt 3: Ermittlung der tiefsten Verschachtelung
    uint_fast8_t deepest_nesting = 0;
    uint_fast8_t branch_opened_bracket = 0;
    for (uint_fast8_t i = 0; i < lexer_data.next_free_token; ++ i)
    {
        if (lexer_data.token_type [i] == TOKEN_TYPE_OPEN_BRACKET)
        {
            ++ branch_opened_bracket;
            // Ist die aktuelle Tiefe der Verschachtelung groesser als die bisher bekannte ?
            if (branch_opened_bracket > deepest_nesting)
            {
                deepest_nesting = branch_opened_bracket;
            }
        }
        else if (lexer_data.token_type [i] == TOKEN_TYPE_CLOSE_BRACKET)
        {
            -- branch_opened_bracket;
        }
    }

    // => Schritt 4: Hauptkette zeichnen
    char* const drawing_middle_line = drawing->drawing [TEXT_BASED_ALKANE_DRAWING_DIM_1 / 2];
    size_t middle_line_size_left = TEXT_BASED_ALKANE_DRAWING_DIM_2 - 1;

    for (uint_fast8_t i = 0; i < main_chain_length; ++ i)
    {
        strncat (drawing_middle_line, "C", middle_line_size_left);
        middle_line_size_left -= strlen ("C");
        if ((i + 1) < main_chain_length)
        {
            strncat (drawing_middle_line, " ", middle_line_size_left);
            middle_line_size_left -= strlen (" ");
            // Anzahl an Minuszeichen anhand der maximalen Verschachtelungstiefe ausrichten, um keine Platzprobleme bei
            // der Zeichnung der Unterketten zu bekommen
            Append_X_Times_Char (drawing_middle_line, '-', (deepest_nesting == 0) ? 1 : deepest_nesting);
            middle_line_size_left -= (size_t) (deepest_nesting + 1);
            strncat (drawing_middle_line, " ", middle_line_size_left);
            middle_line_size_left -= strlen (" ");
        }
    }

    // => Schritt 5: Nebenketten zeichnen
    // Hier gibt es grundsaetzlich zwei verschiedene Situationen, die betrachtet werden muessen. Zu Beginn muessen alle
    // Nebenketten auf oberster Ebene ermittelt werden
    //  1.
    //  Ein Ast besitzt KEINE weitere Verschachtelung. (Z.B.: 2,3,4-TriMethyl)
    //  Das Ende eines solchen Astes wird erkannt, wenn ein Alkanwort gefunden wurde oder wenn ein Minuszeichen
    //  auftritt und man sich gleichzeitig auf oberster Ebene befindet
    //  2.
    //  Ein Ast beendet (alle) seine Verschachtelungen. (Z.B.: ...(2-MethylEthyl)...
    //  oder ...(2-(3-MethylEthyl)Propyl)...

    // Die Indexe sind wiefolgt zu verstehen: Hinter dem X.ten Token beginnt die naechste Nebenkette auf oberster Ebene
    uint_fast8_t branch_end [MAX_NUMBER_OF_C_ATOMS];
    memset (branch_end, '\0', sizeof (branch_end));
    uint_fast8_t next_free_branch_end = 0;
    uint_fast8_t current_nesting_depth = 0;

    for (uint_fast8_t i = 0; i < lexer_data.next_free_token; ++ i)
    {
        if (lexer_data.token_type [i] == TOKEN_TYPE_OPEN_BRACKET)
        {
            ++ current_nesting_depth;
        }
        else if (lexer_data.token_type [i] == TOKEN_TYPE_CLOSE_BRACKET)
        {
            -- current_nesting_depth;
            // 2. Situation eingetreten (Siehe Beschreibung weiter oben) =
            if (current_nesting_depth == 0)
            {
                branch_end [next_free_branch_end] = i;
                ++ next_free_branch_end;
            }
        }
        // 1. Situation eingetreten (Siehe Beschreibung weiter oben) =
        else if ((current_nesting_depth == 0 && lexer_data.token_type [i] == TOKEN_TYPE_ALKYL_WORD) ||
                (lexer_data.token_type [i + 1] == TOKEN_TYPE_ALKANE_WORD))
        {
            branch_end [next_free_branch_end] = i;
            ++ next_free_branch_end;
        }
    }


    current_nesting_depth = 0;
    for (uint_fast8_t i = 0; i < next_free_branch_end; ++ i)
    {
        // "+ 2", da sowohl das Alkyl-Token als auch das Minus nach dem Alkyl-Token ignoriert werden muss
        uint_fast8_t i2 = (i == 0) ? 0 : (uint_fast8_t) (branch_end [i - 1] + 2);

        if (Count_Branch_Tokens_In_Partition (lexer_data.token_type, i2, branch_end [i]) == 0)
        {
            const uint_fast8_t alkyl_length =
                    Get_Length_From_Alkyl_Token(lexer_data.result_tokens [branch_end [i]]);

            while (i2 < branch_end [i] &&
                    (lexer_data.token_type [i2] == TOKEN_TYPE_NUMBER ||
                    lexer_data.token_type [i2] == TOKEN_TYPE_COMMA_CHAR))
            {
                if (lexer_data.token_type [i2] == TOKEN_TYPE_NUMBER)
                {
                    long int position = 0;
                    str2int (&position, lexer_data.result_tokens [i2], 10);

                    // Kette zeichnen
                    const int_fast32_t pos_x = TEXT_BASED_ALKANE_DRAWING_DIM_1 / 2;
                    int_fast32_t pos_y = 0;
                    if (position > 0)
                    {
                        // + 2 fuer die Leerzeichen zwischen dem Minuszeichen !
                        pos_y = position + ((position - 1) * ((deepest_nesting == 0) ? (1 + 2) : (deepest_nesting + 2)));
                        pos_y -= 1; // 0-Indexierung
                    }

                    // Wenn unterhalb des C-Atoms noch Platz ist -> verwenden (S)
                    // Ansonsten ueberhalb des C-Atoms den Platz verwenden (N)
                    const enum Direction draw_direction = (drawing->drawing [pos_x + 1][pos_y] == '\0'
                            || drawing->drawing [pos_x + 1][pos_y] == ' ') ? S : N;

                    // Ast zeichnen
                    Draw_Branch (drawing, pos_x, pos_y, draw_direction, alkyl_length,
                            (deepest_nesting == 0) ? 1 : deepest_nesting);
                }

                ++ i2;
            }
        }
    }

    Adjust_Drawing_For_Terminal (drawing);

    return drawing;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Eine text-basierte Zeichnung komplett loeschen.
 *
 * Asserts:
 *         text_based_drawing != NULL
 *
 * @param[in] text_based_drawing Text-Zeichnung, welches geloescht werden soll
 */
extern void
Delete_Text_Based_Alkane_Drawing
(
        struct Text_Based_Alkane_Drawing* restrict text_based_drawing   // Objekt, welches geloescht werden soll
)
{
    ASSERT_MSG(text_based_drawing != NULL, "drawing is NULL !");

    text_based_drawing->state = TEXT_BASED_ALKANE_DRAWING_DELETED;
    FREE_AND_SET_TO_NULL(text_based_drawing);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Die textbasierte Zeichnung auf stdout ausgegeben.
 *
 * Dabei wird wirklich nur die Zeichenflache (ein zweidimensionales Array) sowie der IUPAC-Name ausgegeben.
 *
 * Asserts:
 *          text_based_drawing != NULL
 *
 * @param[in] Text_Based_Alkane_Drawing, wo die Zeichenflache ausgegeben wird
 */
extern void
Show_Text_Based_Alkane_Drawing
(
        const struct Text_Based_Alkane_Drawing* const restrict text_based_drawing
)
{
    ASSERT_MSG(text_based_drawing != NULL, "drawing is NULL !");

    const char* drawing [TEXT_BASED_ALKANE_DRAWING_DIM_1];
    for (size_t i = 0; i < TEXT_BASED_ALKANE_DRAWING_DIM_1; ++ i)
    {
        drawing [i] = text_based_drawing->drawing [i];
    }

    Print_2D_String_Array(drawing, TEXT_BASED_ALKANE_DRAWING_DIM_1, TEXT_BASED_ALKANE_DRAWING_DIM_2);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Die textbasierte Zeichnung auf stdout ausgegeben. OHNE Zeilen am Anfang und am Ende, die nur aus Leerzeichen
 * bestehen.
 *
 * Asserts:
 *          text_based_drawing != NULL
 *
 * @param[in] Text_Based_Alkane_Drawing, wovon die Zeichenflache ausgegeben wird
 */
extern void
Show_Text_Based_Alkane_Drawing_W_O_Empty_Lines
(
        const struct Text_Based_Alkane_Drawing* const restrict text_based_drawing
)
{
    ASSERT_MSG(text_based_drawing != NULL, "drawing is NULL !");

    const char* drawing [TEXT_BASED_ALKANE_DRAWING_DIM_1];
    uint_fast8_t drawing_index = 0;
    _Bool first_non_empty_line_found = false;
    // Alle leeren Zeilen entfernen
    for (size_t i = 0; i < TEXT_BASED_ALKANE_DRAWING_DIM_1; ++ i)
    {
        const uint_fast8_t spaces_in_line =
                Count_Char_In_String (text_based_drawing->drawing [i], TEXT_BASED_ALKANE_DRAWING_DIM_2 - 1, ' ');

        // Leere Zeile gefunden ?
        if (spaces_in_line == strlen (text_based_drawing->drawing [i]))
        {
            if (! first_non_empty_line_found /* == false */)
            {
                continue;
            }
            else
            {
                break;
            }
        }

        first_non_empty_line_found = true;
        drawing [drawing_index] = text_based_drawing->drawing [i];
        ++ drawing_index;
    }

    Print_2D_String_Array(drawing, drawing_index, TEXT_BASED_ALKANE_DRAWING_DIM_2);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static uint_fast8_t Get_Length_From_Alkane_Token
(
        const char* const token
)
{
    uint_fast8_t alkane_length = 0;

    for (uint_fast8_t i2 = 0; i2 < NUMBER_OF_ALKAN_WORDS; ++ i2)
    {
        // Wo stimmt der Token mit einem Alkannamen ueberein ?
        if (Compare_Strings_Case_Insensitive (token, ALKAN_WORDS_DE [i2]) == 0 ||
                Compare_Strings_Case_Insensitive (token, ALKAN_WORDS_EN [i2]) == 0)
        {
            alkane_length = (uint_fast8_t) (i2 + 1);
            break;
        }
    }

    return alkane_length;
}

//---------------------------------------------------------------------------------------------------------------------

static uint_fast8_t Get_Length_From_Alkyl_Token
(
        const char* const token
)
{
    uint_fast8_t alkyl_length = 0;

    for (uint_fast8_t i2 = 0; i2 < NUMBER_OF_ALKAN_WORDS; ++ i2)
    {
        // Wo stimmt der Token mit einem Alkannamen ueberein ?
        if (Compare_Strings_Case_Insensitive (token, ALKYL_WORDS [i2]) == 0)
        {
            alkyl_length = (uint_fast8_t) (i2 + 1);
            break;
        }
    }

    return alkyl_length;
}

//---------------------------------------------------------------------------------------------------------------------

static uint_fast8_t Count_Branch_Tokens_In_Partition
(
        const enum Token_Type* const token_types,
        const uint_fast8_t start,
        const uint_fast8_t end
)
{
    uint_fast8_t count_branch_tokens = 0;

    for (uint_fast8_t i = start; i < end; ++ i)
    {
        if (token_types [i] == TOKEN_TYPE_OPEN_BRACKET || token_types [i] == TOKEN_TYPE_OPEN_BRACKET)
        {
            ++ count_branch_tokens;
        }
    }

    return count_branch_tokens;
}

//---------------------------------------------------------------------------------------------------------------------

static void Draw_Branch
(
        struct Text_Based_Alkane_Drawing* const drawing,
        int_fast32_t pos_x,
        int_fast32_t pos_y,
        const enum Direction direction,
        const uint_fast8_t num_c_atoms,
        const uint_fast8_t distance
)
{
    int_fast8_t pos_x_change = 0;
    int_fast8_t pos_y_change = 0;
    char draw_char = '\0';

    switch (direction)
    {
    case N:
        pos_x_change = -1;
        draw_char = '|';
        break;
    case E:
        pos_y_change = +1;
        draw_char = '-';
        break;
    case S:
        pos_x_change = +1;
        draw_char = '|';
        break;
    case W:
        pos_y_change = -1;
        draw_char = '-';
        break;
    default:
        // Die Ausfuehrung des Default-Paths ist hier immer ein Fehler !
        ASSERT_MSG(false, "Switch-Case default path executed !")
    }

    for (uint_fast8_t i = 0; i < num_c_atoms; ++ i)
    {
        for (uint_fast8_t i2 = 0; i2 < distance; ++ i2)
        {
            pos_x += pos_x_change;
            pos_y += pos_y_change;
            Make_Line_Drawable (drawing, pos_x);
            drawing->drawing [pos_x][pos_y] = draw_char;
        }
        pos_x += pos_x_change;
        pos_y += pos_y_change;
        Make_Line_Drawable (drawing, pos_x);
        drawing->drawing [pos_x][pos_y] = 'C';
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Wenn die Zeile nur aus Terminatorsymbolen besteht - was der Fall nach der Initialisierung ist - dann wird die Zeile
 * komplett mit Leerzeichen gefuellt, damit diese auf der Konsole ausgegeben werden kann
 */
static void Make_Line_Drawable
(
        struct Text_Based_Alkane_Drawing* const drawing,
        const int_fast32_t line
)
{
    // Alle Zeilen haben die gleiche Laenge
    // COUNT_ARRAY_ELEMENTS arbeitet mit sizeof, sodass die Operation bereits zur Kompilierungszeit durchfuehrbar sein
    // muss !
    const size_t line_length = COUNT_ARRAY_ELEMENTS(drawing->drawing [0]);

    if (Contain_String_Only_Null_Symbols (drawing->drawing [line], line_length) /* == true */)
    {
        memset (drawing->drawing [line], ' ', line_length - 1); // "- 1" !
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Adjust_Drawing_For_Terminal
(
        struct Text_Based_Alkane_Drawing* const drawing
)
{
    const size_t number_of_lines = COUNT_ARRAY_ELEMENTS(drawing->drawing);
    const size_t line_length = COUNT_ARRAY_ELEMENTS(drawing->drawing [0]);

    // Laengste Zeichenkette in der Zeichnung ermitteln
    size_t longest_line = 0;

    for (size_t i = 0; i < number_of_lines; ++ i)
    {
        // Mit der Laenge sind die Anzahl an Zeichen gemeint, bis nur noch Leerzeichen in der Zeichenkette sind
        size_t current_length = line_length - 1;
        for (size_t i2 = line_length - 2; i2 > 0; -- i2)
        {
            if (drawing->drawing [i][i2] == ' ' || drawing->drawing [i][i2] == '\0')
            {
                -- current_length;
            }
            else
            {
                break;
            }
        }

        if (current_length > longest_line)
        {
            longest_line = current_length;
        }
    }
    for (size_t i = 0; i < number_of_lines; ++ i)
    {
        memset (&drawing->drawing [i][longest_line], '\0', line_length - longest_line);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------
