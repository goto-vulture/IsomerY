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



/**
 * Konstanten fuer die Richtung in Zeichenoperationen.
 */
enum Direction
{
    N_A,

    N, E, S, W
};

/**
 * Position und Zeichenrichtung, die beim letzten Zeichenvorgang verwendet wurden.
 */
struct Last_Data
{
    enum Direction last_direction;                  // Letzte verwendete Zeichenrichtung
    int_fast32_t last_first_drawn_c_atom_x_pos;     // Letzte verwendete X Position des ersten gezeichneten C-Atoms
    int_fast32_t last_first_drawn_c_atom_y_pos;     // Letzte verwendete Y Position des ersten gezeichneten C-Atoms
};

/**
 * Ein Leeres Last_Data-Objekt erzeugen. Als Variablenwerte werden Platzhalterwerte verwendet, die eine Nichtverwendung
 * anzeigen.
 */
static struct Last_Data Create_Empty_Last_Data (void);
static struct Last_Data Create_Empty_Last_Data (void)
{
    const struct Last_Data empty_last_data =
    {
            .last_direction = N_A,
            .last_first_drawn_c_atom_x_pos = -1,
            .last_first_drawn_c_atom_y_pos = -1
    };

    return empty_last_data;
}



/**
 * Anzahl von C-Atomen eines Alkan-Strings ermitteln.
 */
static uint_fast8_t                 // Laege des Eingabetokens (Anzahl an C-Atomen)
Get_Length_From_Alkane_Token
(
        const char* const token     // Eingabetoken
);

/**
 * Anzahl von C-Atomen eines Alkyl-Strings ermitteln.
 */
static uint_fast8_t
Get_Length_From_Alkyl_Token         // Laenge des Eingabetoken (Anzahl an C-Atomen)
(
        const char* const token     // Eingabetoken
);

/**
 * Maximale Verschachtelungstiefe im angegebenen Intervall bestimmen.
 */
static uint_fast8_t                                 // Maximale Verschachtelungstiefe im angegebenen Tokenintervall
Determine_Deepest_Nesting_In_Token_Partition
(
        const enum Token_Type* const token_types,   // Token-Typen
        const uint_fast8_t start,                   // Startindex
        const uint_fast8_t end                      // Endindex
);

/**
 * Branch Tokens in einem Array mit einer unteren und oberen Grenze zaehlen und zurueckgeben. Sowohl oeffnende und
 * schliessende Klammern werden gezaehlt.
 */
static uint_fast8_t                                 // Anzahl an Branch-Tokens (Oeffnende und chliessende Klammern)
Count_Branch_Tokens_In_Partition
(
        const enum Token_Type* const token_types,   // Token-Typen
        const uint_fast8_t start,                   // Startindex
        const uint_fast8_t end                      // Endindex
);

/**
 * Die Startposition fuer Zeichenvorgaenge bestimmen.
 */
static void
Calculate_Start_Position
(
        struct Text_Based_Alkane_Drawing* const restrict drawing,   // Drawing-Objekt
        const long int number_token_as_int,                         // Positionstoken bereits zum Integer konvertiert
        const uint_fast8_t deepest_nesting,                         // Tiefste Verschachtelung, die im gesamten Isomer
                                                                    // moeglich ist
        int_fast32_t* const restrict out_pos_x,                     // Ausgabeposition X
        int_fast32_t* const restrict out_pos_y,                     // Ausgabeposition Y

        const struct Last_Data* const last_data                     // Last-Data (wird benoetigt, wenn die Startposition
                                                                    // an einer gezeichneten Verschachtelung bestimmt
                                                                    // werden muss)
);

/**
 * Die Richtung der naechsten Zeichnung bestimmen.
 */
static enum Direction                                       // Ermittelte Zeichenrichtung
Calculate_Direction
(
        struct Text_Based_Alkane_Drawing* const drawing,    // Zeichnungs-Objekt
        const enum Direction last_direction,                // Die letzte verwendete Zeichenrichtung
        const int_fast32_t pos_x,                           // Position X, von wo die Zeichenrichtung bestimmt wird
        const int_fast32_t pos_y                            // Position Y, von wo die Zeichenrichtung bestimmt wird
);

/**
 * Alle notwendigen Informationen, die fuer das Zeichnen eines Astes notwendig sind, bestimmen und dann den Ast
 * zeichnen.
 */
