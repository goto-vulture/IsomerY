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
#include "../Alkane/Alkane.h"
#include "../Alkane/Alkane_Branch.h"
#include "../Alkane/Alkane_Container.h"
#include "../Alkane/Alkane_Create_Constitutional_Isomer.h"
#include "../Alkane/Alkane_Info_Constitutional_Isomer.h"
#include "../Alkane/Alkane_To_IUPAC_Name.h"
#include "../Alkane/Alkane_Chain_To_IUPAC_Name.h"
#include "../Misc.h"
#include "../Print_Tools.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Error_Handling/Assert_Msg.h"
#include "../CLI_Parameter.h"
#include "IUPAC_Chain_Lexer.h"
#include "../str2int.h"



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
 * Eine Zeichenkette kopieren, in der nur Kleinbuchstaben aus der Original-Zeichenkette vorkommen.
 */
static void
String_To_Lower
(
        const char* const restrict orig_string,     // Originale Zeichenkette, die konvertiert werden soll
        char* const restrict to_lower_string,       // Zielspeicher fuer die konvertierte Zeichenkette
        const size_t to_lower_string_size           // Groesse des Zielspeichers
);

/**
 * Vergleichen zweier C-Strings OHNE Beachtung der Gross- und Kleinschreibung.
 */
static int                                              // 0, falls die Zeichenketten uebereinstimmen, ansonsten != 0
Compare_Strings_Case_Insensitive
(
        const char* const restrict string_1,            // 1. C-String
        const char* const restrict string_2             // 2. C-String
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

    // Makro, um aus dem Funktionsnamen automatisch die passende Zeichenkette zu generieren
    #ifndef CREATE_Test_Function_And_Their_Name
    #define CREATE_Test_Function_And_Their_Name(function) { (function), (#function) }
    #else
    #error "The macro \"CREATE_Test_Function_And_Their_Name\" is already defined !"
    #endif /* CREATE_Test_Function_And_Their_Name */

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
            CREATE_Test_Function_And_Their_Name(TEST_Group_Compression)
    };

    #ifdef CREATE_Test_Function_And_Their_Name
    #undef CREATE_Test_Function_And_Their_Name
    #endif /* CREATE_Test_Function_And_Their_Name */

    // Soll eine Testfunktion anhand einer dynamischen Auswahl ausgefuehrt werden ?
    if (GLOBAL_SELECT_TEST_FUNCTION /* == true */)
    {
        char input_buffer [10] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
        long int selected_test_function = 0;

        puts("===>>> Testfunctions available <<<===");
        for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(test_functions); ++ i)
        {
            printf ("%2zu: %s\n", i + 1, test_functions[i].function_name);
        }
        fflush(stdout);

        // Einleseschleife
        while (true)
        {
            int c = 0;
            printf ("(1 - %zu) => ", COUNT_ARRAY_ELEMENTS((test_functions)));
            scanf ("%9s", input_buffer);

            // Idee von: https://stackoverflow.com/questions/28297306/how-to-limit-input-length-with-scanf
            while ((c = fgetc(stdin)) != '\n' && c != EOF); /* Flush stdin */

            // Ist die Eingabe ueberhaupt ein Integer?
            const enum str2int_errno conversion_errno = str2int(&selected_test_function, input_buffer, 10);

            if (conversion_errno == STR2INT_SUCCESS && selected_test_function > 0 &&
                    selected_test_function <= (long int) COUNT_ARRAY_ELEMENTS(test_functions))
            {
                break;
            }
            puts("Invalid input !");
            memset (input_buffer, '\0', sizeof(input_buffer));
        }

        PRINTF_FFLUSH("\nUsing the test function \"%s\"\n", test_functions [selected_test_function - 1].function_name);

        // Testfunktion aufrufen
        RUN_2(test_functions [selected_test_function - 1].test_function,
                test_functions [selected_test_function - 1].function_name);
    }
    // Testfunktion anhand der Anzahl an C-Atomen, die eingegeben worden sind, aufrufen
    if (GLOBAL_MAX_C_ATOMS_FOR_TESTS != 0)
    {
        RUN_2(test_functions [GLOBAL_MAX_C_ATOMS_FOR_TESTS - 1].test_function,
                test_functions [GLOBAL_MAX_C_ATOMS_FOR_TESTS - 1].function_name);
    }

    // Ergebnisse aller durchgefuehrten Tests abfragen
    return TEST_REPORT();
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Die Erstellung von Alkanen aus Alkanaesten erstellen.
 *
 * Wird der Nummerncode richtig gebildet ?
 */
