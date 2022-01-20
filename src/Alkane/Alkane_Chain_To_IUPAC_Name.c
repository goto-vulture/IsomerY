/**
 * @file Alkane_Chain_To_IUPAC_Name.c
 *
 * @brief Konstitutionsisomere in Form von Alkan-Objekten sind schoen und gut. Aber fuer die Verwendung im Bereich der
 * organischen Chemie sind sie unbrauchbar !
 *
 * @date 23.03.2021
 * @author x86 / Gyps
 */

#include "Alkane_Chain_To_IUPAC_Name.h"
#include <string.h>
#include <stdbool.h>
#include "Alkane_Info_Constitutional_Isomer.h"
#include "../int2str.h"
#include "../Print_Tools.h"
#include "../Misc.h"
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"

/**
 * @brief Konstante, um die Groesse des Arrays festzulegen. Der Wert ist ein Erfahrungswert. Selbst bei sehr komplexen
 * Molekuelen werden praktisch immer weniger als 10 Elemente verwendet.
 *
 * Siehe Testfunktion: TEST_Convert_Alkane_To_IUPAC_Name_With_Manual_Chain_Objects
 */
#ifndef SIZE_OF_DYNAMIC_CREATED_OBJECTS_ARRAY
#define SIZE_OF_DYNAMIC_CREATED_OBJECTS_ARRAY 15
#else
#error "The marco \"SIZE_OF_DYNAMIC_CREATED_OBJECTS_ARRAY\" is already defined !"
#endif /* SIZE_OF_DYNAMIC_CREATED_OBJECTS_ARRAY */




/**
 * @brief Objekt, welches fuer die Zusammenfuehrung von Chain Objekten verwendet wird.
 *
 * Hierbei handelt es sich um eine einfache verkettete Liste. Die Combined_Chain Objekte werden aufsteigend anhand der
 * Verschachtelung mittels Zeiger verknuepft.
 */
struct Combined_Chain
{
    uint_fast8_t positions [NUMBER_OF_NUMBER_WORDS]; ///< Positionsinformationen
    uint_fast8_t next_free_position; ///< Naechstes freies Objekt im Array positions

    uint_fast8_t branch_length; ///< Astlaenge
    uint_fast8_t nesting_depth; ///< Verschachtelungstiefe

    struct Combined_Chain* deeper [NUMBER_OF_NUMBER_WORDS]; ///< Objekte, die in einer tieferen Verschachtelung liegen
    uint_fast8_t next_free_depper; ///< Naechstes freies Objekt im Array deeper
};



/**
 * Eine Ebene tiefer im Combined_Chain-Konstrukt gehen.
 */
static void
Go_Deeper
(
        const struct Combined_Chain* const restrict combined,   // Aktuelles Combined_Chain-Objekt
        const struct Combined_Chain* const restrict back,       // Combined_Chain-Objekt, von denen der Aufruf gekommen
                                                                // ist
        char* const restrict str,                               // Speicher fuer den IUPAC-Namen
        size_t size_left                                        // Anzahl an Zeichen, die noch zur Verfuegung stehen
);

/**
 * Ein neues Combined_Chain-Objekt erzeugen und mit den Werten eines Chain-Objektes initialisieren.
 *
 * Das Objekt wird dynamisch erzeugt.
 *
 * Um das Loeschen der dynamischen Objekte zu vereinfachen, werden alle Adressen der dynamischen Objekte gesichert.
 */
static struct Combined_Chain*                                                   // Neues dynamisch erzeugte
                                                                                // Combined_Chain-Objekt
Create_New_Combined_Chain
(
        const struct Chain* const restrict chain,                               // Chain-Objekt, welches fuer die
                                                                                // Initialisierung verwendet wird
        struct Combined_Chain** const restrict dynamic_created_combined_chain,  // Array um festzuhalten, welche
                                                                                // Objekte dynamisch erzeugt wurden
        size_t* const restrict next_free_array_element                          // Naechstes freies Element im Array
);

