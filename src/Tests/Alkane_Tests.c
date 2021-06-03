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
        const uint_fast64_t number_of_expected_results, // Anzahl an erwarteten Loesungen
        uint_fast64_t* const restrict result_index      // Index des gefundenen IUPAC-Namen in den erwarteten Loesungen
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
//    RUN(TEST_Create_Alkane_Constitutional_Isomers);
//    RUN(TEST_Create_Alkane);
//    RUN(TEST_Convert_Alkane_To_IUPAC_Name);
//    RUN(TEST_Convert_Alkane_To_IUPAC_Name_2);
//    RUN(TEST_Convert_Alkane_With_Nested_2_To_IUPAC_Name);
//
//    RUN(TEST_All_Possible_Butan_Constitutional_Isomers);
//    RUN(TEST_All_Possible_Pentan_Constitutional_Isomers);
//    RUN(TEST_All_Possible_Hexan_Constitutional_Isomers);
//    RUN(TEST_All_Possible_Heptan_Constitutional_Isomers);
//    RUN(TEST_All_Possible_Octan_Constitutional_Isomers);
    RUN(TEST_All_Possible_Nonan_Constitutional_Isomers);
    RUN(TEST_All_Possible_Decan_Constitutional_Isomers);

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
                        number_of_constitutional_isomers, NULL);

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
                        number_of_constitutional_isomers, NULL);

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
                        number_of_constitutional_isomers, NULL);

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
                        number_of_constitutional_isomers, NULL);

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

    // Die Verwendung der erwarteten Ergebnisse zaehlen, um moegliche Mehrfachverwendungen oder fehlende Verwendungen
    // erkennen zu koennen
    // Wenn alles richtig laeuft, dann muss jedes erwartete Ergebnis GENAU einmal verwendet werden
    uint_fast8_t count_expected_results [COUNT_ARRAY_ELEMENTS(expected_results)];
    memset (count_expected_results, '\0', sizeof (count_expected_results));

    // Fuer alle gerade erzeugten Alkane den IUPAC-Namen bilden
    for (uint_fast64_t i = 0; i < number_of_constitutional_isomers; ++ i)
    {
        Convert_Alkane_To_IUPAC_Name (octane_alkanes->data [i]);

        // Welches erwartete Ergebnis genau wurde verwendet ?
        uint_fast64_t index_in_the_expected_results = UINT_FAST64_MAX;

        // Befindet sich das gerade erzeugte Ergebnis in der Liste an gueltigen Ergebnissen ?
        const _Bool result_found_in_the_expected_results =
                Search_IUPAC_Name_In_The_List_Of_Expected_Results (octane_alkanes->data [i]->iupac_name, expected_results,
                        number_of_constitutional_isomers, &index_in_the_expected_results);

        // Wenn sich das Ergebnis nicht in der Liste befindet, dann wird das Programm mit einer Fehlermeldung beendet
        if (! result_found_in_the_expected_results /* == false */)
        {
            FPRINTF_FFLUSH(stderr, "Cannot find the current result \"%s\" in the list of expected results !\n",
                    octane_alkanes->data [i]->iupac_name);
        }
        else
        {
            count_expected_results [index_in_the_expected_results] ++;
        }
        ASSERT("Cannot find the current result in the list of expected results !",
                result_found_in_the_expected_results == true);
    }

    // Welche erwarteten Ergebnisse wurden nicht GENAU einmal verwendet ?
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(expected_results); ++ i)
    {
        if (count_expected_results [i] != 1)
        {
            FPRINTF_FFLUSH(stderr, "Expected results: %40s    used %" PRIuFAST8 " time(s) !\n", expected_results [i],
                    count_expected_results [i]);
        }
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

    // Alle 35 Nonane
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
            "4,4-Dimethylheptan",
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

    // Die Verwendung der erwarteten Ergebnisse zaehlen, um moegliche Mehrfachverwendungen oder fehlende Verwendungen
    // erkennen zu koennen
    // Wenn alles richtig laeuft, dann muss jedes erwartete Ergebnis GENAU einmal verwendet werden
    uint_fast8_t count_expected_results [COUNT_ARRAY_ELEMENTS(expected_results)];
    memset (count_expected_results, '\0', sizeof (count_expected_results));

    // Fuer alle gerade erzeugten Alkane den IUPAC-Namen bilden
    for (uint_fast64_t i = 0; i < number_of_constitutional_isomers; ++ i)
    {
        Convert_Alkane_To_IUPAC_Name (nonane_alkanes->data [i]);

        // Welches erwartete Ergebnis genau wurde verwendet ?
        uint_fast64_t index_in_the_expected_results = UINT_FAST64_MAX;

        // Befindet sich das gerade erzeugte Ergebnis in der Liste an gueltigen Ergebnissen ?
        const _Bool result_found_in_the_expected_results =
                Search_IUPAC_Name_In_The_List_Of_Expected_Results (nonane_alkanes->data [i]->iupac_name, expected_results,
                        number_of_constitutional_isomers, &index_in_the_expected_results);

        // Wenn sich das Ergebnis nicht in der Liste befindet, dann wird das Programm mit einer Fehlermeldung beendet
        if (! result_found_in_the_expected_results /* == false */)
        {
            FPRINTF_FFLUSH(stderr, "Cannot find the current result \"%s\" in the list of expected results !\n",
                    nonane_alkanes->data [i]->iupac_name);
        }
        else
        {
            count_expected_results [index_in_the_expected_results] ++;
        }
//        ASSERT("Cannot find the current result in the list of expected results !",
//                result_found_in_the_expected_results == true);
    }

    // Welche erwarteten Ergebnisse wurden nicht GENAU einmal verwendet ?
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(expected_results); ++ i)
    {
        if (count_expected_results [i] != 1)
        {
            FPRINTF_FFLUSH(stderr, "Expected results: %40s    used %" PRIuFAST8 " time(s) !\n", expected_results [i],
                    count_expected_results [i]);
        }
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
    const uint_fast8_t number_of_c_atoms = 10;
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

    // Die Verwendung der erwarteten Ergebnisse zaehlen, um moegliche Mehrfachverwendungen oder fehlende Verwendungen
    // erkennen zu koennen
    // Wenn alles richtig laeuft, dann muss jedes erwartete Ergebnis GENAU einmal verwendet werden
    uint_fast8_t count_expected_results [COUNT_ARRAY_ELEMENTS(expected_results)];
    memset (count_expected_results, '\0', sizeof (count_expected_results));

    // Fuer alle gerade erzeugten Alkane den IUPAC-Namen bilden
    for (uint_fast64_t i = 0; i < number_of_constitutional_isomers; ++ i)
    {
        Convert_Alkane_To_IUPAC_Name (decane_alkanes->data [i]);

        // Welches erwartete Ergebnis genau wurde verwendet ?
        uint_fast64_t index_in_the_expected_results = UINT_FAST64_MAX;

        // Befindet sich das gerade erzeugte Ergebnis in der Liste an gueltigen Ergebnissen ?
        const _Bool result_found_in_the_expected_results =
                Search_IUPAC_Name_In_The_List_Of_Expected_Results (decane_alkanes->data [i]->iupac_name, expected_results,
                        number_of_constitutional_isomers, &index_in_the_expected_results);

        // Wenn sich das Ergebnis nicht in der Liste befindet, dann wird das Programm mit einer Fehlermeldung beendet
        if (! result_found_in_the_expected_results /* == false */)
        {
            FPRINTF_FFLUSH(stderr, "Cannot find the current result \"%s\" in the list of expected results !\n",
                    decane_alkanes->data [i]->iupac_name);
        }
        else
        {
            count_expected_results [index_in_the_expected_results] ++;
        }
//        ASSERT("Cannot find the current result in the list of expected results !",
//                result_found_in_the_expected_results == true);
    }

    // Welche erwarteten Ergebnisse wurden nicht GENAU einmal verwendet ?
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(expected_results); ++ i)
    {
        if (count_expected_results [i] != 1)
        {
            FPRINTF_FFLUSH(stderr, "Expected results: %40s    used %" PRIuFAST8 " time(s) !\n", expected_results [i],
                    count_expected_results [i]);
        }
    }

    // Erzeugten Alkane_Container wieder loeschen
    Delete_Alkane_Container (decane_alkanes);

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
        "n-Undecan",

        "2-Methyldecan", "3-Methyldecan", "4-Methyldecan", "5-Methyldecan",

        "2,2-Dimethylnonan", "2,3-Dimethylnonan", "2,4-Dimethylnonan", "2,5-Dimethylnonan", "2,6-Dimethylnonan",
        "2,7-Dimethylnonan", "2,8-Dimethylnonan", "3,3-Dimethylnonan", "3,4-Dimethylnonan", "3,5-Dimethylnonan",
        "3,6-Dimethylnonan", "3,7-Dimethylnonan", "4,4-Dimethylnonan", "4,5-Dimethylnonan", "4,6-Dimethylnonan",
        "5,5-Dimethylnonan",

        "2,2,3-Trimethyloctan", "2,2,4-Trimethyloctan", "2,2,5-Trimethyloctan", "2,2,6-Trimethyloctan",
        "2,2,7-Trimethyloctan", "2,3,3-Trimethyloctan", "2,3,4-Trimethyloctan", "2,3,5-Trimethyloctan",
        "2,3,6-Trimethyloctan", "2,3,7-Trimethyloctan", "2,4,4-Trimethyloctan", "2,4,5-Trimethyloctan",
        "2,4,6-Trimethyloctan", "2,4,7-Trimethyloctan", "2,5,5-Trimethyloctan", "2,5,6-Trimethyloctan",
        "2,6,6-Trimethyloctan", "3,3,4-Trimethyloctan", "3,3,5-Trimethyloctan", "3,3,6-Trimethyloctan",
        "3,4,4-Trimethyloctan", "3,4,5-Trimethyloctan", "3,4,6-Trimethyloctan", "3,5,5-Trimethyloctan",
        "4,4,5-Trimethyloctan",

        "2,2,3,3-Tetramethylheptan", "2,2,3,4-Tetramethylheptan", "2,2,3,5-Tetramethylheptan",
        "2,2,3,6-Tetramethylheptan", "2,2,4,4-Tetramethylheptan", "2,2,4,5-Tetramethylheptan",
        "2,2,4,6-Tetramethylheptan", "2,2,5,5-Tetramethylheptan", "2,2,5,6-Tetramethylheptan",
        "2,2,6,6-Tetramethylheptan", "2,3,3,4-Tetramethylheptan", "2,3,3,5-Tetramethylheptan",
        "2,3,3,6-Tetramethylheptan", "2,3,4,4-Tetramethylheptan", "2,3,4,5-Tetramethylheptan",
        "2,3,4,6-Tetramethylheptan", "2,3,5,5-Tetramethylheptan", "2,3,5,6-Tetramethylheptan",
        "2,4,4,5-Tetramethylheptan", "2,4,4,6-Tetramethylheptan", "2,4,5,5-Tetramethylheptan",
        "3,3,4,4-Tetramethylheptan", "3,3,4,5-Tetramethylheptan", "3,3,5,5-Tetramethylheptan",
        "3,4,4,5-Tetramethylheptan",

        "2,2,3,3,4-Pentamethylhexan", "2,2,3,3,5-Pentamethylhexan", "2,2,3,4,4-Pentamethylhexan",
        "2,2,3,4,5-Pentamethylhexan", "2,2,3,5,5-Pentamethylhexan", "2,2,4,4,5-Pentamethylhexan",
        "2,3,3,4,4-Pentamethylhexan", "2,3,3,4,5-Pentamethylhexan",

        "2,2,3,3,4,4-Hexamethylpentan",

        "3-Ethylnonan", "4-Ethylnonan", "5-Ethylnonan",

        "3,3-Diethylheptan", "3,4-Diethylheptan", "3,5-Diethylheptan", "4,4-Diethylheptan",

        "3-Ethyl-2-methyloctan", "3-Ethyl-3-methyloctan", "3-Ethyl-4-methyloctan", "3-Ethyl-5-methyloctan",
        "3-Ethyl-6-methyloctan", "4-Ethyl-2-methyloctan", "4-Ethyl-3-methyloctan", "4-Ethyl-4-methyloctan",
        "4-Ethyl-5-methyloctan", "5-Ethyl-2-methyloctan", "5-Ethyl-3-methyloctan", "6-Ethyl-2-methyloctan",

        "3-Ethyl-2,2-dimethylheptan", "3-Ethyl-2,3-dimethylheptan", "3-Ethyl-2,4-dimethylheptan",
        "3-Ethyl-2,5-dimethylheptan", "3-Ethyl-2,6-dimethylheptan", "3-Ethyl-3,4-dimethylheptan",
        "3-Ethyl-3,5-dimethylheptan", "3-Ethyl-4,4-dimethylheptan", "3-Ethyl-4,5-dimethylheptan",
        "4-Ethyl-2,2-dimethylheptan", "4-Ethyl-2,3-dimethylheptan", "4-Ethyl-2,4-dimethylheptan",
        "4-Ethyl-2,5-dimethylheptan", "4-Ethyl-2,6-dimethylheptan", "4-Ethyl-3,3-dimethylheptan",
        "4-Ethyl-3,4-dimethylheptan", "4-Ethyl-3,5-dimethylheptan", "5-Ethyl-2,2-dimethylheptan",
        "5-Ethyl-2,3-dimethylheptan", "5-Ethyl-2,4-dimethylheptan", "5-Ethyl-2,5-dimethylheptan",
        "5-Ethyl-3,3-dimethylheptan",

        "3-Ethyl-2,2,3-trimethylhexan", "3-Ethyl-2,2,4-trimethylhexan", "3-Ethyl-2,2,5-trimethylhexan",
        "3-Ethyl-2,3,4-trimethylhexan", "3-Ethyl-2,3,5-trimethylhexan", "3-Ethyl-2,4,4-trimethylhexan",
        "3-Ethyl-2,4,5-trimethylhexan", "3-Ethyl-3,4,4-trimethylhexan", "4-Ethyl-2,2,3-trimethylhexan",
        "4-Ethyl-2,2,4-trimethylhexan", "4-Ethyl-2,2,5-trimethylhexan", "4-Ethyl-2,3,3-trimethylhexan",
        "4-Ethyl-2,3,4-trimethylhexan", "3,3-Diethyl-2-methylhexan", "3,3-Diethyl-4-methylhexan",
        "3,4-Diethyl-2-methylhexan", "3,4-Diethyl-3-methylhexan", "4,4-Diethyl-2-methylhexan",

        "3-Ethyl-2,2,3,4-tetramethylpentan", "3-Ethyl-2,2,4,4-tetramethylpentan", "3,3-Diethyl-2,2-dimethylpentan",
        "3,3-Diethyl-2,4-dimethylpentan",

        "4-Propyloctan",

        "4-(1-Methylethyl)octan",

        "2-Methyl-3-(1-methylethyl)heptan", "2-Methyl-4-(1-methylethyl)heptan", "3-Methyl-4-(1-methylethyl)heptan",
        "4-Methyl-4-(1-methylethyl)heptan",

        "2,2-Dimethyl-3-(1-methylethyl)hexan", "2,3-Dimethyl-3-(1-methylethyl)hexan",
        "2,4-Dimethyl-3-(1-methylethyl)hexan", "2,5-Dimethyl-3-(1-methylethyl)hexan",

        "2,2,4-Trimethyl-3-(1-methylethyl)pentan", "2,3,4-Trimethyl-3-(1-methylethyl)pentan",

        "2-Methyl-4-propylheptan", "3-Methyl-4-propylheptan", "4-Methyl-4-propylheptan",

        "4-Butylheptan"
    };

    // Alle Alkane erzeugen
    struct Alkane_Container* undecane_alkanes = Create_Alkane_Constitutional_Isomers (number_of_c_atoms);

    // Fuer alle gerade erzeugten Alkane den IUPAC-Namen bilden
    for (uint_fast64_t i = 0; i < number_of_constitutional_isomers; ++ i)
    {
        Convert_Alkane_To_IUPAC_Name (undecane_alkanes->data [i]);

        // Befindet sich das gerade erzeugte Ergebnis in der Liste an gueltigen Ergebnissen ?
        const _Bool result_found_in_the_expected_results =
                Search_IUPAC_Name_In_The_List_Of_Expected_Results (undecane_alkanes->data [i]->iupac_name, expected_results,
                        number_of_constitutional_isomers);

        // Wenn sich das Ergebnis nicht in der Liste befindet, dann wird das Programm mit einer Fehlermeldung beendet
        if (! result_found_in_the_expected_results /* == false */)
        {
            FPRINTF_FFLUSH(stderr, "Cannot find the current result \"%s\" in the list of expected results !\n",
                    undecane_alkanes->data [i]->iupac_name);
        }
        ASSERT("Cannot find the current result in the list of expected results !",
                result_found_in_the_expected_results == true);
    }

    // Erzeugten Alkane_Container wieder loeschen
    Delete_Alkane_Container (undecane_alkanes);

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
        "n-Dodecan",

        "2-Methylundecan", "3-Methylundecan", "4-Methylundecan", "5-Methylundecan", "6-Methylundecan",

        "2,2-Dimethyldecan", "2,3-Dimethyldecan", "2,4-Dimethyldecan", "2,5-Dimethyldecan", "2,6-Dimethyldecan",
        "2,7-Dimethyldecan", "2,8-Dimethyldecan", "2,9-Dimethyldecan", "3,3-Dimethyldecan", "3,4-Dimethyldecan",
        "3,5-Dimethyldecan", "3,6-Dimethyldecan", "3,7-Dimethyldecan", "3,8-Dimethyldecan", "4,4-Dimethyldecan",
        "4,5-Dimethyldecan", "4,6-Dimethyldecan", "4,7-Dimethyldecan", "5,5-Dimethyldecan", "5,6-Dimethyldecan",

        "2,2,3-Trimethylnonan", "2,2,4-Trimethylnonan", "2,2,5-Trimethylnonan", "2,2,6-Trimethylnonan",
        "2,2,7-Trimethylnonan", "2,2,8-Trimethylnonan", "2,3,3-Trimethylnonan", "2,3,4-Trimethylnonan",
        "2,3,5-Trimethylnonan", "2,3,6-Trimethylnonan", "2,3,7-Trimethylnonan", "2,3,8-Trimethylnonan",
        "2,4,4-Trimethylnonan", "2,4,5-Trimethylnonan", "2,4,6-Trimethylnonan", "2,4,7-Trimethylnonan",
        "2,4,8-Trimethylnonan", "2,5,5-Trimethylnonan", "2,5,6-Trimethylnonan", "2,5,7-Trimethylnonan",
        "2,5,8-Trimethylnonan", "2,6,6-Trimethylnonan", "2,6,7-Trimethylnonan", "2,7,7-Trimethylnonan",
        "3,3,4-Trimethylnonan", "3,3,5-Trimethylnonan", "3,3,6-Trimethylnonan", "3,3,7-Trimethylnonan",
        "3,4,4-Trimethylnonan", "3,4,5-Trimethylnonan", "3,4,6-Trimethylnonan", "3,4,7-Trimethylnonan",
        "3,5,5-Trimethylnonan", "3,5,6-Trimethylnonan", "3,5,7-Trimethylnonan", "3,6,6-Trimethylnonan",
        "4,4,5-Trimethylnonan", "4,4,6-Trimethylnonan", "4,5,5-Trimethylnonan", "4,5,6-Trimethylnonan",

        "2,2,3,3-Tetramethyloctan", "2,2,3,4-Tetramethyloctan", "2,2,3,5-Tetramethyloctan", "2,2,3,6-Tetramethyloctan",
        "2,2,3,7-Tetramethyloctan", "2,2,4,4-Tetramethyloctan", "2,2,4,5-Tetramethyloctan", "2,2,4,6-Tetramethyloctan",
        "2,2,4,7-Tetramethyloctan", "2,2,5,5-Tetramethyloctan", "2,2,5,6-Tetramethyloctan", "2,2,5,7-Tetramethyloctan",
        "2,2,6,6-Tetramethyloctan", "2,2,6,7-Tetramethyloctan", "2,2,7,7-Tetramethyloctan", "2,3,3,4-Tetramethyloctan",
        "2,3,3,5-Tetramethyloctan", "2,3,3,6-Tetramethyloctan", "2,3,3,7-Tetramethyloctan", "2,3,4,4-Tetramethyloctan",
        "2,3,4,5-Tetramethyloctan", "2,3,4,6-Tetramethyloctan", "2,3,4,7-Tetramethyloctan", "2,3,5,5-Tetramethyloctan",
        "2,3,5,6-Tetramethyloctan", "2,3,5,7-Tetramethyloctan", "2,3,6,6-Tetramethyloctan", "2,3,6,7-Tetramethyloctan",
        "2,4,4,5-Tetramethyloctan", "2,4,4,6-Tetramethyloctan", "2,4,4,7-Tetramethyloctan", "2,4,5,5-Tetramethyloctan",
        "2,4,5,6-Tetramethyloctan", "2,4,5,7-Tetramethyloctan", "2,4,6,6-Tetramethyloctan", "2,5,5,6-Tetramethyloctan",
        "2,5,6,6-Tetramethyloctan", "3,3,4,4-Tetramethyloctan", "3,3,4,5-Tetramethyloctan", "3,3,4,6-Tetramethyloctan",
        "3,3,5,5-Tetramethyloctan", "3,3,5,6-Tetramethyloctan", "3,3,6,6-Tetramethyloctan", "3,4,4,5-Tetramethyloctan",
        "3,4,4,6-Tetramethyloctan", "3,4,5,5-Tetramethyloctan", "3,4,5,6-Tetramethyloctan", "4,4,5,5-Tetramethyloctan",

        "2,2,3,3,4-Pentamethylheptan", "2,2,3,3,5-Pentamethylheptan", "2,2,3,3,6-Pentamethylheptan",
        "2,2,3,4,4-Pentamethylheptan", "2,2,3,4,5-Pentamethylheptan", "2,2,3,4,6-Pentamethylheptan",
        "2,2,3,5,5-Pentamethylheptan", "2,2,3,5,6-Pentamethylheptan", "2,2,3,6,6-Pentamethylheptan",
        "2,2,4,4,5-Pentamethylheptan", "2,2,4,4,6-Pentamethylheptan", "2,2,4,5,5-Pentamethylheptan",
        "2,2,4,5,6-Pentamethylheptan", "2,2,4,6,6-Pentamethylheptan", "2,2,5,5,6-Pentamethylheptan",
        "2,3,3,4,4-Pentamethylheptan", "2,3,3,4,5-Pentamethylheptan", "2,3,3,4,6-Pentamethylheptan",
        "2,3,3,5,5-Pentamethylheptan", "2,3,3,5,6-Pentamethylheptan", "2,3,4,4,5-Pentamethylheptan",
        "2,3,4,4,6-Pentamethylheptan", "2,3,4,5,5-Pentamethylheptan", "2,3,4,5,6-Pentamethylheptan",
        "2,4,4,5,5-Pentamethylheptan", "3,3,4,4,5-Pentamethylheptan", "3,3,4,5,5-Pentamethylheptan",

        "2,2,3,3,4,4-Hexamethylhexan", "2,2,3,3,4,5-Hexamethylhexan", "2,2,3,3,5,5-Hexamethylhexan",
        "2,2,3,4,4,5-Hexamethylhexan", "2,2,3,4,5,5-Hexamethylhexan", "2,3,3,4,4,5-Hexamethylhexan",

        "3-Ethyldecan", "4-Ethyldecan", "5-Ethyldecan",

        "3,3-Diethyloctan", "3,4-Diethyloctan", "3,5-Diethyloctan", "3,6-Diethyloctan", "4,4-Diethyloctan",
        "4,5-Diethyloctan",

        "3,3,4-Triethylhexan",

        "3-Ethyl-2-methylnonan", "3-Ethyl-3-methylnonan", "3-Ethyl-4-methylnonan", "3-Ethyl-5-methylnonan",
        "3-Ethyl-6-methylnonan", "3-Ethyl-7-methylnonan", "4-Ethyl-2-methylnonan", "4-Ethyl-3-methylnonan",
        "4-Ethyl-4-methylnonan", "4-Ethyl-5-methylnonan", "4-Ethyl-6-methylnonan", "5-Ethyl-2-methylnonan",
        "5-Ethyl-3-methylnonan", "5-Ethyl-4-methylnonan", "5-Ethyl-5-methylnonan", "6-Ethyl-2-methylnonan",
        "6-Ethyl-3-methylnonan", "7-Ethyl-2-methylnonan",

        "3-Ethyl-2,2-dimethyloctan", "3-Ethyl-2,3-dimethyloctan", "3-Ethyl-2,4-dimethyloctan",
        "3-Ethyl-2,5-dimethyloctan", "3-Ethyl-2,6-dimethyloctan", "3-Ethyl-2,7-dimethyloctan",
        "3-Ethyl-3,4-dimethyloctan", "3-Ethyl-3,5-dimethyloctan", "3-Ethyl-3,6-dimethyloctan",
        "3-Ethyl-4,4-dimethyloctan", "3-Ethyl-4,5-dimethyloctan", "3-Ethyl-4,6-dimethyloctan",
        "3-Ethyl-5,5-dimethyloctan", "4-Ethyl-2,2-dimethyloctan", "4-Ethyl-2,3-dimethyloctan",
        "4-Ethyl-2,4-dimethyloctan", "4-Ethyl-2,5-dimethyloctan", "4-Ethyl-2,6-dimethyloctan",
        "4-Ethyl-2,7-dimethyloctan", "4-Ethyl-3,3-dimethyloctan", "4-Ethyl-3,4-dimethyloctan",
        "4-Ethyl-3,5-dimethyloctan", "4-Ethyl-3,6-dimethyloctan", "4-Ethyl-4,5-dimethyloctan",
        "5-Ethyl-2,2-dimethyloctan", "5-Ethyl-2,3-dimethyloctan", "5-Ethyl-2,4-dimethyloctan",
        "5-Ethyl-2,5-dimethyloctan", "5-Ethyl-2,6-dimethyloctan", "5-Ethyl-3,3-dimethyloctan",
        "5-Ethyl-3,4-dimethyloctan", "5-Ethyl-3,5-dimethyloctan", "5-Ethyl-4,4-dimethyloctan",
        "6-Ethyl-2,2-dimethyloctan", "6-Ethyl-2,3-dimethyloctan", "6-Ethyl-2,4-dimethyloctan",
        "6-Ethyl-2,5-dimethyloctan", "6-Ethyl-2,6-dimethyloctan", "6-Ethyl-3,3-dimethyloctan",
        "6-Ethyl-3,4-dimethyloctan",

        "3-Ethyl-2,2,3-trimethylheptan", "3-Ethyl-2,2,4-trimethylheptan", "3-Ethyl-2,2,5-trimethylheptan",
        "3-Ethyl-2,2,6-trimethylheptan", "3-Ethyl-2,3,4-trimethylheptan", "3-Ethyl-2,3,5-trimethylheptan",
        "3-Ethyl-2,3,6-trimethylheptan", "3-Ethyl-2,4,4-trimethylheptan", "3-Ethyl-2,4,5-trimethylheptan",
        "3-Ethyl-2,4,6-trimethylheptan", "3-Ethyl-2,5,5-trimethylheptan", "3-Ethyl-2,5,6-trimethylheptan",
        "3-Ethyl-3,4,4-trimethylheptan", "3-Ethyl-3,4,5-trimethylheptan", "3-Ethyl-3,5,5-trimethylheptan",
        "3-Ethyl-4,4,5-trimethylheptan", "4-Ethyl-2,2,3-trimethylheptan", "4-Ethyl-2,2,4-trimethylheptan",
        "4-Ethyl-2,2,5-trimethylheptan", "4-Ethyl-2,2,6-trimethylheptan", "4-Ethyl-2,3,3-trimethylheptan",
        "4-Ethyl-2,3,4-trimethylheptan", "4-Ethyl-2,3,5-trimethylheptan", "4-Ethyl-2,3,6-trimethylheptan",
        "4-Ethyl-2,4,5-trimethylheptan", "4-Ethyl-2,4,6-trimethylheptan", "4-Ethyl-2,5,5-trimethylheptan",
        "4-Ethyl-3,3,4-trimethylheptan", "4-Ethyl-3,3,5-trimethylheptan", "4-Ethyl-3,4,5-trimethylheptan",
        "5-Ethyl-2,2,3-trimethylheptan", "5-Ethyl-2,2,4-trimethylheptan", "5-Ethyl-2,2,5-trimethylheptan",
        "5-Ethyl-2,2,6-trimethylheptan", "5-Ethyl-2,3,3-trimethylheptan", "5-Ethyl-2,3,4-trimethylheptan",
        "5-Ethyl-2,3,5-trimethylheptan", "5-Ethyl-2,4,4-trimethylheptan", "5-Ethyl-2,4,5-trimethylheptan",
        "5-Ethyl-3,3,4-trimethylheptan",

        "3-Ethyl-2,2,3,4-tetramethylhexan", "3-Ethyl-2,2,3,5-tetramethylhexan", "3-Ethyl-2,2,4,4-tetramethylhexan",
        "3-Ethyl-2,2,4,5-tetramethylhexan", "3-Ethyl-2,2,5,5-tetramethylhexan", "3-Ethyl-2,3,4,4-tetramethylhexan",
        "3-Ethyl-2,3,4,5-tetramethylhexan", "4-Ethyl-2,2,3,3-tetramethylhexan", "4-Ethyl-2,2,3,4-tetramethylhexan",
        "4-Ethyl-2,2,3,5-tetramethylhexan", "4-Ethyl-2,2,4,5-tetramethylhexan", "4-Ethyl-2,3,3,4-tetramethylhexan",
        "4-Ethyl-2,3,3,5-tetramethylhexan", "3,3-Diethyl-2,2-dimethylhexan", "3,3-Diethyl-2,4-dimethylhexan",
        "3,3-Diethyl-2,5-dimethylhexan", "3,3-Diethyl-4,4-dimethylhexan", "3,4-Diethyl-2,2-dimethylhexan",
        "3,4-Diethyl-2,3-dimethylhexan", "3,4-Diethyl-2,4-dimethylhexan", "3,4-Diethyl-2,5-dimethylhexan",
        "3,4-Diethyl-3,4-dimethylhexan", "4,4-Diethyl-2,2-dimethylhexan", "4,4-Diethyl-2,3-dimethylhexan",

        "3-Ethyl-2,2,3,4,4-pentamethylpentan", "3,3-Diethyl-2,2,4-trimethylpentan",

        "4-Propylnonan", "5-Propylnonan",

        "4-(1-Methylethyl)nonan", "5-(1-Methylethyl)nonan",

        "2-Methyl-4-propyloctan", "3-Methyl-4-propyloctan", "4-Methyl-4-propyloctan", "4-Methyl-5-propyloctan",
        "2-Methyl-5-propyloctan", "3-Methyl-5-propyloctan",

        "2-Methyl-3-(1-methylethyl)octan", "2-Methyl-4-(1-methylethyl)octan", "3-Methyl-4-(1-methylethyl)octan",
        "4-Methyl-4-(1-methylethyl)octan", "4-Methyl-5-(1-methylethyl)octan", "2-Methyl-5-(1-methylethyl)octan",
        "3-Methyl-5-(1-methylethyl)octan",

        "2,2,3-Trimethyl-3-(1-methylethyl)hexan", "2,2,4-Trimethyl-3-(1-methylethyl)hexan",
        "2,2,5-Trimethyl-3-(1-methylethyl)hexan", "2,3,4-Trimethyl-3-(1-methylethyl)hexan",
        "2,3,5-Trimethyl-3-(1-methylethyl)hexan", "2,4,4-Trimethyl-3-(1-methylethyl)hexan",
        "2,3,5-Trimethyl-4-(1-methylethyl)hexan", "2,2,5-Trimethyl-4-(1-methylethyl)hexan",

        "2,2,3,4-Tetramethyl-3-(1-methylethyl)pentan", "2,2,4,4-Tetramethyl-3-(1-methylethyl)pentan",

        "3-Ethyl-4-propylheptan", "4-Ethyl-4-propylheptan",

        "3-Ethyl-4-(1-methylethyl)heptan", "4-Ethyl-4-(1-methylethyl)heptan",

        "3-Ethyl-2-methyl-3-(1-methylethyl)hexan", "4-Ethyl-2-methyl-3-(1-methylethyl)hexan",

        "3-Ethyl-2,4-dimethyl-3-(1-methylethyl)pentan",

        "4-(1,1-Dimethylethyl)octan",

        "4-(1,1-Dimethylethyl)-2-methylheptan", "4-(1,1-Dimethylethyl)-3-methylheptan",
        "4-(1,1-Dimethylethyl)-4-methylheptan",

        "3-(1,1-Dimethylethyl)-2,2-dimethylhexan"
    };

    // Alle Alkane erzeugen
    struct Alkane_Container* dodecane_alkanes = Create_Alkane_Constitutional_Isomers (number_of_c_atoms);

    // Fuer alle gerade erzeugten Alkane den IUPAC-Namen bilden
    for (uint_fast64_t i = 0; i < number_of_constitutional_isomers; ++ i)
    {
        Convert_Alkane_To_IUPAC_Name (dodecane_alkanes->data [i]);

        // Befindet sich das gerade erzeugte Ergebnis in der Liste an gueltigen Ergebnissen ?
        const _Bool result_found_in_the_expected_results =
                Search_IUPAC_Name_In_The_List_Of_Expected_Results (dodecane_alkanes->data [i]->iupac_name, expected_results,
                        number_of_constitutional_isomers);

        // Wenn sich das Ergebnis nicht in der Liste befindet, dann wird das Programm mit einer Fehlermeldung beendet
        if (! result_found_in_the_expected_results /* == false */)
        {
            FPRINTF_FFLUSH(stderr, "Cannot find the current result \"%s\" in the list of expected results !\n",
                    dodecane_alkanes->data [i]->iupac_name);
        }
        ASSERT("Cannot find the current result in the list of expected results !",
                result_found_in_the_expected_results == true);
    }

    // Erzeugten Alkane_Container wieder loeschen
    Delete_Alkane_Container (dodecane_alkanes);

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