static void
Start_Drawing_Branch
(
        struct Text_Based_Alkane_Drawing* const restrict drawing,   // Zeichnungs-Objekt
        const long int position,                                    // Positionstoken bereits zum Integer konvertiert
        const uint_fast8_t current_nesting_depth,                   // Aktuelle Verschachtelungstiefe
        const uint_fast8_t deepest_nesting,                         // Tiefste Verschachtelung, die im gesamten Isomer
                                                                    // moeglich ist
        const uint_fast8_t alkyl_length,                            // Laenge (Anzahl an C-Atomen) des Alkyl-Token

        struct Last_Data* const restrict last_data                  // Last-Data (wird benoetigt, wenn die Startposition
                                                                    // an einer gezeichneten Verschachtelung bestimmt
                                                                    // werden muss) Verwendung in einer Unterfunktion
);

/**
 * Eine Verschachtelungstiefe tiefer gehen und die Aeste zeichnen.
 */
static void
Go_Deeper_Drawing
(
        const struct Alkane_Lexer* const restrict lexer_data,       // Lexer-Daten
        struct Text_Based_Alkane_Drawing* const restrict drawing,   // Zeichnungs-Objekt
        const uint_fast8_t current_token,                           // Aktuelles Token
        const uint_fast8_t current_nesting_depth,                   // Aktuelle Verschachtelungstife
        const uint_fast8_t deepest_nesting,                         // Tiefste Verschachtelung, die im gesamten Isomer
                                                                    // moeglich ist
        const uint_fast8_t end_token,                               // Letztes Token des aktuellen Astes

        struct Last_Data* const restrict last_data                  // Last-Data (wird benoetigt, wenn die Startposition
                                                                    // an einer gezeichneten Verschachtelung bestimmt
                                                                    // werden muss) Verwendung in einer Unterfunktion
);

/**
 * Einen Ast in die Zeichnung hinzufuegen.
 */
static void
Draw_Branch
(
        struct Text_Based_Alkane_Drawing* const drawing,    // Zeichnungs-Objekt
        int_fast32_t pos_x,                                 // Startposition X fuer das Zeichnen
        int_fast32_t pos_y,                                 // Startposition Y fuer das Zeichnen
        const enum Direction direction,                     // Zeichnungsrichtung
        const uint_fast8_t num_c_atoms,                     // Anzahl an C-Atomen
        const uint_fast8_t distance                         // Distanz (Anzahl an Zeichen zwischen jedem C-Atom)
);

/**
 * Zeilen zeichenbar machen, indem die gesamte Zeile mit Leerzeichen ueberschrieben wird. Dadurch werden die
 * Terminalsymbole - die nach der Initialisierung standardmaessing vorhanden sind - ueberschrieben.
 *
 * Dies ist insbesondere dann notwendig, wenn Zeichen mittig in einer Zeichenkette hinzugefuegt werden. Ohne Anpassung
 * wird die Zeile nur bis zum ersten Terminalsymbol ausgegeben.
 */
static void
Make_Line_Drawable
(
        struct Text_Based_Alkane_Drawing* const drawing,    // Zeichnungs-Objekt
        const int_fast32_t line                             // Zeile, die zeichenbar (ausgebbar) gemacht wird
);

/**
 * Zeichnung fuer die Ausgabe auf das Terminal anpassen.
 *
 * Das Ziel ist, dass Leerzeichen, die sich hinter dem letzten richtig verwendeten Zeichen befinden, mit
 * Terminalsymbolen ueberschrieben werden. Dadurch wird die Ausgabe der Zeichnung uebersichtlicher dargestellt.
 *
 * Beispiel:
 *
 * Die Zeichnung
 *
 *    +--------------------------------------------------+
 *  1 |        C                                         |
 *  2 |        |                                         |
 *  3 |        C   C                                     |
 *  4 |        |   |                                     |
 *  5 |C - C - C - C - C - C                             |
 *  6 |    |   |   |                                     |
 *  7 |    C   C   C                                     |
 *  8 |        |   |                                     |
 *  9 |        C   C                                     |
 *    +--------------------------------------------------+
 *
 * wird zu:
 *
 *    +---------------------+
 *  1 |        C            |
 *  2 |        |            |
 *  3 |        C   C        |
 *  4 |        |   |        |
 *  5 |C - C - C - C - C - C|
 *  6 |    |   |   |        |
 *  7 |    C   C   C        |
 *  8 |        |   |        |
 *  9 |        C   C        |
 *    +---------------------+
 */
