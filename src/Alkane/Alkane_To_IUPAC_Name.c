/**
 * Alkane_To_IUPAC_Name.c
 *
 *  Created on: 14.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane_To_IUPAC_Name.h"
#include <string.h>
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"





/**
 * Daten, die fuer die Bestimmung des Pfades benoetigt und erzeugt werden.
 */
struct Path_Data
{
    unsigned char adj_matrix [MAX_NUMBER_OF_C_ATOMS][MAX_NUMBER_OF_C_ATOMS];    // Kopie der Alkan Adjazenzmatrix
    uint_fast8_t start_node;                                                    // Startknoten
    uint_fast8_t goal_node;                                                     // Zielknoten
    uint_fast8_t current_node;                                                  // Aktueller Knoten

    unsigned char result_path [MAX_NUMBER_OF_C_ATOMS];                          // Pfad, um vom Start- zum Zielknoten
                                                                                // zu kommen
    uint_fast8_t result_path_length;                                            // Laenge des Pfades
    uint_fast8_t path_index;                                                    // Das naechste zu verwendene Objekt im
                                                                                // result_path-Array
                                                                                // Diese Information wird nur fuer die
                                                                                // Erzeugung des Pfades benoetigt
    uint_fast8_t max_possible_nesting_depth;                                    // Maximal moegliche Verschachtelungs-
                                                                                // tiefe der Aeste im Molekuel, wenn
                                                                                // der Pfad als Hauptkette gewaehlt
                                                                                // wird
};

static struct Path_Data*
Find_Main_Chain
(
        const struct Alkane* const alkane
);

static uint_fast8_t
Do_DFS
(
        const struct Alkane* const alkane,
        struct Path_Data* const path_data
);

static uint_fast8_t
Select_Suitable_Chain
(
        const struct Alkane* const alkane,
        struct Path_Data* const path_data,
        const size_t count_path_data
);

/**
 * Die wichtigsten Infos ueber ein Path_Data Objekt ausgeben.
 */
static void
Print_Path_Data
(
        const struct Path_Data* const path_data // Path_Data Objekt, deren wichtigsten Infos ausgegeben werden
);

/**
 * Tiefensuche - und deren benoetigte Strukturen - initialisieren.
 */
static void Depth_First_Search_Start
(
        const uint_fast8_t start_node,      // Startknoten
        const uint_fast8_t goal_node,       // Zielknoten
        struct Path_Data* const path_data   // Daten, die fuer die Bestimmung des Pfades benoetigt und erzeugt werden
);

/**
 * Tiefensuche im Alkan-Graphen rekursiv durchfuehren.
 */
static void Depth_First_Search_Step
(
        struct Path_Data* const path_data   // Daten, die fuer die Bestimmung des Pfades benoetigt und erzeugt werden
);

/**
 * Anzahl an Bindungen eines C-Atoms zaehlen.
 */
static uint_fast8_t                                                             // Anzahl an Bindungen
Count_Connections
(
        const uint_fast8_t c_atom,                                              // C-Atom, deren Bindungen gezaehlt
                                                                                // werden sollen
        const uint_fast8_t number_of_c_atoms,                                   // Maximale Anzahl an C-Atomen in der
                                                                                // Adjazenzmatrix
        const unsigned char adj_matrix [MAX_NUMBER_OF_C_ATOMS][MAX_NUMBER_OF_C_ATOMS] // Adjazenzmatrix (Stellt die Bindungen
                                                                                // zwischen den C-Atomen dar)
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
        const unsigned char current_numercode_element = alkane->merged_numbercode [current_c_atom];

        // Adjazenzmatrix ist symmetrisch zur Diagonalen, da die Bindungen zwischen zwei C-Atomen ungerichtet sind
        alkane->structure [current_c_atom][current_numercode_element - 1] = 1;
        alkane->structure [current_numercode_element - 1][current_c_atom] = 1;
    }

    struct Path_Data* main_chain = Find_Main_Chain (alkane);
    // ===== ===== ===== ===== ===== ===== ===== ENDE Hauptkette bestimmen ===== ===== ===== ===== ===== ===== =====

    FREE_AND_SET_TO_NULL(main_chain);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static struct Path_Data*