void TEST_Create_Alkane (void)
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
void TEST_Create_Alkane_Constitutional_Isomers (void)
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
void TEST_Convert_Alkane_To_IUPAC_Name (void)
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
void TEST_Convert_Alkane_To_IUPAC_Name_2 (void)
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
void TEST_Convert_Alkane_To_IUPAC_Name_With_Manual_Chain_Objects (void)
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
void TEST_Convert_Alkane_With_Nested_2_To_IUPAC_Name (void)
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
void TEST_All_Possible_Methan_Constitutional_Isomers (void)
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
void TEST_All_Possible_Ethan_Constitutional_Isomers (void)
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
void TEST_All_Possible_Propan_Constitutional_Isomers (void)
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
void TEST_All_Possible_Butan_Constitutional_Isomers (void)
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
void TEST_All_Possible_Pentan_Constitutional_Isomers (void)
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
void TEST_All_Possible_Hexan_Constitutional_Isomers (void)
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
void TEST_All_Possible_Heptan_Constitutional_Isomers (void)
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
void TEST_All_Possible_Octan_Constitutional_Isomers (void)
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
void TEST_All_Possible_Nonan_Constitutional_Isomers (void)
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
void TEST_All_Possible_Decan_Constitutional_Isomers (void)
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
void TEST_All_Possible_Undecan_Constitutional_Isomers (void)
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
void TEST_All_Possible_Dodecan_Constitutional_Isomers (void)
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
void TEST_All_Possible_Tridecan_Constitutional_Isomers (void)
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
void TEST_All_Possible_Tetradecan_Constitutional_Isomers (void)
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
void TEST_Group_Compression (void)
{
    const char expected_iupac_name [] = "4,4-Bis(1-methylethyl)heptane";

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
 * Eine Zeichenkette kopieren, in der nur Kleinbuchstaben aus der Original-Zeichenkette vorkommen.
 */
static void
String_To_Lower
(
        const char* const restrict orig_string,     // Originale Zeichenkette, die konvertiert werden soll
        char* const restrict to_lower_string,       // Zielspeicher fuer die konvertierte Zeichenkette
        const size_t to_lower_string_size           // Groesse des Zielspeichers
)
{
    strncpy (to_lower_string, orig_string, to_lower_string_size);

    for (size_t current_char = 0;
            (current_char < strlen (orig_string)) && (current_char < to_lower_string_size);
            ++ current_char)
    {
        // Alle Zeichen der Zeichenkette in Kleinbuchstaben umwandeln.
        // Eine Ueberpruefung, ob das aktuelle Zeichen ein Grossbuchstabe ist, ist nicht notwendig, da tolower() diesen
        // Test bereits durchfuehrt.
        // if (isupper (orig_string [current_char]) /* == true */)
            to_lower_string [current_char] = (char) tolower (orig_string [current_char]);
    }

    // Nullterminierung im Zielspeicher garantieren
    to_lower_string [strlen (orig_string)] = '\0';

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Vergleichen zweier C-Strings OHNE Beachtung der Gross- und Kleinschreibung.
 */
static int                                              // 0, falls die Zeichenketten uebereinstimmen, ansonsten != 0
Compare_Strings_Case_Insensitive
(
        const char* const restrict string_1,            // 1. C-String
        const char* const restrict string_2             // 2. C-String
)
{
    // Wenn die Laenge der Zeichenketten nicht identisch sind, dann koennen sie - auch ohne Beachtung der Gross- und
    // Keinschreibung - nicht gleich sein !
    if (strlen (string_1) != strlen (string_2))
    {
        return -1;
    }

    char string_1_lowercase [255];
    char string_2_lowercase [255];
    memset (string_1_lowercase, '\0', sizeof (string_1_lowercase));
    memset (string_2_lowercase, '\0', sizeof (string_2_lowercase));

    // Alle alphabetischen Zeichen in Kleinbuchstaben konvertieren, damit spaeter ein Vergleich unabhaengig von der
    // Gross- und Kleinschreibung stattfinden kann
    String_To_Lower(string_1, string_1_lowercase, COUNT_ARRAY_ELEMENTS(string_1_lowercase));
    String_To_Lower(string_2, string_2_lowercase, COUNT_ARRAY_ELEMENTS(string_2_lowercase));

    // Vergleich mit den angepassten Zeichenketten durchfuehren
    // Es gibt auch die Funktion "strncasecmp()" die vorzeichenlos Zeichenketten vergleicht. Diese ist aber leider eine
    // GNU-Extension und daher nicht auf allen Systemen verfuegbar !
    return strncmp (string_1_lowercase, string_2_lowercase, strlen (string_1_lowercase));
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

#ifdef MAX_WRONG_RESULTS
#undef MAX_WRONG_RESULTS
#endif /* MAX_WRONG_RESULTS */

#ifdef MAX_LOOP_NESTING
#undef MAX_LOOP_NESTING
#endif /* MAX_LOOP_NESTING */
