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
    const unsigned char branch_1_content [] = { 1 };
    const unsigned char branch_2_content [] = { 1, 1, 1 };
    const unsigned char branch_3_content [] = { 1, 1, 1, 1 };

    const unsigned char expected_result [] = { 1, 1, 2, 2, 1, 5, 5, 5 };

    struct Alkane_Branch* branch_1 = Create_Alkane_Branch (branch_1_content, COUNT_ARRAY_ELEMENTS(branch_1_content));
    struct Alkane_Branch* branch_2 = Create_Alkane_Branch (branch_2_content, COUNT_ARRAY_ELEMENTS(branch_2_content));
    struct Alkane_Branch* branch_3 = Create_Alkane_Branch (branch_3_content, COUNT_ARRAY_ELEMENTS(branch_3_content));

    struct Alkane* alkane = Create_Alkane (branch_1, branch_2, branch_3, NULL);
    // Print_Alkane (alkane);

    ASSERT_EQUALS(true, Compare_Alkane_Numbercodes (alkane, expected_result));

    Delete_Alkane (alkane);
    Delete_Alkane_Branch (branch_1);
    Delete_Alkane_Branch (branch_2);
    Delete_Alkane_Branch (branch_3);

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
 * Vergleichen zweier C-Strings OHNE Beachtung der Gross- und Kleinschreibung.
 */
static int                                              // 0, falls die Zeichenketten uebereinstimmen, ansonsten != 0
Compare_Strings_Case_Insensitive
(
        const char* const restrict string_1,            // 1. C-String
        const char* const restrict string_2             // 2. C-String
)
{
    char string_1_lowercase [255];
    char string_2_lowercase [255];
    memset (string_1_lowercase, '\0', sizeof (string_1_lowercase));
    memset (string_2_lowercase, '\0', sizeof (string_2_lowercase));
    memcpy (string_1_lowercase, string_1, strlen (string_1));
    memcpy (string_2_lowercase, string_2, strlen (string_2));

    // Alle alphabetischen Zeichen in Kleinbuchstaben konvertieren, damit spaeter ein Vergleich unabhaengig von der
    // Gross- und Kleinschreibung stattfinden kann
    for (size_t string_1_char = 0; string_1_char < strlen (string_1_lowercase); ++ string_1_char)
    {
        if (isupper (string_1_lowercase [string_1_char]) /* == true */)
        {
            string_1_lowercase [string_1_char] = (char) tolower (string_1_lowercase [string_1_char]);
        }
    }
    for (size_t string_2_char = 0; string_2_char < strlen (string_2_lowercase); ++ string_2_char)
    {
        if (isupper (string_2_lowercase [string_2_char]) /* == true */)
        {
            string_2_lowercase [string_2_char] = (char) tolower (string_2_lowercase [string_2_char]);
        }
    }

    // Nullterminierung nach den Operationen garantieren
    string_1_lowercase [sizeof (string_1_lowercase) - 1] = '\0';
    string_2_lowercase [sizeof (string_2_lowercase) - 1] = '\0';

    const size_t char_to_compare = (strlen (string_1_lowercase) < strlen (string_2_lowercase))
            ? strlen (string_1_lowercase) : strlen (string_2_lowercase);

    // Vergleich mit den angepassten Zeichenketten durchfuehren
    return strncmp (string_1_lowercase, string_2_lowercase, char_to_compare);
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

    // Den IUPAC-Namen mit allen erwarteten Ergebnissen vergleichen (Ohne Beachtung der Gross- und Kleinschreibung !)
    for (uint_fast64_t i = 0; i < number_of_expected_results; ++ i)
    {
        if (Compare_Strings_Case_Insensitive (iupac_name, expected_results [i]) == 0)
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
            FPRINTF_FFLUSH(stderr, "Expected results: %60s    used %" PRIuFAST8 " time(s) !\n", expected_results [i],
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
