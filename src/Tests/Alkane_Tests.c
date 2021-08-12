/**
 * Alkane_Tests.c
 *
 *  Created on: 13.03.2021
 *      Author: x86 / Gyps
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
    // Alkan-Tests ausfuehren
    RUN(TEST_Create_Alkane_Constitutional_Isomers);
    RUN(TEST_Create_Alkane);
    RUN(TEST_Convert_Alkane_To_IUPAC_Name);
    RUN(TEST_Convert_Alkane_To_IUPAC_Name_2);
    RUN(TEST_Convert_Alkane_To_IUPAC_Name_With_Manual_Chain_Objects);
    RUN(TEST_Convert_Alkane_With_Nested_2_To_IUPAC_Name);

    RUN(TEST_All_Possible_Butan_Constitutional_Isomers);
    RUN(TEST_All_Possible_Pentan_Constitutional_Isomers);
    RUN(TEST_All_Possible_Hexan_Constitutional_Isomers);
    RUN(TEST_All_Possible_Heptan_Constitutional_Isomers);
    RUN(TEST_All_Possible_Octan_Constitutional_Isomers);
    RUN(TEST_All_Possible_Nonan_Constitutional_Isomers);
    RUN(TEST_All_Possible_Decan_Constitutional_Isomers);
    RUN(TEST_All_Possible_Undecan_Constitutional_Isomers);
    RUN(TEST_All_Possible_Dodecan_Constitutional_Isomers);
    RUN(TEST_All_Possible_Tridecan_Constitutional_Isomers);
    RUN(TEST_All_Possible_Tetradecan_Constitutional_Isomers);

    // Um unused-Warnungen beim Programm Cppcheck zu vermeiden
    (void) TEST_All_Possible_Decan_Constitutional_Isomers;
    (void) TEST_All_Possible_Undecan_Constitutional_Isomers;
    (void) TEST_All_Possible_Dodecan_Constitutional_Isomers;
    (void) TEST_All_Possible_Tridecan_Constitutional_Isomers;
    (void) TEST_All_Possible_Tetradecan_Constitutional_Isomers;

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
    const char* expected_result            = "7-Methyl-5-PropylOctan";

    struct Alkane_Branch* branch_1 = Create_Alkane_Branch (branch_1_content, COUNT_ARRAY_ELEMENTS(branch_1_content));
    struct Alkane_Branch* branch_2 = Create_Alkane_Branch (branch_2_content, COUNT_ARRAY_ELEMENTS(branch_2_content));
    struct Alkane_Branch* branch_3 = Create_Alkane_Branch (branch_3_content, COUNT_ARRAY_ELEMENTS(branch_3_content));

    struct Alkane* alkane = Create_Alkane (branch_2, branch_1, branch_3, NULL);

    // Konvertierung durchfuehren
    Convert_Alkane_To_IUPAC_Name (alkane);

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

    struct Alkane_Branch* branch_1 = Create_Alkane_Branch (branch_1_content, COUNT_ARRAY_ELEMENTS(branch_1_content));
    struct Alkane_Branch* branch_2 = Create_Alkane_Branch (branch_2_content, COUNT_ARRAY_ELEMENTS(branch_2_content));
    struct Alkane_Branch* branch_3 = Create_Alkane_Branch (branch_3_content, COUNT_ARRAY_ELEMENTS(branch_3_content));

    struct Alkane* alkane = Create_Alkane (branch_1, branch_2, branch_3, NULL);

    // Konvertierung durchfuehren
    Convert_Alkane_To_IUPAC_Name (alkane);

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
    Convert_Alkane_To_IUPAC_Name (alkane);

    ASSERT_STRING_EQUALS(expected_result, alkane->iupac_name);

    Delete_Alkane (alkane);
    Delete_Alkane_Branch (branch_1);
    Delete_Alkane_Branch (branch_2);
    Delete_Alkane_Branch (branch_3);

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
        strncpy (converted_iupac_name, iupac_name, strlen (iupac_name));

        memset (converted_expected_results, '\0', sizeof (converted_expected_results));
        strncpy (converted_expected_results, expected_results [i], strlen (expected_results [i]));

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

    // ===== ===== ===== BEGINN Testbereich ===== ===== =====
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

        Convert_Alkane_To_IUPAC_Name (all_alkanes->data [i]);

        // Falls ein erwartetes Ergebnis verwendet wurde, dann wird in dieser Variable der Index des erwarteten
        // Ergebnisses hinterlegt
        uint_fast64_t index_in_the_expected_results = UINT_FAST64_MAX;

        // Befindet sich das gerade erzeugte Ergebnis in der Liste an gueltigen Ergebnissen ?
        const _Bool result_found_in_the_expected_results =
                Search_IUPAC_Name_In_The_List_Of_Expected_Results (all_alkanes->data [i]->iupac_name, expected_results,
                        number_of_constitutional_isomers, &index_in_the_expected_results);

        // Wenn sich das Ergebnis nicht in der Liste befindet, dann wird das Programm mit einer Fehlermeldung beendet
        if (! result_found_in_the_expected_results /* == false */)
        {
            FPRINTF_FFLUSH(stderr, "Cannot find the current result \"%s\" in the list of expected results !\n",
                    all_alkanes->data [i]->iupac_name);
        }
        else
        {
            count_expected_results_usage [index_in_the_expected_results] ++;
        }
    }

    puts ("");
    size_t count_unused_expected_results = 0;

    // Welche erwarteten Ergebnisse wurden nicht GENAU einmal verwendet ?
    for (size_t i = 0; i < number_of_expected_results; ++ i)
    {
        if (count_expected_results_usage [i] != 1)
        {
            FPRINTF_FFLUSH(stderr, "Expected results: %60s    used %" PRIuFAST8 " times !\n", expected_results [i],
                    count_expected_results_usage [i]);
            ++ count_unused_expected_results;
        }
    }

    // Wurden alle erwarteten Ergebnisse verwendet ?
    if (count_unused_expected_results != 0)
    {
        FPRINTF_FFLUSH(stderr, "\n==>> Unused expected results: %zu ! <<==\n", count_unused_expected_results);
    }
    // ===== ===== ===== ENDE Testbereich ===== ===== =====

    FREE_AND_SET_TO_NULL(count_expected_results_usage);
    Delete_Alkane_Container (all_alkanes);

    ASSERT_EQUALS(0, count_unused_expected_results);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
