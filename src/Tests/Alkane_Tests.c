/**
 * @file Alkane_Tests.c
 *
 * @brief In dieser Uebersetzungseinheit werden viele Tests der Alkanerstellung und der Erstellung von IUPAC-Namen
 * durchgefuehrt.
 *
 * @date 13.03.2021
 * @author x86 / Gyps
 */

#include "Alkane_Tests.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "../Alkane/Alkane.h"
#include "../Alkane/Alkane_Branch.h"
#include "../Alkane/Alkane_Container.h"
#include "../Alkane/Alkane_Create_Constitutional_Isomer.h"
#include "../Alkane/Alkane_Info_Constitutional_Isomer.h"
#include "../Alkane/Alkane_To_IUPAC_Name.h"
#include "../Alkane/Alkane_Chain_To_IUPAC_Name.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Error_Handling/Assert_Msg.h"
#include "../Drawings/Text_Based_Alkane_Drawing.h"
#include "../Drawings/Alkane_Parser.h"
#include "../Print_Tools.h"
#include "../Misc.h"
#include "../CLI_Parameter.h"
#include "../str2int.h"
#include "IUPAC_Chain_Lexer.h"
#include "../String_Tools.h"
#include "../Drawings/To_XPM.h"



#ifndef MAX_WRONG_RESULTS
#define MAX_WRONG_RESULTS 50
#else
#error "The macro \"MAX_WRONG_RESULTS\" is already defined !"
#endif /* MAX_WRONG_RESULTS */

#ifndef MAX_LOOP_NESTING
#define MAX_LOOP_NESTING 6
#else
#error "The macro \"MAX_LOOP_NESTING\" is already defined !"
#endif /* MAX_LOOP_NESTING */

// Makro, um aus dem Funktionsnamen automatisch die passende Zeichenkette zu generieren
#ifndef CREATE_Test_Function_And_Their_Name
#define CREATE_Test_Function_And_Their_Name(function) { (function), (#function) }
#else
#error "The macro \"CREATE_Test_Function_And_Their_Name\" is already defined !"
#endif /* CREATE_Test_Function_And_Their_Name */

// Makro, um alle Testfunktionen, die - mit Aussnahme der Testfunktion, die alle anderen Testfunktionen aufruft -
// auszufuehren
#ifndef CALL_ALL_TEST_FUNCTIONS
#define CALL_ALL_TEST_FUNCTIONS(test_function_array)                                                                    \
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(test_function_array) - 1; ++ i)                                         \
    {                                                                                                                   \
        RUN_2(test_function_array [i].test_function, test_function_array [i].function_name);                            \
    }
#else
#error "The macro \"CALL_ALL_TEST_FUNCTIONS\" is already defined !"
#endif /* CALL_ALL_TEST_FUNCTIONS */

#ifndef READ_BUFFER_SIZE
#define READ_BUFFER_SIZE 100000
#else
#error "The macro \"READ_BUFFER_SIZE\" is already defined !"
#endif /* READ_BUFFER_SIZE */



/**
 * Einlesefunktion fuer die Auswahl der Testfunktion.
 */
static long int                                         // Durchgefuehrte Eingabe
Get_Terminal_Input
(
    const size_t num_testfunctions,                     // Anzahl an Testfunktionen (wird fuer die Eingabeueberpruefung
                                                        // benoetigt)
    const long int cli_input_use_all_testfunctions      // Konstante die den Wert anzeigt, der fuer die Ausfuehrung
                                                        // ALLER Testfunktionen verwendet wird
);

/**
 * Vergleichen eines Zahlencodes eines Akans mit einem vorgegebenen Ergebnis.
 */
static _Bool                                            // true, falls der komplette Zahlencode uebereinstimmt;
                                                        // ansonsten false
Compare_Alkane_Numbercodes
(
        const struct Alkane* const restrict alkane,     // Alkan, dessen Zahlencode fuer den Vergleich verwendet wird
        const unsigned char numbercode []               // Zahlencode mit denen das Alkan-Objekt verglichen wird
);

/**
 * Ueberpruefung, ob der uebergebene IUPAC-Name in den erwarteten Loesungen vorkommt. Die Zeichenketten werden OHNE
 * Beachtung der Gross- und Kleinschreibung miteinander verglichen.
 *
 * Von der Funktionalitaet stellt die Funktion "strncasecmp" aus der Standard-Lib das gleiche zur Verfuegung. Allerdings
 * ist dies eine GNU-Erweiterung und deren Verwendung macht den Code u.U. nicht mehr portabel. -> NICHT VERWENDEN !
 */
static _Bool                                            // true, falls der IUPAC-Name in den erwarteten Loesungen
                                                        // vorkommt, ansonsten false
Search_IUPAC_Name_In_The_List_Of_Expected_Results
(
        const char* const restrict iupac_name,          // IUPAC-Name, der in den erwarteten Loesungen gesucht werden
                                                        // soll
        const char* const restrict expected_results [], // Erwartete Loesungen
        const uint_fast64_t number_of_expected_results, // Anzahl an erwarteten Loesungen
        uint_fast64_t* const restrict result_index      // Index des gefundenen IUPAC-Namen in den erwarteten Loesungen
);

/**
 * Exponentialfunktion fuer Ganzzahlen.
 */
static size_t
Pow_Int
(
        const size_t base,      // Basis
        const size_t exponent   // Exponent
);

/**
 * Alkane und die IUPAC-Namen mit einer vorgegebenen Anzahl an C-Atomen erzeugen und schauen ob die erzeugten
 * IUPAC-Namen in der Liste an erwarteten Loesungen vorhanden sind.
 */
static void
Execute_Creation_Test_With_Expected_Results
(
        const uint_fast8_t number_of_c_atoms,                   // Anzahl an C-Atomen
        const uint_fast64_t number_of_constitutional_isomers,   // Anzahl an Konstitutionsisomeren
        const char* const restrict expected_results [],         // Liste der erwarteten Loesungen
        const size_t number_of_expected_results                 // Anzahl an erwarteten Loesungen
);

/**
 * Erwartete textbasierte Zeichnung mit einer erzeugten Zeichnung vergleichen.
 */
static _Bool
Compare_Expected_Drawing_With_Created_Drawing
(
        const char* const* restrict expected_drawing,                               // Erwartete textbasierte Zeichnung
        const struct Text_Based_Alkane_Drawing* const restrict created_drawing,     // Erzeugte textbasierte Zeichnung
        const size_t expected_drawing_dim_1,                                        // 1. Dimension erwartetes Ergebnis
        const size_t expected_drawing_dim_2,                                        // 2. Dimension erwartetes Ergebnis
        const size_t created_drawing_dim_1,                                         // 1. Dimension erzeugtes Ergebnis
        const size_t created_drawing_dim_2                                          // 2. Dimension erzeugtes Ergebnis
);

/**
 * Zwei XPM-Dateien byteweise miteinander vergleichen.
 */
static _Bool
Compare_Two_XPM_Drawings
(
        const char* const restrict reference_file,          // Datei 1
        const char* const restrict test_file,               // Datei 2
        int_fast32_t* const restrict first_error_position   // Erste Position, wo die Dateien nicht uebereinstimmen
);

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alle Alkan-Tests ausfuehren.
 */
