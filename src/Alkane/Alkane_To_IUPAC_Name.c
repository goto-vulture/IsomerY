/**
 * Alkane_To_IUPAC_Name.c
 *
 *  Created on: 14.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane_To_IUPAC_Name.h"
#include <string.h>
#include "../Error_Handling/Assert_Msg.h"





/**
 * Tiefensuche - und deren benoetigte Strukturen - initialisieren.
 */
static void Depth_First_Search_Start
(
        const uint_fast8_t start_node,                                          // Startknoten
        const uint_fast8_t goal_node,                                           // Zielknoten
        const struct Alkane* const alkane,                                      // Alkanobjket, deren Struktur
                                                                                // betrachtet wird
        unsigned char adj_matrix [MAX_NUMBER_OF_C_ATOMS][MAX_NUMBER_OF_C_ATOMS] // Kopie der Alkan Adjazenzmatrix
                                                                                // In der Adjazenzmatrix werden alle
                                                                                // Verbindungen entfernt, als waere der
                                                                                // Pfad, der sich aus dem Start- und
                                                                                // Zielknoten zusammensetzt, die
                                                                                // Hauptkette
);

/**
 * Tiefensuche im Alkan-Graphen rekursiv durchfuehren.
 */
static void Depth_First_Search_Step
(
        const uint_fast8_t current_node,                                            // Startknoten
        const uint_fast8_t goal_node,                                               // Endknoten
        unsigned char adj_matrix [MAX_NUMBER_OF_C_ATOMS][MAX_NUMBER_OF_C_ATOMS],    // Kopie der Alkan Adjazenzmatrix
        uint_fast8_t* const result_path_length                                      // Laenge des Pfades vom Start- zum
                                                                                    // Zielknoten
);

//---------------------------------------------------------------------------------------------------------------------

/**
 * IUPAC-Name des uebergebenen Alkans erzeugen.
 *
 * ...
 *
 * Asserts:
 *      alkane != NULL
 *      main_chain_length > 0
 */
