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

#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include "Error_Handling/Dynamic_Memory.h"
#include "Alkane/Alkane.h"
#include "Alkane/Alkane_Create_Constitutional_Isomer.h"
#include "Tests/Alkane_Tests.h"



// Die static_asserts funktionieren aktuell nur unter Linux
#if defined(__linux__) || defined(unix) || defined(__unix__) || defined(__unix)
// Ist auf der Zielplattform ein Byte genau 8 Bit gross ?
// Wenn nicht dann wird die Kompilierung nicht ausgefuehrt, da alle verwendeten Mechanismen davon ausgehen das dies der
// Fall ist !
static_assert (CHAR_BIT == 8,               "A char needs to be exact eight bit !");

// Ein int / unsigned int / size_t muss mindestens 4 Byte gross sein ! Bei Systemen wo dies nicht der Fall ist, ist
// i.d.R. ohne groesseren Aufwand kaum zu arbeiten.
static_assert (sizeof (int) >= 4,           "The type \"int\" needs to be at least 4 byte !");
static_assert (sizeof (unsigned int) >= 4,  "The type \"unsigned int\" needs to be at least 4 byte !");
static_assert (sizeof (size_t) >= 4,        "The type \"size_t\" needs to be at least 4 byte !");

// Weitere Variable(n), die mindestens 4 Byte gross sein sollte(n)
static_assert (sizeof (time_t) >= 4,        "The type \"time_t\" needs to be at least 4 byte !");
static_assert (sizeof (clock_t) >= 4,       "The type \"clock_t\" needs to be at least 4 byte !");

// Der Standard schreibt vor, dass SIZE_MAX mindestens 65535 betragen muss. Fuer einige Operationen (insb. calloc (),
// malloc () und Co.) ist dies hier aber zu wenig ! Daher muss sichergestellt sein, dass SIZE_MAX eine akzeptable
// Groesse besitzt.
// I.d.R. ist SIZE_MAX (2^32) - 1 oder (2^64) - 1
static_assert (SIZE_MAX >= UINT32_MAX,      "SIZE_MAX needs to be at least 2^32 - 1 !");
#endif /* defined(__linux__) || defined(unix) || defined(__unix__) || defined(__unix) */



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

    // Ergebnisse aller durchgefuehrten Tests anzeigen
    TEST_REPORT();

    Show_Dynamic_Memory_Status ();

    return EXIT_SUCCESS;
}

//=====================================================================================================================
