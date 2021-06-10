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
#include "../Print_Tools.h"
#include "../Misc.h"
#include "Alkane_Chain_To_IUPAC_Name.h"





/**
 * Daten, die fuer die Bestimmung des Pfades sowie fuer das Abspeichern des Pfades benoetigt werden. Aus diesen Infos
 * kann die passende Hauptkette bestimmt werden (indem mehrere Path_Data-Objekte miteinander verglichen werden).
 */
struct Path_Data
{
    unsigned char adj_matrix                            // Kopie der Alkan Adjazenzmatrix
    [MAX_NUMBER_OF_C_ATOMS][MAX_NUMBER_OF_C_ATOMS];
    uint_fast8_t start_node;                            // Startknoten
    uint_fast8_t goal_node;                             // Zielknoten
    uint_fast8_t current_node;                          // Aktueller Knoten (wird nur waehrend der Tiefensuche
                                                        // benoetigt)

    unsigned char result_path [MAX_NUMBER_OF_C_ATOMS];  // Pfad, um vom Start- zum Zielknoten zu kommen
    uint_fast8_t result_path_length;                    // Laenge des Pfades
    uint_fast8_t path_index;                            // Das naechste zu verwendene Objekt im result_path-Array.
                                                        // Diese Information wird nur fuer die Erzeugung des Pfades
                                                        // benoetigt
    uint_fast8_t max_possible_nesting_depth;            // Maximal moegliche Verschachtelungstiefe der Aeste im
                                                        // Molekuel, wenn der Pfad als Hauptkette gewaehlt wird
};



static struct Path_Data*                            // Zeiger auf das dynamisch erzeugte Ergebnisobjekt
Find_Main_Chain
(
        const struct Alkane* const restrict alkane, // Alkan, bei dem die Hauptkette bestimmt werden soll
        const uint_fast8_t start_c_atom             // OPTIONAL: Angabe eines Start C-Atoms, von dem die laengste
                                                    // Kette aus gebildet wird. Dies ist bei der Bestimmung einiger
                                                    // Verschachtelungen notwendig
                                                    // Bei Nichtverwendung: UINT_FAST8_MAX
);

/**
 * Tiefensuchen bei allen moeglichen Pfaden, die die Hauptkette darstellen koennten, durchfuehren.
 */
static uint_fast8_t                                 // Anzahl an durchgefuehrten Tiefensuchen und dadurch auch Anzahl
                                                    // an verwendeten Path_Data-Objekten
Do_DFS
(
        const struct Alkane* const restrict alkane, // Alkan-Objekt, welches betrachtet werden soll
        struct Path_Data* const restrict path_data, // Zeiger auf die bereits angelegten Path_Data-Objekte
        const uint_fast8_t start_c_atom             // OPTIONAL: Angabe eines Start C-Atoms, von dem die laengste
                                                    // Kette aus gebildet wird. Dies ist bei der Bestimmung einiger
                                                    // Verschachtelungen notwendig
                                                    // Bei Nichtverwendung: UINT_FAST8_MAX
);

/**
 * Hauptkette aus den uebergebenen Path_Data-Objekten auswaehlen.
 *
 * Die Hauptkette ist der laengste Pfad. Wenn es mehrere Pfade mit der maximalen Laenge gibt (was die Regel ist), dann
 * wird der Pfad ausgewaehlt, bei denen die uebrigen Aeste die GERINGSTE Verschachtelungstiefe besitzen. Dies dient
 * dazu, dass der IUPAC-Name moeglichst kurz und einfach wird.
 */
static uint_fast8_t                                 // Index des Path_Data-Objektes, welches die Hauptkette darstellt
Select_Suitable_Chain
(
        const struct Alkane* const restrict alkane, // Alkan-Objekt, welches betrachtet werden soll
        struct Path_Data* const restrict path_data, // Path_Data-Objekte, aus denen die Hauptkette ausgewaehlt werden
                                                    // soll
        const size_t count_path_data                // Anzahl an Path_Data-Objekte
);

/**
 * Die wichtigsten Infos ueber ein Path_Data Objekt ausgeben.
 */
static void
Print_Path_Data
(
        const struct Path_Data* const restrict path_data    // Path_Data Objekt, deren wichtigsten Infos ausgegeben
                                                            // werden
);

/**
 * Tiefensuche - und deren benoetigte Strukturen - initialisieren.
 */
static void
Depth_First_Search_Start
(
        const uint_fast8_t start_node,              // Startknoten
        const uint_fast8_t goal_node,               // Zielknoten
        struct Path_Data* const restrict path_data  // Daten, die fuer die Bestimmung des Pfades benoetigt und erzeugt
                                                    // werden
);