void Convert_Alkane_To_IUPAC_Name
(
        struct Alkane* const alkane,            // Alkan, von dem der IUPAC-Name gebildet werden soll
        const uint_fast8_t main_chain_length    // Laenge der Hauptkette
)
{
    ASSERT_MSG(alkane != NULL, "alkane is NULL !");
    ASSERT_MSG(main_chain_length > 0, "main_chain_length is 0 !");

    // ===== ===== ===== BEGINN Zusammengesetzten Zahlencode des uebergebenen Alkans ueberpruefen ===== ===== =====
    uint_fast8_t count_number_occur [MAX_NUMBER_OF_C_ATOMS];
    memset (count_number_occur, '\0', sizeof (count_number_occur));

    for (uint_fast8_t i = 0; i < alkane->number_of_c_atoms; ++ i)
    {
        count_number_occur [alkane->merged_numbercode [i] - 1] ++;
    }
    for (uint_fast8_t i = 0; i < alkane->number_of_c_atoms; ++ i)
    {
        if (i == 0)
        {
            ASSERT_FMSG(count_number_occur [0] <= 5, "Invalid merged numbercode ! The number %" PRIdFAST8 " occurs %"
                    PRIdFAST8 " times ! Max. valid: 5 !", (uint_fast8_t) (i + 1), count_number_occur [0]);
        }
        else
        {
            ASSERT_FMSG(count_number_occur [i] <= 4, "Invalid merged numbercode ! The number %" PRIdFAST8 " occurs %"
                    PRIdFAST8 " times ! Max. valid: 4 !", (uint_fast8_t) (i + 1), count_number_occur [i]);
        }
    }
    // ===== ===== ===== ENDE Zusammengesetzten Zahlencode des uebergebenen Alkans ueberpruefen ===== ===== =====

    // ===== ===== ===== ===== ===== ===== ===== BEGINN Hauptkette bestimmen ===== ===== ===== ===== ===== ===== =====
    // Graphen erstellen -> Adjazenzmatrix mit Werten fuellen
    // current_c_atom = 1: Das erste Element ueberspringen, da dies die Ausnahmen-1 ist und keine verbunden-mit-Aussage
    //                     ausdrueckt !
    for (uint_fast8_t current_c_atom = 1; current_c_atom < alkane->number_of_c_atoms; ++ current_c_atom)
    {
        const unsigned char current_numercode_element   = alkane->merged_numbercode [current_c_atom];

        // Adjazenzmatrix ist symmetrisch dir Diagonalen, da die Bindungen zwischen zwei C-Atomen ungerichtet ist
        alkane->structure [current_c_atom][current_numercode_element - 1] = 1;
        alkane->structure [current_numercode_element - 1][current_c_atom] = 1;
    }

    // Alle Elemente des Graphens ermitteln, die genau eine Bindung haben. Dies sind die CH3-Gruppen, mit denen die
    // moeglichen Hauptketten starten und enden.
    uint_fast8_t ch3_elements [MAX_NUMBER_OF_C_ATOMS];
    uint_fast8_t next_free_ch3_element = 0;
    memset (ch3_elements, '\0', sizeof (ch3_elements));

    for (uint_fast8_t current_c_atom = 0; current_c_atom < alkane->number_of_c_atoms; ++ current_c_atom)
    {
        uint_fast8_t connections_found = 0;

        for (uint_fast8_t current_possible_connection = 0; current_possible_connection < alkane->number_of_c_atoms;
                ++ current_possible_connection)
        {
            if (alkane->structure [current_c_atom][current_possible_connection] == 1)
            {
                ++ connections_found;
            }
        }

        // Wurde eine CH3-Gruppe gefunden ?
        if (connections_found == 1)
        {
            ch3_elements [next_free_ch3_element] = current_c_atom;
            ++ next_free_ch3_element;
        }
    }

    // Tiefensuche durchfuehren
    // Das erste CH3-Element ist immer der Start; die restlichen CH3-Elemente werden jeweils als Ziel verwendet

    // Speicher fuer die Adjazenzmatrizen, die entstehen, wenn bei jeder moeglichen Hauptkette die Verbindungen der
    // Hauptkette entfernt werden.
    // In den Adjazenzmatrizen bleiben also nur noch die Aeste uebrig. Mit diesen Infos laesst sich schnell die
    // Verschachtelungstiefe bestimmen, wenn die jeweilige Kette als Hauptkette gewaehlt werden wuerde.
    unsigned char adj_matrices [MAX_NUMBER_OF_C_ATOMS][MAX_NUMBER_OF_C_ATOMS][MAX_NUMBER_OF_C_ATOMS];
    memset (adj_matrices, '\0', sizeof (adj_matrices));
    // Damit die Bennenung der oberen Schleifengrenzen besser ist
    const uint_fast8_t count_ch3_elemets = next_free_ch3_element;
    uint_fast8_t next_free_adj_matrix = 0;

    // Alle Kombinationen an CH3-Elementen ausprobieren, wobei die Reihenfolge der CH3-Elemente KEINE Rolle spielt !
    // Gleichzeitig muessen die Faelle, wo Start und Ziel gleich sind, nicht betrachtet werden
    for (uint_fast8_t current_ch3_element_start = 0; current_ch3_element_start < count_ch3_elemets;
            ++ current_ch3_element_start)
    {
        for (uint_fast8_t current_ch3_element_end = (uint_fast8_t) (current_ch3_element_start + 1);
                current_ch3_element_end < count_ch3_elemets;
                ++ current_ch3_element_end)
        {
            Depth_First_Search_Start (ch3_elements [current_ch3_element_start], ch3_elements [current_ch3_element_end], alkane,
                    adj_matrices [next_free_adj_matrix]);
            ++ next_free_adj_matrix;
        }
    }
    // ===== ===== ===== ===== ===== ===== ===== ENDE Hauptkette bestimmen ===== ===== ===== ===== ===== ===== =====

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Tiefensuche - und deren benoetigte Strukturen - initialisieren.
 */
static void Depth_First_Search_Start
(
        const uint_fast8_t start_node,                                          // Startknoten
        const uint_fast8_t goal_node,                                           // Zielknoten
        const struct Alkane* const alkane,                                      // Alkanobjket, deren Struktur
                                                                                // betrachtet wird
        unsigned char adj_matrix [MAX_NUMBER_OF_C_ATOMS][MAX_NUMBER_OF_C_ATOMS] // Kopie der Alkan Adjazenzmatrix
                                                                                // In der Adjazenzmatrix werden alle
                                                                                // Verbindungen entfernt, als waere der
                                                                                // Pfad, der sich aus dem Start- und
                                                                                // Zielknoten zusammensetzt, die
                                                                                // Hauptkette
)
{
    // Adjazenzmatrix des Alkans in die temporaere Adjazenzmatrix kopieren
    memcpy (adj_matrix, alkane->structure, sizeof (alkane->structure));

    uint_fast8_t path_length = 0;
    Depth_First_Search_Step (start_node, goal_node, adj_matrix, &path_length);

    printf ("Start: %2d; End: %2d; Length: %2d\n", start_node, goal_node, path_length);

    return;
}


//---------------------------------------------------------------------------------------------------------------------

/**
 * Tiefensuche im Alkan-Graphen rekursiv durchfuehren.
 */
static void Depth_First_Search_Step
(
        const uint_fast8_t current_node,                                            // Startknoten
        const uint_fast8_t goal_node,                                               // Endknoten
        unsigned char adj_matrix [MAX_NUMBER_OF_C_ATOMS][MAX_NUMBER_OF_C_ATOMS],    // Kopie der Alkan Adjazenzmatrix
        uint_fast8_t* const result_path_length                                      // Laenge des Pfades vom Start- zum
                                                                                    // Zielknoten
)
{
    // Die aktuelle Aufruftiefe, die der Laenge des Pfades entspricht, aufrufuebergreifend sichern
    static uint_fast8_t path_length = 0;
    path_length ++;

    // Ziel gefunden ?
    if (current_node == goal_node)
    {
        *result_path_length = path_length;

        // Aufrufuebergreifenden Wert zuruecksetzen, damit diese Zaehlmethode auch fuer weitere Aufrufe funktioniert
        path_length = 0;
        // printf ("Result path length: %d\n", *result_path_length);

        return;
    }
    else
    {
        unsigned char stack [MAX_NUMBER_OF_C_ATOMS];
        uint_fast8_t next_free_stack_element = 0;
        memset (stack, '\0', sizeof (stack));

        // Aktuellen Knoten expandieren -> Mit welchen Knoten ist der aktuelle Knoten verbunden ?
        for (uint_fast8_t i = 0; i < MAX_NUMBER_OF_C_ATOMS; ++ i)
        {
            if (adj_matrix [current_node][i] == 1)
            {
                stack [next_free_stack_element] = i;
                ++ next_free_stack_element;

                // Aktuellen Wert sowie den Diagonalwert auf 0 setzen, damit Pfade nicht mehrfach betrachtet werden
                adj_matrix [current_node][i] = 0;
                adj_matrix [i][current_node] = 0;
            }
        }

        // Bei allen Elementen im Stack die Tiefensuche rekursiv ausfuehren
        while (next_free_stack_element > 0)
        {
            const unsigned char new_node = stack [next_free_stack_element - 1];
            -- next_free_stack_element;

            // Rekursiv in die Tiefe gehen
            Depth_First_Search_Step (new_node, goal_node, adj_matrix, result_path_length);
        }
    }

    // Wert nur dekrementieren, wenn der Wert nicht zuvor zurueckgesetzt wurde. Sonst kommt es zu falschen Ergebnissen !
    if (path_length != 0)
    {
        path_length --;
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------
