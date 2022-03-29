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

    // "Zeichenflache" mit Leerzeichen fuellen (+ Terminatorsymbol)
//    for (size_t i = 0; i < TEXT_BASED_ALKANE_DRAWING_DIM_1; ++ i)
//    {
//        memset(drawing->drawing [i], ' ', sizeof (char) * (TEXT_BASED_ALKANE_DRAWING_DIM_2 - 1));
//        drawing->drawing [i][TEXT_BASED_ALKANE_DRAWING_DIM_2 - 1] = '\0';
//    }

    drawing->state = TEXT_BASED_ALKANE_DRAWING_INITIALIZED_WITH_DATA;

    // => Schritt 1: Name zerlegen
    const struct IUPAC_Chain_Lexer_Result lexer_result = Create_Chain_Tokens (iupac_name);
    const uint_fast8_t lexer_next_free_token = lexer_result.next_free_token;

    // => Schritt 2: Laenge der Hauptkette bestimmen
    uint_fast8_t length_main_chain = 0;
    for (uint_fast8_t i = 0; i < NUMBER_OF_ALKAN_WORDS; ++ i)
    {
        if (Compare_Strings_Case_Insensitive (lexer_result.result_tokens [lexer_next_free_token - 1],
                ALKAN_WORDS_DE [i]) == 0)
        {
            length_main_chain = (uint_fast8_t) (i + 1u);
            break;
        }
        if (Compare_Strings_Case_Insensitive (lexer_result.result_tokens [lexer_next_free_token - 1],
                ALKAN_WORDS_EN [i]) == 0)
        {
            length_main_chain = (uint_fast8_t) (i + 1u);
            break;
        }
    }

    // => Schritt 3: Ermittlung der tiefsten Verschachtelung
    // Beschreibung zum verwendeten Lexer:
    //      "Einfacher Lexer, um IUPAC-Namen an den Grenzen der Ketten (OHNE Beachtung von Verschachtelungen) zu
    //       trennen."
    // D.h., dass sich die tiefste Verschachtelung bestimmen laesst indem die hoechste Anzahl an oeffnenden oder
    // schliessenden Klammern in den Tokens verwendet wird.
    uint_fast8_t deepest_nesting = 0;
    for (uint_fast8_t i = 0; i < lexer_result.next_free_token; ++ i)
    {
        const uint_fast8_t char_occurence =
                Count_Char_In_String (lexer_result.result_tokens [i], TEXT_BASED_ALKANE_DRAWING_DIM_2, '(');

        if (char_occurence > deepest_nesting)
        {
            deepest_nesting = char_occurence;
        }
    }

    // => Schritt 4: Hauptkette zeichnen
    char* const drawing_middle_line = drawing->drawing [TEXT_BASED_ALKANE_DRAWING_DIM_1 / 2];
    for (uint_fast8_t i = 0; i < length_main_chain; ++ i)
    {
        strncat (drawing_middle_line, "C", TEXT_BASED_ALKANE_DRAWING_DIM_2 - strlen (drawing_middle_line) - 1);

        if ((i + 1) < length_main_chain)
        {
            strncat (drawing_middle_line, " ", TEXT_BASED_ALKANE_DRAWING_DIM_2 - strlen (drawing_middle_line) - 1);
            for (uint_fast8_t i2 = 0; i2 < deepest_nesting; ++ i2)
            {
                strncat (drawing_middle_line, "-", TEXT_BASED_ALKANE_DRAWING_DIM_2 - strlen (drawing_middle_line) - 1);
            }
            strncat (drawing_middle_line, " ", TEXT_BASED_ALKANE_DRAWING_DIM_2 - strlen (drawing_middle_line) - 1);
        }
    }
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
