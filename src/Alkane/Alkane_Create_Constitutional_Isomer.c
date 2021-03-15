/*
 * Alkane_Create_Constitutional_Isomer.c
 *
 *  Created on: 08.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane_Create_Constitutional_Isomer.h"
#include <stdlib.h>
#include <time.h>
#include "Alkane_Info_Constitutional_Isomer.h"
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "Alkane.h"
#include "Alkane_Container.h"
#include "Alkane_Branch.h"
#include "Alkane_Branch_Container.h"
#include "../Print_Tools.h"
#include "../Misc.h"

/**
 * Die Anzahl an Schleifendurchlaeufen, die erreicht werden muessen, damit das Programm eine Fortschrittsausgabe
 * durchfuehrt.
 * Dies dient hauptsaechlich dazu, dass die Anzahl an Ausgaben reduziert werden, um die Laufzeit zu beschleunigen.
 */
#ifndef PROGRESS_OUTPUT_INTERVAL
#define PROGRESS_OUTPUT_INTERVAL 47500000
#else
#error "The macro \"PROGRESS_OUTPUT_INTERVAL\" is already defined !"
#endif /* PROGRESS_OUTPUT_INTERVAL */

/**
 * Die Anzahl an Schleifendurchlaeufen, die erreicht werden muessen, damit das Programm eine Fortschrittsausgabe
 * bei der Berechnung der moeglichen innersten Schleifendurchlaeufe.
 * Dies dient hauptsaechlich dazu, dass die Anzahl an Ausgaben reduziert werden, um die Laufzeit zu beschleunigen.
 */
#ifndef PROGRESS_OUTPUT_LOOP_COUNTER_INTERVAL
#define PROGRESS_OUTPUT_LOOP_COUNTER_INTERVAL 22
#else
#error "The macro \"PROGRESS_OUTPUT_LOOP_COUNTER_INTERVAL\" is already defined !"
#endif /* PROGRESS_OUTPUT_LOOP_COUNTER_INTERVAL */





/**
 * Den aktuellen Fortschritt - mit verbleibener Zeit - prozentual bestimmen und auf stdout ausgeben.
 */
static void Print_Percent_Done
(
        const char* string_prefix,          // Prefix, der vor der Prozentangabe ausgegeben wird
        const uint_fast64_t current_run,    // Aktueller Wert -> Pozentwert bezogen auf max_count_run wir ermtttelt
        const uint_fast64_t max_count_run   // Maximaler Wert -> Stellt 100 % dar
);

//---------------------------------------------------------------------------------------------------------------------

/**
 * Die Alkan Konstitutionsisomere bei gegebener Anzahl an C-Atomen bestimmen.
 *
 * Die Grundidee stammt aus dem Dokument "Strukturisomere der Alkane" von
 * Sascha Kurz, Universität Bayreuth. E-Mail: sascha.kurz@uni-bayreuth.de
 * und
 * Armin Moritz, Johanneum-Gymnasium in Herborn, E-Mail: arminmoritz@gmx.de
 *
 *
 * >>> Zusammengefasst <<<
 * Der Prozess der Erzeugung laesst sich in drei Schritte unterteilen:
 * 1. Teil: Liste der Alkanaeste erzeugen
 *          Siehe Dokument "Strukturisomere der Alkane" Seite 2 - 8
 * 2. Teil: Alle moeglichen Alkane aus den Alkanastlisten bilden
 *          Siehe Dokument "Strukturisomere der Alkane" Seite 8 - 13
 * 3. Teil: Die Alkane, die genau die gesuchte Laenge haben, auswaehlen
 *          => Dies sind die gesuchten Konstitutionsisomere
 *          Siehe Dokument "Strukturisomere der Alkane" Seite 13 - 14
 *
 * Asserts:
 *      number_of_c_atoms < MAX_NUMBER_OF_C_ATOMS
 *      number_of_created_isomers != NULL
 */
