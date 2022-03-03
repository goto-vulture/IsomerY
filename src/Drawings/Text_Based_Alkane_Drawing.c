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



/**
 * Die Anzahl eines bestimmten Zeichens in einer Zeichenkette ermitteln und zurueckgeben.
 */
static uint_fast8_t                             // Anzahl der gesuchten Zeichen in der Zeichenkette
Count_Char_In_String
(
        const char* const restrict c_string,    // Zeichenkette, die durchsucht wird
        const size_t string_length,             // Laenge der Zeichenkette
        const char searched_char                // Zu suchendes Zeichen
);



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Eine text-basierte Zeichnung eines Alkans erstellen.
 *
 * Grundsaetziches Vorgehen:
 * - Aufspaltung des Namens an bestimmten Minuszeichnen mithilfe des Chain-Lexers, der urspruenglich fuer die
 *   Testfunktionen geschrieben wurde
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


    struct Text_Based_Alkane_Drawing* drawing = CALLOC(1, sizeof (struct Text_Based_Alkane_Drawing));
    ASSERT_ALLOC(drawing, "Cannot allocate an Text_Based_Alkane_Drawing object !",
            sizeof (struct Text_Based_Alkane_Drawing));

    memcpy (drawing->iupac_name, iupac_name, name_length);

    // "Zeichenflache" mit Leerzeichen fuellen (+ Terminatorsymbol)
    for (size_t i = 0; i < TEXT_BASED_ALKANE_DRAWING_DIM_1; ++ i)
    {
        memset(drawing->drawing [i], ' ', sizeof (char) * (TEXT_BASED_ALKANE_DRAWING_DIM_2 - 1));
        drawing->drawing [i][TEXT_BASED_ALKANE_DRAWING_DIM_2 - 1] = '\0';
    }

    drawing->state = TEXT_BASED_ALKANE_DRAWING_INITIALIZED_WITH_DATA;

    // Name zerlegen
    const struct IUPAC_Chain_Lexer_Result lexer_result = Create_Chain_Tokens (iupac_name);

    // => Schritt 1: Ermittlung der tiefsten Verschachtelung
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

    return drawing;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Eine text-basierte Zeichnung komplett loeschen.
 *
 * Asserts:
 *         alkane != NULL
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

    printf("%s\n", text_based_drawing->iupac_name);
    printf("+");
    for (int i = 0; i < TEXT_BASED_ALKANE_DRAWING_DIM_2 + 2; ++ i)
    {
        printf("-");
    }
    printf("+\n");
    for (int i = 0; i < TEXT_BASED_ALKANE_DRAWING_DIM_1; ++ i)
    {
        printf("| %s |\n", text_based_drawing->drawing [i]);
    }
    printf("+");
    for (int i = 0; i < TEXT_BASED_ALKANE_DRAWING_DIM_2 + 2; ++ i)
    {
        printf("-");
    }
    PUTS_FFLUSH("+");

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Die Anzahl eines bestimmten Zeichens in einer Zeichenkette ermitteln und zurueckgeben.
 */
static uint_fast8_t                             // Anzahl der gesuchten Zeichen in der Zeichenkette
Count_Char_In_String
(
        const char* const restrict c_string,    // Zeichenkette, die durchsucht wird
        const size_t string_length,             // Laenge der Zeichenkette
        const char searched_char                // Zu suchendes Zeichen
)
{
    uint_fast8_t result = 0;

    for (size_t i = 0; i < string_length; ++ i)
    {
        if (c_string [i] == searched_char)
        {
            ++ result;
        }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------
