/*
 * Alkane_Create_Constitutional_Isomer.c
 *
 *  Created on: 08.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane_Create_Constitutional_Isomer.h"
#include <stdlib.h>
#include "Alkane_Info_Constitutional_Isomer.h"
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "Alkane.h"
#include "Alkane_Branch.h"
#include "Alkane_Branch_Container.h"





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
 */
void Create_Alkane_Constitutional_Isomers
(
       const uint_fast8_t number_of_c_atoms
)
{
    ASSERT_FMSG(number_of_c_atoms <= MAX_NUMBER_OF_C_ATOMS, "Number of C atoms is too large ! Got: %" PRIuFAST8
            "; Max. valid: %" PRIuFAST8,  number_of_c_atoms, MAX_NUMBER_OF_C_ATOMS);

    // ===== ===== ===== ===== ===== BEGINN 1. Teil ===== ===== ===== ===== =====
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

    printf ("Alkane_Branch container %2zu / %2zu fully created. (%" PRIuFAST64 " braches were build)\n",
            (size_t) 1u, count_alkane_branch_container, Container_Height_1->size);
    fflush (stdout);


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

    printf ("Alkane_Branch container %2zu / %2zu fully created. (%" PRIuFAST64 " braches were build)\n",
            (size_t) 2u, count_alkane_branch_container, Container_Height_2->size);
    fflush (stdout);



    // Jetzt wird es interessant:
    // Aus den hart codierten Alkane_Branch_Container der Hoehe 1 und 2 werden die Container mit einer groesseren Hoehe

    // Container fuer die Alkane_Branch_Container-Zeiger
    struct Alkane_Branch_Container* container_height_x [(MAX_NUMBER_OF_C_ATOMS + 1) / 2];
    for (size_t i = 2; i < ((MAX_NUMBER_OF_C_ATOMS + 1) / 2); ++ i)
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

        // Naeherungsweise die Anzahl an Durchlaeufen der innersten Schleife berechnen, um eine ungefaehre Aussage
        // treffen zu koennen, wie viele Aeste fuer den aktuellen Container noch erzeugt werden muessen
        // Die Bestimmung der Durchlaeufe ist nicht trivial moeglich, da die Schleifengrenzen der inneren Schleifen
        // sich auf den Schleifenzaehler der jeweils aeusseren Schleife beziehen !
        const uint_fast64_t max_inner_loop_runs = (count_branches - container_height_x [next_container - 2]->size + 1) *
                ((count_branches + 1) / 2) * ((count_branches + 1) / 2);
        uint_fast64_t current_inner_loop_run    = 0;

        // Die Aeste des aktuellsten Containers verwenden, damit die Laenge des Astes aus drei C-Atomen besteht
        for (size_t i2 = (size_t) container_height_x [next_container - 2]->size; i2 < count_branches; ++ i2) // < !
        {
            for (size_t i3 = 0; i3 <= i2; ++ i3) // <= !
            {
                // Aeste aussortieren, die zu gross sind
                // Wenn die Groesse des zu konstruierenden Asts groesser als die uebergebene Anzahl an C-Atomen ist,
                // dann kann dieser Ast zu keinem gueltigen Ergebnis fuehren !
                if ((flat_alkane_branch_container [i2]->length + flat_alkane_branch_container [i3]->length) >
                        number_of_c_atoms)
                {
                    // Die inneren Schleifendurchlaeufe beachten, die durch den Schleifenneustart nicht ausgefuehrt
                    // werden
                    current_inner_loop_run += i3;
                    continue;
                }

                for (size_t i4 = 0; i4 <= i3; ++ i4) // <= !
                {
                    static uint_fast16_t local_run_counter = 0;
                    ++ current_inner_loop_run;

                    // printf ("%10" PRIuFAST64 " / %10" PRIuFAST64 "\r", current_inner_loop_run, max_inner_loop_runs);

                    // Auch hier das Gleiche: Aeste aussortieren, die zu gross sind
                    // Wenn die Groesse des zu konstruierenden Asts groesser als die uebergebene Anzahl an C-Atomen ist,
                    // dann kann dieser Ast zu keinem gueltigen Ergebnis fuehren !
                    if ((flat_alkane_branch_container [i2]->length + flat_alkane_branch_container [i3]->length +
                            flat_alkane_branch_container [i4]->length) > number_of_c_atoms)
                    {
                        continue;
                    }

                    ++ local_run_counter;

                    // Aus Effizienzgruenden soll nur jedes 1000. Mal eine Ausgabe stattfinden
                    // Einfache Konsolenausgaben sind langsame Operationen, sodass die Anzahl begrenzt werden sollte
                    if (local_run_counter == 1000)
                    {
                        // Prozentualen Fortschritt bestimmen und ausgeben
                        const float percent_done =
                                (float) ((float) current_inner_loop_run / ((float) max_inner_loop_runs / 100.0f));
                        printf ("Build Alkane_Branch container %zu ... (~ %5.2f %%)\r", next_container + 1,
                                (percent_done > 100.0f) ? 100.0f : percent_done);
                        fflush (stdout);
                        local_run_counter = 0;
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

        printf ("Alkane_Branch container %2zu / %2zu fully created. (%" PRIuFAST64 " braches were build)\n",
                next_container + 1, count_alkane_branch_container, container_height_x [next_container]->size);
        fflush (stdout);
    }
    puts ("");
    // ===== ===== ===== ===== ===== ENDE 1. Teil ===== ===== ===== ===== =====



    // Aufraeumen
    for (size_t next_container = 0; next_container < count_alkane_branch_container; ++ next_container)
    {
        Delete_Alkane_Branch_Container (container_height_x [next_container]);
        container_height_x [next_container] = NULL;
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------