/**
 * Vergleichfunktion fuer qsort()
 *
 * Hierbei geht es darum, dass der Name der Aeste aufsteigend alphabetisch sortiert werden. Das Ziel ist, dass z.B.
 * eine Ethyl-Abzweigung vor einer Methyl-Abzweigung im IUPAC-Namen erscheint.
 */
static int compare_1 (const void* element_1, const void* element_2);



//=====================================================================================================================

/*
 * Diese Funktion konvertiert die zuvor erzeugten Chain-Objekte in den IUPAC-Namen.
 */
void
Chain_To_IUPAC
(
        char* const restrict iupac_name,        // Speicher fuer den IUPAC-Namen
        const size_t iupac_name_length,         // Groesse des Speichers, welches fuer den IUPAC-Namen reserviert wurde
        struct Alkane* const restrict alkane    // Alkan-Objekt (enthaelt die Chain-Objekte)
)
{
    memset (iupac_name, '\0', iupac_name_length * sizeof (char));

    struct Combined_Chain main_combined_chain;
    memset (&main_combined_chain, '\0', sizeof (struct Combined_Chain));
    SET_POINTER_ARRAY_TO_NULL(main_combined_chain.deeper, COUNT_ARRAY_ELEMENTS(main_combined_chain.deeper));

    // Das 1. Objekt (die Hauptkette) von Hand reinschreiben
    main_combined_chain.nesting_depth = alkane->chains [0].nesting_depth;
    main_combined_chain.branch_length = alkane->chains [0].length;

    // Alle Combined_Chain-Objekte - bzw. deren Adressen - werden in diesem Array gesichert, um spaeter die
    // Aufraeumarbeiten einfach durchfueren zu koennen
    struct Combined_Chain* dynamic_created_objects [SIZE_OF_DYNAMIC_CREATED_OBJECTS_ARRAY];
    SET_POINTER_ARRAY_TO_NULL(dynamic_created_objects, COUNT_ARRAY_ELEMENTS(dynamic_created_objects));
    size_t next_free_array_element = 0;

    // Dieses Array enthaelt die zuletzt verwendeten Combined_Chain-Objekte.
    // Die Verschachtelunstiefe ist der Index fuer die weitere Verarbeitung.
    struct Combined_Chain* last_cc [MAX_NUMBER_OF_NESTING_DEPTH];
    SET_POINTER_ARRAY_TO_NULL(last_cc, COUNT_ARRAY_ELEMENTS(last_cc));
    last_cc [0] = &main_combined_chain;

    size_t iupac_name_space_left = iupac_name_length;

    // Das 1. Objekt ist immer die Hauptkette !
    // ===== ===== ===== BEGINN Aeste in passende Struktur fuer die Bennenung einfuegen ===== ===== =====
    /**
     * Die Grundidee bei der Erzeugung der Combined_Chain-Objekte ist, dass immer die Verschachtelungstiefen des
     * aktuellen Objektes mit dem zukuenftigen Objekt verglichen werden.
     *
     * Allerdings gibt es dabei ein paar Dinge zu beachten:
     *
     * - Wenn nicht der Fall ist, dass die Verschachtelunstiefe im zukuenftigen Objekt groesser als im aktuellen
     *   Objekt ist, dann koennte es sein, dass das aktuelle Objekt mit bereits vorhandenen Eintraegen im
     *   Combined_Chain-Objekt zusammengefasst werden kann ! Man moechte ja z.B. "DiMethyl" und nicht zweimal "Methyl"
     * - Wenn eine Zusammenfassung nicht moeglich ist, dann wird ebenfalls ein neues Combined_Chain-Objekt erzeugt.
     */
    for (uint_fast8_t i = 1; i < alkane->next_free_chain; ++ i) // i = 1 !
    {
        // Aktueles Combined_Chain-Objekt
        struct Combined_Chain* current_combined_chain = last_cc [alkane->chains [i].nesting_depth - 1];

        if ((i + 1) < alkane->next_free_chain)
        {
            // Gibt es eine Steigerung der Verschachtelungstiefe ?
            if (alkane->chains [i].nesting_depth < alkane->chains [i + 1].nesting_depth)
            {
                current_combined_chain->deeper [current_combined_chain->next_free_depper] =
                        Create_New_Combined_Chain (&(alkane->chains [i]), dynamic_created_objects, &next_free_array_element);

                last_cc [alkane->chains [i].nesting_depth] = current_combined_chain->deeper [current_combined_chain->next_free_depper];

                current_combined_chain->next_free_depper ++;
            }
            // Wenn nicht ...
            else
            {
                _Bool element_conbined = false;

                // ... kann das aktuelle Combined_Chain-Objekt mit bereits vorhandenen Eintraegen zusammengefasst
                // werden ?
                for (uint_fast8_t i2 = 0; i2 < current_combined_chain->next_free_depper; ++ i2)
                {
                    // Laenge identisch und Verschachtelungstiefe identisch ? -> Zusammenfassung moeglich !
                    if (current_combined_chain->deeper [i2]->branch_length == alkane->chains [i].length &&
                            current_combined_chain->deeper [i2]->nesting_depth == alkane->chains [i].nesting_depth)
                    {
                        element_conbined = true;
                        current_combined_chain->deeper [i2]->positions [current_combined_chain->deeper [i2]->next_free_position] =
                                alkane->chains [i].position;
                        current_combined_chain->deeper [i2]->next_free_position ++;
                        last_cc [alkane->chains [i].nesting_depth] = current_combined_chain->deeper [i2];
                        break;
                    }
                }
                // Wenn nicht, dann wird ebenfalls ein neues Combined_Chain-Objekt erstellt
                if (element_conbined == false)
                {
                    current_combined_chain->deeper [current_combined_chain->next_free_depper] =
                            Create_New_Combined_Chain (&(alkane->chains [i]), dynamic_created_objects, &next_free_array_element);

                    last_cc [alkane->chains [i].nesting_depth] = current_combined_chain->deeper [current_combined_chain->next_free_depper];

                    current_combined_chain->next_free_depper ++;
                }
            }
        }
        // Beim letzten Combined_Chain-Objekt ist die Abarbeitung fast gleich wie weiter oben. Der Hauptunterschied
        // ist, dass eine Steigerung in der Verschachtelungstiefe nicht mehr moeglich ist und daher auch nicht mehr
        // abgefragt wird
        else
        {
            _Bool element_conbined = false;

            // Kann das aktuelle Combined_Chain-Objekt mit bereits vorhandenen Eintraegen zusammengefasst
            // werden ?
            for (uint_fast8_t i2 = 0; i2 < current_combined_chain->next_free_depper; ++ i2)
            {
                if (current_combined_chain->deeper [i2]->branch_length == alkane->chains [i].length &&
                        current_combined_chain->deeper [i2]->nesting_depth == alkane->chains [i].nesting_depth)
                {
                    element_conbined = true;
                    current_combined_chain->deeper [i2]->positions [current_combined_chain->deeper [i2]->next_free_position] =
                            alkane->chains [i].position;
                    current_combined_chain->deeper [i2]->next_free_position ++;
                    last_cc [alkane->chains [i].nesting_depth] = current_combined_chain->deeper [i2];
                    break;
                }
            }
            // Wenn nicht, dann wird ebenfalls ein neues Combined_Chain-Objekt erstellt
            if (element_conbined == false)
            {
                current_combined_chain->deeper [current_combined_chain->next_free_depper] =
                        Create_New_Combined_Chain (&(alkane->chains [i]), dynamic_created_objects, &next_free_array_element);

                last_cc [alkane->chains [i].nesting_depth] = current_combined_chain->deeper [current_combined_chain->next_free_depper];

                current_combined_chain->next_free_depper ++;
            }
        }
    }
    // ===== ===== ===== ENDE Aeste in passende Struktur fuer die Bennenung einfuegen ===== ===== =====

    // ===== ===== ===== BEGINN Namen erzeugen ===== ===== =====
    // Die Abzweigungen 1. Verschachtelungstiefe alphabetisch sortieren
    // An sich ist dieser Prozess fuer eine eindeutige Benennung nicht erforderlich. Aber in der IUPAC-Nomenklatur ist
    // eine alphabetische Sortierung vorgeschrieben !
    qsort (main_combined_chain.deeper, main_combined_chain.next_free_depper, sizeof (struct Combined_Chain*), compare_1);

    // Alle Combined_Chain-Objekte durchgehen, die sich auf erster Ebene befinden
    for (uint_fast8_t i = 0; i < main_combined_chain.next_free_depper; ++ i)
    {
        if (main_combined_chain.deeper [i]->next_free_depper == 0)
        {
            // Alle Positionsnummern in Strings konvertieren
            for (uint_fast8_t i2 = 0; i2 < main_combined_chain.deeper [i]->next_free_position; ++ i2)
            {
                char int_to_str [10] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
                int2str (int_to_str, COUNT_ARRAY_ELEMENTS(int_to_str) - 1,
                        main_combined_chain.deeper [i]->positions [i2]);

                strncat (iupac_name, int_to_str, iupac_name_space_left);
                iupac_name_space_left -= strlen (int_to_str);

                if ((i2 + 1) < main_combined_chain.deeper [i]->next_free_position)
                {
                    strncat (iupac_name, ",", iupac_name_space_left);
                    iupac_name_space_left -= strlen (",");
                }
            }

            strncat (iupac_name, "-", iupac_name_space_left);
            iupac_name_space_left -= strlen ("-");

            // Zahlenwort anbringen, falls notwendig
            // Theoretisch koennte man auch bei einem Objekt das Zahlenwort "Mono" anfuegen. Dies wird in der Praxis
            // aber fast nie gemacht. Hoechstens dann, um Unklarheiten auszuraeumen. Daher wird es auch hier nicht
            // gemacht, da Unklarheiten in der Praxis fast nie auftreten.
            if (main_combined_chain.deeper [i]->next_free_position > 1)
            {
                strncat (iupac_name, NUMBER_WORDS [main_combined_chain.deeper [i]->next_free_position - 1], iupac_name_space_left);
                iupac_name_space_left -= strlen (NUMBER_WORDS [main_combined_chain.deeper [i]->next_free_position - 1]);
            }

            strncat (iupac_name, ALKYL_WORDS [main_combined_chain.deeper [i]->branch_length - 1], iupac_name_space_left);
            iupac_name_space_left -= strlen (ALKYL_WORDS [main_combined_chain.deeper [i]->branch_length - 1]);

            strncat (iupac_name, "-", iupac_name_space_left);
            iupac_name_space_left -= strlen ("-");
        }
    }

    // Das Combined_Chain-Objekt durchgehen und die Fragmente im IUPAC-Namen eintragen
    for (uint_fast8_t i = 0; i < main_combined_chain.next_free_depper; ++ i)
    {
        // Die Durchsuchung macht nur Sinn, wenn es tiefere Combined_Chain-Objekt gibt
        if (main_combined_chain.deeper [i]->next_free_depper > 0)
        {
            Go_Deeper (main_combined_chain.deeper [i], &main_combined_chain, iupac_name, iupac_name_space_left);
        }
    }

    // Am Ende der Verarbeitung bleibt ein Minuszeichen im Namen zurueck, der nicht dorthin gehoert
    if (strlen (iupac_name) > 0)
    {
        if (iupac_name [strlen (iupac_name) - 1] == '-')
        {
            iupac_name [strlen (iupac_name) - 1] = '\0';
        }
    }
    // Wenn bisher kein Teil des Namens erzeugt wurde, dann handelt es sich um eine gerade Kette. Bei diesen ist es
    // ueblich ein "n-" voranzustellen, um explizit zu sagen, dass es eine gerade Kette ist.
    else
    {
        strncat (iupac_name, "n-", iupac_name_space_left);
        iupac_name_space_left -= strlen ("n-");
    }

    // Alkan-Name am Ende einfuegen
    strncat (iupac_name, ALKAN_WORDS_DE [main_combined_chain.branch_length - 1], iupac_name_space_left);
    iupac_name_space_left -= strlen (ALKAN_WORDS_DE [main_combined_chain.branch_length - 1]);
    // ===== ===== ===== BEGINN Namen erzeugen ===== ===== =====

    // PRINTF_FFLUSH ("Name: %s\n", iupac_name)

    // Nullterminierung garantieren
    iupac_name [iupac_name_length - 1] = '\0';

    // Am Ende alles wieder aufraeumen
    for (size_t i = 0; i < next_free_array_element; ++ i)
    {
        FREE_AND_SET_TO_NULL(dynamic_created_objects [i]);
    }

    return;
}