extern int                          // 0, falls alle Tests erfolgreich waren; -1, falls mindestens ein Test fehlschlug
Execute_All_Alkane_Tests
(void)
{
    // Testfunktion und deren Name als Zeichenkette
    struct Test_Function_And_Their_Name
    {
        void (*test_function) (void);
        const char* const function_name;
    };

    // Alle Testfunktionen fuer die Alkanerzeugung
    struct Test_Function_And_Their_Name test_functions [] =
    {
            CREATE_Test_Function_And_Their_Name(TEST_All_Possible_Methan_Constitutional_Isomers),
            CREATE_Test_Function_And_Their_Name(TEST_All_Possible_Ethan_Constitutional_Isomers),
            CREATE_Test_Function_And_Their_Name(TEST_All_Possible_Propan_Constitutional_Isomers),
            CREATE_Test_Function_And_Their_Name(TEST_All_Possible_Butan_Constitutional_Isomers),
            CREATE_Test_Function_And_Their_Name(TEST_All_Possible_Pentan_Constitutional_Isomers),
            CREATE_Test_Function_And_Their_Name(TEST_All_Possible_Hexan_Constitutional_Isomers),
            CREATE_Test_Function_And_Their_Name(TEST_All_Possible_Heptan_Constitutional_Isomers),
            CREATE_Test_Function_And_Their_Name(TEST_All_Possible_Octan_Constitutional_Isomers),
            CREATE_Test_Function_And_Their_Name(TEST_All_Possible_Nonan_Constitutional_Isomers),
            CREATE_Test_Function_And_Their_Name(TEST_All_Possible_Decan_Constitutional_Isomers),

            CREATE_Test_Function_And_Their_Name(TEST_All_Possible_Undecan_Constitutional_Isomers),
            CREATE_Test_Function_And_Their_Name(TEST_All_Possible_Dodecan_Constitutional_Isomers),
            CREATE_Test_Function_And_Their_Name(TEST_All_Possible_Tridecan_Constitutional_Isomers),
            CREATE_Test_Function_And_Their_Name(TEST_All_Possible_Tetradecan_Constitutional_Isomers),
            CREATE_Test_Function_And_Their_Name(TEST_Group_Compression),

            CREATE_Test_Function_And_Their_Name(TEST_Alkane_Lexer),
            CREATE_Test_Function_And_Their_Name(TEST_Alkane_Parser),

            CREATE_Test_Function_And_Their_Name(TEST_Text_Based_Alkane_Drawing_1),
            CREATE_Test_Function_And_Their_Name(TEST_Text_Based_Alkane_Drawing_2),
            CREATE_Test_Function_And_Their_Name(TEST_Convert_Text_Based_Alane_Drawing_To_XPM),

            CREATE_Test_Function_And_Their_Name(TEST_IUPAC_Chain_Lexer_1),
            CREATE_Test_Function_And_Their_Name(TEST_IUPAC_Chain_Lexer_2),

            CREATE_Test_Function_And_Their_Name(TEST_Use_All_Testfunctions)
    };

    // Positionen, wo bei der Uebersicht Newlines ausgegeben werden sollen. Dies dient fuer eine bessere Uebersicht
    const size_t newline_positions [] = { 14, 17, COUNT_ARRAY_ELEMENTS(test_functions) - 1 };

    #ifndef CLI_INPUT_USE_ALL_TESTFUNCTIONS
    #define CLI_INPUT_USE_ALL_TESTFUNCTIONS COUNT_ARRAY_ELEMENTS(test_functions)
    #else
    #error "The macro \"CLI_INPUT_USE_ALL_TESTFUNCTIONS\" is already defined !"
    #endif /* CLI_INPUT_USE_ALL_TESTFUNCTIONS */

    // ===== ===== BEGINN CLI-Parameter: Eine bestimmte Testfunktion nach einer Auswahl ausfuehren ===== =====
    // Soll eine Testfunktion anhand einer dynamischen Auswahl ausgefuehrt werden ?
    if (GLOBAL_SELECT_TEST_FUNCTION /* == true */)
    {
        puts("===>>> Testfunctions available <<<===");
        for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(test_functions); ++ i)
        {
            // Newlines an passende Positionen ausgeben
            for (size_t i2 = 0; i2 < COUNT_ARRAY_ELEMENTS(newline_positions); ++ i2)
            {
                if (newline_positions [i2] == i) { PRINT_NEWLINE; break; }
            }
            printf ("%2zu: %s\n", i + 1, test_functions[i].function_name);
        }
        fflush(stdout);

        const long int selected_test_function = Get_Terminal_Input (COUNT_ARRAY_ELEMENTS(test_functions),
                CLI_INPUT_USE_ALL_TESTFUNCTIONS);

        PRINTF_FFLUSH("\nUsing the test function \"%s\"\n", test_functions [selected_test_function - 1].function_name);

        // Testfunktion aufrufen
        if (selected_test_function != COUNT_ARRAY_ELEMENTS(test_functions))
        {
            RUN_2(test_functions [selected_test_function - 1].test_function,
                    test_functions [selected_test_function - 1].function_name);
        }
        else
        {
            // ALLE Testfunktionen nacheinander ausfuehren
            CALL_ALL_TEST_FUNCTIONS(test_functions)
        }
    }
    // ===== ===== ENDE CLI-Parameter: Eine bestimmte Testfunktion nach einer Auswahl ausfuehren ===== =====

    // ===== ===== BEGINN CLI-Parameter: Alle Testfunktionen ausfuehren ===== =====
    if (GLOBAL_RUN_ALL_TEST_FUNCTIONS /* == true */)
    {
        // ALLE Testfunktionen nacheinander ausfuehren
        CALL_ALL_TEST_FUNCTIONS(test_functions)
    }
    // ===== ===== ENDE CLI-Parameter: Alle Testfunktionen ausfuehren ===== =====

    // ===== ===== BEGINN CLI-Parameter: Konstitutionsisomere mit eingegebener Anzahl an C-Atomen erzeugen ===== =====
    // Testfunktion anhand der Anzahl an C-Atomen, die eingegeben worden sind, aufrufen
    if (GLOBAL_MAX_C_ATOMS_FOR_TESTS != 0)
    {
        RUN_2(test_functions [GLOBAL_MAX_C_ATOMS_FOR_TESTS - 1].test_function,
                test_functions [GLOBAL_MAX_C_ATOMS_FOR_TESTS - 1].function_name);
    }
    // ===== ===== ENDE CLI-Parameter: Konstitutionsisomere mit eingegebener Anzahl an C-Atomen erzeugen ===== =====
    if (GLOBAL_RUN_TEST_FUNCTION_INDEX != -1)
    {
        RUN_2(test_functions [GLOBAL_RUN_TEST_FUNCTION_INDEX - 1].test_function,
                test_functions [GLOBAL_RUN_TEST_FUNCTION_INDEX - 1].function_name);
    }

    #ifdef CLI_INPUT_USE_ALL_TESTFUNCTIONS
    #undef CLI_INPUT_USE_ALL_TESTFUNCTIONS
    #endif /* CLI_INPUT_USE_ALL_TESTFUNCTIONS */

    // Ergebnisse aller durchgefuehrten Tests abfragen
    return TEST_REPORT();
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Die Erstellung von Alkanen aus Alkanaesten erstellen.
 *
 * Wird der Nummerncode richtig gebildet ?
 */
