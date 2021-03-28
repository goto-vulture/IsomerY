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
        const uint_fast64_t number_of_expected_results  // Anzahl an erwarteten Loesungen
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

    // Ergebnisse aller durchgefuehrten Tests anzeigen
    const int test_report = TEST_REPORT();

    return test_report;
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
 * Aus Laufzeitgruenden wird nur bis inklusive 12 getestet.
 */
void TEST_Create_Alkane_Constitutional_Isomers (void)
{
    for (uint_fast8_t i = 1; i <= 12; ++ i)
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
    const char* expected_result            = "5-Propyl-7-MethylOctan";

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

    const char* expected_result            = "6-Propyl-(2,2-DiMethyl)Hexadecan";

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
            "n-Butan",
            "2-Methylpropan"
    };

    // Alle Alkane erzeugen
    struct Alkane_Container* butane_alkanes = Create_Alkane_Constitutional_Isomers (number_of_c_atoms);

    // Fuer alle gerade erzeugten Alkane den IUPAC-Namen bilden
    for (uint_fast64_t i = 0; i < number_of_constitutional_isomers; ++ i)
    {
        Convert_Alkane_To_IUPAC_Name (butane_alkanes->data [i]);

        // Befindet sich das gerade erzeugte Ergebnis in der Liste an gueltigen Ergebnissen ?
        const _Bool result_found_in_the_expected_results =
                Search_IUPAC_Name_In_The_List_Of_Expected_Results (butane_alkanes->data [i]->iupac_name, expected_results,
                        number_of_constitutional_isomers);

        // Wenn sich das Ergebnis nicht in der Liste befindet, dann wird das Programm mit einer Fehlermeldung beendet
        if (! result_found_in_the_expected_results /* == false */)
        {
            FPRINTF_FFLUSH(stderr, "Cannot find the current result \"%s\" in the list of expected results !\n",
                    butane_alkanes->data [i]->iupac_name);
        }
        ASSERT("Cannot find the current result in the list of expected results !",
                result_found_in_the_expected_results == true);
    }

    // Erzeugten Alkane_Container wieder loeschen
    Delete_Alkane_Container (butane_alkanes);

    puts ("\n");

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
            "n-Pentan",
            "2-Methylbutan",
            "2,2-Dimethylpropan"
    };

    // Alle Alkane erzeugen
    struct Alkane_Container* butane_alkanes = Create_Alkane_Constitutional_Isomers (number_of_c_atoms);

    // Fuer alle gerade erzeugten Alkane den IUPAC-Namen bilden
    for (uint_fast64_t i = 0; i < number_of_constitutional_isomers; ++ i)
    {
        Convert_Alkane_To_IUPAC_Name (butane_alkanes->data [i]);

        // Befindet sich das gerade erzeugte Ergebnis in der Liste an gueltigen Ergebnissen ?
        const _Bool result_found_in_the_expected_results =
                Search_IUPAC_Name_In_The_List_Of_Expected_Results (butane_alkanes->data [i]->iupac_name, expected_results,
                        number_of_constitutional_isomers);

        // Wenn sich das Ergebnis nicht in der Liste befindet, dann wird das Programm mit einer Fehlermeldung beendet
        if (! result_found_in_the_expected_results /* == false */)
        {
            FPRINTF_FFLUSH(stderr, "Cannot find the current result \"%s\" in the list of expected results !\n",
                    butane_alkanes->data [i]->iupac_name);
        }
        ASSERT("Cannot find the current result in the list of expected results !",
                result_found_in_the_expected_results == true);
    }

    // Erzeugten Alkane_Container wieder loeschen
    Delete_Alkane_Container (butane_alkanes);

    puts ("\n");

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
            "n-Hexan",
            "2-Methylpentan",
            "3-Methylpentan",
            "2,2-Dimethylbutan",
            "2,3-Dimethylbutan"
    };

    // Alle Alkane erzeugen
    struct Alkane_Container* hexane_alkanes = Create_Alkane_Constitutional_Isomers (number_of_c_atoms);

    // Fuer alle gerade erzeugten Alkane den IUPAC-Namen bilden
    for (uint_fast64_t i = 0; i < number_of_constitutional_isomers; ++ i)
    {
        Convert_Alkane_To_IUPAC_Name (hexane_alkanes->data [i]);

        // Befindet sich das gerade erzeugte Ergebnis in der Liste an gueltigen Ergebnissen ?
        const _Bool result_found_in_the_expected_results =
                Search_IUPAC_Name_In_The_List_Of_Expected_Results (hexane_alkanes->data [i]->iupac_name, expected_results,
                        number_of_constitutional_isomers);

        // Wenn sich das Ergebnis nicht in der Liste befindet, dann wird das Programm mit einer Fehlermeldung beendet
        if (! result_found_in_the_expected_results /* == false */)
        {
            FPRINTF_FFLUSH(stderr, "Cannot find the current result \"%s\" in the list of expected results !\n",
                    hexane_alkanes->data [i]->iupac_name);
        }
        ASSERT("Cannot find the current result in the list of expected results !",
                result_found_in_the_expected_results == true);
    }

    // Erzeugten Alkane_Container wieder loeschen
    Delete_Alkane_Container (hexane_alkanes);

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
            "n-Heptan",
            "2-Methylhexan",
            "3-Methylhexan",
            "2,2-Dimethylpentan",
            "2,3-Dimethylpentan",
            "2,4-Dimethylpentan",
            "3,3-Dimethylpentan",
            "3-Ethylpentan",
            "2,2,3-Trimethylbutan"
    };

    // Alle Alkane erzeugen
    struct Alkane_Container* heptane_alkanes = Create_Alkane_Constitutional_Isomers (number_of_c_atoms);

    // Fuer alle gerade erzeugten Alkane den IUPAC-Namen bilden
    for (uint_fast64_t i = 0; i < number_of_constitutional_isomers; ++ i)
    {
        Convert_Alkane_To_IUPAC_Name (heptane_alkanes->data [i]);

        // Befindet sich das gerade erzeugte Ergebnis in der Liste an gueltigen Ergebnissen ?
        const _Bool result_found_in_the_expected_results =
                Search_IUPAC_Name_In_The_List_Of_Expected_Results (heptane_alkanes->data [i]->iupac_name, expected_results,
                        number_of_constitutional_isomers);

        // Wenn sich das Ergebnis nicht in der Liste befindet, dann wird das Programm mit einer Fehlermeldung beendet
        if (! result_found_in_the_expected_results /* == false */)
        {
            FPRINTF_FFLUSH(stderr, "Cannot find the current result \"%s\" in the list of expected results !\n",
                    heptane_alkanes->data [i]->iupac_name);
        }
        ASSERT("Cannot find the current result in the list of expected results !",
                result_found_in_the_expected_results == true);
    }

    // Erzeugten Alkane_Container wieder loeschen
    Delete_Alkane_Container (heptane_alkanes);

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
            "n-Octan",
            "2-Methylheptan",
            "3-Methylheptan",
            "4-Methylheptan",
            "2,2-Dimethylhexan",
            "2,3-Dimethylhexan",
            "2,4-Dimethylhexan",
            "2,5-Dimethylhexan",
            "3,3-Dimethylhexan",
            "3,4-Dimethylhexan",

            "3-Ethylhexan",
            "2,2,3-Trimethylpentan",
            "2,2,4-Trimethylpentan",
            "2,3,3-Trimethylpentan",
            "2,3,4-Trimethylpentan",
            "3-Ethyl-2-methylpentan",
            "3-Ethyl-3-methylpentan",
            "2,2,3,3-Tetramethylbutan"
    };

    // Alle Alkane erzeugen
    struct Alkane_Container* octane_alkanes = Create_Alkane_Constitutional_Isomers (number_of_c_atoms);

    // Fuer alle gerade erzeugten Alkane den IUPAC-Namen bilden
    for (uint_fast64_t i = 0; i < number_of_constitutional_isomers; ++ i)
    {
        Convert_Alkane_To_IUPAC_Name (octane_alkanes->data [i]);

        // Befindet sich das gerade erzeugte Ergebnis in der Liste an gueltigen Ergebnissen ?
        const _Bool result_found_in_the_expected_results =
                Search_IUPAC_Name_In_The_List_Of_Expected_Results (octane_alkanes->data [i]->iupac_name, expected_results,
                        number_of_constitutional_isomers);

        // Wenn sich das Ergebnis nicht in der Liste befindet, dann wird das Programm mit einer Fehlermeldung beendet
        if (! result_found_in_the_expected_results /* == false */)
        {
            FPRINTF_FFLUSH(stderr, "Cannot find the current result \"%s\" in the list of expected results !\n",
                    octane_alkanes->data [i]->iupac_name);
        }
        ASSERT("Cannot find the current result in the list of expected results !",
                result_found_in_the_expected_results == true);
    }

    // Erzeugten Alkane_Container wieder loeschen
    Delete_Alkane_Container (octane_alkanes);

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

    // Alle 35 Octane
    // Siehe: https://de.wikipedia.org/wiki/Nonane
    const char* expected_results [] =
    {
            "n-Nonan",
            "2-Methyloctan",
            "3-Methyloctan",
            "4-Methyloctan",
            "2,2-Dimethylheptan",
            "2,3-Dimethylheptan",
            "2,4-Dimethylheptan",
            "2,5-Dimethylheptan",
            "2,6-Dimethylheptan",
            "3,3-Dimethylheptan",

            "3,4-Dimethylheptan",
            "3,5-Dimethylheptan",
            "4,4-Dimethylheptan"
            "3-Ethylheptan",
            "4-Ethylheptan",
            "2,2,3-Trimethylhexan",
            "2,2,4-Trimethylhexan",
            "2,2,5-Trimethylhexan",
            "2,3,3-Trimethylhexan",
            "2,3,4-Trimethylhexan",

            "2,3,5-Trimethylhexan",
            "2,4,4-Trimethylhexan",
            "3,3,4-Trimethylhexan",
            "3-Ethyl-2-methylhexan",
            "4-Ethyl-2-methylhexan",
            "3-Ethyl-3-methylhexan",
            "4-Ethyl-3-methylhexan",
            "2,2,3,3-Tetramethylpentan",
            "2,2,3,4-Tetramethylpentan",
            "2,2,4,4-Tetramethylpentan",

            "2,3,3,4-Tetramethylpentan",
            "3-Ethyl-2,2-dimethylpentan",
            "3-Ethyl-2,3-dimethylpentan",
            "3-Ethyl-2,4-dimethylpentan",
            "3,3-Diethylpentan"
    };

    // Alle Alkane erzeugen
    struct Alkane_Container* nonane_alkanes = Create_Alkane_Constitutional_Isomers (number_of_c_atoms);

    // Fuer alle gerade erzeugten Alkane den IUPAC-Namen bilden
    for (uint_fast64_t i = 0; i < number_of_constitutional_isomers; ++ i)
    {
        Convert_Alkane_To_IUPAC_Name (nonane_alkanes->data [i]);

        // Befindet sich das gerade erzeugte Ergebnis in der Liste an gueltigen Ergebnissen ?
        const _Bool result_found_in_the_expected_results =
                Search_IUPAC_Name_In_The_List_Of_Expected_Results (nonane_alkanes->data [i]->iupac_name, expected_results,
                        number_of_constitutional_isomers);

        // Wenn sich das Ergebnis nicht in der Liste befindet, dann wird das Programm mit einer Fehlermeldung beendet
        if (! result_found_in_the_expected_results /* == false */)
        {
            FPRINTF_FFLUSH(stderr, "Cannot find the current result \"%s\" in the list of expected results !\n",
                    nonane_alkanes->data [i]->iupac_name);
        }
        ASSERT("Cannot find the current result in the list of expected results !",
                result_found_in_the_expected_results == true);
    }

    // Erzeugten Alkane_Container wieder loeschen
    Delete_Alkane_Container (nonane_alkanes);

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
    const uint_fast8_t number_of_c_atoms = 9;
    const uint_fast64_t number_of_constitutional_isomers = NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [number_of_c_atoms - 1];

    // Alle 75 Decane
    // Siehe: https://de.wikipedia.org/wiki/Decane
    const char* expected_results [] =
    {
            "n-Decan",                          "2-Methylnonan",                        "3-Methylnonan",
            "4-Methylnonan",                    "5-Methylnonan",

            "2,2-Dimethyloctan",                "2,3-Dimethyloctan",                    "2,4-Dimethyloctan",
            "2,5-Dimethyloctan",                "2,6-Dimethyloctan",

            "2,7-Dimethyloctan",                "3,3-Dimethyloctan",                    "3,4-Dimethyloctan",
            "3,5-Dimethyloctan",                "3,6-Dimethyloctan",

            "4,4-Dimethyloctan",                "4,5-Dimethyloctan",                    "2,2,3-Trimethylheptan",
            "2,2,4-Trimethylheptan",            "2,2,5-Trimethylheptan",

            "2,2,6-Trimethylheptan",            "2,3,3-Trimethylheptan",                "2,3,4-Trimethylheptan",
            "2,3,5-Trimethylheptan",            "2,3,6-Trimethylheptan",

            "2,4,4-Trimethylheptan",            "2,4,5-Trimethylheptan",                "2,4,6-Trimethylheptan",
            "2,5,5-Trimethylheptan",            "3,3,4-Trimethylheptan",

            "3,3,5-Trimethylheptan",            "3,4,4-Trimethylheptan",                "3,4,5-Trimethylheptan",
            "2,2,3,3-Tetramethylhexan",         "2,2,3,4-Tetramethylhexan",

            "2,2,3,5-Tetramethylhexan",         "2,2,4,4-Tetramethylhexan",             "2,2,4,5-Tetramethylhexan",
            "2,2,5,5-Tetramethylhexan",         "2,3,3,4-Tetramethylhexan",

            "2,3,3,5-Tetramethylhexan",         "2,3,4,4-Tetramethylhexan",             "2,3,4,5-Tetramethylhexan",
            "3,3,4,4-Tetramethylhexan",         "2,2,3,3,4-Pentamethylpentan",

            "2,2,3,4,4-Pentamethylpentan",      "3-Ethyloctan",                         "4-Ethyloctan",
            "3,3-Diethylhexan",                 "3,4-Diethylhexan",

            "3-Ethyl-2-methylheptan",           "3-Ethyl-3-methylheptan",               "3-Ethyl-4-methylheptan",
            "3-Ethyl-5-methylheptan",           "4-Ethyl-2-methylheptan",

            "4-Ethyl-3-methylheptan",           "4-Ethyl-4-methylheptan",               "5-Ethyl-2-methylheptan",
            "3-Ethyl-2,2-dimethylhexan",        "3-Ethyl-2,3-dimethylhexan",

            "3-Ethyl-2,4-dimethylhexan",        "3-Ethyl-2,5-dimethylhexan",            "3-Ethyl-3,4-dimethylhexan",
            "4-Ethyl-2,2-dimethylhexan",        "4-Ethyl-2,3-dimethylhexan",

            "4-Ethyl-2,4-dimethylhexan",        "4-Ethyl-3,3-dimethylhexan",            "3-Ethyl-2,2,3-trimethylpentan",
            "3-Ethyl-2,2,4-trimethylpentan",    "3-Ethyl-2,3,4-trimethylpentan",

            "3,3-Diethyl-2-methylpentan",       "4-Propylheptan",                       "4-(1-Methylethyl)heptan",
            "2-Methyl-3-(1-methylethyl)-hexan", "2,4-Dimethyl-3-(1-methylethyl)-pentan"
    };

    // Alle Alkane erzeugen
    struct Alkane_Container* decane_alkanes = Create_Alkane_Constitutional_Isomers (number_of_c_atoms);

    // Fuer alle gerade erzeugten Alkane den IUPAC-Namen bilden
    for (uint_fast64_t i = 0; i < number_of_constitutional_isomers; ++ i)
    {
        Convert_Alkane_To_IUPAC_Name (decane_alkanes->data [i]);

        // Befindet sich das gerade erzeugte Ergebnis in der Liste an gueltigen Ergebnissen ?
        const _Bool result_found_in_the_expected_results =
                Search_IUPAC_Name_In_The_List_Of_Expected_Results (decane_alkanes->data [i]->iupac_name, expected_results,
                        number_of_constitutional_isomers);

        // Wenn sich das Ergebnis nicht in der Liste befindet, dann wird das Programm mit einer Fehlermeldung beendet
        if (! result_found_in_the_expected_results /* == false */)
        {
            FPRINTF_FFLUSH(stderr, "Cannot find the current result \"%s\" in the list of expected results !\n",
                    decane_alkanes->data [i]->iupac_name);
        }
        ASSERT("Cannot find the current result in the list of expected results !",
                result_found_in_the_expected_results == true);
    }

    // Erzeugten Alkane_Container wieder loeschen
    Delete_Alkane_Container (decane_alkanes);

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
        const uint_fast64_t number_of_expected_results  // Anzahl an erwarteten Loesungen
)
{
    _Bool result_found = false;

    // Den IUPAC-Namen mit allen erwarteten Ergebnissen vergleichen (Ohne Beachtung der Gross- und Kleinschreibung !)
    for (uint_fast64_t i = 0; i < number_of_expected_results; ++ i)
    {
        if (Compare_Strings_Case_Insensitive (iupac_name, expected_results [i]) == 0)
        {
            result_found = true;
            break;
        }
    }

    return result_found;
}

//---------------------------------------------------------------------------------------------------------------------