//=====================================================================================================================

/**
 * Eine Ebene tiefer im Combined_Chain-Konstrukt gehen.
 */
static void
Go_Deeper
(
        const struct Combined_Chain* const restrict combined,   // Aktuelles Combined_Chain-Objekt
        const struct Combined_Chain* const restrict back,       // Combined_Chain-Objekt, von denen der Aufruf gekommen
                                                                // ist
        char* const restrict str,                               // Speicher fuer den IUPAC-Namen
        size_t size_left                                        // Anzahl an Zeichen, die noch zur Verfuegung stehen
)
{
    // PRINTF_FFLUSH ("Name: \"%s\" (size_left: %zu)\n", str, size_left)

    // Alle Positionsnummern in Strings konvertieren
    for (uint_fast8_t i = 0; i < combined->next_free_position; ++ i)
    {
        char int_to_str [10] = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
        int2str (int_to_str, COUNT_ARRAY_ELEMENTS(int_to_str) - 1, combined->positions [i]);

        strncat (str, int_to_str, size_left);
        size_left -= strlen (int_to_str);

        if ((i + 1) < combined->next_free_position)
        {
            strncat (str, ",", size_left);
            size_left -= strlen (",");
        }
    }

    // Wenn das aktuelle Combined_Chain-Objekt mehr als weitere Objekte enthaelt, dann muss ein Minuszeichen und das
    // passende Zahlenwort eingefuegt werden
    // Theoretisch koennte man auch bei einem Objekt das Zahlenwort "Mono" anfuegen. Dies wird in der Praxis aber fast
    // nie gemacht. Hoechstens dann, um Unklarheiten auszuraeumen. Daher wird es auch hier nicht gemacht
    if (combined->next_free_position > 1)
    {
        strncat (str, "-", size_left);
        size_left -= strlen ("-");

        strncat (str, NUMBER_WORDS [combined->next_free_position - 1], size_left);
        size_left -= strlen (NUMBER_WORDS [combined->next_free_position - 1]);
    }

    // Wenn im aktuellen Combined_Chain-Objekt es keine weiteren Objekte gibt, dann wurde die maximale Tiefe der
    // aktuellen Objekte erreicht
    if (combined->next_free_depper == 0 && back->next_free_depper > 0)
    {
        if (combined->next_free_position <= 1)
        {
            strncat (str, "-", size_left);
            size_left -= strlen ("-");
        }

        // Beide Alkylwoerter anbringen ...
        strncat (str, ALKYL_WORDS [combined->branch_length - 1], size_left);
        size_left -= strlen (ALKYL_WORDS [combined->branch_length - 1]);
        strncat (str, ALKYL_WORDS [back->branch_length - 1], size_left);
        size_left -= strlen (ALKYL_WORDS [back->branch_length - 1]);

        // ... und aktuelle Ebene abschliessen
        strncat (str, ")-", size_left);
        size_left -= strlen (")-");
    }

    // Alle erreichbaren Combined_Chain-Objekte abarbeiten
    for (uint_fast8_t i = 0; i < combined->next_free_depper; ++ i)
    {
        if (strlen (str) >= 2)
        {
            // Wenn nicht gerade zuvor eine schliessende Klammer hinzugefuegt wurde, dann wird eine neue Ebene (mittels
            // Klammern) im IUPAC-Namen erstellt
            if (str [strlen (str) - 1] != '-' && str [strlen (str) - 2] != ')')
            {
                strncat (str, "-(", size_left);
                size_left -= strlen ("-(");
            }
        }
        // Wenn es weniger als zwei Zeichen in der aktuell erzeugten Zeichenkette gibt, dann kann nicht zuvor eine
        // schliessende Klammer (und das folgende Minuszeichen) enthalten sein
        // => Immer "-(" anhaengen
        else
        {
            strncat (str, "-(", size_left);
            size_left -= strlen ("-(");
        }

        // Eine Ebene tiefer gehen
        Go_Deeper (combined->deeper [i], combined, str, size_left);
    }

    // PRINTF_FFLUSH ("Name: \"%s\" (size_left: %zu)\n", str, size_left)

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Ein neues Combined_Chain-Objekt erzeugen und mit den Werten eines Chain-Objektes initialisieren.
 *
 * Das Objekt wird dynamisch erzeugt.
 *
 * Um das spaetere Loeschen der dynamischen Objekte zu vereinfachen, werden alle Adressen der dynamischen Objekte
 * gesichert.
 */
static struct Combined_Chain*                                                   // Neues dynamisch erzeugte
                                                                                // Combined_Chain-Objekt
Create_New_Combined_Chain
(
        const struct Chain* const restrict chain,                               // Chain-Objekt, welches fuer die
                                                                                // Initialisierung verwendet wird
        struct Combined_Chain** const restrict dynamic_created_combined_chain,  // Array um festzuhalten, welche
                                                                                // Objekte dynamisch erzeugt wurden
        size_t* const restrict next_free_array_element                          // Naechstes freies Element im Array
)
{
    struct Combined_Chain* new_combined_chain = (struct Combined_Chain*) CALLOC(1, sizeof (struct Combined_Chain));
    ASSERT_ALLOC(new_combined_chain, "Cannot create a Combined_Chain object !",
            sizeof (struct Combined_Chain));
    memset (new_combined_chain, '\0', sizeof (struct Combined_Chain));
    SET_POINTER_ARRAY_TO_NULL(new_combined_chain->deeper, COUNT_ARRAY_ELEMENTS(new_combined_chain->deeper));

    new_combined_chain->branch_length                                       = chain->length;
    new_combined_chain->nesting_depth                                       = chain->nesting_depth;
    new_combined_chain->positions [new_combined_chain->next_free_position]  = chain->position;
    new_combined_chain->next_free_position ++;

    ASSERT_MSG(*next_free_array_element < SIZE_OF_DYNAMIC_CREATED_OBJECTS_ARRAY,
            "dynamic_created_combined_chain array is too small !");

    dynamic_created_combined_chain [*next_free_array_element] = new_combined_chain;
    (*next_free_array_element) ++;

    return new_combined_chain;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Vergleichfunktion fuer qsort()
 *
 * Hierbei geht es darum, dass der Name der Aeste aufsteigend alphabetisch sortiert werden. Das Ziel ist, dass z.B.
 * eine Ethyl-Abzweigung vor einer Methyl-Abzweigung im IUPAC-Namen erscheint.
 */
static int compare_1 (const void* element_1, const void* element_2)
{
    const struct Combined_Chain* const e_1 = *((const struct Combined_Chain** const) element_1);
    const struct Combined_Chain* const e_2 = *((const struct Combined_Chain** const) element_2);

    const size_t length_str_1 = strlen (ALKYL_WORDS [e_1->branch_length - 1]);
    const size_t length_str_2 = strlen (ALKYL_WORDS [e_2->branch_length - 1]);

    return strncmp (ALKYL_WORDS [e_1->branch_length - 1], ALKYL_WORDS [e_2->branch_length - 1],
            (length_str_1 < length_str_2) ? length_str_1 : length_str_2);
}

//---------------------------------------------------------------------------------------------------------------------





// Alle hier erzeugten #defines werden wieder deaktiviert, da sie nur in dieser Uebersetzungseinheit gueltig sein
// sollen !
#ifdef SIZE_OF_DYNAMIC_CREATED_OBJECTS_ARRAY
#undef SIZE_OF_DYNAMIC_CREATED_OBJECTS_ARRAY
#endif /* SIZE_OF_DYNAMIC_CREATED_OBJECTS_ARRAY */