Find_Main_Chain
(
        const struct Alkane* const alkane
)
{
    // Speicher fuer die Bestimmungen der Tiefensuche
    struct Path_Data path_data [MAX_NUMBER_OF_C_ATOMS];
    memset (path_data, '\0', sizeof (path_data));

    // Tiefensuche fuer alle moeglichen Pfade durchfuehren
    const uint_fast8_t count_created_paths = Do_DFS (alkane, path_data);

    // Den passenden Pfad aus den gerade erzeugten Pfaden auswaehlen
    const uint_fast8_t result_path_index = Select_Suitable_Chain (alkane, path_data, count_created_paths);

    // Den Pfad, der als Hauptkette ausgewaehlt wurde, dynamisch erzeugen, damit das Ergebnis zurueckgegeben werden
    // kann
    struct Path_Data* result_path_data = (struct Path_Data*) CALLOC (1, sizeof (struct Path_Data));
    ASSERT_ALLOC(result_path_data, "Cannot allocate memory for the main chain path !", sizeof (struct Path_Data));

    // Ergebnisobjekt kopieren
    memcpy (result_path_data, &(path_data [result_path_index]), sizeof (struct Path_Data));

    printf ("Result Index: %d\n", result_path_index);

    return result_path_data;
}

//---------------------------------------------------------------------------------------------------------------------

static uint_fast8_t
Do_DFS
(
        const struct Alkane* const alkane,
        struct Path_Data* const path_data
)
{
    // Alle Elemente des Graphens ermitteln, die genau eine Bindung haben. Dies sind die CH3-Gruppen, mit denen die
    // moeglichen Hauptketten starten und enden.
    uint_fast8_t ch3_elements [MAX_NUMBER_OF_C_ATOMS];
    uint_fast8_t next_free_ch3_element = 0;
    memset (ch3_elements, '\0', sizeof (ch3_elements));

    for (uint_fast8_t current_c_atom = 0; current_c_atom < alkane->number_of_c_atoms; ++ current_c_atom)
    {
        // Hat das aktuelle C-Atom genau eine Bindung bzw. wurde eine CH3-Gruppe gefunden ?
        if (Count_Connections (current_c_atom, alkane->number_of_c_atoms, alkane->structure) == 1)
        {
            ch3_elements [next_free_ch3_element] = current_c_atom;
            ++ next_free_ch3_element;
        }
    }

    // Damit die Bennenung der oberen Schleifengrenzen besser ist
    const uint_fast8_t count_ch3_elemets = next_free_ch3_element;
    uint_fast8_t next_free_path_data = 0;

    // Alle Kombinationen an CH3-Elementen ausprobieren, wobei die Reihenfolge der CH3-Elemente KEINE Rolle spielt !
    // Gleichzeitig muessen die Faelle, wo Start und Ziel gleich sind, nicht betrachtet werden
    for (uint_fast8_t current_ch3_element_start = 0; current_ch3_element_start < count_ch3_elemets;
            ++ current_ch3_element_start)
    {
        for (uint_fast8_t current_ch3_element_end = (uint_fast8_t) (current_ch3_element_start + 1);
                current_ch3_element_end < count_ch3_elemets;
                ++ current_ch3_element_end)
        {
            // Adjazenzmatrix des Alkans in das passende Path_Data-Objekt kopieren
            memcpy (path_data [next_free_path_data].adj_matrix, alkane->structure, sizeof (alkane->structure));

            // Tiefensuche durchfuehren, um bei der aktuellen Variante des Start- und Zielknotens den Pfad und deren
            // Laenge zu bestimmen
            Depth_First_Search_Start (ch3_elements [current_ch3_element_start], ch3_elements [current_ch3_element_end],
                    &(path_data [next_free_path_data]));
            ++ next_free_path_data;
        }
    }

    return next_free_path_data;
}

//---------------------------------------------------------------------------------------------------------------------

