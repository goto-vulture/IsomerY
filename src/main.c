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
#include "Alkane/Alkane_Container.h"
#include "Alkane/Alkane_Branch.h"
#include "Alkane/Alkane_Branch_Container.h"
#include "Alkane/Alkane_Create_Constitutional_Isomer.h"
#include "Tests/Alkane_Tests.h"



//=====================================================================================================================

int main (const int argc, const char* const argv [])
{
    (void) argc;
    (void) argv;

    // Einige Debug-Informationen ausgeben
    printf ("Sizeof Alkane:                  %6zu byte\n", sizeof (struct Alkane));
    printf ("Sizeof Alkane_Container:        %6zu byte\n", sizeof (struct Alkane_Container));
    printf ("Sizeof Alkane_Branch:           %6zu byte\n", sizeof (struct Alkane_Branch));
    printf ("Sizeof Alkane_Branch_Container: %6zu byte\n", sizeof (struct Alkane_Branch_Container));
    puts ("\n\n");
    fflush (stdout);

    // Alle Alkan-Tests durchfuehren
    Execute_All_Alkane_Tests ();

    // Infos ueber die getaetigten Speicheraufrufe anzeigen
    puts ("");
    Show_Dynamic_Memory_Status ();

    return EXIT_SUCCESS;
}

//=====================================================================================================================
