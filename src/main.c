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
#include "argparse.h"
#include "CLI_Parameter.h"



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
int main (const int argc, const char* argv [])
{
    // ===== ===== ===== BEGINN CLI-Parameter parsen ===== ===== =====
    struct argparse_option cli_options [] =
    {
            OPT_HELP(),

            OPT_GROUP("Hauptfunktionen"),
            OPT_INTEGER('c', "num_c", &GLOBAL_MAX_C_ATOMS_FOR_TESTS,
                    "Anzahl an C-Atome, die fuer die Tests verwendet werden sollen", NULL, 0, 0),

            OPT_END()
    };

    struct argparse argparse_object;
    argparse_init(&argparse_object, cli_options, GLOBAL_USAGES, 0);
    argparse_describe(&argparse_object, GLOBAL_PROGRAM_DESCRIPTION, GLOBAL_ADDITIONAL_PROGRAM_DESCRIPTION);
    const int new_argc = argparse_parse(&argparse_object, argc, argv);

    if (GLOBAL_MAX_C_ATOMS_FOR_TESTS != 0)
    {
        printf ("Anzahl an C-Atome fuer die Tests: %d\n", GLOBAL_MAX_C_ATOMS_FOR_TESTS);
        Check_CLI_Parameter_MAX_C_ATOMS_FOR_TESTS();
    }
    if (new_argc != 0)
    {
        printf ("argc: %d\n", new_argc);
        for (int i = 0; i < new_argc; ++ i)
        {
            printf("argv [%d]: %s\n", i, *(argv + i));
        }
    }
    // ===== ===== ===== ENDE CLI-Parameter parsen ===== ===== =====


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
#elif defined(DEBUG_BUILD)
    printf ("Debug");
#else
    printf (N_A);
#endif /* RELEASE_BUILD */

    // Dieser Check der Bitbreite ist nicht 100% exakt, da ein Byte nicht zwingend 8 Bit gross sein muss ...
    // ... zumindest in der Theorie. In der Praxis wird man ueber dieses Problem nicht stolpern.
    // Es gibt keinen sicheren portablen Weg, um die Bitbreite zu bestimmen ! Besseres als das hier gibt es
    // leider nicht ...
    printf(" (%zu Bit) | OS: ", (CHAR_BIT * sizeof(void*)));

#ifdef LINUX
    printf ("Linux");
#elif defined(WINDOWS)
    printf ("Windows");
#else
    printf (N_A);
#endif /* LINUX */

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
