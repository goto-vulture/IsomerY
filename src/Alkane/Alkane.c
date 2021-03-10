/**
 * Alkane.c
 *
 *  Created on: 10.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane.h"
#include <stdlib.h>
#include <string.h>
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Error_Handling/Assert_Msg.h"





//---------------------------------------------------------------------------------------------------------------------

/**
 * Alkane Objekt dynamisch erzeugen.
 *
 * Alle Zeiger duerfen NULL sein, in diesem Fall wird davon ausgegangen, dass dieser Ast nur aus einem H-Atom besteht.
 * Damit die Adressen, die ungleich NULL sind, aufsteigend im Alkane-Objekt gespeichert werden, (dies vereinfacht die
 * weiteren Operationen) werden die uebergebenen Adressen im Objekt neu angeordnet, wenn die Alkane_Branch-Objekte
 * nicht "von links nach rechts" der Funktion uebergeben werden.
 *
 * Asserts:
 *      N/A
 */
struct Alkane*                          // Erzeugtes Alkane-Objekt
Create_Alkane
(
        struct Alkane_Branch* branch_1, // 1. Ast des neuen Alkane-Objektes
        struct Alkane_Branch* branch_2, // 2. Ast des neuen Alkane-Objektes
        struct Alkane_Branch* branch_3, // 3. Ast des neuen Alkane-Objektes
        struct Alkane_Branch* branch_4  // 4. Ast des neuen Alkane-Objektes
)
{
    // Neues Objekt erzeugen
    struct Alkane* new_alkane = (struct Alkane*) CALLOC (1, sizeof (struct Alkane));
    ASSERT_ALLOC(new_alkane, "Cannot allocate an Alkane object !", sizeof (struct Alkane));

    // Die uebergebenen Adressen linksbuendig in das Alkane-Objekt eintragen
    uint_fast8_t next_free_branch = 0;
    if (branch_1 != NULL) { new_alkane->branches [next_free_branch] = branch_1; next_free_branch ++; }
    if (branch_2 != NULL) { new_alkane->branches [next_free_branch] = branch_2; next_free_branch ++; }
    if (branch_3 != NULL) { new_alkane->branches [next_free_branch] = branch_3; next_free_branch ++; }
    if (branch_4 != NULL) { new_alkane->branches [next_free_branch] = branch_4; next_free_branch ++; }

    new_alkane->next_free_branch = next_free_branch;

    // Anzahl an C-Atomen im gesamten Alkan bestimmen
    unsigned int number_of_c_atoms = 0;
    for (size_t i = 0; i < (sizeof (new_alkane->branches) / sizeof (new_alkane->branches [0])); ++ i)
    {
        number_of_c_atoms += new_alkane->branches [i]->length;
    }
    new_alkane->number_of_c_atoms = (uint_fast8_t) number_of_c_atoms;


    /**
     * Jetzt kommt der spanendste Teil dieser Funktion:
     * Das Zusammenfuehren der Zahlencodes zu einen Zahlencode
     *
     * Ein Beispiel fuer das Zusammenfuehren der Zahlencodes:
     * Ast 1: (1)
     * Ast 2: (1, 1, 1)
     * Ast 3: (1, 1, 1, 1)
     * Ast 4: ()
     *
     * Ergebnis: (1, 1, 2, 2, 1, 5, 5, 5)
     */
    unsigned int c_atoms_wrote = 0;
    for (uint_fast8_t i = 0; i < new_alkane->next_free_branch; ++ i)
    {
        // Der Zahlencode des ersten Alkane_Branch kann ohne Aenderungen uebernommen werden
        if (i == 0)
        {
            memcpy (new_alkane->merged_numbercode, new_alkane->branches [0], new_alkane->branches [0]->length);
            c_atoms_wrote += new_alkane->branches [0]->length;
        }
        // Ansonsten muss der Zahlencode der Aeste angepasst werden, damit ein Zahlencode fuer das gesamte Molekuel
        // gebildet wird
        else
        {
            // Der Offset wird aus den aufsummierten Laengen, der bereits vollstaendig betrachteten Alkane_Branch
            // Objekte, gebildet
            unsigned int numbercode_offset = 0;
            for (uint_fast8_t i2 = 0; i2 < i; ++ i2)
            { numbercode_offset += new_alkane->branches [i2]->length; }

            for (uint_fast8_t i2 = 0; i2 < new_alkane->branches [i]->length; ++ i2)
            {
                // Jedes 1. Objekt eines Zahlencodes MUSS unveraendert uebernommen werden
                if (i2 == 0)
                {
                    new_alkane->merged_numbercode [c_atoms_wrote] = new_alkane->branches [i]->branch [0];
                }
                else
                {
                    new_alkane->merged_numbercode [c_atoms_wrote] =
                            (unsigned char) (new_alkane->branches [i]->branch [i2] + numbercode_offset);
                }

                ++ c_atoms_wrote;
            }
        }
    }


    // Den Zustand anpassen => Wurde das Objekt mit bereits vorhandenen Daten oder mit Nullbytes initialisiert ?
    new_alkane->state = (next_free_branch == 0) ? ALKANE_INITIALIZED_WITH_ZERO_BYTES : ALKANE_INITIALIZED_WITH_DATA;

    return new_alkane;
}

//---------------------------------------------------------------------------------------------------------------------
