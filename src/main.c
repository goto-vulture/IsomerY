/**
 * main.c
 *
 *  Created on: 07.03.2021
 *      Author: x86 / Gyps
 *
 *         .-..-.__
 *       /`  |\_( "\__
 *       |   / __/`'--)
 *       |  /  | |
 *       /_/ _/_/
 * jgs  `|/))))
 *
 * For me, a vulture is more desirable than money.
 */

/**
 * C-Version ueberpruefen, um Schluesselwoerter u.U. durch leere Makros zu ersetzen
 *
 * Das Makro __STDC_VERSION__ ist fuer C90 nicht definiert
 * ->   Schluesselwoerter inline und restrict durch leere Markos ersetzen, damit eine Uebersetzung auch mit C90
 *      moeglich ist.
 */
#ifndef __STDC_VERSION__
    #ifndef inline
        #define inline
    #else
        #error "The macro \"inline\" is already defined !"
    #endif /* inline */

    #ifndef restrict
        #define restrict
    #else
        #error "The macro \"restrict\" is already defined !"
    #endif /* restrict */
#endif /* __STDC_VERSION__ */

#include <stdlib.h>
#include <assert.h>
#include "OS_Specific_Configurations.h"
#include "Error_Handling/Dynamic_Memory.h"
#include "Alkane/Alkane.h"
#include "Alkane/Alkane_Create_Constitutional_Isomer.h"
#include "Tests/Alkane_Tests.h"



//=====================================================================================================================

int main (const int argc, const char* const argv [])
{
    (void) argc;
    (void) argv;

    // Alkane + Alkane_Branch Test Code
//    const unsigned char branch_1_content [] = { 1 };
//    const unsigned char branch_2_content [] = { 1, 1, 1 };
//    const unsigned char branch_3_content [] = { 1, 1, 1, 1 };
//
//    struct Alkane_Branch* branch_1 = Create_Alkane_Branch (branch_1_content, sizeof (branch_1_content));
//    struct Alkane_Branch* branch_2 = Create_Alkane_Branch (branch_2_content, sizeof (branch_2_content));
//    struct Alkane_Branch* branch_3 = Create_Alkane_Branch (branch_3_content, sizeof (branch_3_content));
//
//    struct Alkane* alkane = Create_Alkane (branch_1, branch_2, branch_3, NULL);
//    Print_Alkane (alkane);
//
//    Delete_Alkane (alkane);
//    Delete_Alkane_Branch (branch_1);
//    Delete_Alkane_Branch (branch_2);
//    Delete_Alkane_Branch (branch_3);

    // Testcode fuer die Funktion, die die Konstitutionsisomere erzeugt
//    uint_fast64_t number_of_created_isomers = 0;
//    Create_Alkane_Constitutional_Isomers (12, &number_of_created_isomers);


    RUN(TEST_Create_Alkane_Constitutional_Isomers);
    RUN(TEST_Create_Alkane);
    RUN(TEST_Convert_Alkane_To_IUPAC_Name);
    RUN(TEST_Convert_Alkane_With_Nested_2_To_IUPAC_Name);

    // Ergebnisse aller durchgefuehrten Tests anzeigen
    TEST_REPORT();

    puts ("");
    Show_Dynamic_Memory_Status ();

    return EXIT_SUCCESS;
}

//=====================================================================================================================