/**
 * Tiefensuche im Alkan-Graphen rekursiv durchfuehren.
 */
static void
Depth_First_Search_Step
(
        struct Path_Data* const restrict path_data  // Daten, die fuer die Bestimmung des Pfades benoetigt und erzeugt
                                                    // werden
);

/**
 * Anzahl an Bindungen eines C-Atoms in einer Adjazenzmatrix zaehlen.
 */
static uint_fast8_t                                     // Anzahl an Bindungen
Count_Connections
(
        const uint_fast8_t c_atom,                      // C-Atom, deren Bindungen gezaehlt werden sollen
        const uint_fast8_t number_of_c_atoms,           // Anzahl an C-Atomen, die in der Adjazenzmatrix tatsaechlich
                                                        // hinterlegt sind (Diese Anzahl dient dazu, dass das Zaehlen
                                                        // schneller funktioniert)
        const unsigned char adj_matrix                  // Adjazenzmatrix (Stellt die Bindungen zwischen den C-Atomen
        [MAX_NUMBER_OF_C_ATOMS][MAX_NUMBER_OF_C_ATOMS]  // dar)
);

/**
 * Alle Aeste eines Alkanobjektes ermitteln und mit zusaetzlichen Informationen abspeichern. Diese zusaetzlichen
 * Informationen werden fuer die Erstellung des IUPAC-Namens benoetigt.
 */
static void
Chains_Go_Deeper
(
        struct Alkane* const restrict alkane,       // Alkane-Objekt, dessen Aeste durchsucht werden
        struct Path_Data* const restrict path_data, // Path_Data-Objekt, des letzten Astes
        const uint_fast8_t nesting_depth            // Aktuelle Verschachtelungstiefe
);

/**
 * Suche nach dem Inhalt eines Astes.
 * Der Inhalt des Astes wird mithilfe einer Adjazenzmatrix im uebergebenen Alkane-Objekt gesichert. Das Path_Data
 * Objekt dient als Quelle der Informationen. Die gefundenen Verbindungen werden im Path_Data-Objekt geloescht, damit
 * eine Verbindung nicht mehrfach gezaehlt wird.
 */
static void
Search_For_Chain_Content
(
        struct Path_Data* const restrict path_data, // Quellobjekt, welches durchsucht wird
        struct Alkane* const restrict temp_alkane,  // Temporaeres Alkan-Objekt, welches die gefundenen Verbindungen in
                                                    // einer Adjazenzmatrix speichert
        const uint_fast8_t next_c_atom_index,       // Index des C-Atoms, welches als naechstes betrachtet wird
        const uint_fast8_t number_of_c_atoms        // Anzahl an C-Atomen im Alkan
);

/**
 * Vergleichsfunktion fuer die qsort-Funktion.
 *
 * Bei der Sortierung von Chain-Objekten gibt es zwei Kriterien: die alphabetische Sortierung des Alkylnamen und die
 * Position des Alkylrests.
 * Die alphabetische Sortierung hat immer eine hoehere Prioritaet ! Nur wenn die Alkylnamen gleich sind, wird nach der
 * Position aufsteigend sortiert.
 */
static inline int Compare_Chain_Information (const void* a, const void* b);

static int Compare_Chains (struct Chain chain_a, struct Chain chain_b);

/**
 * Aeste, die auf gleicher Verschachtelungstiefe liegen, anhand der Laenge der Aeste aufsteigend sortieren.
 * Dies vereinfacht die spaetere Erzeugung des IUPAC-Namen bei der Zusammenfassung gleicher Aeste, da der Algorithmus
 * davon ausgehen darf, dass gleiche Aeste - falls vorhanden - immer zusammen im Alkane-Objekt hinterlegt sind.
 *
 * Kleines Beispiel:
 * Drei Aeste auf gleicher Verschachtelungstiefe. 2x Methyl, 1x Ethyl.
 * Bei der Anordnung 'Methyl, Ethyl, Methyl' muessen bei der Zusammenfassung immer alle Aeste der gleichen
 * Verschachtelungstiefe betrachtet werden, da sonst die Zusammenfassung der beiden Methyl-Aeste nicht stattfindet.
 * Wenn die Aeste umgeordnet werden ('Methyl, Methyl, Ethyl'), dann muss bei der Zusammenfassung nur das folgende
 * Objekt betrachtet werden. Ist dies gleich, dann wird zusammengefasst; wenn nicht, dann koennen keine weiteren Aeste
 * existieren, die zusammengefasst werden muessen.
 */