extern void TEST_Create_Alkane (void)
{
    /**
     * 24 C-Atome
     * Bis zu dreifache Verschachtelungstiefe.
     *
     * C - C - C - C - C - C - C - C - C - C - C - C - C
     *                         |
     *                     C   C - C - C
     *                     |   |   |
     *                 C - C - C   C
     *                         |
     *                         C
     *                         |
     *                         C
     *                         |
     *                         C
     */
    const unsigned char branch_1_content [] =
    { 1, 1, 2, 3, 4, 5 };
    const unsigned char branch_2_content [] =
    { 1, 1, 2, 3, 4, 5, 6 };
    const unsigned char branch_3_content [] =
    { 1, 1, 2, 3, 4, 1, 6, 6, 2, 9, 9 };

    const unsigned char expected_result [] =
    { 1, 1, 2, 3, 4, 5, 1, 7, 8, 9, 10, 11, 12, 1, 14, 15, 16, 17, 14, 19, 19, 15, 22, 22 };

    struct Alkane_Branch* branch_1 = Create_Alkane_Branch(branch_1_content, COUNT_ARRAY_ELEMENTS(branch_1_content));
    struct Alkane_Branch* branch_2 = Create_Alkane_Branch(branch_2_content, COUNT_ARRAY_ELEMENTS(branch_2_content));
    struct Alkane_Branch* branch_3 = Create_Alkane_Branch(branch_3_content, COUNT_ARRAY_ELEMENTS(branch_3_content));

    struct Alkane* alkane = Create_Alkane(branch_1, branch_2, branch_3, NULL);
    // Print_Alkane (alkane);

    ASSERT_EQUALS(true, Compare_Alkane_Numbercodes (alkane, expected_result));

    Delete_Alkane(alkane);
    Delete_Alkane_Branch(branch_1);
    Delete_Alkane_Branch(branch_2);
    Delete_Alkane_Branch(branch_3);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Die Erstellung der Alkane testen. Vorerst nur ueber die Anzahl an Ergebnissen.
 * Die Anzahl an Ergebnissen wird mit der Anzahl an moeglichen Konstitutionsisomeren verglichen. Wenn die Werte gleich
 * sind, dann ist ein Fehler in der Berechnung relativ unwahrscheinlich.
 *
 * Die Liste an moeglichen Konstitutionsisomeren bis 40 sind in der Datei "Alkane_Info_Constitutional_Isomer.h" zu
 * finden.
 *
 * Aus Laufzeitgruenden wird nur bis inklusive 11 getestet.
 */
extern void TEST_Create_Alkane_Constitutional_Isomers (void)
{
    for (uint_fast8_t i = 1; i <= 11; ++ i)
    {
        struct Alkane_Container* result_container = Create_Alkane_Constitutional_Isomers (i);

        ASSERT_EQUALS(NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [i - 1], result_container->size);

        Delete_Alkane_Container (result_container);
        result_container = NULL;
    }

    return;
}

//--------------------------------------------------------------------------------------------------------------------

/**
 * Die Erstellung des IUPAC-Namen aus einem Alkan testen.
 *
 * Wird der IUPAC-Name richtig gebildet ?
 */
extern void TEST_Convert_Alkane_To_IUPAC_Name (void)
{
    const unsigned char branch_1_content [] = { 1, 1, 2 };
    const unsigned char branch_2_content [] = { 1, 1, 2, 3, 4 };
    const unsigned char branch_3_content [] = { 1, 1, 2, 2 };

    // Dieser Name passt u.a. nicht von den Positionsnummern her, da diese kuerzer dargestellt werden koennen; der
    // richtige Name waere: 2-Methyl-4-Propyl-Octan
    // Dennoch wird dieser Name vorerst als richtiger Name angesehen, da damit die grundlegende Arbeitsweise des Codes
    // in vielen Punkten ueberprueft werden kann
    const char* expected_result            = "2-Methyl-4-PropylOctan";

    struct Alkane_Branch* branch_1 = Create_Alkane_Branch (branch_1_content, COUNT_ARRAY_ELEMENTS(branch_1_content));
    struct Alkane_Branch* branch_2 = Create_Alkane_Branch (branch_2_content, COUNT_ARRAY_ELEMENTS(branch_2_content));
    struct Alkane_Branch* branch_3 = Create_Alkane_Branch (branch_3_content, COUNT_ARRAY_ELEMENTS(branch_3_content));

    struct Alkane* alkane = Create_Alkane (branch_2, branch_1, branch_3, NULL);

    // Konvertierung durchfuehren
    Convert_Alkane_To_IUPAC_Name (alkane, false);

    ASSERT_STRING_EQUALS(expected_result, alkane->iupac_name);

    Delete_Alkane (alkane);
    Delete_Alkane_Branch (branch_1);
    Delete_Alkane_Branch (branch_2);
    Delete_Alkane_Branch (branch_3);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Die Erstellung des IUPAC-Namen aus einem Alkan testen (2).
 *
 * Wird der IUPAC-Name richtig gebildet ?
 */
extern void TEST_Convert_Alkane_To_IUPAC_Name_2 (void)
{
    const unsigned char branch_1_content [] = { 1 };
    const unsigned char branch_2_content [] = { 1, 1, 1 };
    const unsigned char branch_3_content [] = { 1, 1, 1, 1 };

    const char expected_result [] = "2,2,4-TriMethylPentan";

    struct Alkane_Branch* branch_1 = Create_Alkane_Branch (branch_1_content, COUNT_ARRAY_ELEMENTS(branch_1_content));
    struct Alkane_Branch* branch_2 = Create_Alkane_Branch (branch_2_content, COUNT_ARRAY_ELEMENTS(branch_2_content));
    struct Alkane_Branch* branch_3 = Create_Alkane_Branch (branch_3_content, COUNT_ARRAY_ELEMENTS(branch_3_content));

    struct Alkane* alkane = Create_Alkane (branch_1, branch_2, branch_3, NULL);

    // Konvertierung durchfuehren
    Convert_Alkane_To_IUPAC_Name (alkane, false);

    ASSERT_MSG (Compare_Strings_Case_Insensitive (alkane->iupac_name, expected_result) == 0, "Created name and expected name are not equal !");

    Delete_Alkane (alkane);
    Delete_Alkane_Branch (branch_1);
    Delete_Alkane_Branch (branch_2);
    Delete_Alkane_Branch (branch_3);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Die Erstellung des IUPAC-Namen aus einem Alkan testen, bei dem die Chain-Objekte von Hand erzeugt wurden. Hierbei
 * geht es in erster Linie darum den Konvertierungsprozess von den Chain-Objekten hin zu dem IUPAC-Namen zu testen.
 *
 * Wird der IUPAC-Name richtig gebildet ?
 */
extern void TEST_Convert_Alkane_To_IUPAC_Name_With_Manual_Chain_Objects (void)
{
    const char* expected_result = "9-Ethyl-6,7,8-TriMethyl-7-(1-(1-Methylethyl)-2-(1-Methylethyl)-3,4-Dimethylpentyl)Tridecan";

    struct Alkane alkane;
    memset (&alkane, '\0', sizeof(struct Alkane));

    const size_t iupac_name_length = COUNT_ARRAY_ELEMENTS(alkane.iupac_name) - 1;

    /**
     * 31 C-Atome
     * Bis zu dreifache Verschachtelungstiefe.
     *
     *                 C
     *                 |
     *                 C   C   C   C
     *                 |   |   |   |
     * C - C - C - C - C - C - C - C - C - C - C - C - C
     *                         |
     *                     C   C - C - C
     *                     |   |   |
     *                 C - C - C   C
     *                         |
     *                         C - C
     *                         |
     *                         C - C
     *                         |
     *                         C
     */
    // Von Hand Chain-Objekte erzeugen ...
// @formatter:off
    const struct Chain chain_data [] =
    {
            // Hauptkette
            { .length = 13,  .position = 255,   .nesting_depth = 0 },

            // DiMethyl
            { .length = 1,  .position = 6,      .nesting_depth = 1 },
            { .length = 1,  .position = 7,      .nesting_depth = 1 },

            // Pentyl
            { .length = 5,  .position = 7,      .nesting_depth = 1 },

            // MethylEthyl
            { .length = 2,  .position = 1,      .nesting_depth = 2 },
            { .length = 1,  .position = 1,      .nesting_depth = 3 },

            // MethylEthyl
            { .length = 2,  .position = 2,      .nesting_depth = 2 },
            { .length = 1,  .position = 1,      .nesting_depth = 3 },

            // DiEthyl auf zweiter Ebene
            { .length = 1,  .position = 3,      .nesting_depth = 2 },
            { .length = 1,  .position = 4,      .nesting_depth = 2 },

            // Drittes Methyl auf der ersten Ebene => TriMethyl
            { .length = 1,  .position = 8,      .nesting_depth = 1 },

            // Ethyl
            { .length = 2,  .position = 9,      .nesting_depth = 1 }
    };

    // ... und einfuegen
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(chain_data); ++ i)
    {
        alkane.chains [alkane.next_free_chain].length           = chain_data [i].length;
        alkane.chains [alkane.next_free_chain].position         = chain_data [i].position;
        alkane.chains [alkane.next_free_chain].nesting_depth    = chain_data [i].nesting_depth;
        alkane.next_free_chain ++;
    }
// @formatter:on

    // Konvertierung durchfuehren
    Chain_To_IUPAC (alkane.iupac_name, iupac_name_length, &alkane);

    // Ist der erzeugte IUPAC-Name und der erwartete IUPAC-Name gleich ?
    ASSERT_MSG(Compare_Strings_Case_Insensitive (alkane.iupac_name, expected_result) == 0, "Created name and expected name are not equal !")

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Die Erstellung des IUPAC-Namen aus einem Alkan testen.
 *
 * Wird der IUPAC-Name richtig gebildet ?
 */
extern void TEST_Convert_Alkane_With_Nested_2_To_IUPAC_Name (void)
{
    const unsigned char branch_1_content [] = { 1, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    const unsigned char branch_2_content [] = { 1, 1, 2, 3, 4, 5 };
    const unsigned char branch_3_content [] = { 1, 1, 2, 2, 2 };

    const char* expected_result            = "6-(2,2-DiMethylPropyl)Hexadecan";

    struct Alkane_Branch* branch_1 = Create_Alkane_Branch (branch_1_content, COUNT_ARRAY_ELEMENTS(branch_1_content));
    struct Alkane_Branch* branch_2 = Create_Alkane_Branch (branch_2_content, COUNT_ARRAY_ELEMENTS(branch_2_content));
    struct Alkane_Branch* branch_3 = Create_Alkane_Branch (branch_3_content, COUNT_ARRAY_ELEMENTS(branch_3_content));

    struct Alkane* alkane = Create_Alkane (branch_2, branch_1, branch_3, NULL);

    // Konvertierung durchfuehren
    Convert_Alkane_To_IUPAC_Name (alkane, false);

    ASSERT_STRING_EQUALS(expected_result, alkane->iupac_name);

    Delete_Alkane (alkane);
    Delete_Alkane_Branch (branch_1);
    Delete_Alkane_Branch (branch_2);
    Delete_Alkane_Branch (branch_3);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Alibi Test-Funktion, damit das Testframework eine Funktion hat, selbst wenn die Erzeugung der Isomere vom
 * Methan keinen Sinn ergibt.
 */
extern void TEST_All_Possible_Methan_Constitutional_Isomers (void)
{
    puts("Empty test function.");
    ASSERT_EQUALS(0, 0);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Alibi Test-Funktion, damit das Testframework eine Funktion hat, selbst wenn die Erzeugung der Isomere vom
 * Ethan keinen Sinn ergibt.
 */
extern void TEST_All_Possible_Ethan_Constitutional_Isomers (void)
{
    puts("Empty test function.");
    ASSERT_EQUALS(0, 0);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Alibi Test-Funktion, damit das Testframework eine Funktion hat, selbst wenn die Erzeugung der Isomere vom
 * Propan keinen Sinn ergibt.
 */
extern void TEST_All_Possible_Propan_Constitutional_Isomers (void)
{
    puts("Empty test function.");
    ASSERT_EQUALS(0, 0);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alle moeglichen Konstitutionsisomere des Butan mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Butan_Constitutional_Isomers (void)
{
    const uint_fast8_t number_of_c_atoms = 4;
    const uint_fast64_t number_of_constitutional_isomers = NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [number_of_c_atoms - 1];

    // Alle zwei Butane
    // Siehe: https://de.wikipedia.org/wiki/Butane
    const char* expected_results [] =
    {
            #include "./Expected_Results/Alkane/Butane.txt"
    };

    // Test durchfuehren
    Execute_Creation_Test_With_Expected_Results (number_of_c_atoms, number_of_constitutional_isomers, expected_results,
            COUNT_ARRAY_ELEMENTS(expected_results));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alle moeglichen Konstitutionsisomere des Pentan mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Pentan_Constitutional_Isomers (void)
{
    const uint_fast8_t number_of_c_atoms = 5;
    const uint_fast64_t number_of_constitutional_isomers = NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [number_of_c_atoms - 1];

    // Alle drei Pentane
    // Siehe: https://de.wikipedia.org/wiki/Pentane
    const char* expected_results [] =
    {
            #include "./Expected_Results/Alkane/Pentane.txt"
    };

    // Test durchfuehren
    Execute_Creation_Test_With_Expected_Results (number_of_c_atoms, number_of_constitutional_isomers, expected_results,
            COUNT_ARRAY_ELEMENTS(expected_results));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alle moeglichen Konstitutionsisomere des Hexans mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Hexan_Constitutional_Isomers (void)
{
    const uint_fast8_t number_of_c_atoms = 6;
    const uint_fast64_t number_of_constitutional_isomers = NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [number_of_c_atoms - 1];

    // Alle fuenf Hexane
    // Siehe: https://de.wikipedia.org/wiki/Hexane
    const char* expected_results [] =
    {
            #include "./Expected_Results/Alkane/Hexane.txt"
    };

    // Test durchfuehren
    Execute_Creation_Test_With_Expected_Results (number_of_c_atoms, number_of_constitutional_isomers, expected_results,
            COUNT_ARRAY_ELEMENTS(expected_results));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alle moeglichen Konstitutionsisomere des Heptans mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Heptan_Constitutional_Isomers (void)
{
    const uint_fast8_t number_of_c_atoms = 7;
    const uint_fast64_t number_of_constitutional_isomers = NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [number_of_c_atoms - 1];

    // Alle neun Heptane
    // Siehe: https://de.wikipedia.org/wiki/Heptane
    const char* expected_results [] =
    {
            #include "./Expected_Results/Alkane/Heptane.txt"
    };

    // Test durchfuehren
    Execute_Creation_Test_With_Expected_Results (number_of_c_atoms, number_of_constitutional_isomers, expected_results,
            COUNT_ARRAY_ELEMENTS(expected_results));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alle moeglichen Konstitutionsisomere des Octan mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Octan_Constitutional_Isomers (void)
{
    const uint_fast8_t number_of_c_atoms = 8;
    const uint_fast64_t number_of_constitutional_isomers = NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [number_of_c_atoms - 1];

    // Alle 18 Octane
    // Siehe: https://de.wikipedia.org/wiki/Octane
    const char* expected_results [] =
    {
            #include "./Expected_Results/Alkane/Octane.txt"
    };

    // Test durchfuehren
    Execute_Creation_Test_With_Expected_Results (number_of_c_atoms, number_of_constitutional_isomers, expected_results,
            COUNT_ARRAY_ELEMENTS(expected_results));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alle moeglichen Konstitutionsisomere des Nonan mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Nonan_Constitutional_Isomers (void)
{
    const uint_fast8_t number_of_c_atoms = 9;
    const uint_fast64_t number_of_constitutional_isomers = NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [number_of_c_atoms - 1];

    // Alle 35 Nonane
    // Siehe: https://de.wikipedia.org/wiki/Nonane
    const char* expected_results [] =
    {
            #include "./Expected_Results/Alkane/Nonane.txt"
    };

    // Test durchfuehren
    Execute_Creation_Test_With_Expected_Results (number_of_c_atoms, number_of_constitutional_isomers, expected_results,
            COUNT_ARRAY_ELEMENTS(expected_results));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alle moeglichen Konstitutionsisomere des Decans mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Decan_Constitutional_Isomers (void)
{
    const uint_fast8_t number_of_c_atoms = 10;
    const uint_fast64_t number_of_constitutional_isomers = NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [number_of_c_atoms - 1];

    // Alle 75 Decane
    // Siehe: https://de.wikipedia.org/wiki/Decane
    const char* expected_results [] =
    {
            #include "./Expected_Results/Alkane/Decane.txt"
    };

    // Test durchfuehren
    Execute_Creation_Test_With_Expected_Results (number_of_c_atoms, number_of_constitutional_isomers, expected_results,
            COUNT_ARRAY_ELEMENTS(expected_results));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alle moeglichen Konstitutionsisomere des Undecans mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Undecan_Constitutional_Isomers (void)
{
    const uint_fast8_t number_of_c_atoms = 11;
    const uint_fast64_t number_of_constitutional_isomers = NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [number_of_c_atoms - 1];

    // Alle 159 Undecane
    // Siehe: https://de.wikipedia.org/wiki/Undecane
    const char* expected_results [] =
    {
            #include "./Expected_Results/Alkane/Undecane.txt"
    };

    // Test durchfuehren
    Execute_Creation_Test_With_Expected_Results (number_of_c_atoms, number_of_constitutional_isomers, expected_results,
            COUNT_ARRAY_ELEMENTS(expected_results));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alle moeglichen Konstitutionsisomere des Dodecans mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Dodecan_Constitutional_Isomers (void)
{
    const uint_fast8_t number_of_c_atoms = 12;
    const uint_fast64_t number_of_constitutional_isomers = NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [number_of_c_atoms - 1];

    // Alle 355 Dodecane
    // Siehe: https://de.wikipedia.org/wiki/Dodecane
    const char* expected_results [] =
    {
            #include "./Expected_Results/Alkane/Dodecane.txt"
    };

    // Test durchfuehren
    Execute_Creation_Test_With_Expected_Results (number_of_c_atoms, number_of_constitutional_isomers, expected_results,
            COUNT_ARRAY_ELEMENTS(expected_results));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alle moeglichen Konstitutionsisomere des Tridecans mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Tridecan_Constitutional_Isomers (void)
{
    const uint_fast8_t number_of_c_atoms = 13;
    const uint_fast64_t number_of_constitutional_isomers = NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [number_of_c_atoms - 1];

    // Alle 802 Tridecane
    // Siehe: https://de.wikipedia.org/wiki/Tridecane
    const char* expected_results [] =
    {
            #include "./Expected_Results/Alkane/Tridecane.txt"
    };

    // Test durchfuehren
    Execute_Creation_Test_With_Expected_Results (number_of_c_atoms, number_of_constitutional_isomers, expected_results,
            COUNT_ARRAY_ELEMENTS(expected_results));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alle moeglichen Konstitutionsisomere des Tetradecabn mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Tetradecan_Constitutional_Isomers (void)
{
    const uint_fast8_t number_of_c_atoms = 14;
    const uint_fast64_t number_of_constitutional_isomers = NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [number_of_c_atoms - 1];

    // Alle 1858 Tetradecane
    // Siehe: https://de.wikipedia.org/wiki/Tetradecane
    const char* expected_results [] =
    {
            #include "./Expected_Results/Alkane/Tetradecane.txt"
    };

    // Test durchfuehren
    Execute_Creation_Test_With_Expected_Results (number_of_c_atoms, number_of_constitutional_isomers, expected_results,
            COUNT_ARRAY_ELEMENTS(expected_results));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Das Zusammenfassen von Gruppen im IUPAC-Namen testen.
 *
 * Was ist damit genau gemeint?
 *
 * Mit "Gruppen" sind kombinierte Aeste gemeint, die mehrfach vorkommen.
 * Z.B.: (1-methylethyl)
 *
 * Ein Alkan wie "4-(1-methylethyl)-4-(1-methylethyl)heptane"
 *
 *             C
 *             |
 *             C - C
 *             |
 * C - C - C - C - C - C - C
 *             |
 *             C - C
 *             |
 *             C
 *
 * Die beiden Gruppen "(1-methylehtyl)" koennen zusammengefasst werden:
 * 4,4-Bis(1-methylethyl)heptane
 *
 * "Bis" zeigt an, dass es sich um genau zwei gleiche Gruppen handelt, die zusammengefasst wurden.
 *
 */
extern void TEST_Group_Compression (void)
{
    const char expected_iupac_name [] = "4,4-Bis(1-MethylEthyl)Heptan";

    const unsigned char branch_1_content [] = { 1, 1, 2, 3 };
    const unsigned char branch_2_content [] = { 1, 1, 2 };
    const unsigned char branch_3_content [] = { 1, 1 ,1 };
    const unsigned char branch_4_content [] = { 1, 1, 1 };

    struct Alkane_Branch* branch_1 = Create_Alkane_Branch(branch_1_content, COUNT_ARRAY_ELEMENTS(branch_1_content));
    struct Alkane_Branch* branch_2 = Create_Alkane_Branch(branch_2_content, COUNT_ARRAY_ELEMENTS(branch_2_content));
    struct Alkane_Branch* branch_3 = Create_Alkane_Branch(branch_3_content, COUNT_ARRAY_ELEMENTS(branch_3_content));
    struct Alkane_Branch* branch_4 = Create_Alkane_Branch(branch_4_content, COUNT_ARRAY_ELEMENTS(branch_4_content));

    struct Alkane* alkane = Create_Alkane(branch_1, branch_2, branch_3, branch_4);

    // Kovertierung durchfuehren
    Convert_Alkane_To_IUPAC_Name(alkane, false);

    // Wurde der richtige Name erzeugt?
    ASSERT_STRING_EQUALS(expected_iupac_name, alkane->iupac_name);

    Delete_Alkane(alkane);
    Delete_Alkane_Branch(branch_1);
    Delete_Alkane_Branch(branch_2);
    Delete_Alkane_Branch(branch_3);
    Delete_Alkane_Branch(branch_4);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Testen, ob der Alkan-Lexer die gewuenschen Ergebnisse erzeugt.
 *
 * So soll z.B. "4-(1-methylethyl)heptane" in folgende Tokens zerlegt werden:
 * - "4"
 * - "-"
 * - "("
 * - "1"
 * - "-"
 * - "methyl"
 * - "ethyl"
 * - ")"
 * - "heptane"
 */
extern void TEST_Alkane_Lexer (void)
{
    #include "Test_Data/Alkane/Alkane_Lexer.txt"

    // Lexer-Test durchfuehren
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(expected_tokens); ++ i)
    {
        const size_t name_length = strlen (iupac_names [i]);
        const struct Alkane_Lexer lexer_data = Create_Alkane_Tokens (iupac_names [i], name_length);

        for (uint_fast8_t i2 = 0; i2 < lexer_data.next_free_token; ++ i2)
        {
            ASSERT_MSG(expected_tokens [i][i2].token != NULL, "expected token is NULL ! Lexer created not enough tokens !");
            ASSERT_STRING_CASE_INSENSITIVE_EQUALS(expected_tokens [i][i2].token, lexer_data.result_tokens [i2]);
            ASSERT_EQUALS(expected_tokens [i][i2].type, lexer_data.token_type [i2]);
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Testen, ob der Alkan-Parser seine Aufgaben richtig erledigt.
 *
 * Im Hintergrund wird der Alkan-Lexer verwendet. Dieser erzeugt die Tokens, die dann vom Parser ueberprueft werden.
 * Die Tests in dieser Testfunktion beziehen sich alleine auf die des Parsers. Wenn Fehler beim Lexer auftauchen, dann
 * koennen diese von den daraus resultierenden Fehlern des Parsers nicht unterschieden werden. Daher gibt es auch eine
 * eigene Testfunktion fuer den Lexer (TEST_Alkane_Lexer).
 */
extern void TEST_Alkane_Parser (void)
{
    // Als Testnamen werden die erwarteten Ergebnisse bei der Bildung der Konstitutionsisomere verwendet
    const char* iupac_names [] =
    {
            #include "./Expected_Results/Alkane/Tetradecane.txt"
            //"3,4-bis(1-methylethyl)-2,5-Dimethylhexan" // <- Prefixe, die Gruppen zusammenfassen, werden aktuell noch
                                                         // nicht richtig verarbeitet. Solche Prefixe werden durch den
                                                         // Lexer einfach uebersprungen. Aber auch der Parser besitzt
                                                         // noch keine passenden Regeln fuer diese Prefixe !
    };

    size_t wrong_results = 0;

    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(iupac_names); ++ i)
    {
        if (! Parse_Alkane (iupac_names [i], strlen (iupac_names [i])) /* == false */)
        {
            wrong_results ++;
        }
    }
    if (wrong_results != 0)
    {
        PRINTF_FFLUSH("\nWrong parser results ! Got %zu times false from %zu test names.\n\n", wrong_results,
                COUNT_ARRAY_ELEMENTS(iupac_names))
    }

    ASSERT_EQUALS(0, wrong_results);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Das textbasierte Zeichnen eines Alkans testen. Erster Zeichnungstest.
 *
 * Es soll "4-(1-methylethyl)heptane" gezeichnet werden.
 *
 * Das Ergebnis soll etwa so aussehen:
 *
 * C - C - C - C - C - C - C
 *             |
 *             C - C
 *             |
 *             C
 */
extern void TEST_Text_Based_Alkane_Drawing_1 (void)
{
    #include "Test_Data/Alkane/Drawings_With_Nestings.txt"

    _Bool at_least_one_wrong_result_created = false;
    // Alle textbasierte Zeichnung erzeugen
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(expected_drawings); ++ i)
    {
        // Textbasierte Zeichnung erzeugen
        struct Text_Based_Alkane_Drawing* result_drawing =
                Create_Text_Based_Alkane_Drawing (iupac_names [i], strlen (iupac_names [i]),
                        CREATE_TEXT_BASED_DRAWING_DEFAULTS);

        // Beim Test, ob die Zeichnung richtig ist, wird Zeile fuer Zeile miteinander verglichen
        const _Bool wrong_result_created = Compare_Expected_Drawing_With_Created_Drawing (expected_drawings [i],
                result_drawing, drawing_sizes [i], strlen (expected_drawings [i][0]),TEXT_BASED_ALKANE_DRAWING_DIM_1,
                TEXT_BASED_ALKANE_DRAWING_DIM_2);

        if (wrong_result_created /* == true */)
        {
            at_least_one_wrong_result_created = true;
        }

        Delete_Text_Based_Alkane_Drawing (result_drawing);
        result_drawing = NULL;
    }

    // War der Test aller Zeichnungen erfolgreich ?
    ASSERT_EQUALS (false, at_least_one_wrong_result_created);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Das textbasierte Zeichnen eines Alkans testen. Weiterer Zeichnungstest mit Namen ohne Verschachtelung.
 *
 * Es soll "3,3,4-Triethyl-2,4-dimethylhexan" gezeichnet werden. Bewusst die Wahl, wo die gleichen Positionen von
 * verschiedenen Asttypen verwendet werden. Hier Position 4
 *
 * Das Ergebnis soll etwa so aussehen:
 *
 *         C
 *         |
 *         C   C
 *         |   |
 * C - C - C - C - C - C
 *     |   |   |
 *     C   C   C
 *         |   |
 *         C   C
 */
extern void TEST_Text_Based_Alkane_Drawing_2 (void)
{
    #include "Test_Data/Alkane/Drawings_Without_Nestings.txt"

    _Bool at_least_one_wrong_result_created = false;
    // Alle textbasierte Zeichnung erzeugen
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(expected_drawings); ++ i)
    {
        // Textbasierte Zeichnung erzeugen
        struct Text_Based_Alkane_Drawing* result_drawing =
                Create_Text_Based_Alkane_Drawing (iupac_names [i], strlen (iupac_names [i]),
                        CREATE_TEXT_BASED_DRAWING_DEFAULTS);

        // Beim Test, ob die Zeichnung richtig ist, wird Zeile fuer Zeile miteinander verglichen
        const _Bool wrong_result_created = Compare_Expected_Drawing_With_Created_Drawing (expected_drawings [i],
                result_drawing, drawing_sizes [i], strlen (expected_drawings [i][0]),TEXT_BASED_ALKANE_DRAWING_DIM_1,
                TEXT_BASED_ALKANE_DRAWING_DIM_2);

        if (wrong_result_created /* == true */)
        {
            at_least_one_wrong_result_created = true;
        }

        Delete_Text_Based_Alkane_Drawing (result_drawing);
        result_drawing = NULL;
    }

    // War der Test aller Zeichnungen erfolgreich ?
    ASSERT_EQUALS (false, at_least_one_wrong_result_created);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Diese Testfunktion ueberprueft, ob das Erstellen einer textbasierten Zeichnung und deren Export als XPM Datei
 * funktioniert.
 *
 * Als Teststruktur wird "3,3,4-Triethyl-2,4-dimethylhexan" verwendet.
 *
 * Textbasierte Zeichnung:
 *
 *         C
 *         |
 *         C   C
 *         |   |
 * C - C - C - C - C - C
 *     |   |   |
 *     C   C   C
 *         |   |
 *         C   C
 */
extern void TEST_Convert_Text_Based_Alane_Drawing_To_XPM (void)
{
    const char* iupac_name = "3,3,4-Triethyl-2,4-dimethylhexan";

    // Textbasierte Zeichnung erzeugen
    struct Text_Based_Alkane_Drawing* result_drawing =
            Create_Text_Based_Alkane_Drawing (iupac_name, strlen (iupac_name), CREATE_TEXT_BASED_DRAWING_NO_SPACES);

    puts("Export image:");
    Show_Text_Based_Alkane_Drawing(result_drawing);
    PUTS_FFLUSH("");

    // Textbasierte Zeichnung exportieren
    Export_Text_Based_Drawing_To_XPM(result_drawing, EXPORT_XPM_CHAR_SIZE_32_32 | EXPORT_XPM_CHAR_PER_PIXEL_1);
    // Variante mit 64x64 Pixel pro Zeichen
    // Export_Text_Based_Drawing_To_XPM(result_drawing, EXPORT_XPM_CHAR_SIZE_64_64| EXPORT_XPM_CHAR_PER_PIXEL_1);
    // Variante mit 24+24 Pixel pro Zeichen
    // Export_Text_Based_Drawing_To_XPM(result_drawing, EXPORT_XPM_CHAR_SIZE_24_24 | EXPORT_XPM_CHAR_PER_PIXEL_1);

    // Name und Pfad der Referenzdatei zusammenbauen
    char reference_file [100];
    Multi_strncat(reference_file, COUNT_ARRAY_ELEMENTS(reference_file) - 1, 3,
    		"./src/Tests/Expected_Results/Alkane/XPM_Pictures/", iupac_name, " NO_SPACES.xpm");

    // Name der Testdatei zusammenbauen
    char test_file [IUPAC_NAME_LENGTH + 4 /*strlen(".xpm")*/];
    Multi_strncat (test_file, COUNT_ARRAY_ELEMENTS(test_file) - 1, 2, iupac_name, ".xpm");

    printf("\nReference file for comparison: %s\n", reference_file);
    int_fast32_t first_error_position = 0;

    // Erzeugte Datei mit der Referenzdatei byteweise vergleichen
    const clock_t start_time = clock ();
    const _Bool comparison_result = Compare_Two_XPM_Drawings(reference_file, test_file, &first_error_position);
    const clock_t end_time = clock ();

    Delete_Text_Based_Alkane_Drawing (result_drawing);
    result_drawing = NULL;

    if (! comparison_result /* == false */)
    {
        fprintf (stderr, "The files \"%s\" and \"%s\" are not equal ! First difference on byte %" PRIdFAST32 " !",
                reference_file, test_file, first_error_position);
        fflush(stderr);
    }
    ASSERT_EQUALS(true, comparison_result);

    printf("Files are equal. Done comparison in %f sec.\n", (double)(end_time - start_time) / CLOCKS_PER_SEC);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Den Lexer fuer die Aufspaltung des IUPAC-Namen testen.
 *
 * Der Lexer soll z.B. "2-Methyl-4-(1,1-DiMethylEthyl)Heptan" in folgende Fragmente aufspalten:
 *
 * - 2-Methyl
 * - 4-(1,1-DiMethylEthyl)
 * - Heptan
 */
extern void TEST_IUPAC_Chain_Lexer_1 (void)
{
    const char test_name [] = "2-Methyl-4-(1,1-DiMethylEthyl)Heptan";

    const char* const expected_lexer_result [] =
    {
         "2-Methyl",
         "4-(1,1-DiMethylEthyl)",
         "Heptan"
    };

    // Name aufspalten
    const struct IUPAC_Chain_Lexer_Result lexer_result = Create_Chain_Tokens (test_name);

    _Bool wrong_lexer_result_occured = false;
    size_t first_wrong_result_string = SIZE_MAX;

    if (COUNT_ARRAY_ELEMENTS(expected_lexer_result) != lexer_result.next_free_token)
    {
        wrong_lexer_result_occured = true;
    }
    else
    {
        for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(expected_lexer_result); ++ i)
        {
            if (strncmp (expected_lexer_result [i], lexer_result.result_tokens [i],
                    strlen (expected_lexer_result [i])) != 0)
            {
                wrong_lexer_result_occured = true;
                first_wrong_result_string = i;
                break;
            }
        }
    }

    puts ("Expected result:");
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(expected_lexer_result); ++ i)
    {
        printf ("%2zu: %s\n", i + 1, expected_lexer_result [i]);
    }
    printf ("First error in drawing line: ");
    if (first_wrong_result_string != SIZE_MAX)
    {
        printf ("%zu", first_wrong_result_string);
    }
    else
    {
        printf ("N/A");
    }

    puts ("\n\nCreated:");
    for (size_t i = 0; i < lexer_result.next_free_token; ++ i)
    {
        printf ("%2zu: %s\n", i + 1, lexer_result.result_tokens [i]);
    }
    fflush (stdout);

    // War der Test aller Zeichenketten erfolgreich ?
    ASSERT_EQUALS (false, wrong_lexer_result_occured);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Den Lexer fuer die Aufspaltung des IUPAC-Namen testen.
 *
 * Der Lexer soll z.B. "2-Methyl-4-(1,1-DiMethylEthyl)Heptan" in folgende Fragmente aufspalten:
 *
 * - 2-Methyl
 * - 4-(1,1-DiMethylEthyl)
 * - Heptan
 *
 * Nun wird ein Name verwendet, der keinen realen Stoff beschreibt ! Es soll getestet werden, ob der Lexer auch bei
 * groesseren Namen richtig arbeitet.
 */
extern void TEST_IUPAC_Chain_Lexer_2 (void)
{
    const char test_name [] = "5-Hexyl-4-Ethyl-4-(1-(1,1-DiMethylEthyl)Ethyl)-2-Methyl-4-(1,1-DiMethylEthyl)"
            "-2,4-DiDecyl-Eicosan";

    const char* const expected_lexer_result [] =
    {
         "5-Hexyl",
         "4-Ethyl",
         "4-(1-(1,1-DiMethylEthyl)Ethyl)",
         "2-Methyl",
         "4-(1,1-DiMethylEthyl)",
         "2,4-DiDecyl",
         "Eicosan"
    };

    // Name aufspalten
    const struct IUPAC_Chain_Lexer_Result lexer_result = Create_Chain_Tokens (test_name);

    _Bool wrong_lexer_result_occured = false;
    size_t first_wrong_result_string = SIZE_MAX;

    if (COUNT_ARRAY_ELEMENTS(expected_lexer_result) != lexer_result.next_free_token)
    {
        wrong_lexer_result_occured = true;
    }
    else
    {
        for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(expected_lexer_result); ++ i)
        {
            if (strncmp (expected_lexer_result [i], lexer_result.result_tokens [i],
                    strlen (expected_lexer_result [i])) != 0)
            {
                wrong_lexer_result_occured = true;
                first_wrong_result_string = i;
                break;
            }
        }
    }

    puts ("Expected result:");
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(expected_lexer_result); ++ i)
    {
        printf ("%2zu: %s\n", i + 1, expected_lexer_result [i]);
    }
    printf ("First error in drawing line: ");
    if (first_wrong_result_string != SIZE_MAX)
    {
        printf ("%zu", first_wrong_result_string);
    }
    else
    {
        printf ("N/A");
    }

    puts ("\n\nCreated:");
    for (size_t i = 0; i < lexer_result.next_free_token; ++ i)
    {
        printf ("%2zu: %s\n", i + 1, lexer_result.result_tokens [i]);
    }
    fflush (stdout);

    // War der Test aller Zeichenketten erfolgreich ?
    ASSERT_EQUALS (false, wrong_lexer_result_occured);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Alle Testfunktionen ausfuehren.
 *
 * Die Funktionalitaet liegt nicht direkt im Koerper dieser Funktion. Vielmehr dient diese Funktion als eine Art
 * "Platzhalterfunktion", da in der Funktion "Execute_All_Alkane_Tests" alle Testfunktionen mittels Fuktionszeiger
 * verfuegbar gemacht werden, indem sie in die Liste der Testfunktionen uebernommen wird.
 */
extern inline void TEST_Use_All_Testfunctions (void)
{
    return;
}

//--------------------------------------------------------------------------------------------------------------------

/**
 * Einlesefunktion fuer die Auswahl der Testfunktion.
 */
static long int Get_Terminal_Input
(
    const size_t num_testfunctions,
    const long int cli_input_use_all_testfunctions
)
{
    char input_buffer [10] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
    long int selected_test_function = -1;

    // Einleseschleife
    while (true)
    {
        int c = 0;
        printf ("(1 - %zu) => ", num_testfunctions);
        const int char_read = scanf ("%9s", input_buffer);
        if (char_read == EOF)
        {
            puts("Got EOF ! Retry ...");
            memset (input_buffer, '\0', sizeof(input_buffer));
        }

        // Idee von: https://stackoverflow.com/questions/28297306/how-to-limit-input-length-with-scanf
        while ((c = fgetc(stdin)) != '\n' && c != EOF); /* Flush stdin */

        // Ist die Eingabe ueberhaupt ein Integer?
        const enum str2int_errno conversion_errno = str2int(&selected_test_function, input_buffer, 10);

        if (conversion_errno == STR2INT_SUCCESS)
        {
            if ((selected_test_function > 0 && selected_test_function <= (long int) num_testfunctions) ||
                    (selected_test_function == cli_input_use_all_testfunctions))
            {
                break;
            }
        }
        puts("Invalid input !");
        memset (input_buffer, '\0', sizeof(input_buffer));
    }

    return selected_test_function;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Vergleichen eines Zahlencodes eines Akans mit einem vorgegebenen Ergebnis.
 */
static _Bool                                            // true, wenn der komplette Zahlencode uebereinstimmt;
                                                        // ansonsten false
Compare_Alkane_Numbercodes
(
        const struct Alkane* const restrict alkane,     // Alkan, dessen Zahlencode fuer den Vergleich verwendet wird
        const unsigned char numbercode []               // Zahlencode mit denen das Alkan-Objekt verglichen wird
)
{
    for (uint_fast8_t current_element = 0; current_element < alkane->number_of_c_atoms; ++ current_element)
    {
        if (alkane->merged_numbercode [current_element] != numbercode [current_element])
        {
            return false;
        }
    }

    return true;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Ueberpruefung, ob der uebergebene IUPAC-Name in den erwarteten Loesungen vorkommt. Die Zeichenketten werden OHNE
 * Beachtung der Gross- und Kleinschreibung miteinander verglichen.
 *
 * Von der Funktionalitaet stellt die Funktion "strncasecmp" aus der Standard-Lib das gleiche zur Verfuegung. Allerdings
 * ist dies eine GNU-Erweiterung und deren Verwendung macht den Code u.U. nicht mehr portabel. -> NICHT VERWENDEN !
 */
static _Bool                                            // true, falls der IUPAC-Name in den erwarteten Loesungen
                                                        // vorkommt, ansonsten false
Search_IUPAC_Name_In_The_List_Of_Expected_Results
(
        const char* const restrict iupac_name,          // IUPAC-Name, der in den erwarteten Loesungen gesucht werden
                                                        // soll
        const char* const restrict expected_results [], // Erwartete Loesungen
        const uint_fast64_t number_of_expected_results, // Anzahl an erwarteten Loesungen
        uint_fast64_t* const restrict result_index      // Index des gefundenen IUPAC-Namen in den erwarteten Loesungen
)
{
    _Bool result_found = false;

    char converted_iupac_name [255];
    char converted_expected_results [255];

    // Den IUPAC-Namen mit allen erwarteten Ergebnissen vergleichen (Ohne Beachtung der Gross- und Kleinschreibung !)
    for (uint_fast64_t i = 0; i < number_of_expected_results; ++ i)
    {
        memset (converted_iupac_name, '\0', sizeof (converted_iupac_name));
        strncpy (converted_iupac_name, iupac_name, COUNT_ARRAY_ELEMENTS(converted_iupac_name) - 1);

        memset (converted_expected_results, '\0', sizeof (converted_expected_results));
        strncpy (converted_expected_results, expected_results [i], COUNT_ARRAY_ELEMENTS(converted_expected_results) - 1);

        // Alle Zeichenketten als englische Variante vergleichen
        // Also das "e" am Ende hinzufuegen, falls es noch nicht da ist
        // Dadurch wird auch sichergestellt, dass es keine Rolle spielt, ob in den erwarteten Ergebnissen die deutsche oder
        // englische Variante steht
        if (iupac_name [strlen (iupac_name) - 1] != 'e')
        {
            converted_iupac_name [strlen (iupac_name)] = 'e';
        }
        if (expected_results [i][strlen (expected_results [i]) - 1] != 'e')
        {
            converted_expected_results [strlen (expected_results [i])] = 'e';
        }

        if (Compare_Strings_Case_Insensitive (converted_iupac_name, converted_expected_results) == 0)
        {
            result_found = true;
            if (result_index != NULL)
            {
                *result_index = i;
            }
            break;
        }
    }

    return result_found;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Exponentialfunktion fuer Ganzzahlen.
 */
static size_t
Pow_Int
(
        const size_t base,      // Basis
        const size_t exponent   // Exponent
)
{
    size_t result = (exponent == 0) ? 1 : base;

    for (size_t i = 0; i < exponent; i ++) { result *= base; }

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alkane und die IUPAC-Namen mit einer vorgegebenen Anzahl an C-Atomen erzeugen und schauen ob die erzeugten
 * IUPAC-Namen in der Liste an erwarteten Loesungen vorhanden sind.
 */
static void
Execute_Creation_Test_With_Expected_Results
(
        const uint_fast8_t number_of_c_atoms,                   // Anzahl an C-Atomen
        const uint_fast64_t number_of_constitutional_isomers,   // Anzahl an Konstitutionsisomeren
        const char* const restrict expected_results [],         // Liste der erwarteten Loesungen
        const size_t number_of_expected_results                 // Anzahl an erwarteten Loesungen
)
{
    // Bei weniger als 4 C-Atome existieren - bis auf eine gerade Verbindung - keine weiteren Isomere
    if (number_of_c_atoms < 4)
    {
        return;
    }

    // Alle Alkane erzeugen
    struct Alkane_Container* all_alkanes = Create_Alkane_Constitutional_Isomers (number_of_c_atoms);

    // Anzahl an erzeugten Konstitutionsisomeren ueberpruefen
    ASSERT_EQUALS(number_of_constitutional_isomers, all_alkanes->size);
    ASSERT_EQUALS(number_of_constitutional_isomers, number_of_expected_results);

    // Die Verwendung der erwarteten Ergebnisse zaehlen, um moegliche Mehrfachverwendungen oder fehlende Verwendungen
    // erkennen zu koennen
    // Wenn alles richtig laeuft, dann muss jedes erwartete Ergebnis GENAU einmal verwendet werden
    uint_fast8_t* count_expected_results_usage = (uint_fast8_t*) CALLOC (number_of_expected_results, sizeof (uint_fast8_t));
    ASSERT_ALLOC(count_expected_results_usage, "Cannot allocate memory for the count_expected_results array !",
            number_of_expected_results * sizeof (uint_fast8_t));

    // Die Liste an Namen, die nicht in den erwarteten Loesungen gefunden wurden
    // Dadurch kann am Ende eine Zusammenfassung an falschen Ergebnissen erzeugt werden
    uint_fast8_t next_free_wrong_results = 0;
    char wrong_results [MAX_WRONG_RESULTS][IUPAC_NAME_LENGTH];
    memset(wrong_results, '\0', sizeof(wrong_results));


    // ===== ===== ===== ===== ===== BEGINN Testbereich ===== ===== ===== ===== =====
    // ===== ===== ===== BEGINN Testschleife ===== ===== =====
    // Fuer alle gerade erzeugten Alkane den IUPAC-Namen bilden
    for (uint_fast64_t i = 0; i < number_of_constitutional_isomers; ++ i)
    {
        // Fuer die Benennungsroutinen ist es wichtig, dass es keine verschachtelten Verschachtelungen gibt !
        // U.a. weil bei einer IUPAC konformen Benennung solche Konstrukte nicht auftreten koennen.
        // => Alle Chain-Objekte kontrollieren, falls welche vorhanden sind
        // Hinweis: Nur das Isomer, welches aus einer geraden Kette besteht, besitzt keine Chain-Objekte.
        //
        // Gueltige Verschachtelungstiefen:
        // 0: Die Hauptkette (darf nur einmal vorkommen !)
        // 1: Abzweigungen, die sich direkt an der Hauptkette befinden
        // 2: Abzweigungen, die sich an einer anderen Abzweigung befinden und nicht direkt mit der Hauptkette verbunden
        // sind
        if (all_alkanes->data [i]->next_free_chain > 0)
        {
            uint_fast8_t count_chains_with_nesting_depth_0 = 0;
            for (uint_fast8_t i2 = 0; i2 < all_alkanes->data [i]->next_free_chain; i2 ++)
            {
                ASSERT_FMSG(all_alkanes->data [i]->chains [i2].nesting_depth <= 2,
                        "Invalid nesting depth ! Max. valid: 2; Got %" PRIuFAST8 " !",
                        all_alkanes->data [i]->chains [i2].nesting_depth);

                if (all_alkanes->data [i]->chains [i2].nesting_depth == 0)
                {
                    count_chains_with_nesting_depth_0 ++;
                }
            }

            // Doppelte Ueberpruefung per Hand und mit Makro, damit der Speicher richtig freigegeben wird, falls der
            // Assert fehlschlaegt
            if (count_chains_with_nesting_depth_0 != 1)
            {
                FREE_AND_SET_TO_NULL(count_expected_results_usage)
            }
            ASSERT_EQUALS(1, count_chains_with_nesting_depth_0);
        }

        // Kopie des aktuellen Alkan-Objektes
        // Wenn die Erstellung des IUPAC-Namens im ersten Verlauf nicht gelingt, dann wird in einigen Faellen ein
        // weiterer Verlauf mit anderen Optionen durchgefuehrt
        // Da beim Aufruf von "Convert_Alkane_To_IUPAC_Name()" das Alkan-Objekt veraendert wird, fuehrt dies dazu, dass
        // bei den weiteren Verlaeufen teilweise komplett andere Ergebnisse erzeugt werden !
        struct Alkane temp_alkane_copy;
        memcpy (&temp_alkane_copy, all_alkanes->data[i], sizeof (temp_alkane_copy));

        Convert_Alkane_To_IUPAC_Name (all_alkanes->data [i], false);

        // Falls ein erwartetes Ergebnis verwendet wurde, dann wird in dieser Variable der Index des erwarteten
        // Ergebnisses hinterlegt
        uint_fast64_t index_in_the_expected_results = UINT_FAST64_MAX;

        // Befindet sich das gerade erzeugte Ergebnis in der Liste an gueltigen Ergebnissen ?
        _Bool result_found_in_the_expected_results =
                Search_IUPAC_Name_In_The_List_Of_Expected_Results (all_alkanes->data [i]->iupac_name, expected_results,
                        number_of_constitutional_isomers, &index_in_the_expected_results);

        // ===== BEGINN Umformungen versuchen =====
        // Wenn sich das Ergebnis nicht in der Liste befindet, dann wird der 1. Umformungsverusch unternommen, um ein
        // gueltiges Ergebnis zu bilden
        if (! result_found_in_the_expected_results /* == false */)
        {
            // In einigen (wenigen) Faellen sind die verschachtelten Strukturen an einer anderen Stelle als in den
            // Loesungen
            // Wichtig dabei: Das Ergebnis an sich ist richtig ! Es ist lediglich eine "Dekoentscheidung", um den
            // erwarteten Ergebnissen zu entsprechen.
            //
            // Ein Beispiel:
            // "2-Methyl-4-(1,1-DiMethylEthyl)Heptan" ist NICHT in den erwarteten Ergebnissen enthalten
            // ABER "4-(1,1-Dimethylethyl)-2-methylheptan" schon.
            // Nur die Position der Verschachtelung - der Inhalt der Klammern + die Klammern - ist unterschiedlich
            //
            // Der Lexer teilt den IUPAC-Namen in folgende Fragmente auf:
            // - 2-Methyl
            // - 4-(1,1-DiMethylEthyl)
            // - Heptan
            //
            // Ist eine Verschachtelte Struktur vorhanden, um sie umzupositionieren ?

            // Bereits erzeugten Namen aufspalten, um die Fragmente fuer die Neupositionierung zu ermitteln
            const struct IUPAC_Chain_Lexer_Result chain_lexer_result =
                    Create_Chain_Tokens(all_alkanes->data [i]->iupac_name);

            // Alle Kombinationen - mit allen Tokens - ausprobieren. Ausser mit dem Alkanrest (immer letzter Token !)
            char temp_iupac_name [IUPAC_NAME_LENGTH];
            memset (temp_iupac_name, '\0', sizeof(temp_iupac_name));

            // Ueberall "< next_free_token - 1", da das letzte Token immer die gerade Kette, ist die bei der Erzeugung
            // aller Kombinationen nicht betrachet werden darf !
            const uint_fast8_t next_free_token_sub_1 = (uint_fast8_t) (chain_lexer_result.next_free_token - 1);

            // Speicher fuer die moeglichen Kombinationen
            uint_fast8_t** combinations = (uint_fast8_t**) MALLOC(Pow_Int(next_free_token_sub_1, next_free_token_sub_1)
                    * sizeof(uint_fast8_t*));
            ASSERT_ALLOC(combinations, "Could not allocate memory for the combinations array !",
                    Pow_Int(next_free_token_sub_1, next_free_token_sub_1) * sizeof(uint_fast8_t*));
            SET_POINTER_ARRAY_TO_NULL(combinations, Pow_Int(next_free_token_sub_1, next_free_token_sub_1));

            // Inneren Elemente erzeugen (Die eigentlichen Arrays mit den Werten)
            for (size_t combinations_array_index = 0;
                    combinations_array_index < (Pow_Int(next_free_token_sub_1, next_free_token_sub_1));
                    combinations_array_index ++)
            {
                combinations [combinations_array_index] = (uint_fast8_t*)
                        CALLOC(next_free_token_sub_1, sizeof(uint_fast8_t));
                ASSERT_ALLOC(combinations [combinations_array_index],
                        "Memory for a combinations array could not created !",
                        next_free_token_sub_1 * sizeof(uint_fast8_t));
            }

            // ===== BEGINN Arraywerte fuer die Neukombination bilden =====
            register size_t next_combinations_array_index = 0;

            uint_fast8_t tokens [MAX_LOOP_NESTING];
            memset (tokens, '\0', sizeof (tokens));

            // Sehr billige Herangehensweise. Aber mir fiel nichts besseres ein ... :/
            for (tokens[0] = 0; tokens[0] < next_free_token_sub_1; ++ tokens[0])
            {
                if (next_free_token_sub_1 > 1)
                {
                for (tokens[1] = 0; tokens[1] < next_free_token_sub_1; ++ tokens[1])
                {
                    if (next_free_token_sub_1 > 2)
                    {
                    for (tokens[2] = 0; tokens[2] < next_free_token_sub_1; ++ tokens[2])
                    {
                        if (next_free_token_sub_1 > 3)
                        {
                        for (tokens[3] = 0; tokens[3] < next_free_token_sub_1; ++ tokens[3])
                        {
                            if (next_free_token_sub_1 > 4)
                            {
                            for (tokens[4] = 0; tokens[4] < next_free_token_sub_1; ++ tokens[4])
                            {
                                if (next_free_token_sub_1 > 5)
                                {
                                for (tokens[5] = 0; tokens[5] < next_free_token_sub_1; ++ tokens[5])
                                {
                                    for (uint_fast8_t token_index = 0; token_index < MAX_LOOP_NESTING; token_index ++)
                                    { combinations [next_combinations_array_index][token_index] = tokens[token_index]; }
                                    ++ next_combinations_array_index;
                                }
                                }
                                else
                                {
                                    for (uint_fast8_t token_index = 0; token_index < MAX_LOOP_NESTING - 1; token_index ++)
                                    { combinations [next_combinations_array_index][token_index] = tokens[token_index]; }
                                    ++ next_combinations_array_index;
                                }
                            }
                            }
                            else
                            {
                                for (uint_fast8_t token_index = 0; token_index < MAX_LOOP_NESTING - 2; token_index ++)
                                { combinations [next_combinations_array_index][token_index] = tokens[token_index]; }
                                ++ next_combinations_array_index;
                            }
                        }
                        }
                        else
                        {
                            for (uint_fast8_t token_index = 0; token_index < MAX_LOOP_NESTING - 3; token_index ++)
                            { combinations [next_combinations_array_index][token_index] = tokens[token_index]; }
                            ++ next_combinations_array_index;
                        }
                    }
                    }
                    else
                    {
                        for (uint_fast8_t token_index = 0; token_index < MAX_LOOP_NESTING - 4; token_index ++)
                        { combinations [next_combinations_array_index][token_index] = tokens[token_index]; }
                        ++ next_combinations_array_index;
                    }
                }
                }
            }
            // ===== ENDE Arraywerte fuer die Neukombination bilden =====

            _Bool next_try_successful = false;

            // Arrays und deren Inhalte elementweise durchgehen und den neu kombinierten Namen bilden
            for (size_t i2 = 0; i2 < next_combinations_array_index; ++ i2)
            {
                size_t char_left = IUPAC_NAME_LENGTH - 1;
                for (size_t i3 = 0; i3 < next_free_token_sub_1; ++ i3)
                {
                    strncat (temp_iupac_name, chain_lexer_result.result_tokens [combinations[i2][i3]], char_left);
                    char_left -= strlen(chain_lexer_result.result_tokens [combinations[i2][i3]]);
                    if ((i3 + 1) < next_free_token_sub_1)
                    {
                        temp_iupac_name [IUPAC_NAME_LENGTH - char_left - 1] = '-';
                        char_left --;
                    }
                }
                // Alkanrest anbringen
                strncat (temp_iupac_name, chain_lexer_result.result_tokens [next_free_token_sub_1], char_left);
                char_left -= strlen(chain_lexer_result.result_tokens [next_free_token_sub_1]);

                // PRINTF_FFLUSH("%s\n", temp_iupac_name);
                temp_iupac_name [IUPAC_NAME_LENGTH - char_left - 1] = '\0';

                // Die neuen kombinierten Elemente in der Liste suchen
                // Befindet sich das gerade erzeugte neu kombinierte Element in der Liste an gueltigen Ergebnissen ?
                next_try_successful = Search_IUPAC_Name_In_The_List_Of_Expected_Results (temp_iupac_name,
                        expected_results, number_of_constitutional_isomers, &index_in_the_expected_results);
                memset (temp_iupac_name, '\0', sizeof(temp_iupac_name));

                if (next_try_successful == true) { break; }
            }

            for (size_t combinations_array_index = 0;
                    combinations_array_index < (Pow_Int(next_free_token_sub_1, next_free_token_sub_1));
                    combinations_array_index ++)
            {
                FREE_AND_SET_TO_NULL(combinations[combinations_array_index]);
            }
            FREE_AND_SET_TO_NULL(combinations);

            // Doch noch richtig :D Eine Neukombination war ausreichend
            if (next_try_successful == true)
            {
                result_found_in_the_expected_results = true;
            }
        }
        // 2. Umformungsversuch
        if (! result_found_in_the_expected_results /* == false */)
        {
            // In seltenen Faellen koennen beide Richtungen bei der Positionierung verwendet werden
            // Manchmal wird daher in den erwarteten Loesungen die andere Richtung verwendet, als die, die vom
            // Algorithmus verwendet wurde
            // IUPAC-Name mit anderer Positionsierungsrichtung nochmal erstellen und schauen, ob dieses Ergebnis in den
            // erwarteten Loesungen enthalten ist
            Convert_Alkane_To_IUPAC_Name (&temp_alkane_copy, true);

            // PRINTF_FFLUSH("!!! %s !!!\n", temp_alkane_copy.iupac_name)

            _Bool name_with_reversed_number_order_successful = false;

            name_with_reversed_number_order_successful = Search_IUPAC_Name_In_The_List_Of_Expected_Results
                    (temp_alkane_copy.iupac_name, expected_results, number_of_constitutional_isomers,
                            &index_in_the_expected_results);

            // Doch noch richtig ?
            if (name_with_reversed_number_order_successful /* == true */)
            {
                result_found_in_the_expected_results = true;
            }
        }
        // ===== ENDE Umformungen versuchen =====

        // Ist das Element - mit oder ohne Umformung - in der Liste der erwarteten Elemente ?
        if (result_found_in_the_expected_results /* == true */)
        {
            count_expected_results_usage [index_in_the_expected_results] ++;
        }
        // Keine Chance. Das Element ist - zumindest in der erzeugten Form - nicht richtig !
        else
        {
            if (next_free_wrong_results < MAX_WRONG_RESULTS)
            {
                // Falsche erzeugten Namen kopieren
                strncpy(wrong_results[next_free_wrong_results], all_alkanes->data[i]->iupac_name,
                        IUPAC_NAME_LENGTH);
                next_free_wrong_results ++;
            }
            FPRINTF_FFLUSH(stderr, "Cannot find the current result \"%s\" in the list of expected results !\n",
                    all_alkanes->data [i]->iupac_name);
        }
    }
    // ===== ===== ===== ENDE Testschleife ===== ===== =====

    if (next_free_wrong_results > 0) { PRINT_NEWLINE }
    size_t count_unused_expected_results = 0;

    // Welche erwarteten Ergebnisse wurden nicht GENAU einmal verwendet ?
    for (size_t i = 0; i < number_of_expected_results; ++ i)
    {
        if (count_expected_results_usage [i] != 1)
        {
            FPRINTF_FFLUSH(stderr, "Expected result: %60s    used %" PRIuFAST8 " times !\n", expected_results [i],
                    count_expected_results_usage [i]);
            ++ count_unused_expected_results;
        }
    }

    if (next_free_wrong_results > 0) { PRINT_NEWLINE }
    // Alle falschen Ergebnisse ausgeben
    for (uint_fast8_t i = 0; i < next_free_wrong_results; ++ i)
    {
        FPRINTF_FFLUSH(stderr, "Wrong result:    %60s !\n", wrong_results[i]);
    }
    if (next_free_wrong_results > MAX_WRONG_RESULTS)
    {
        PUTS_FFLUSH("...")
    }

    // Wurden alle erwarteten Ergebnisse verwendet ?
    if (count_unused_expected_results != 0)
    {
        FPRINTF_FFLUSH(stderr, "\n==>> Unused expected results: %zu ! <<==\n", count_unused_expected_results);
    }
    // ===== ===== ===== ===== ===== ENDE Testbereich ===== ===== ===== ===== =====

    FREE_AND_SET_TO_NULL(count_expected_results_usage);
    Delete_Alkane_Container (all_alkanes);

    ASSERT_EQUALS(0, count_unused_expected_results);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Erwartete textbasierte Zeichnung mit einer erzeugten Zeichnung vergleichen.
 */
static _Bool
Compare_Expected_Drawing_With_Created_Drawing
(
        const char* const* restrict expected_drawing,                               // Erwartete textbasierte Zeichnung
        const struct Text_Based_Alkane_Drawing* const restrict created_drawing,     // Erzeugte textbasierte Zeichnung
        const size_t expected_drawing_dim_1,                                        // 1. Dimension erwartetes Ergebnis
        const size_t expected_drawing_dim_2,                                        // 2. Dimension erwartetes Ergebnis
        const size_t created_drawing_dim_1,                                         // 1. Dimension erzeugtes Ergebnis
        const size_t created_drawing_dim_2                                          // 2. Dimension erzeugtes Ergebnis
)
{
    // Beim Test, ob die Zeichnung richtig ist, wird Zeile fuer Zeile gebildet. Wenn alle Zeilen gleich sind, dann ist
    // das Ergebnis richtig
    _Bool wrong_result_occured                  = false;
    size_t first_wrong_result_string            = SIZE_MAX;
    size_t first_wrong_result_char_in_string    = SIZE_MAX;
    // Offset, damit beim Text_Based_Alkane_Drawing die richtige Zeile verwendet wird
    size_t line_index_offset                    = SIZE_MAX;

    for (size_t i = 0; i < created_drawing_dim_1; ++ i)
    {
        // Leere Zeilen werden ignoriert
        if (Contain_String_Only_Null_Symbols(created_drawing->drawing [i], created_drawing_dim_2)) { continue; }

        if (line_index_offset == SIZE_MAX)
        {
            line_index_offset = i;
        }
        // Zeichen fuer Zeichen das erwartete Ergebnis mit dem erzeugten Ergebnis vergleichen
        for (size_t i2 = 0; i2 < expected_drawing_dim_1; ++ i2)
        {
            for (size_t i3 = 0; i3 < expected_drawing_dim_2; ++ i3)
            {
                if (expected_drawing [i2][i3] != created_drawing->drawing [i2 + line_index_offset][i3])
                {
                    first_wrong_result_string           = i2;
                    first_wrong_result_char_in_string   = i3;
                    wrong_result_occured                = true;
                    break;
                }
            }
            if (wrong_result_occured /* == true */) { break; }
        }
        if (wrong_result_occured /* == true */) { break; }
    }

    // Ausgabe der erwarteten Loesung und der erzeugten Loesung
    // Dies dient fuer die bessere Uebersicht, falls Fehler auftauchen
    puts ("Expected result:");
    Print_2D_String_Array(expected_drawing, expected_drawing_dim_1, expected_drawing_dim_2);
    if (wrong_result_occured /* == true */)
    {
        printf ("Error in (%zu, %zu). ", first_wrong_result_string + 1, first_wrong_result_char_in_string + 1);
        printf ("Expected '%c'; Got '%c'\n\n",
                expected_drawing [first_wrong_result_string][first_wrong_result_char_in_string],
                created_drawing->drawing [first_wrong_result_string + line_index_offset][first_wrong_result_char_in_string]);
    }

    puts ("Created:");
    Show_Text_Based_Alkane_Drawing_W_O_Empty_Lines (created_drawing);
    fflush (stdout);

    return wrong_result_occured;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Zwei XPM-Dateien byteweise miteinander vergleichen.
 */
static _Bool
Compare_Two_XPM_Drawings
(
        const char* const restrict reference_file,          // Datei 1
        const char* const restrict test_file,               // Datei 2
        int_fast32_t* const restrict first_error_position   // Erste Position, wo die Dateien nicht uebereinstimmen
)
{
    _Bool result = true;

    char* read_buffer_1 = (char*) CALLOC(READ_BUFFER_SIZE, sizeof (char));
    ASSERT_ALLOC(read_buffer_1, "Cannot create a buffer for reading the reference file !",
            READ_BUFFER_SIZE * sizeof (char));
    char* read_buffer_2 = (char*) CALLOC(READ_BUFFER_SIZE, sizeof (char));
    ASSERT_ALLOC(read_buffer_2, "Cannot create a buffer for reading the test file !",
            READ_BUFFER_SIZE * sizeof (char));

    FILE* file_1 = FOPEN(reference_file, "r");
    ASSERT_FMSG(file_1 != NULL, "Cannot open file: \"%s\" !", reference_file);
    FILE* file_2 = FOPEN(test_file, "r");
    ASSERT_FMSG(file_2 != NULL, "Cannot open file: \"%s\" !", test_file);

    int setvbuf_result = setvbuf(file_1, read_buffer_1, _IOFBF, READ_BUFFER_SIZE * sizeof (char));
    ASSERT_FMSG(setvbuf_result == 0, "Cannot use a user defined buffer for file reading. Used buffer size: %zu bytes",
            (size_t) READ_BUFFER_SIZE * sizeof (char));
    setvbuf_result = setvbuf(file_2, read_buffer_2, _IOFBF, READ_BUFFER_SIZE * sizeof (char));
    ASSERT_FMSG(setvbuf_result == 0, "Cannot use a user defined buffer for file reading. Used buffer size: %zu bytes",
            (size_t) READ_BUFFER_SIZE * sizeof (char));

    // Dateien byteweise vergleichen
    int c1 = 0;
    int c2 = 0;
    int_fast32_t counter = 0;
    do
    {
        c1 = fgetc(file_1);
        c2 = fgetc(file_2);
        ++ counter;

        if (c1 != c2)
        {
            *first_error_position = counter;
            result = false;
            break;
        }
    }
    while (c1 != EOF && c2 != EOF);

    // Gab es Fehler bei den Dateioperationen?
    ASSERT_FMSG(ferror(file_1) == 0, "Error while reading the file \"%s\" !", reference_file);
    ASSERT_FMSG(ferror(file_2) == 0, "Error while reading the file \"%s\" !", test_file);

    FCLOSE_AND_SET_TO_NULL(file_1);
    FCLOSE_AND_SET_TO_NULL(file_2);
    FREE_AND_SET_TO_NULL(read_buffer_1);
    FREE_AND_SET_TO_NULL(read_buffer_2);

    return result;
}

//---------------------------------------------------------------------------------------------------------------------

#ifdef MAX_WRONG_RESULTS
#undef MAX_WRONG_RESULTS
#endif /* MAX_WRONG_RESULTS */

#ifdef MAX_LOOP_NESTING
#undef MAX_LOOP_NESTING
#endif /* MAX_LOOP_NESTING */

#ifdef CREATE_Test_Function_And_Their_Name
#undef CREATE_Test_Function_And_Their_Name
#endif /* CREATE_Test_Function_And_Their_Name */

#ifdef READ_BUFFER_SIZE
#undef READ_BUFFER_SIZE
#endif /* READ_BUFFER_SIZE */