static uint_fast8_t
Select_Suitable_Chain
(
        const struct Alkane* const alkane,
        struct Path_Data* const path_data,
        const size_t count_path_data
)
{
    // Die maximale Laenge des laengsten Pfades aus der Menge der moeglichen Pfade auswaehlen
    // Es ist durchaus moeglich, dass mehrere Pfade die maximale Laenge besitzen
    uint_fast8_t main_chain_length = 0;
    for (size_t current_path_data_index = 0; current_path_data_index < count_path_data; ++ current_path_data_index)
    {
        if (path_data [current_path_data_index].result_path_length > main_chain_length)
        {
            main_chain_length = path_data [current_path_data_index].result_path_length;
        }
    }


    // Ast mit der geringsten Verschachtelungstiefe
    uint_fast8_t index_smallest_nesting_depth = 0;

    // Maximale Verschachtelungstiefe bei allen Pfaden ermitteln, wenn diese als Hauptkette gewaehlt werden wuerden
    for (uint_fast8_t current_path_data_index = 0; current_path_data_index < count_path_data; ++ current_path_data_index)
    {
        struct Path_Data* const current_path_data = &(path_data [current_path_data_index]);

        // Die Pfade, wo die Laenge exakt mit der Laenge der Hauptkette uebereinstimmt, sind die moeglichen Hauptketten
        if (current_path_data->result_path_length == main_chain_length)
        {
            // Originale Adjazenzmatrix wiederherstellen
            memcpy (current_path_data->adj_matrix, alkane->structure, sizeof (alkane->structure));

            // Alle Bindungen des Pfades entfernen, um die uebrigen Aeste betrachten zu koennen
            for (uint_fast8_t current_c_atom = 0; current_c_atom < current_path_data->result_path_length - 1;
                    ++ current_c_atom)
            {
                const unsigned char c_atom_1 = current_path_data->result_path [current_c_atom];
                const unsigned char c_atom_2 = current_path_data->result_path [current_c_atom + 1];

                // Beide Richtungen in der Adjazenzmatrix entfernen
                current_path_data->adj_matrix [c_atom_1][c_atom_2] = 0;
                current_path_data->adj_matrix [c_atom_2][c_atom_1] = 0;
            }

            // Verschachtelungstiefe ermitteln, die bei der Wahl des Pfades als Hauptkette entsteht => Anzahl an
            // C-Atomen mit mehr als 2 Bindungen zaehlen
            // Der Pfad, bei denen die Verschachtelungstielfe am geringsten ist, ist die Hauptkette
            uint_fast8_t possible_nesting_depth = 0;

            for (uint_fast8_t current_c_atom = 0; current_c_atom < alkane->number_of_c_atoms; ++ current_c_atom)
            {
                // Temporaer ein Zeiger mit beiden const-Qualifier anlegen, damit beim Zahelen der Verbindungen keine
                // Warnung wegen verschiedenen Qualifiern entsteht:
                // warning: pointers to arrays with different qualifiers are incompatible in ISO C [-Wpedantic]
                const struct Path_Data* const temp_const_ptr = current_path_data;

                // Hat das aktuelle C-Atom mehr als 2 Bindungen ?
                if (Count_Connections (current_c_atom, alkane->number_of_c_atoms, temp_const_ptr->adj_matrix) > 2)
                {
                    ++ possible_nesting_depth;
                }
            }

            current_path_data->max_possible_nesting_depth = possible_nesting_depth;

            // Ast mit der geringsten Verschachtelungstiefe ermitteln
            // Hat der aktuelle Ast eine geringere Verschachtelungstiefe ?
            if (path_data [current_path_data_index].max_possible_nesting_depth <
                    path_data [index_smallest_nesting_depth].max_possible_nesting_depth)
            {
                index_smallest_nesting_depth = current_path_data_index;
            }

            printf ("Max nesting depth %d (index: %d)\n", possible_nesting_depth, current_path_data_index);
        }
    }

    return index_smallest_nesting_depth;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Die wichtigsten Infos ueber ein Path_Data Objekt ausgeben.
 */
static void
Print_Path_Data
(
        const struct Path_Data* const path_data // Path_Data Objekt, deren wichtigsten Infos ausgegeben werden
)
{
    printf ("Start: %2" PRIuFAST8 "; End: %2" PRIuFAST8 "; Length: %2" PRIuFAST8 "\n", path_data->start_node,
            path_data->goal_node, path_data->result_path_length);
    printf ("Path: (");
    for (uint_fast8_t i = 0; i < path_data->result_path_length; ++ i)
    {
        printf ("%2" PRIuFAST8, path_data->result_path [i]);

        if ((i + 1) < path_data->result_path_length)
        {
            printf (", ");
        }
    }
    puts (")");
    fflush (stdout);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Tiefensuche - und deren benoetigte Strukturen - initialisieren.
 */
static void Depth_First_Search_Start
(
        const uint_fast8_t start_node,      // Startknoten
        const uint_fast8_t goal_node,       // Zielknoten
        struct Path_Data* const path_data   // Daten, die fuer die Bestimmung des Pfades benoetigt und erzeugt werden
)
{
    path_data->start_node   = start_node;
    path_data->goal_node    = goal_node;
    // Zu Beginn den Startknoten betrachten !
    path_data->current_node = start_node;

    // Tiefensuche beginnen
    Depth_First_Search_Step (path_data);

    // Ausgabe der wichtigsten Infos ueber den gefundenen Pfad
    Print_Path_Data (path_data);

    return;
}


//---------------------------------------------------------------------------------------------------------------------

/**
 * Tiefensuche im Alkan-Graphen rekursiv durchfuehren.
 */
static void Depth_First_Search_Step
(
        struct Path_Data* const path_data   // Daten, die fuer die Bestimmung des Pfades benoetigt und erzeugt werden
)
{
    // Wenn das Ziel noch nicht erreicht wurde, dann koennte der aktuelle Knoten Teil des Pfades sein
    // Und wenn das Ziel noch nicht erreicht wurde, dann wird der Index inkrementiert
    if (path_data->path_index != UINT_FAST8_MAX)
    {
        path_data->result_path [path_data->path_index]  = (unsigned char) path_data->current_node;
        path_data->path_index                           = (uint_fast8_t) (path_data->path_index + 1);
    }

    // Ziel gefunden ? (REKURSIONSABBRUCH)
    if (path_data->current_node == path_data->goal_node)
    {
        // Ziel wurde erreicht -> Pfadlaenge abspeichern
        path_data->result_path_length = path_data->path_index;

        // Ueber einen Wert im Index anzeigen, dass das Ziel gefunden wurde => Index darf nicht mehr verwendet werden
        // Und der Ergebnis-Pfad wird nicht mehr angefasst
        path_data->path_index = UINT_FAST8_MAX;

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
            // Welche Verbindungen hat der aktuelle Knoten ?
            if (path_data->adj_matrix [path_data->current_node][i] == 1)
            {
                stack [next_free_stack_element] = i;
                ++ next_free_stack_element;

                // Aktuellen Wert sowie den Diagonalwert auf 0 setzen, damit Pfade nicht mehrfach betrachtet werden
                path_data->adj_matrix [path_data->current_node][i]  = 0;
                path_data->adj_matrix [i][path_data->current_node]  = 0;
            }
        }

        // Bei allen Elementen im Stack die Tiefensuche rekursiv ausfuehren
        while (next_free_stack_element > 0)
        {
            // Aktuelles Stackobjekt
            const unsigned char new_node = stack [next_free_stack_element - 1];
            -- next_free_stack_element;
            path_data->current_node = new_node;

            // In die Tiefe gehen (REKURSION)
            Depth_First_Search_Step (path_data);
        }
    }

    // Index nur dekrementieren, wenn das Ziel noch nicht erreicht wurde
    if (path_data->path_index != UINT_FAST8_MAX)
    {
        path_data->path_index = (uint_fast8_t) (path_data->path_index - 1);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Anzahl an Bindungen eines C-Atoms zaehlen.
 */
static uint_fast8_t                                                             // Anzahl an Bindungen
Count_Connections
(
        const uint_fast8_t c_atom,                                              // C-Atom, deren Bindungen gezaehlt
                                                                                // werden sollen
        const uint_fast8_t number_of_c_atoms,                                   // Maximale Anzahl an C-Atomen in der
                                                                                // Adjazenzmatrix
        const unsigned char adj_matrix [MAX_NUMBER_OF_C_ATOMS][MAX_NUMBER_OF_C_ATOMS] // Adjazenzmatrix (Stellt die Bindungen
                                                                                // zwischen den C-Atomen dar)
)
{
    uint_fast8_t connections_found = 0;

    // Bindungen des aktuellen C-Atoms zaehlen
    for (uint_fast8_t current_possible_connection = 0; current_possible_connection < number_of_c_atoms;
            ++ current_possible_connection)
    {
        if (adj_matrix [c_atom][current_possible_connection] == 1)
        {
            ++ connections_found;
        }
    }

    return connections_found;
}

//---------------------------------------------------------------------------------------------------------------------
