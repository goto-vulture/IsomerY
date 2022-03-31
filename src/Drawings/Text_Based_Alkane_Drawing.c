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
            for (uint_fast8_t i2 = 0; i2 < NUMBER_OF_ALKAN_WORDS; ++ i2)
            {
                // Wo stimmt der Token mit einem Alkannamen ueberein ?
                if (Compare_Strings_Case_Insensitive (lexer_data.result_tokens [i], ALKAN_WORDS_DE [i2]) == 0 ||
                        Compare_Strings_Case_Insensitive (lexer_data.result_tokens [i], ALKAN_WORDS_EN [i2]) == 0)
                {
                    main_chain_length = (uint_fast8_t) (i2 + 1);
                    break;
                }
            }
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
    // Hier gibt es grundsaetzlich zwei verschiedene Situationen, die betrachtet werden muessen:
    //  1.
    //  Ein Ast besitzt KEINE weitere Verschachtelung. (Z.B.: 2,3,4-TriMethyl)
    //  Das Ende eines solchen Astes wird erkannt, wenn ein Alkanwort gefunden wurde oder wenn ein Minuszeichen
    //  auftritt, nachdem ein neues noch nicht gezeichnetes Alkylwort entdeckt wurde
    //  2.
    //  ...

    // Nullterminierung garantieren
    drawing_middle_line [TEXT_BASED_ALKANE_DRAWING_DIM_2 - 1] = '\0';

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