void Create_Alkane_Constitutional_Isomers
(
       const uint_fast8_t number_of_c_atoms,
       uint_fast64_t* const number_of_created_isomers
)
{
    ASSERT_FMSG(number_of_c_atoms <= MAX_NUMBER_OF_C_ATOMS, "Number of C atoms is too large ! Got: %" PRIuFAST8
            "; Max. valid: %" PRIuFAST8,  number_of_c_atoms, MAX_NUMBER_OF_C_ATOMS);
    ASSERT_MSG(number_of_created_isomers != NULL, "number_of_created_isomers is NULL !");

    // ===== ===== ===== ===== ===== ===== ===== ===== BEGINN 1. Teil ===== ===== ===== ===== ===== ===== ===== =====
    // Die Anzahl an Alkane_Branch_Container ist die aufgerundete Haelfte der Anzahl an C-Atomen
    const size_t count_alkane_branch_container = (size_t) ((number_of_c_atoms + 1) / 2);

    // Alkanastliste mit Objekten, deren maximale Laenge aus EINEM C-Atom besteht, erzeugen
    // const unsigned char empty_branch_numbercode []  = { };   // So wuerde ein leerer Ast aussehen, der nur aus einem
                                                                // H-Atom besteht. Leider ist die kein gueltiger Code,
                                                                // da das Array keine Elemente enthalten wuerde
    const unsigned char single_c_atom_numbercode [] = { 1 };    // Ein einfaches C-Atom als Ast

    // Aeste fuer den 1. Container erzeugen
    struct Alkane_Branch* empty_branch          = Create_Alkane_Branch (NULL, 0);
    struct Alkane_Branch* single_c_atom_branch  = Create_Alkane_Branch (single_c_atom_numbercode,
            sizeof (single_c_atom_numbercode));

    struct Alkane_Branch_Container* Container_Height_1 = Create_Alkane_Branch_Container ();
    Add_Alkane_Branch_To_Container (Container_Height_1, empty_branch);
    Add_Alkane_Branch_To_Container (Container_Height_1, single_c_atom_branch);

    PRINTF_FFLUSH("Alkane_Branch container %2zu / %2zu fully created. (%" PRIuFAST64 " braches were build)\n",
            (size_t) 1u, count_alkane_branch_container, Container_Height_1->size);

    // Alkanastliste mit Objekten, deren maximale Laenge aus ZWEI C-Atoms besteht, erzeugen
    const unsigned char two_c_atoms_numbercode []   = { 1, 1 };         // Zwei C-Atome als Ast
    const unsigned char three_c_atoms_numbercode [] = { 1, 1, 1 };      // Drei C-Atome als Ast
                                                                        //      2 3
                                                                        //      |/
                                                                        //      1
    const unsigned char four_c_atoms_numbercode []  = { 1, 1, 1, 1 };   // Vier C-Atome als Ast
                                                                        //    2 3 4
                                                                        //     \|/
                                                                        //      1
    // Aeste fuer den 2. Container erzeugen
    struct Alkane_Branch* two_c_atoms_branch    = Create_Alkane_Branch (two_c_atoms_numbercode,
            sizeof (two_c_atoms_numbercode));
    struct Alkane_Branch* three_c_atoms_branch  = Create_Alkane_Branch (three_c_atoms_numbercode,
            sizeof (three_c_atoms_numbercode));
    struct Alkane_Branch* four_c_atoms_branch   = Create_Alkane_Branch (four_c_atoms_numbercode,
            sizeof (four_c_atoms_numbercode));

    struct Alkane_Branch_Container* Container_Height_2 = Create_Alkane_Branch_Container ();
    Add_Alkane_Branch_To_Container (Container_Height_2, two_c_atoms_branch);
    Add_Alkane_Branch_To_Container (Container_Height_2, three_c_atoms_branch);
    Add_Alkane_Branch_To_Container (Container_Height_2, four_c_atoms_branch);

    PRINTF_FFLUSH("Alkane_Branch container %2zu / %2zu fully created. (%" PRIuFAST64 " braches were build)\n",
            (size_t) 2u, count_alkane_branch_container, Container_Height_2->size);



    // Jetzt wird es interessant:
    // Aus den hart codierten Alkane_Branch_Container der Hoehe 1 und 2 werden die Container mit einer groesseren Hoehe

    // Container fuer die Alkane_Branch_Container-Zeiger
    struct Alkane_Branch_Container* container_height_x [(MAX_NUMBER_OF_C_ATOMS + 1) / 2];
    for (size_t i = 2; i < COUNT_ARRAY_ELEMENTS(container_height_x); ++ i)
    {
        container_height_x [i] = NULL;
    }
    container_height_x [0] = Container_Height_1;
    container_height_x [1] = Container_Height_2;

    // Siehe Pseudocode I auf Seite 17 von "Strukturisomere der Alkane"
    for (size_t next_container = 2; next_container < count_alkane_branch_container; ++ next_container)
    {
        // Neuen Container initialisieren
        container_height_x [next_container] = Create_Alkane_Branch_Container ();

        // Damit das Iterieren ueber die Aeste bequem ueber einfache Schleifen funktioniert, muessen die
        // Alkane_Branches in einem 1-dimensionalen Array sein !
        size_t count_branches = 0;
        for (size_t i = 0; i < next_container; ++ i)
        {
            // Die Anzahl an bisher erzeugten Alkane_Branch-Objekte ermitteln
            count_branches += (size_t) container_height_x [i]->size;
        }

        struct Alkane_Branch** flat_alkane_branch_container = (struct Alkane_Branch**) CALLOC (count_branches,
                sizeof (struct Alkane_Branch*));
        ASSERT_ALLOC(flat_alkane_branch_container, "Cannot create memory for a flat address model !",
                count_branches * sizeof (struct Alkane_Branch*));

        // Flaches Speichermodell erzeugen
        size_t current_element_index = 0;

        for (size_t i = 0; i < next_container; ++ i)
        {
            for (size_t i2 = 0; i2 < container_height_x [i]->size; ++ i2)
            {
                flat_alkane_branch_container [current_element_index] = container_height_x [i]->data [i2];
                ++ current_element_index;
            }
        }
        ASSERT_MSG(current_element_index == count_branches, "count_branches needs to be equal with current_element_index !");


        size_t loop_start = 0;
        for (size_t i = 0; i < next_container - 1; ++ i)
        {
            loop_start += (size_t) container_height_x [i]->size;
        }

        // Die Aeste des aktuellsten Containers verwenden, damit die Laenge des Astes aus drei C-Atomen besteht
        for (size_t i2 = loop_start; i2 < count_branches; ++ i2) // < !
        {
            for (size_t i3 = 0; i3 <= i2; ++ i3) // <= !
            {
                // Aeste aussortieren, die zu gross sind
                // Wenn die Groesse des zu konstruierenden Asts GROESSER als die uebergebene Anzahl an C-Atomen ist,
                // dann kann dieser Ast zu keinem gueltigen Ergebnis fuehren !
                if ((flat_alkane_branch_container [i2]->length + flat_alkane_branch_container [i3]->length + 1) >
                        number_of_c_atoms)
                {
                    continue;
                }

                for (size_t i4 = 0; i4 <= i3; ++ i4) // <= !
                {
                    // Auch hier das Gleiche: Aeste aussortieren, die zu gross sind
                    // Wenn die Groesse des zu konstruierenden Asts GROESSER als die uebergebene Anzahl an C-Atomen ist,
                    // dann kann dieser Ast zu keinem gueltigen Ergebnis fuehren !
                    if ((flat_alkane_branch_container [i2]->length + flat_alkane_branch_container [i3]->length +
                            flat_alkane_branch_container [i4]->length + 1) > number_of_c_atoms)
                    {
                        continue;
                    }

                    // Aeste miteinander verbinden, wobei die Aeste an genau ein C-Atom angehaengt wird
                    //      +-- flat_alkane_branch_container [i2]
                    //    --C-- flat_alkane_branch_container [i3]
                    //      +-- flat_alkane_branch_container [i4]
                    //
                    // Fuer diesen Mechanismus muessen die Zahlencodes der Aeste angepasst werden
                    // Dies ist der gleiche Prozess wie bei der Erstellung eines Alkans aus Aesten. Daher kann der
                    // Mechanismus vom Alkane-Objket verwendet werden, auch wenn hier natuerlich KEIN Alkan gebildet
                    // wird !!!
                    // Das C-Atom, an denen die drei Aeste angehaengt werden, wird durch ein Ast mit genau einem C-Atom
                    // dargestellt
                    struct Alkane* temp = Create_Alkane (single_c_atom_branch, flat_alkane_branch_container [i2],
                            flat_alkane_branch_container [i3], flat_alkane_branch_container [i4]);

                    // Der Nummerncode im zweckentfremdeten Alkane-Objekt ist der Zahlencode unseres neuen Alkanasts
                    struct Alkane_Branch* new_branch = Create_Alkane_Branch (temp->merged_numbercode,
                            temp->number_of_c_atoms);

                    Add_Alkane_Branch_To_Container (container_height_x [next_container], new_branch);

                    // Zweckentfremdetes Objekt wieder loeschen
                    Delete_Alkane (temp);
                    temp = NULL;
                }
            }
        }

        FREE_AND_SET_TO_NULL (flat_alkane_branch_container);
        flat_alkane_branch_container = NULL;

        PRINTF_FFLUSH ("Alkane_Branch container %2zu / %2zu fully created. (%" PRIuFAST64 " braches were build)\n",
                next_container + 1, count_alkane_branch_container, container_height_x [next_container]->size);
    }
    puts ("");
    // ===== ===== ===== ===== ===== ===== ===== ===== ENDE 1. Teil ===== ===== ===== ===== ===== ===== ===== =====



    // Der 3. Teil wird quasi implizit gemacht, indem nur die Alkane konstruiert werden, die genau die passende Groesse
    // haben !
    // ===== ===== ===== ===== ===== ===== ===== ===== BEGINN 2. + 3. Teil ===== ===== ===== ===== ===== ===== ===== =====
    // Nun muessen aus den Aesten die Alkane gebildet werden
    // Dafuer werden Alkan_Container erzeugt, die die Objekte sichern, deren laengste Kette aus genau x C-Atomen
    // besteht
    // Die ersten drei Container werden wieder direkt im Quelltext angegeben
    // Alkan-Container, deren laengste Kette aus GENAU EINEM C-Atom besteht
    struct Alkane* single_c_atom_alkane = Create_Alkane (single_c_atom_branch, NULL, NULL, NULL);
    struct Alkane_Container* alkane_container_main_chain_length_1 = Create_Alkane_Container ();
    Add_Alkane_To_Container (alkane_container_main_chain_length_1, single_c_atom_alkane);

    PRINTF_FFLUSH("Building alkane container %2zu / %2zu\n", (size_t) 1u, (size_t) number_of_c_atoms);
    PRINTF_FFLUSH("Alkane container %2zu / %2zu fully created. (%" PRIuFAST64 " alkanes were build)\n", (size_t) 1u,
            (size_t) number_of_c_atoms, alkane_container_main_chain_length_1->size);

    // Alkan-Container, deren laengste Kette aus GENAU ZWEI C-Atom besteht
    struct Alkane* two_c_atom_alkane = Create_Alkane (single_c_atom_branch, single_c_atom_branch, NULL, NULL);
    struct Alkane_Container* alkane_container_main_chain_length_2 = Create_Alkane_Container ();
    Add_Alkane_To_Container (alkane_container_main_chain_length_2, two_c_atom_alkane);

    PRINTF_FFLUSH("Building alkane container %2zu / %2zu\n", (size_t) 2u, (size_t) number_of_c_atoms);
    PRINTF_FFLUSH("Alkane container %2zu / %2zu fully created. (%" PRIuFAST64 " alkanes were build)\n", (size_t) 2u,
            (size_t) number_of_c_atoms, alkane_container_main_chain_length_2->size);

    // Alkan-Container, deren laengste Kette aus GENAU DREI C-Atom besteht
    struct Alkane* three_c_atom_alkane = Create_Alkane (single_c_atom_branch, single_c_atom_branch, single_c_atom_branch,
            NULL);
    struct Alkane* four_c_atom_alkane = Create_Alkane (single_c_atom_branch, single_c_atom_branch, single_c_atom_branch,
            single_c_atom_branch);
    struct Alkane* five_c_atom_alkane = Create_Alkane (two_c_atoms_branch, single_c_atom_branch, single_c_atom_branch,
            single_c_atom_branch);
    struct Alkane_Container* alkane_container_main_chain_length_3 = Create_Alkane_Container ();
    Add_Alkane_To_Container (alkane_container_main_chain_length_3, three_c_atom_alkane);
    Add_Alkane_To_Container (alkane_container_main_chain_length_3, four_c_atom_alkane);
    Add_Alkane_To_Container (alkane_container_main_chain_length_3, five_c_atom_alkane);

    PRINTF_FFLUSH("Building alkane container %2zu / %2zu\n", (size_t) 3u, (size_t) number_of_c_atoms);
    PRINTF_FFLUSH("Alkane container %2zu / %2zu fully created. (%" PRIuFAST64 " alkanes were build)\n", (size_t) 3u,
            (size_t) number_of_c_atoms, alkane_container_main_chain_length_3->size);


    // Jetzt wird es hier wieder interessant:
    // Aus den zuvor gebildeten Alkan_Branch-Objekten werden nun die Alkane gebildet

    // Container fuer die Alkane_Container-Zeiger
    struct Alkane_Container* alkane_container_main_chain_length_x [MAX_NUMBER_OF_C_ATOMS];
    for (size_t i = 3; i < COUNT_ARRAY_ELEMENTS(alkane_container_main_chain_length_x); ++ i)
    {
        alkane_container_main_chain_length_x [i] = NULL;
    }
    alkane_container_main_chain_length_x [0] = alkane_container_main_chain_length_1;
    alkane_container_main_chain_length_x [1] = alkane_container_main_chain_length_2;
    alkane_container_main_chain_length_x [2] = alkane_container_main_chain_length_3;

    for (size_t next_alkane_container = 3; next_alkane_container < number_of_c_atoms; ++ next_alkane_container)
    {
        PRINTF_FFLUSH("Building alkane container %2zu / %2zu\n", (size_t) next_alkane_container + 1,
                (size_t) number_of_c_atoms);

        // Neuen Container initialisieren
        alkane_container_main_chain_length_x [next_alkane_container] = Create_Alkane_Container ();

        const size_t container_height_index = (next_alkane_container) / 2;

        size_t count_branches = 0;
        for (size_t i = 0; i <= container_height_index; ++ i) // <= !
        {
            // Die Anzahl an Aesten in den zu verwendenen Containern ermitteln
            count_branches += (size_t) container_height_x [i]->size;
        }

        struct Alkane_Branch** flat_alkane_branch_container = (struct Alkane_Branch**) CALLOC (count_branches,
                sizeof (struct Alkane_Branch*));
        ASSERT_ALLOC(flat_alkane_branch_container, "Cannot create memory for a flat address model !",
                count_branches * sizeof (struct Alkane_Branch*));

        // Flaches Speichermodell erzeugen
        size_t current_element_index = 0;
        for (size_t i = 0; i <= container_height_index; ++ i) // <= !
        {
            for (size_t i2 = 0; i2 < container_height_x [i]->size; ++ i2)
            {
                flat_alkane_branch_container [current_element_index] = container_height_x [i]->data [i2];
                ++ current_element_index;
            }
        }

        uint_fast64_t max_inner_loop_runs   = 0;
        uint_fast64_t count_inner_loop_runs = 0;

        // Je nachdem, ob das zentrale Objekt ein C-Atom oder eine Bindung ist, muss anders verfahren werden
        // next_alkane_container beginnt bei der Zaehlung mit 0 !
        // ===== ===== ===== BEGINN Zentrales Objekt ist eine BINDUNG ===== ===== =====
        if (((next_alkane_container + 1) % 2) == 0)
        {
            size_t loop_start = 0;
            for (size_t i = 0; i < container_height_index; ++ i)
            {
                loop_start += (size_t) container_height_x [i]->size;
            }

            PRINTF_NO_VA_ARGS_FFLUSH("Start building. Calculate start information ...");
            // Anzahl der moeglichen inneren Schleifendurchlaeufe berechnen
            for (size_t i2 = loop_start; i2 < count_branches; ++ i2)
            {
                max_inner_loop_runs += (i2 - loop_start + 1);
            }
            // Die Ausgabezeile von der letzten Ausgabe komplett bereinigen
            CLEAN_LINE();

            // Siehe Pseudocode II auf Seite 18 von "Strukturisomere der Alkane"
            // ===== BEGINN Berechnungsschleifen =====
            for (size_t i2 = loop_start; i2 < count_branches; ++ i2)
            {
                for (size_t i3 = loop_start; i3 <= i2; ++ i3) // <= !
                {
                    static uint_fast32_t local_run_counter = 0;
                    ++ local_run_counter;
                    ++ count_inner_loop_runs;

                    // Aus Effizienzgruenden soll nur jedes PROGRESS_OUTPUT_INTERVAL. Mal eine Ausgabe stattfinden
                    // Einfache Konsolenausgaben sind langsame Operationen, sodass die Anzahl begrenzt werden sollte
                    if (local_run_counter == PROGRESS_OUTPUT_INTERVAL)
                    {
                        // Prozentualen Fortschritt bestimmen und ausgeben
                        Print_Percent_Done ("Building ...", count_inner_loop_runs, max_inner_loop_runs);
                        local_run_counter = 0;
                    }

                    // Besitzt das Objekt, welches gleich erstellt wird, die GENAU passende Anzahl an C-Atomen ?
                    if ((flat_alkane_branch_container [i2]->length + flat_alkane_branch_container [i3]->length)
                            != number_of_c_atoms)
                    {
                        continue;
                    }

                    struct Alkane* new_alkane = Create_Alkane (flat_alkane_branch_container [i2],
                            flat_alkane_branch_container [i3], NULL, NULL);
                    Add_Alkane_To_Container (alkane_container_main_chain_length_x [next_alkane_container], new_alkane);
                }
            }
            // ===== ENDE Berechnungsschleifen =====
        }
        // ===== ===== ===== ENDE Zentrales Objekt ist eine BINDUNG ===== ===== =====
        // ===== ===== ===== BEGINN Zentrales Objekt ist ein C-ATOM ===== ===== =====
        else
        {
            size_t loop_start   = 0;
            size_t loop_end     = 0;
            for (size_t i = 0; i < container_height_index - 1; ++ i)
            {
                loop_start += (size_t) container_height_x [i]->size;
            }
            for (size_t i = 0; i < container_height_index; ++ i)
            {
                loop_end += (size_t) container_height_x [i]->size;
            }

            // Anzahl der moeglichen inneren Schleifendurchlaeufe berechnen
            for (size_t i2 = loop_start; i2 < loop_end; ++ i2)
            {
                static uint_fast32_t local_run_counter = 0;
                ++ local_run_counter;

                for (size_t i3 = loop_start; i3 <= i2; ++ i3)
                {
                    for (size_t i4 = 0; i4 <= i3; ++ i4)
                    {
                        max_inner_loop_runs += (i4 /* - i5 */ + 1);
                    }
                }

                if (local_run_counter == PROGRESS_OUTPUT_LOOP_COUNTER_INTERVAL)
                {
                    Print_Percent_Done ("Start building. Calculate start information ...", i2 - loop_start,
                            loop_end - loop_start);
                    local_run_counter = 0;
                }
            }
            // Die Ausgabezeile von der letzten Ausgabe komplett bereinigen
            CLEAN_LINE();

            // Zwei der vier Schleifen duerfen nur die Aeste aus dem aktuellen Container verwenden. Dies ist notwendig,
            // damit eine Hauptkette mit der gesuchten Laenge gebildet wird
            // Die restlichen beiden Schleifen duerfen auch die Aeste der niedrigeren Container verwenden. Auch hier
            // wird fuer die bessere Umsetzbarkeit ein flaches Speichermodell der Aeste verwendet.
            // Siehe Pseudocode III auf Seite 18 von "Strukturisomere der Alkane"
            // ===== BEGINN Berechnungsschleifen =====
            for (size_t i2 = loop_start; i2 < loop_end; ++ i2)
            {
                for (size_t i3 = loop_start; i3 <= i2; ++ i3) // <= !
                {
                    for (size_t i4 = 0; i4 <= i3; ++ i4) // <= !
                    {
                        static uint_fast32_t local_run_counter = 0;
                        ++ local_run_counter;
                        ++ count_inner_loop_runs;

                        // Aus Effizienzgruenden soll nur jedes PROGRESS_OUTPUT_INTERVAL. Mal eine Ausgabe stattfinden
                        // Einfache Konsolenausgaben sind langsame Operationen, sodass die Anzahl begrenzt werden sollte
                        if (local_run_counter == PROGRESS_OUTPUT_INTERVAL)
                        {
                            // Prozentualen Fortschritt bestimmen und ausgeben
                            Print_Percent_Done ("Building ...", count_inner_loop_runs, max_inner_loop_runs);
                            local_run_counter = 0;
                        }

                        // Besitzt das Objekt, welches in der naechsten inneren Schleife erstellt wird, ZU VIELE C-Atome ?
                        // Wenn ja, dann kann dieses Objekt kein gueltiges Ergebnis sein !
                        if ((flat_alkane_branch_container [i2]->length + flat_alkane_branch_container [i3]->length +
                                flat_alkane_branch_container [i4]->length + 1) > number_of_c_atoms)
                        {
                            count_inner_loop_runs += (i4 + 1);
                            continue;
                        }

                        for (size_t i5 = 0; i5 <= i4; ++ i5) // <= !
                        {
                            // Besitzt das Objekt, welches gleich erstellt wird, die GENAU passende Anzahl an C-Atomen ?
                            if ((flat_alkane_branch_container [i2]->length + flat_alkane_branch_container [i3]->length +
                                    flat_alkane_branch_container [i4]->length + flat_alkane_branch_container [i5]->length + 1)
                                    != number_of_c_atoms)
                            {
                                continue;
                            }

                            // Verbinden der vier Aeste miteinander durch ein zentrales C-Atom
                            // Das zentrale C-Atom muss irgendwie in den Zahlencode eines Astes eingebaut werden, damit
                            // es in der Struktur des Alkans erhalten bleibt.
                            // Das zentrale C-Atom wird immer dem ersten Alkan_Branch zugeordnet. Dafuer wird ein
                            // temporaerer Alkanast erzeugt, welches den urspruenglichen Ast mit einem Ast, der nur aus
                            // einem C-Atom besteht, beinhaltet
                            struct Alkane* temp_alkane = Create_Alkane (flat_alkane_branch_container [i2],
                                    single_c_atom_branch, NULL, NULL);
                            struct Alkane_Branch* temp_alkane_branch = Create_Alkane_Branch (temp_alkane->merged_numbercode,
                                    temp_alkane->number_of_c_atoms);

                            struct Alkane* new_alkane = Create_Alkane (temp_alkane_branch,
                                    flat_alkane_branch_container [i3],
                                    flat_alkane_branch_container [i4],
                                    flat_alkane_branch_container [i5]);

                            Add_Alkane_To_Container (alkane_container_main_chain_length_x [next_alkane_container], new_alkane);

                            Delete_Alkane (temp_alkane);
                            temp_alkane = NULL;
                            Delete_Alkane_Branch (temp_alkane_branch);
                            temp_alkane_branch = NULL;
                        }
                    }
                }
            }
            // ===== ENDE Berechnungsschleifen =====
        }
        // ===== ===== ===== ENDE Zentrales Objekt ist ein C-ATOM ===== ===== =====

        // printf ("Count runs: %12" PRIuFAST64 "\nMax. runs:  %12" PRIuFAST64 "\n", count_inner_loop_runs, max_inner_loop_runs);

        FREE_AND_SET_TO_NULL(flat_alkane_branch_container);
        flat_alkane_branch_container = NULL;

        PRINTF_FFLUSH("\rAlkane container %2zu / %2zu fully created. (%" PRIuFAST64 " alkanes were build)\n",
                (size_t) next_alkane_container + 1, (size_t) number_of_c_atoms,
                alkane_container_main_chain_length_x [next_alkane_container]->size);
    }
    puts ("");
    // ===== ===== ===== ===== ===== ===== ===== ===== ENDE 2. + 3. Teil ===== ===== ===== ===== ===== ===== ===== =====

    // Ergebnisse zaehlen
    uint_fast64_t count_results = 0;
    for (size_t next_alkane_container = 0; next_alkane_container < number_of_c_atoms; ++ next_alkane_container)
    {
        uint_fast64_t count_container_results = 0;

        for (size_t i = 0; i < alkane_container_main_chain_length_x [next_alkane_container]->size; ++ i)
        {
            if (alkane_container_main_chain_length_x [next_alkane_container]->data [i]->number_of_c_atoms == number_of_c_atoms)
            {
                ++ count_results;
                ++ count_container_results;
            }
        }

        printf ("RESULTS %2zu: %5" PRIuFAST64 " !\n", next_alkane_container + 1, count_container_results);
    }

    printf ("\n> RESULTS SUM: %" PRIuFAST64 " ! <\n\n", count_results);

    // Aufraeumen
    for (size_t next_container = 0; next_container < COUNT_ARRAY_ELEMENTS(container_height_x); ++ next_container)
    {
        if (container_height_x [next_container] != NULL)
        {
            Delete_Alkane_Branch_Container (container_height_x [next_container]);
            container_height_x [next_container] = NULL;
        }
    }
    for (size_t next_alkane_container = 0; next_alkane_container < COUNT_ARRAY_ELEMENTS(alkane_container_main_chain_length_x);
            ++ next_alkane_container)
    {
        if (alkane_container_main_chain_length_x [next_alkane_container] != NULL)
        {
            Delete_Alkane_Container (alkane_container_main_chain_length_x [next_alkane_container]);
            alkane_container_main_chain_length_x [next_alkane_container] = NULL;
        }
    }

    // Anzahl an erzeugten Isomeren - per Zeiger - uebergeben
    *number_of_created_isomers = count_results;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Den aktuellen Fortschritt - mit verbleibener Zeit - prozentual bestimmen und auf stdout ausgeben.
 */
static void Print_Percent_Done
(
        const char* string_prefix,          // Prefix, der vor der Prozentangabe ausgegeben wird
        const uint_fast64_t current_run,    // Aktueller Wert -> Pozentwert bezogen auf max_count_run wir ermtttelt
        const uint_fast64_t max_count_run   // Maximaler Wert -> Stellt 100 % dar
)
{
    static clock_t last_call                = 0;
    static uint_fast64_t last_current_run   = 0;

    // Differenzen seit dem letzten Aufruf bestimmen
    const clock_t time_delayed_since_last_call      = clock () - last_call;
    const uint_fast64_t runs_done_since_last_call   = current_run - last_current_run;

    // Verbleibene Zeit ermitteln
    const float runs_per_clock = (float) runs_done_since_last_call / (float) time_delayed_since_last_call;
    const float ETA = ((float) (max_count_run - current_run) / (float) runs_per_clock) / (float) CLOCKS_PER_SEC;

    // Prozentualen Fortschritt bestimmen und ausgeben
    const float percent_done = (float) ((float) current_run / ((float) max_count_run / 100.0f));

    printf ("\r");
    PRINTF_FFLUSH("%s (~ %5.2f %%) ETA: %8.2f sec.", string_prefix, (percent_done > 100.0f) ? 100.0f : percent_done, ETA);

    last_call           = clock ();
    last_current_run    = current_run;

    return;
}

//---------------------------------------------------------------------------------------------------------------------





#ifdef PROGRESS_OUTPUT_INTERVAL
#undef PROGRESS_OUTPUT_INTERVAL
#endif /* PROGRESS_OUTPUT_INTERVAL */

#ifdef PROGRESS_OUTPUT_LOOP_COUNTER_INTERVAL
#undef PROGRESS_OUTPUT_LOOP_COUNTER_INTERVAL
#endif /* PROGRESS_OUTPUT_LOOP_COUNTER_INTERVAL */
