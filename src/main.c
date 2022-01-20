/**
 * @file main.c
 * @mainpage IsomerY
 *
 * @brief Dieses Programm besimmt die Konstitutionsisomere (mit deren IUPAC-Namen) von Alkanen bei gegebener Anzahl an
 * C-Atomen.
 *
 * @author x86 / Gyps
 */

/*         .-..-.__
 *       /`  |\_( "\__
 *       |   / __/`'--)
 *       |  /  | |
 *       /_/ _/_/
 * jgs  `|/))))
 *
 * For me, a vulture is more desirable than money.
 */

/*
 * C-Version ueberpruefen, um Schluesselwoerter u.U. durch leere Makros bzw. Alternativwerte zu ersetzen
 *
 * Das Makro __STDC_VERSION__ ist fuer C90 nicht definiert
 * ->   Schluesselwoerter inline und restrict durch leere Markos ersetzen, damit eine Uebersetzung auch mit C90
 *      moeglich ist.
 */
#ifndef __STDC_VERSION__
    /**
     * @brief Das Schluesselwort "inline" als Makro definieren, wenn der C90 Standard verwendet wird.
     *
     * In diesem Standard existiert "inline" als Schluesselwort noch nicht. Die Uebersetzung wuerde an diesen
     * Stellen mit einem Fehler abbrechen.
     * Die Loesung: "inline" wird ein leeres Makro
     */
    #ifndef inline
        #define inline
    #else
        #error "The macro \"inline\" is already defined !"
    #endif /* inline */

    /**
     * @brief Das Schluesselwort "restrict" als Makro definieren, wenn der C90 Standard verwendet wird.
     *
     * In diesem Standard existiert "restrict" als Schluesselwort noch nicht. Die Uebersetzung wuerde an diesen
     * Stellen mit einem Fehler abbrechen.
     * Die Loesung: "restrict" wird ein leeres Makro
     */
    #ifndef restrict
        #define restrict
    #else
        #error "The macro \"restrict\" is already defined !"
    #endif /* restrict */

    /**
     * @brief Die Problematik mit Wahrheitswerten vor C99 loesen, indem "_Bool" in diesen Faellen mittels Makro mit "int"
     * ersetzt wird.
     */
    #ifndef _Bool
        #define _Bool int
    #else
        #error "The macro \"_Bool\" is already defined !"
    #endif /* _Bool */

    /**
     * @brief In seltenen Faellen ist "true" als Makro nicht vordefiniert. Z.B. vor C99
     */
    #ifndef true
        #define true 1
    #else
        #error "The macro \"true\" is already defined !"
    #endif /* true */

    /**
     * @brief In seltenen Faellen ist "false" als Makro nicht vordefiniert. Z.B. vor C99
     */
    #ifndef false
        #define false 0
    #else
        #error "The macro \"false\" is already defined !"
    #endif /* false */
#endif /* __STDC_VERSION__ */

/**
 * @brief Ein einfache Zeichenkette die anzeigt, dass eine Information nicht vorhanden ist.
 */
#ifndef N_A
    #define N_A "N/A"
#else
    #error "The macro \"N_A\" is already defined !"
#endif /* N_A */

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
#include "str2int.h"
#include "Print_Tools.h"
#include "Beautiful.h"



/**
 * Einige wesentliche Informationen des Programms ausgeben.
 */
static inline void Show_Program_Details (void);

//=====================================================================================================================

/**
 * @brief main-Funktion des Programms.
 *
 * Aktuell werden die CLI-Parameter (noch) nicht verwendet.
 *
 * @param[in] argc argc-Parameter
 * @param[in] argv Anzahl an argc-Parametern
 *
 * @return 0 bei erfolgreicher Ausfuehrung; Ansonsten != 0
 */
int main (const int argc, const char* const argv [])
{
    (void) argc;
    (void) argv;

    // Um unused-Warnungen beim Programm Cppcheck zu vermeiden
    (void) str2int;

    Show_Program_Details ();

    // Einige Debug-Informationen ausgeben
    printf ("Sizeof Alkane:                  %6zu byte\n", sizeof (struct Alkane));
    printf ("Sizeof Alkane_Container:        %6zu byte\n", sizeof (struct Alkane_Container));
    printf ("Sizeof Alkane_Branch:           %6zu byte\n", sizeof (struct Alkane_Branch));
    printf ("Sizeof Alkane_Branch_Container: %6zu byte\n", sizeof (struct Alkane_Branch_Container));
    PUTS_FFLUSH ("\n\n");

    // Alle Alkan-Tests durchfuehren
    Execute_All_Alkane_Tests ();

    // Infos ueber die getaetigten Speicheraufrufe anzeigen
    puts ("");
    Show_Dynamic_Memory_Status ();

//    Beautiful_1();
//    Beautiful_2();
//    Beautiful_3();
//    Beautiful_4();

    // Alle I/O Puffer vor Beendigung des Programms leeren
    fflush (NULL);

    return EXIT_SUCCESS;
}

//=====================================================================================================================

/**
 * @brief Einige wesentliche Informationen des Programms ausgeben.
 */
static inline void Show_Program_Details (void)
{
    printf ("IsomerY (Build type: ");
#ifdef RELEASE_BUILD
    printf ("Release");
#elif DEBUG_BUILD
    printf ("Debug");
#else
    printf (N_A);
#endif /* RELEASE_BUILD */

    printf (") Version: ");
#ifdef VERSION
    printf ("%s\n", VERSION);
#else
    printf (N_A "\n");
#endif /* VERSION */

    // Das wichtigste Bild !
    puts ("          .-..-.__");
    puts ("        /`  |\\_( \"\\__");
    puts ("        |   / __/`'--)");
    puts ("        |  /  | |");
    puts ("        /_/ _/_/");
    puts (" jgs  `|/))))\n");
    // Die wichtigste Informaion !
    PUTS_FFLUSH ("For me, a vulture is more desirable than money !");

    return;
}

//---------------------------------------------------------------------------------------------------------------------