static void
Adjust_Drawing_For_Terminal
(
        struct Text_Based_Alkane_Drawing* const drawing     // Zeichnungs-Objekt
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


    // ===== ===== ===== BEGINN Schritt 1: Name zerlegen ===== ===== =====
    // => Schritt 1: Name zerlegen
    const struct Alkane_Lexer lexer_data = Create_Alkane_Tokens (iupac_name, name_length);
    // ===== ===== ===== ENDE Schritt 1: Name zerlegen ===== ===== =====

    // ===== ===== ===== BEGINN Schritt 2: Laenge der Hauptkette bestimmen ===== ===== =====
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
    // ===== ===== ===== ENDE Schritt 2: Laenge der Hauptkette bestimmen ===== ===== =====

    // ===== ===== ===== BEGINN Schritt 3: Ermittlung der tiefsten Verschachtelung ===== ===== =====
    // => Schritt 3: Ermittlung der tiefsten Verschachtelung
    const uint_fast8_t deepest_nesting =
            Determine_Deepest_Nesting_In_Token_Partition (lexer_data.token_type, 0, lexer_data.next_free_token);
    // ===== ===== ===== ENDE Schritt 3: Ermittlung der tiefsten Verschachtelung ===== ===== =====

    // ===== ===== ===== BEGINN Schritt 4: Hauptkette zeichnen ===== ===== =====
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
    // ===== ===== ===== ENDE Schritt 4: Hauptkette zeichnen ===== ===== =====

    // ===== ===== ===== BEGINN Schritt 5: Nebenketten zeichnen ===== ===== =====
    // => Schritt 5: Nebenketten zeichnen
    // Hier gibt es grundsaetzlich zwei verschiedene Situationen, die betrachtet werden muessen. Zu Beginn muessen alle
    // Nebenketten auf oberster Ebene ermittelt werden
    //  1.
    //  Ein Ast besitzt KEINE weitere Verschachtelung. (Z.B.: 2,3,4-TriMethyl)
    //  Das Ende eines solchen Astes wird erkannt, wenn ein Alkanwort gefunden wurde oder wenn ein Minuszeichen
    //  auftritt und man sich gleichzeitig auf oberster Ebene befindet
    //  2.
    //  Ein Ast beendet (alle) seine Verschachtelungen. (Z.B.: ...(2-MethylEthyl)...
    //  oder ...5-(2-(2,2-DiMethylEthyl)Propyl)...

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

    for (uint_fast8_t i = 0; i < next_free_branch_end; ++ i)
    {
        // "+ 2", da sowohl das Alkyl-Token als auch das Minus nach dem Alkyl-Token ignoriert werden muss
        uint_fast8_t i2 = (i == 0) ? 0 : (uint_fast8_t) (branch_end [i - 1] + 2);

        // Wenn der Ast keine Verschachtelungen beinhaltet und nur eine gerade Kette ist
        if (Count_Branch_Tokens_In_Partition (lexer_data.token_type, i2, branch_end [i]) == 0)
        {
            const uint_fast8_t alkyl_length =
                    Get_Length_From_Alkyl_Token (lexer_data.result_tokens [branch_end [i]]);

            while (i2 < branch_end [i] &&
                    (lexer_data.token_type [i2] == TOKEN_TYPE_NUMBER ||
                    lexer_data.token_type [i2] == TOKEN_TYPE_COMMA_CHAR))
            {
                if (lexer_data.token_type [i2] == TOKEN_TYPE_NUMBER)
                {
                    long int position = 0;
                    str2int (&position, lexer_data.result_tokens [i2], 10);
                    struct Last_Data empty_last_data = Create_Empty_Last_Data ();

                    Show_Text_Based_Alkane_Drawing_W_O_Empty_Lines (drawing);
                    Start_Drawing_Branch
                    (
                            drawing, position, current_nesting_depth, deepest_nesting, alkyl_length, &empty_last_data
                    );
                    printf("line %d ->\n", __LINE__);
                    Show_Text_Based_Alkane_Drawing_W_O_Empty_Lines (drawing);
                }

                ++ i2;
            }
        }
        // Wenn der Ast Verschachtelungen beinhaltet und nicht komplett gerade ist
        else
        {
            const uint_fast8_t max_nesting_depth_in_branch =
                    Determine_Deepest_Nesting_In_Token_Partition (lexer_data.token_type, i2, branch_end [i]);
            current_nesting_depth                       = 0;
            struct Last_Data last_data = Create_Empty_Last_Data ();

            // ===== BEGINN Alle Tokens des aktuellen Asts durchlaufen =====
            for (uint_fast8_t current_token = i2; current_token < branch_end [i]; ++ current_token)
            {
                if (lexer_data.token_type [current_token] == TOKEN_TYPE_OPEN_BRACKET)
                {
                    ++ current_nesting_depth;
                    Show_Text_Based_Alkane_Drawing_W_O_Empty_Lines (drawing);
                    Go_Deeper_Drawing (&lexer_data, drawing, current_token, current_nesting_depth, deepest_nesting,
                            branch_end [i], &last_data);
                    printf("line %d ->\n", __LINE__);
                    Show_Text_Based_Alkane_Drawing_W_O_Empty_Lines (drawing);

                    // Innerster Ast gefunden ?
                    if (current_nesting_depth == max_nesting_depth_in_branch)
                    {
                        // "- 2", um an das Alkyl-Token zu kommen
                        // "... Ethyl)Decan" Von "Decan" auf "Ethyl" wechseln
                        const uint_fast8_t alkyl_length =
                                Get_Length_From_Alkyl_Token (lexer_data.result_tokens [branch_end [i] - (2 * max_nesting_depth_in_branch)]);

                        // Schleife verwenden, da die innerste Abzweigung durchaus aus mehreren Fragmenten bestehen kann
                        for (uint_fast8_t end = current_token; end < branch_end [i]; ++ end)
                        {
                            if (lexer_data.token_type [end] == TOKEN_TYPE_NUMBER)
                            {
                                long int position = 0;
                                str2int (&position, lexer_data.result_tokens [end], 10);

                                // In der tiefsten Verschachtelung des aktuellen Astes duerfen die letzten Positions-
                                // und Richtungsinformationen NICHT mehr aktuallisiert werden ! Wenn dies doch
                                // geschieht, dann wird das letzte C-Atom als vorherige Unterkette verwendet, was
                                // natuerlich falsch ist. Schlecht beschrieben; daher ein Beispiel, dann wird's
                                // deutlich was ich meine:
                                //
                                // 4-(1,1-Dimethylethyl)heptane
                                //
                                // OHNE Deaktivierung der Aktuallisierung
                                //    +-------------------------+
                                //  1 |C - C - C - C - C - C - C|
                                //  2 |            |            |
                                //  3 |            C-C          |
                                //  4 |            | |          |
                                //  5 |            C C          |
                                //    +-------------------------+
                                //
                                // MIT Deaktivierung der Aktuallisierung
                                //    +-------------------------+
                                //  1 |C - C - C - C - C - C - C|
                                //  2 |            |            |
                                //  3 |          C-C-C          |
                                //  4 |            |            |
                                //  5 |            C            |
                                //    +-------------------------+
                                struct Last_Data copy_of_last_data = last_data;

                                Show_Text_Based_Alkane_Drawing_W_O_Empty_Lines (drawing);
                                Start_Drawing_Branch
                                (
                                        drawing, position, current_nesting_depth, deepest_nesting, alkyl_length,
                                        &copy_of_last_data
                                );
                                printf("line %d ->\n", __LINE__);
                                Show_Text_Based_Alkane_Drawing_W_O_Empty_Lines (drawing);
                            }
                        }
                    }
                }
            }
            // ===== ENDE Alle Tokens des aktuellen Asts durchlaufen =====
        }
    }
    // ===== ===== ===== ENDE Schritt 5: Nebenketten zeichnen ===== ===== =====

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

/**
 * Anzahl von C-Atomen eines Alkan-Strings ermitteln.
 */
static uint_fast8_t                 // Laege des Eingabetokens (Anzahl an C-Atomen)
Get_Length_From_Alkane_Token
(
        const char* const token     // Eingabetoken
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

/**
 * Anzahl von C-Atomen eines Alkyl-Strings ermitteln.
 */
static uint_fast8_t
Get_Length_From_Alkyl_Token         // Laenge des Eingabetoken (Anzahl an C-Atomen)
(
        const char* const token     // Eingabetoken
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

/**
 * Maximale Verschachtelungstiefe im angegebenen Intervall bestimmen.
 */
static uint_fast8_t                                 // Maximale Verschachtelungstiefe im angegebenen Tokenintervall
Determine_Deepest_Nesting_In_Token_Partition
(
        const enum Token_Type* const token_types,   // Token-Typen
        const uint_fast8_t start,                   // Startindex
        const uint_fast8_t end                      // Endindex
)
{
    uint_fast8_t deepest_nesting = 0;
    uint_fast8_t branch_opened_bracket = 0;
    for (uint_fast8_t i = start; i < end; ++ i)
    {
        if (token_types [i] == TOKEN_TYPE_OPEN_BRACKET)
        {
            ++ branch_opened_bracket;
            // Ist die aktuelle Tiefe der Verschachtelung groesser als die bisher bekannte ?
            if (branch_opened_bracket > deepest_nesting)
            {
                deepest_nesting = branch_opened_bracket;
            }
        }
        else if (token_types [i] == TOKEN_TYPE_CLOSE_BRACKET)
        {
            -- branch_opened_bracket;
        }
    }

    return deepest_nesting;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Branch Tokens in einem Array mit einer unteren und oberen Grenze zaehlen und zurueckgeben. Sowohl oeffnende und
 * schliessende Klammern werden gezaehlt.
 */
static uint_fast8_t                                 // Anzahl an Branch-Tokens (Oeffnende und chliessende Klammern)
Count_Branch_Tokens_In_Partition
(
        const enum Token_Type* const token_types,   // Token-Typen
        const uint_fast8_t start,                   // Startindex
        const uint_fast8_t end                      // Endindex
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

/**
 * Die Startposition fuer Zeichenvorgaenge bestimmen.
 */
static void
Calculate_Start_Position
(
        struct Text_Based_Alkane_Drawing* const restrict drawing,   // Drawing-Objekt
        const long int number_token_as_int,                         // Positionstoken bereits zum Integer konvertiert
        const uint_fast8_t deepest_nesting,                         // Tiefste Verschachtelung, die im gesamten Isomer
                                                                    // moeglich ist
        int_fast32_t* const restrict out_pos_x,                     // Ausgabeposition X
        int_fast32_t* const restrict out_pos_y,                     // Ausgabeposition Y

        const struct Last_Data* const last_data                     // Last-Data (wird benoetigt, wenn die Startposition
                                                                    // an einer gezeichneten Verschachtelung bestimmt
                                                                    // werden muss)
)
{
    // Wenn es bisher noch keine Zeichnung - ausser der Hauptkette - gab
    if (last_data->last_direction == N_A && last_data->last_first_drawn_c_atom_x_pos == -1 &&
            last_data->last_first_drawn_c_atom_y_pos == -1)
    {
        *out_pos_x = TEXT_BASED_ALKANE_DRAWING_DIM_1 / 2;
        *out_pos_y = number_token_as_int;
        // + 2 fuer die Leerzeichen zwischen dem Minuszeichen !
        *out_pos_y += (number_token_as_int - 1) * ((deepest_nesting == 0) ? (1 + 2) : (deepest_nesting + 2));
        *out_pos_y -= 1; // 0-Indexierung
    }
    else
    {
        // Anhand der Positionsangabe, die sich auf die Unterkette bezieht, das C-Atom, wo das aktuelle
        // Fragment angebracht wird, ermitteln
        *out_pos_x = last_data->last_first_drawn_c_atom_x_pos;
        *out_pos_y = last_data->last_first_drawn_c_atom_y_pos;
        long int viewed_c_atom = 0;

        while (viewed_c_atom != number_token_as_int)
        {
            int_fast8_t pos_x_change = 0;
            int_fast8_t pos_y_change = 0;
            switch (last_data->last_direction)
            {
            case N:
                pos_x_change = -1;
                break;
            case E:
                pos_y_change = +1;
                break;
            case S:
                pos_x_change = +1;
                break;
            case W:
                pos_y_change = -1;
                break;
            case N_A:
                ASSERT_MSG(false, "Default direction (N_A) used in switch case !")
                break;
            default:
                // Die Ausfuehrung des Default-Paths ist hier immer ein Fehler !
                ASSERT_MSG(false, "Switch-Case default path executed !")
            }
            *out_pos_x += pos_x_change;
            *out_pos_y += pos_y_change;

            if (drawing->drawing [*out_pos_x][*out_pos_y] == 'C')
            {
                ++ viewed_c_atom;
            }
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Die Richtung der naechsten Zeichnung bestimmen.
 */
static enum Direction                                       // Ermittelte Zeichenrichtung
Calculate_Direction
(
        struct Text_Based_Alkane_Drawing* const drawing,    // Zeichnungs-Objekt
        const enum Direction last_direction,                // Die letzte verwendete Zeichenrichtung
        const int_fast32_t pos_x,                           // Position X, von wo die Zeichenrichtung bestimmt wird
        const int_fast32_t pos_y                            // Position Y, von wo die Zeichenrichtung bestimmt wird
)
{
    enum Direction result = N_A;

    if (last_direction == N_A)
    {
        // Wenn unterhalb des C-Atoms noch Platz ist -> verwenden (S)
        // Ansonsten oberhalb des C-Atoms den Platz verwenden (N)
        result = (drawing->drawing [pos_x + 1][pos_y] == '\0' || drawing->drawing [pos_x + 1][pos_y] == ' ') ? S : N;
    }
    else
    {
        switch (last_direction)
        {
        case N:
        case S:
            result = (drawing->drawing [pos_x][pos_y + 1] == ' ') ? E : W;
            break;
        case E:
        case W:
            result = (drawing->drawing [pos_x + 1][pos_y + 1] == ' ') ? S : N;
            break;
        case N_A:
            ASSERT_MSG(false, "Default direction (N_A) used in switch case !")
            break;
        default:
            // Die Ausfuehrung des Default-Paths ist hier immer ein Fehler !
            ASSERT_MSG(false, "Switch-Case default path executed !")
        }
    }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alle notwendigen Informationen, die fuer das Zeichnen eines Astes notwendig sind, bestimmen und dann den Ast
 * zeichnen.
 */
static void
Start_Drawing_Branch
(
        struct Text_Based_Alkane_Drawing* const restrict drawing,   // Zeichnungs-Objekt
        const long int position,                                    // Positionstoken bereits zum Integer konvertiert
        const uint_fast8_t current_nesting_depth,                   // Aktuelle Verschachtelungstiefe
        const uint_fast8_t deepest_nesting,                         // Tiefste Verschachtelung, die im gesamten Isomer
                                                                    // moeglich ist
        const uint_fast8_t alkyl_length,                            // Laenge (Anzahl an C-Atomen) des Alkyl-Token

        struct Last_Data* const restrict last_data                  // Last-Data (wird benoetigt, wenn die Startposition
                                                                    // an einer gezeichneten Verschachtelung bestimmt
                                                                    // werden muss) Verwendung in einer Unterfunktion
)
{
    const uint_fast8_t distance_between_c_atoms = (uint_fast8_t) (deepest_nesting - current_nesting_depth + 1);

    int_fast32_t pos_x              = -1;
    int_fast32_t pos_y              = -1;
    enum Direction draw_direction   = N_A;

    // Uebergabezeiger werden das erste Mal verwendet
    if (last_data->last_direction == N_A)
    {
        const struct Last_Data empty_last_data = Create_Empty_Last_Data ();

        Calculate_Start_Position (drawing, position, deepest_nesting, &pos_x, &pos_y, &empty_last_data);
        draw_direction = Calculate_Direction (drawing, N_A, pos_x, pos_y);
    }
    else
    {
        Calculate_Start_Position (drawing, position, deepest_nesting, &pos_x, &pos_y, last_data);
        draw_direction = Calculate_Direction (drawing, last_data->last_direction, pos_x, pos_y);
    }

    Draw_Branch (drawing, pos_x, pos_y, draw_direction, alkyl_length, distance_between_c_atoms);

    last_data->last_direction = draw_direction;
    last_data->last_first_drawn_c_atom_x_pos = pos_x;
    last_data->last_first_drawn_c_atom_y_pos = pos_y;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Eine Verschachtelungstiefe tiefer gehen und die Aeste zeichnen.
 */
static void
Go_Deeper_Drawing
(
        const struct Alkane_Lexer* const restrict lexer_data,       // Lexer-Daten
        struct Text_Based_Alkane_Drawing* const restrict drawing,   // Zeichnungs-Objekt
        const uint_fast8_t current_token,                           // Aktuelles Token
        const uint_fast8_t current_nesting_depth,                   // Aktuelle Verschachtelungstife
        const uint_fast8_t deepest_nesting,                         // Tiefste Verschachtelung, die im gesamten Isomer
                                                                    // moeglich ist
        const uint_fast8_t end_token,                               // Letztes Token des aktuellen Astes

        struct Last_Data* const restrict last_data                  // Last-Data (wird benoetigt, wenn die Startposition
                                                                    // an einer gezeichneten Verschachtelung bestimmt
                                                                    // werden muss) Verwendung in einer Unterfunktion
)
{
    uint_fast8_t close_brackets_found = 0;

    // Von hinten die Tokens durchgehen
    for (uint_fast8_t current_token_reversed = end_token; current_token_reversed >= current_token;
            -- current_token_reversed)
    {
        if (lexer_data->token_type [current_token_reversed] == TOKEN_TYPE_CLOSE_BRACKET)
        {
            ++ close_brackets_found;

            // Passendes Alkyl-Token gefunden ?
            if (current_nesting_depth == close_brackets_found)
            {
                // "- 1", da das passende Alkyl-Token immer vor der zugehoerigen schliessenden Klammer
                // ist
                const uint_fast8_t alkyl_length =
                        Get_Length_From_Alkyl_Token (lexer_data->result_tokens [current_token_reversed - 1]);
                long int position = 0;
                str2int (&position, lexer_data->result_tokens [current_token - 2], 10);

                Start_Drawing_Branch
                (
                        drawing, position, current_nesting_depth, deepest_nesting, alkyl_length, last_data
                );
            }
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Einen Ast in die Zeichnung hinzufuegen.
 */
static void
Draw_Branch
(
        struct Text_Based_Alkane_Drawing* const drawing,    // Zeichnungs-Objekt
        int_fast32_t pos_x,                                 // Startposition X fuer das Zeichnen
        int_fast32_t pos_y,                                 // Startposition Y fuer das Zeichnen
        const enum Direction direction,                     // Zeichnungsrichtung
        const uint_fast8_t num_c_atoms,                     // Anzahl an C-Atomen
        const uint_fast8_t distance                         // Distanz (Anzahl an Zeichen zwischen jedem C-Atom)
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
    case N_A:
        ASSERT_MSG(false, "Default direction (N_A) used in switch case !")
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
 * Zeilen zeichenbar machen, indem die gesamte Zeile mit Leerzeichen ueberschrieben wird. Dadurch werden die
 * Terminalsymbole - die nach der Initialisierung standardmaessing vorhanden sind - ueberschrieben.
 *
 * Dies ist insbesondere dann notwendig, wenn Zeichen mittig in einer Zeichenkette hinzugefuegt werden. Ohne Anpassung
 * wird die Zeile nur bis zum ersten Terminalsymbol ausgegeben.
 */
static void
Make_Line_Drawable
(
        struct Text_Based_Alkane_Drawing* const drawing,    // Zeichnungs-Objekt
        const int_fast32_t line                             // Zeile, die zeichenbar (ausgebbar) gemacht wird
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

/**
 * Zeichnung fuer die Ausgabe auf das Terminal anpassen.
 *
 * Das Ziel ist, dass Leerzeichen, die sich hinter dem letzten richtig verwendeten Zeichen befinden, mit
 * Terminalsymbolen ueberschrieben werden. Dadurch wird die Ausgabe der Zeichnung uebersichtlicher dargestellt.
 *
 * Beispiel:
 *
 * Die Zeichnung
 *
 *    +--------------------------------------------------+
 *  1 |        C                                         |
 *  2 |        |                                         |
 *  3 |        C   C                                     |
 *  4 |        |   |                                     |
 *  5 |C - C - C - C - C - C                             |
 *  6 |    |   |   |                                     |
 *  7 |    C   C   C                                     |
 *  8 |        |   |                                     |
 *  9 |        C   C                                     |
 *    +--------------------------------------------------+
 *
 * wird zu:
 *
 *    +---------------------+
 *  1 |        C            |
 *  2 |        |            |
 *  3 |        C   C        |
 *  4 |        |   |        |
 *  5 |C - C - C - C - C - C|
 *  6 |    |   |   |        |
 *  7 |    C   C   C        |
 *  8 |        |   |        |
 *  9 |        C   C        |
 *    +---------------------+
 */
static void
Adjust_Drawing_For_Terminal
(
        struct Text_Based_Alkane_Drawing* const drawing     // Zeichnungs-Objekt
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

        // Garantierte Nullterminierung jeder Zeichenkette
        drawing->drawing [i][line_length - 1] = '\0';
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------