static void
Reorder_Chains
(
        struct Alkane* const restrict alkane    // Alkane-Objekt, bei denen die Chain-Objekte neu sortiert werden,
                                                // falls notwendig
);

//=====================================================================================================================

/**
 * IUPAC-Name des uebergebenen Alkans erzeugen.
 *
 * ...
 *
 * Asserts:
 *      alkane != NULL
 */
void
Convert_Alkane_To_IUPAC_Name
(
        struct Alkane* const restrict alkane    // Alkan, von dem der IUPAC-Name gebildet werden soll
)
{
    ASSERT_MSG(alkane != NULL, "alkane is NULL !");

    // Unused Warnung bei dieser statischen Funktion unterbinden. Diese Funktion wird nur fuer Debugzwecke verwendet
    // und ist daher i.d.R. auskommentiert !
    (void) Print_Path_Data;

    // ===== ===== ===== BEGINN Zusammengesetzten Zahlencode des uebergebenen Alkans ueberpruefen ===== ===== =====
    uint_fast8_t count_number_occur [MAX_NUMBER_OF_C_ATOMS];
    memset (count_number_occur, '\0', sizeof (count_number_occur));

    for (uint_fast8_t i = 0; i < alkane->number_of_c_atoms; ++ i)
    {
        count_number_occur [alkane->merged_numbercode [i] - 1] ++;
    }
    for (uint_fast8_t i = 0; i < alkane->number_of_c_atoms; ++ i)
    {
        // Die Null (bzw. bei 1-Indexierung die Eins) ist die einzige Zahl, die maximal 5 Mal vorkommen darf !
        // Dies liegt am Rekursiven Aufbau des Zahlencodes, wobei die Zahlen auf bereits angegebene C-Atome mittels
        // "verbunden mit"-Aussagen verweisen. Dies funktioniert natuerlich nicht, wenn das 1. C-Atom angegeben wird,
        // da dann noch keine anderen C-Atome existieren, womit das C-Atom verbunden sein kann. Dafuer existiert die
        // sogenannte Ausnahmen-Eins, die das 1. C-Atom darstellt. Im Zahlencode ist dies einfach der kleinste gueltige
        // Index -> Also eine Null (bzw. eine Eins bei einer 1-Indexierung)
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
    static volatile size_t result_number = 0;
    ++ result_number;

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

    struct Path_Data* main_chain = Find_Main_Chain (alkane, UINT_FAST8_MAX);

    // Aus der Hauptkette ergibt sich das erste Chain-Objekt
    alkane->chains [alkane->next_free_chain].length         = main_chain->result_path_length;
    alkane->chains [alkane->next_free_chain].nesting_depth  = 0;                // Die Verschachtelungstiefe bei der
                                                                                // Hauptkette ist 0 !
    alkane->chains [alkane->next_free_chain].position       = UINT_FAST8_MAX;   // Die Hauptkette hat keine Position
                                                                                // Platzhalterwert verwenden
    alkane->next_free_chain ++;
    // ===== ===== ===== ===== ===== ===== ===== ENDE Hauptkette bestimmen ===== ===== ===== ===== ===== ===== =====

    // ===== ===== ===== ===== ===== BEGINN Aeste und deren Positionen bestimmen ===== ===== ===== ===== =====
    if (result_number == 42)
    {
        (void) result_number;
    }

    // Durch die Festlegung der Hauptkette koennen - und werden in den meisten Faellen - Aeste gebildet werden, die bei
    // der Benennung beruecksichtigt werden muessen
    Chains_Go_Deeper (alkane, main_chain, 1);

    // Aeste, die auf einer Verschachtelungsebene liegen, anhand der Laenge der Aeste aufsteigend sortieren
    // Dies ist fuer eine einfachere Erzeugung des IUPAC-Namen notwendig
    Reorder_Chains (alkane);
    // ===== ===== ===== ===== ===== ENDE Aeste und deren Positionen bestimmen ===== ===== ===== ===== =====

    // ===== ===== ===== ===== ===== BEGINN Aus den Astinformationen den Namen bestimmen ===== ===== ===== ===== =====
    // Nun haben wir alle Informationen beisammen, die wir brauchen :D
    char iupac_name [IUPAC_NAME_LENGTH];
    memset (iupac_name, '\0', sizeof (iupac_name));
    const size_t iupac_name_length = COUNT_ARRAY_ELEMENTS(iupac_name) - 1;

    // Gerade erzeugte Ketteninformationen benutzen, um daraus den IUPAC-Namen zu bilden
    Chain_To_IUPAC (iupac_name, iupac_name_length, alkane);

    // Erzeugten IUPAC-Name im Alkan-Objekt hinterlegen (tiefe Kopie)
    memcpy (alkane->iupac_name, iupac_name, sizeof (iupac_name));

    PRINTF_FFLUSH("=> IUPAC name: %60s <= (length: %2zu | %6zu)\n", iupac_name, strlen (iupac_name), result_number);
    // ===== ===== ===== ===== ===== ENDE Aus den Astinformationen den Namen bestimmen ===== ===== ===== ===== =====

    FREE_AND_SET_TO_NULL(main_chain);

    return;
}

//=====================================================================================================================

/**
 * Hauptkette im Alkanobjekt suchen. Das Ergebnis wird in der Funktion dynamisch erzeugt, sodass der Aufrufer das
 * Objekt am Ende wieder freigeben muss !
 */
static struct Path_Data*                            // Zeiger auf das dynamisch erzeugte Ergebnisobjekt
Find_Main_Chain
(
        const struct Alkane* const restrict alkane, // Alkan, bei dem die Hauptkette bestimmt werden soll
        const uint_fast8_t start_c_atom             // OPTIONAL: Angabe eines Start C-Atoms, von dem die laengste
                                                    // Kette aus gebildet wird. Dies ist bei der Bestimmung einiger
                                                    // Verschachtelungen notwendig
                                                    // Bei Nichtverwendung: UINT_FAST8_MAX
)
{
    // Speicher fuer die Bestimmungen der Tiefensuche
    struct Path_Data path_data [MAX_NUMBER_OF_C_ATOMS];
    memset (path_data, '\0', sizeof (path_data));

    // Tiefensuche fuer alle moeglichen Pfade durchfuehren
    const uint_fast8_t count_created_paths = Do_DFS (alkane, path_data, start_c_atom);

    // Den passenden Pfad aus den gerade erzeugten Pfaden auswaehlen
    const uint_fast8_t result_path_index = Select_Suitable_Chain (alkane, path_data, count_created_paths);

    // Den Pfad, der als Hauptkette ausgewaehlt wurde, dynamisch erzeugen, damit das Ergebnis zurueckgegeben werden
    // kann
    struct Path_Data* result_path_data = (struct Path_Data*) CALLOC (1, sizeof (struct Path_Data));
    ASSERT_ALLOC(result_path_data, "Cannot allocate memory for the main chain path !", sizeof (struct Path_Data));

    // Ergebnisobjekt kopieren
    memcpy (result_path_data, &(path_data [result_path_index]), sizeof (struct Path_Data));

    // printf ("Result Index: %d\n", result_path_index);

    return result_path_data;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Tiefensuchen bei allen moeglichen Pfaden, die die Hauptkette darstellen koennten, durchfuehren.
 */
static uint_fast8_t                                 // Anzahl an durchgefuehrten Tiefensuchen und dadurch auch Anzahl
                                                    // an verwendeten Path_Data-Objekten
Do_DFS
(
        const struct Alkane* const restrict alkane, // Alkan-Objekt, welches betrachtet werden soll
        struct Path_Data* const restrict path_data, // Zeiger auf die bereits angelegten Path_Data-Objekte
        const uint_fast8_t start_c_atom             // OPTIONAL: Angabe eines Start C-Atoms, von dem die laengste
                                                    // Kette aus gebildet wird. Dies ist bei der Bestimmung einiger
                                                    // Verschachtelungen notwendig
                                                    // Bei Nichtverwendung: UINT_FAST8_MAX
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
            // Verwendung eines manuellen Startknoten ?
            if (start_c_atom != UINT_FAST8_MAX)
            {
                Depth_First_Search_Start (start_c_atom, ch3_elements [current_ch3_element_end],
                                    &(path_data [next_free_path_data]));
            }
            else
            {
                Depth_First_Search_Start (ch3_elements [current_ch3_element_start], ch3_elements [current_ch3_element_end],
                        &(path_data [next_free_path_data]));
            }
            ++ next_free_path_data;
        }
    }

    return next_free_path_data;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Hauptkette aus den uebergebenen Path_Data-Objekten auswaehlen.
 *
 * Die Hauptkette ist der laengste Pfad. Wenn es mehrere Pfade mit der maximalen Laenge gibt (was die Regel ist), dann
 * wird der Pfad ausgewaehlt, bei denen die uebrigen Aeste die GERINGSTE Verschachtelungstiefe besitzen. Dies dient
 * dazu, dass der IUPAC-Name moeglichst kurz und einfach wird.
 */
static uint_fast8_t                                 // Index des Path_Data-Objektes, welches die Hauptkette darstellt
Select_Suitable_Chain
(
        const struct Alkane* const restrict alkane, // Alkan-Objekt, welches betrachtet werden soll
        struct Path_Data* const restrict path_data, // Path_Data-Objekte, aus denen die Hauptkette ausgewaehlt werden
                                                    // soll
        const size_t count_path_data                // Anzahl an Path_Data-Objekte
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

    // PRINTF_FFLUSH("Main chain length: %d\n", main_chain_length);


    // Ast mit der geringsten Verschachtelungstiefe
    uint_fast8_t index_smallest_nesting_depth = 0;
    // Wert mit dem ersten Index initialisieren, deren Laenge mit der Laenge der Hauptkette uebereinstimmt
    for (size_t current_path_data_index = 0; current_path_data_index < count_path_data; ++ current_path_data_index)
    {
        if (path_data [current_path_data_index].result_path_length == main_chain_length)
        {
            index_smallest_nesting_depth = (uint_fast8_t) current_path_data_index;
            break;
        }
    }

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

            // printf ("Max nesting depth %d (index: %d)\n", possible_nesting_depth, current_path_data_index);
        }
    }

    // Die Verbindungen der C-Atome, die nicht zur Hauptkette gehoeren, zur Hauptkette gerichtet machen, damit bei
    // spaeteren Untersuchungen einfacher mit den uebrigen C-Atomen gearbeitet werden kann
    struct Path_Data* const result_ptr = &(path_data [index_smallest_nesting_depth]);

    for (uint_fast8_t i = 0; i < result_ptr->result_path_length; ++ i)
    {
        for (uint_fast8_t i2 = 0; i2 < alkane->number_of_c_atoms; ++ i2)
        {
            // Alle moeglichen C-Atome der Hauptkette nach weiteren Verbindungen, die nicht zur Hauptkette gehoeren,
            // kontrollieren
            if (result_ptr->adj_matrix [result_ptr->result_path [i]][i2] == 1 &&
                    result_ptr->adj_matrix [i2][result_ptr->result_path [i]] == 1)
            {
                // Verbindung zum C-Atom ausserhalb der Hauptkette in eine Richtung entfernen, damit die Bindung
                // gerichtet ist
                result_ptr->adj_matrix [result_ptr->result_path [i]][i2] = 0;
            }
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
        const struct Path_Data* const restrict path_data    // Path_Data Objekt, deren wichtigsten Infos ausgegeben
                                                            // werden
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
static void
Depth_First_Search_Start
(
        const uint_fast8_t start_node,              // Startknoten
        const uint_fast8_t goal_node,               // Zielknoten
        struct Path_Data* const restrict path_data  // Daten, die fuer die Bestimmung des Pfades benoetigt und erzeugt
                                                    // werden
)
{
    path_data->start_node   = start_node;
    path_data->goal_node    = goal_node;
    // Zu Beginn den Startknoten betrachten !
    path_data->current_node = start_node;

    // Tiefensuche beginnen
    Depth_First_Search_Step (path_data);

    // Ausgabe der wichtigsten Infos ueber den gefundenen Pfad
    // Print_Path_Data (path_data);

    return;
}


//---------------------------------------------------------------------------------------------------------------------

/**
 * Tiefensuche im Alkan-Graphen rekursiv durchfuehren.
 */
static void
Depth_First_Search_Step
(
        struct Path_Data* const restrict path_data  // Daten, die fuer die Bestimmung des Pfades benoetigt und erzeugt
                                                    // werden
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
 * Anzahl an Bindungen eines C-Atoms in einer Adjazenzmatrix zaehlen.
 */
static uint_fast8_t                                     // Anzahl an Bindungen
Count_Connections
(
        const uint_fast8_t c_atom,                      // C-Atom, deren Bindungen gezaehlt werden sollen
        const uint_fast8_t number_of_c_atoms,           // Anzahl an C-Atomen, die in der Adjazenzmatrix tatsaechlich
                                                        // hinterlegt sind (Diese Anzahl dient dazu, dass das Zaehlen
                                                        // schneller funktioniert)
        const unsigned char adj_matrix                  // Adjazenzmatrix (Stellt die Bindungen zwischen den C-Atomen
        [MAX_NUMBER_OF_C_ATOMS][MAX_NUMBER_OF_C_ATOMS]  // dar)
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

/**
 * Alle Aeste eines Alkanobjektes ermitteln und mit zusaetzlichen Informationen abspeichern. Diese zusaetzlichen
 * Informationen werden fuer die Erstellung des IUPAC-Namens benoetigt.
 */
static void
Chains_Go_Deeper
(
        struct Alkane* const restrict alkane,       // Alkane-Objekt, dessen Aeste durchsucht werden
        struct Path_Data* const restrict path_data, // Path_Data-Objekt, des letzten Astes
        const uint_fast8_t nesting_depth            // Aktuelle Verschachtelungstiefe
)
{
    // Die komplette Adjazenzmatrix durchsuchen
    for (uint_fast8_t i = 0; i < alkane->number_of_c_atoms; ++ i)
    {
        for (uint_fast8_t i2 = 0; i2 < alkane->number_of_c_atoms; ++ i2)
        {
            // Nach einem C-Atom suchen, bei dem eine gerichtete Verbindung existiert
            if (path_data->adj_matrix [i][i2] == 1 && path_data->adj_matrix [i2][i] == 0)
            {
                // Ast, an dem sich das C-Atom mit der gerichteten Verbindung befindet, kopieren
                // Zuerst die gerichtete Verbindung entfernen, damit man bei den weiteren Schritten beim Ast bleibt
                path_data->adj_matrix [i][i2] = 0;

                // Temp-Alkan ! Dieses Objekt stellt KEIN richtiges Alkan dar !
                // Das Objekt dient hauptsaechlich dazu, den Inhalt des aktuellen Astes zu speichern. Es wurde kein
                // einfaches Array verwendet, da bei der Tiefensuche, die spaeter ausgefuehrt wird, ein Alkan-Objekt
                // benoetigt wird !
                // Ansonsten werden die weiteren Attribute eines Alkan-Objektes nicht benoetigt !
                struct Alkane* temp_alkane = Create_Alkane (NULL, NULL, NULL, NULL);
                ASSERT_ALLOC(temp_alkane, "Cannot create a temporary Alkane object !", sizeof (struct Alkane));
                temp_alkane->number_of_c_atoms ++;

                // Astinhalt des aktuellen Asts rekursiv suchen und in das temporaere Alkan-Objekt kopieren
                Search_For_Chain_Content (path_data, temp_alkane, i, alkane->number_of_c_atoms);

                // Offset anbringen, damit die spaetere Tiefensuche alle relevanten C-Atome beruecksichtigt
                temp_alkane->number_of_c_atoms = (uint_fast8_t) (temp_alkane->number_of_c_atoms + i);

                // Tiefensuche auf diesen Ast durchfuehren, um die laengste Kette im aktuellen Ast zu finden. Dies ist
                // notwendig, da ein Ast nicht zwangslaeufig eine gerade Kette sein muss
                // Als manueller Startknoten wird das aktuelle C-Atom verwendet
                struct Path_Data* temp_chain = Find_Main_Chain (temp_alkane, i);

                // Aus den aktuellen Ast die Werte in das Originalalkan - bzw. das richtige Alkan - eintragen
                // Wenn der aktuelle Ast aus nur einem C-Atom besteht, dann findet die Tiefensuche keine Hauptkette
                // -> In diesem Fall die Laenge 1 von Hand eintragen
                alkane->chains [alkane->next_free_chain].length         = (temp_chain->result_path_length == 0) ? 1 :
                        temp_chain->result_path_length;
                alkane->chains [alkane->next_free_chain].nesting_depth  = nesting_depth;

                // Das Ermitteln der Position aus dem aktuellen Schleifenzaehler (i) ist etwas komplizierter
                const unsigned char merged_numbercode_at_i = alkane->merged_numbercode [i];
                // Jetzt ist die Frage: Bei welchem Element von dem uebergebenen Path_Data-Objekt ist der Wert gleich
                // dem merged Numbercode an der Stelle i ?
                for (uint_fast8_t current_result_data_element = 0;
                        current_result_data_element < path_data->result_path_length; ++ current_result_data_element)
                {
                    // 0 Indexierung beim Vergleich verwenden !
                    if (path_data->result_path [current_result_data_element] == merged_numbercode_at_i - 1)
                    {
                        // 1 Indexierung bei der Positionsinformation verwenden !
                        alkane->chains [alkane->next_free_chain].position =
                                (unsigned char) (current_result_data_element + 1);
                    }
                }
                alkane->next_free_chain ++;

                // Schauen, ob der Ast weitere Verschachtelungen besitzen kann. Wenn ja, dann muessen diese ebenfalls
                // untersucht werden
                if (temp_chain->result_path_length <= (temp_alkane->number_of_c_atoms - i))
                {
                    // Wenn noch weitere Aeste existieren, dann wird mit dem temporaeren Path_Data-Objekt diese Funktion
                    // nochmal rekursiv aufgerufen
                    Chains_Go_Deeper (alkane, temp_chain, (uint_fast8_t) (nesting_depth + 1));
                }

                Delete_Alkane (temp_alkane);
                FREE_AND_SET_TO_NULL(temp_chain);
            }

        } // for (uint_fast8_t i2 = 0; i2 < alkane->number_of_c_atoms; ++ i2)
    } // for (uint_fast8_t i = 0; i < alkane->number_of_c_atoms; ++ i)

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Suche nach dem Inhalt eines Astes.
 * Der Inhalt des Astes wird mithilfe einer Adjazenzmatrix im uebergebenen Alkane-Objekt gesichert. Das Path_Data
 * Objekt dient als Quelle der Informationen. Die gefundenen Verbindungen werden im Path_Data-Objekt geloescht, damit
 * eine Verbindung nicht mehrfach gezaehlt wird.
 */
static void
Search_For_Chain_Content
(
        struct Path_Data* const restrict path_data, // Quellobjekt, welches durchsucht wird
        struct Alkane* const restrict temp_alkane,  // Temporaeres Alkan-Objekt, welches die gefundenen Verbindungen in
                                                    // einer Adjazenzmatrix speichert
        const uint_fast8_t next_c_atom_index,       // Index des C-Atoms, welches als naechstes betrachtet wird
        const uint_fast8_t number_of_c_atoms        // Anzahl an C-Atomen im Alkan
)
{
    // Alle C-Atome durchsuchen
    for (uint_fast8_t temp_y = 0; temp_y < number_of_c_atoms; ++ temp_y)
    {
        // Existiert eine Verbindung, die noch nicht im temp_alkane-Objekt hinterlegt ist ?
        if (path_data->adj_matrix [next_c_atom_index][temp_y] == 1 &&
                temp_alkane->structure [next_c_atom_index][temp_y] == 0)
        {
            // Verbindung aus dem Path_Data-Objekt entfernen, damit eine Verbindung nicht mehrfach betrachtet wird
            path_data->adj_matrix [next_c_atom_index][temp_y]  = 0;
            path_data->adj_matrix [temp_y][next_c_atom_index]  = 0;
            temp_alkane->structure [next_c_atom_index][temp_y] = 1;
            temp_alkane->structure [temp_y][next_c_atom_index] = 1;
            temp_alkane->number_of_c_atoms ++;

            // Bei dem gerade gefundenen C-Atom nach weiteren Bindungen suchen
            Search_For_Chain_Content (path_data, temp_alkane, temp_y, number_of_c_atoms);
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Vergleichsfunktion fuer die qsort-Funktion.
 *
 * Bei der Sortierung von Chain-Objekten gibt es zwei Kriterien: die alphabetische Sortierung des Alkylnamen und die
 * Position des Alkylrests.
 * Die alphabetische Sortierung hat immer eine hoehere Prioritaet ! Nur wenn die Alkylnamen gleich sind, wird nach der
 * Position aufsteigend sortiert.
 */
static inline int Compare_Chain_Information (const void* a, const void* b)
{
    const struct Chain* const chain_a = (const struct Chain* const) a;
    const struct Chain* const chain_b = (const struct Chain* const) b;

    if (chain_a->nesting_depth == chain_b->nesting_depth)
    {
        const size_t chain_a_alkyl_word_length = strlen (ALKYL_WORDS [chain_a->length - 1]);
        const size_t chain_b_alkyl_word_length = strlen (ALKYL_WORDS [chain_b->length - 1]);

        // Namensvergleich der Alkylreste
        const int name_compare = strncmp (ALKYL_WORDS [chain_a->length - 1], ALKYL_WORDS [chain_b->length - 1],
                (chain_a_alkyl_word_length < chain_b_alkyl_word_length) ? chain_a_alkyl_word_length : chain_b_alkyl_word_length);

        // Wenn die Alkylnamen gleich sind, dann wird aufsteigend nach der Position sortiert. Ansonsten ist die
        // alphabetische Sortierung des Alkylnamens entscheidend !
        return (name_compare == 0) ? (((int) chain_a->position) - ((int) chain_b->position)) : name_compare;
    }
    else
    {
        FPRINTF_FFLUSH(stderr, "Else path in the compare function %s executed !\n", __func__);

        return 0;
    }
}

//---------------------------------------------------------------------------------------------------------------------

static int Compare_Chains (struct Chain chain_a, struct Chain chain_b)
{
    if (chain_a.length == chain_b.length && chain_a.nesting_depth == chain_b.nesting_depth &&
            chain_a.position == chain_b.position)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Aeste, die auf gleicher Verschachtelungstiefe liegen, anhand der Laenge der Aeste aufsteigend sortieren.
 * Dies vereinfacht die spaetere Erzeugung des IUPAC-Namen bei der Zusammenfassung gleicher Aeste, da der Algorithmus
 * davon ausgehen darf, dass gleiche Aeste - falls vorhanden - immer zusammen im Alkane-Objekt hinterlegt sind.
 *
 * Kleines Beispiel:
 * Drei Aeste auf gleicher Verschachtelungstiefe. 2x Methyl, 1x Ethyl.
 * Bei der Anordnung 'Methyl, Ethyl, Methyl' muessen bei der Zusammenfassung immer alle Aeste der gleichen
 * Verschachtelungstiefe betrachtet werden, da sonst die Zusammenfassung der beiden Methyl-Aeste nicht stattfindet.
 * Wenn die Aeste umgeordnet werden ('Methyl, Methyl, Ethyl'), dann muss bei der Zusammenfassung nur das folgende
 * Objekt betrachtet werden. Ist dies gleich, dann wird zusammengefasst; wenn nicht, dann koennen keine weiteren Aeste
 * existieren, die zusammengefasst werden muessen.
 */
static void
Reorder_Chains
(
        struct Alkane* const restrict alkane    // Alkane-Objekt, bei denen die Chain-Objekte neu sortiert werden,
                                                // falls notwendig
)
{
    if (alkane->next_free_chain == 1)
    {
        return;
    }

    struct Reorder_Data
    {
        struct Chain sub_main_chain;
        struct Chain data [MAX_NUMBER_OF_C_ATOMS];
        uint_fast8_t next_free_data;
    } reorder_data [MAX_NUMBER_OF_NESTING_DEPTH];

    memset (reorder_data, '\0', sizeof (reorder_data));

    for (size_t i = 0; i < MAX_NUMBER_OF_NESTING_DEPTH; ++ i)
    {
        for (size_t i2 = 1; i2 < alkane->next_free_chain; ++ i2)
        {
            if (alkane->chains [i2].nesting_depth == (i + 1) && alkane->chains [i2 + 1].nesting_depth == (i + 1))
            {
                reorder_data [i].data [reorder_data [i].next_free_data] = alkane->chains [i2];
                reorder_data [i].next_free_data ++;
            }
            else if (alkane->chains [i2].nesting_depth == (i + 1) && alkane->chains [i2 + 1].nesting_depth == 0)
            {
                reorder_data [i].data [reorder_data [i].next_free_data] = alkane->chains [i2];
                reorder_data [i].next_free_data ++;
            }
            else if (alkane->chains [i2].nesting_depth == (i + 1) &&
                    Compare_Chains (alkane->chains [i2 - 1], reorder_data [i].sub_main_chain) == 0)
            {
                reorder_data [i].data [reorder_data [i].next_free_data] = alkane->chains [i2];
                reorder_data [i].next_free_data ++;
            }
            else if (alkane->chains [i2].nesting_depth == (i + 1) && alkane->chains [i2 + 1].nesting_depth == (i + 2)) // + 2 !
            {
                // Sub main chain gefunden
                reorder_data [i + 1].sub_main_chain = alkane->chains [i2];
            }
        }
    }

    // Sortierungen durchfuehren
    for (size_t i = 0; i < MAX_NUMBER_OF_NESTING_DEPTH; ++ i)
    {
        if (reorder_data [i].next_free_data > 0)
        {
            qsort (reorder_data [i].data, (size_t) (reorder_data [i].next_free_data), sizeof (struct Chain),
                    Compare_Chain_Information);
        }
    }

    size_t next_free_chain = 1;
    for (size_t i = 0; i < MAX_NUMBER_OF_NESTING_DEPTH; ++ i)
    {
        if (reorder_data [i].next_free_data > 0)
        {
            if (reorder_data [i].sub_main_chain.length != 0 && reorder_data [i].sub_main_chain.position != 0 &&
                    reorder_data [i].sub_main_chain.nesting_depth != 0)
            {
                alkane->chains [next_free_chain] = reorder_data [i].sub_main_chain;
                next_free_chain ++;
            }

            memcpy (&(alkane->chains [next_free_chain]), reorder_data [i].data, reorder_data [i].next_free_data *
                    sizeof (struct Chain));
            next_free_chain += reorder_data [i].next_free_data;
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------
