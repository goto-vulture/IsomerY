/**
 * @file Alkane_To_IUPAC_Name.c
 *
 * @brief In dieser Uebersetzungseinheit wird der IUPAC-Name eines bereits vollstaendig erzeugten Alkan-Objektes
 * gebildet.
 *
 * @date 14.03.2021
 * @author x86 / Gyps
 */

#include "Alkane_To_IUPAC_Name.h"
#include <string.h>
#include <ctype.h>
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Print_Tools.h"
#include "../Misc.h"
#include "Alkane_Chain_To_IUPAC_Name.h"
#include "Path_Data.h"
#include "Path_Data_Container.h"
#include "../Tests/IUPAC_Chain_Lexer.h"





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
);

/**
 * Tiefensuchen bei allen moeglichen Pfaden, die die Hauptkette darstellen koennten, durchfuehren.
 */
static void
Do_DFS
(
        const struct Alkane* const restrict alkane,                     // Alkan-Objekt, welches betrachtet werden soll
        struct Path_Data_Container* const restrict path_data_container, // Zeiger auf ein bereits angelegtes
                                                                        // Path_Data_Container-Objekt
        const uint_fast8_t start_c_atom                                 // OPTIONAL: Angabe eines Start C-Atoms, von
                                                                        // dem die laengste Kette aus gebildet wird.
                                                                        // Dies ist bei der Bestimmung einiger
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
static uint_fast8_t                                                     // Index des Path_Data-Objektes, welches die
                                                                        // Hauptkette darstellt
Select_Suitable_Chain
(
        const struct Alkane* const restrict alkane,                     // Alkan-Objekt, welches betrachtet werden soll
        struct Path_Data_Container* const restrict path_data_container  // Path_Data_Container, aus denen die
                                                                        // Hauptkette ausgewaehlt werden
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
        const uint_fast8_t nesting_depth,           // Aktuelle Verschachtelungstiefe
        const _Bool reversed_number_order           // Erzeugung der Positionsnummern auf oberster Ebene in umgedrehter
                                                    // Position
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

/**
 * Zwei Chain-Objekte mit allen Attributen vergleichen.
 * 0 als Rueckgabewert, falls alle Attribute gleich sind. Ansonsten 1.
 */
static inline int
Compare_Chains
(
        const struct Chain chain_a, // Erstes Chain-Objekt
        const struct Chain chain_b  // Zweites Chain-Objekt
);

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

/**
 * Mit "Gruppen" sind kombinierte Aeste gemeint, die mehrfach vorkommen.
 * Z.B.: (1-methylethyl)
 *
 * Ein Alkan wie "4-(1-methylethyl)-4-(1-methylethyl)heptane"
 *
 *             C
 *             |
 *             C - C
 *             |
 * C - C - C - C - C - C - C
 *             |
 *             C - C
 *             |
 *             C
 *
 * Die beiden Gruppen "(1-methylehtyl)" koennen zusammengefasst werden:
 * 4,4-Bis(1-methylethyl)heptane
 *
 * "Bis" zeigt an, dass es sich um genau zwei gleiche Gruppen handelt, die zusammengefasst wurden.
 *
 * Das Zusammenfassen von Gruppen geschieht auf den Level des bereits erzeugten IUPAC-Namen. Dafuer wird der erzeugte
 * Name zerlegt und auf gleiche Gruppen durchsucht.
 * Das Zerlegen findet mithilfe des IUPAC_Chain_Lexers statt, der eigentlich nur fuer Testfunktionen entwickelt wurde.
 */
static void
Group_Compression
(
        char* const restrict iupac_name,    // Speicher fuer den IUPAC-Namen
        const size_t iupac_name_length      // Groesse des Speichers, welches fuer den IUPAC-Namen reserviert wurde
);

//=====================================================================================================================

/*
 * IUPAC-Name des uebergebenen Alkans erzeugen.
 *
 * Genaue Beschreibung der Nomenklatur: siehe Header-Datei !
 *
 * Asserts:
 *      alkane != NULL
 */
void
Convert_Alkane_To_IUPAC_Name
(
        struct Alkane* const restrict alkane,   // Alkan, von dem der IUPAC-Name gebildet werden soll
        const _Bool reversed_number_order       // Erzeugung der Positionsnummern auf oberster Ebene in umgedrehter
                                                // Position
)
{
    ASSERT_MSG(alkane != NULL, "alkane is NULL !");

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
    // Das Zaehlen der erzeugten Ergebnisse - waehrend des kompletten Programmlaufs - dient fuer ein besseres Debugging,
    // wenn bei einer bestimmten Erzeugung nach Fehlern gesucht werden soll.
//#ifndef RELEASE_BUILD
    static volatile size_t result_number = 0;
    ++ result_number;
//#endif /* RELEASE_BUILD */

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
    // Debug-if
    // Falls bei einer bestimmten Erzeugung ein Breakpoint gesetzt werden soll, dann ist hier die beste Stelle dafuer !
//#ifndef RELEASE_BUILD
    if (result_number == 42)
    {
        (void) result_number;
    }
//#endif /* RELEASE_BUILD */

    // Durch die Festlegung der Hauptkette koennen - und werden in den meisten Faellen - Aeste gebildet werden, die bei
    // der Benennung beruecksichtigt werden muessen
    Chains_Go_Deeper (alkane, main_chain, 1, reversed_number_order);

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

    // Koennen Gruppen zusammengefasst werden ?
    Group_Compression (iupac_name, iupac_name_length);

    // Erzeugten IUPAC-Name im Alkan-Objekt hinterlegen (tiefe Kopie)
    memcpy (alkane->iupac_name, iupac_name, sizeof (iupac_name));

//#ifndef RELEASE_BUILD
    // Hinweis in der Ausgabe, ob das Ergebnis mit umgekehrter Zahlenreihenfolge erstellt wurde (Hilft bei der
    // Fehlersuche)
    PRINTF_FFLUSH("=> IUPAC name: %60s <= (length: %2zu | %6zu) %s\n", iupac_name, strlen (iupac_name), result_number,
            (reversed_number_order /* == true */) ? "Reversed order !" : "");
//#else
    //PRINTF_FFLUSH("=> IUPAC name: %60s <= (length: %2zu)\n", iupac_name, strlen (iupac_name));
//#endif /* RELEASE_BUILD */
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
    // Container fuer die Bestimmungen der Tiefensuche
    struct Path_Data_Container* path_data_container = Create_Path_Data_Container ();


    // Tiefensuche fuer alle moeglichen Pfade durchfuehren
    Do_DFS (alkane, path_data_container, start_c_atom);

    // Den passenden Pfad aus den gerade erzeugten Pfaden auswaehlen
    const uint_fast8_t result_path_index = Select_Suitable_Chain (alkane, path_data_container);

    // Den Pfad, der als Hauptkette ausgewaehlt wurde, dynamisch erzeugen, damit das Ergebnis zurueckgegeben werden
    // kann
    struct Path_Data* result_path_data = (struct Path_Data*) CALLOC (1, sizeof (struct Path_Data));
    ASSERT_ALLOC(result_path_data, "Cannot allocate memory for the main chain path !", sizeof (struct Path_Data));

    // Ergebnisobjekt kopieren
    if (path_data_container->data [result_path_index] != NULL)
    {
        memcpy (result_path_data, path_data_container->data [result_path_index], sizeof (struct Path_Data));
    }

    // printf ("Result Index: %d\n", result_path_index);

    // Vor dem Loeschen des Containers alle enthaltene Objekte loeschen
    for (uint_fast64_t i = 0; i < path_data_container->size; ++ i)
    {
        Delete_Path_Data (path_data_container->data[i]);
        path_data_container->data[i] = NULL;
    }
    Delete_Path_Data_Container (path_data_container);

    return result_path_data;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Tiefensuchen bei allen moeglichen Pfaden, die die Hauptkette darstellen koennten, durchfuehren.
 */
static void
Do_DFS
(
        const struct Alkane* const restrict alkane,                     // Alkan-Objekt, welches betrachtet werden soll
        struct Path_Data_Container* const restrict path_data_container, // Zeiger auf ein bereits angelegtes
                                                                        // Path_Data_Container-Objekt
        const uint_fast8_t start_c_atom                                 // OPTIONAL: Angabe eines Start C-Atoms, von
                                                                        // dem die laengste Kette aus gebildet wird.
                                                                        // Dies ist bei der Bestimmung einiger
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

    // Alle Kombinationen an CH3-Elementen ausprobieren, wobei die Reihenfolge der CH3-Elemente KEINE Rolle spielt !
    for (uint_fast8_t current_ch3_element_start = 0; current_ch3_element_start < count_ch3_elemets;
            ++ current_ch3_element_start)
    {
        for (uint_fast8_t current_ch3_element_end = 0; current_ch3_element_end < count_ch3_elemets;
                ++ current_ch3_element_end)
        {
            // Wenn KEIN manueller Startknoten gewaehlt wird, dann koennen die Schleifendurchlauefe, wo Start und Ziel
            // identisch sind, uebersprungen werden
            if (start_c_atom == UINT_FAST8_MAX)
            {
                if (current_ch3_element_start == current_ch3_element_end)
                {
                    continue;
                }
            }

            struct Path_Data* current_path_data = Create_Path_Data ();
            Add_Path_Data_To_Container (path_data_container, current_path_data);

            // Adjazenzmatrix des Alkans in das passende Path_Data-Objekt kopieren
            memcpy (current_path_data->adj_matrix, alkane->structure, sizeof (alkane->structure));

            // Tiefensuche durchfuehren, um bei der aktuellen Variante des Start- und Zielknotens den Pfad und deren
            // Laenge zu bestimmen
            // Verwendung eines manuellen Startknoten ?
            if (start_c_atom != UINT_FAST8_MAX)
            {
                Depth_First_Search_Start (start_c_atom, ch3_elements [current_ch3_element_end], current_path_data);
            }
            else
            {
                Depth_First_Search_Start (ch3_elements [current_ch3_element_start], ch3_elements [current_ch3_element_end],
                        current_path_data);
            }
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Hauptkette aus den uebergebenen Path_Data-Objekten auswaehlen.
 *
 * Die Hauptkette ist der laengste Pfad. Wenn es mehrere Pfade mit der maximalen Laenge gibt (was die Regel ist), dann
 * wird der Pfad ausgewaehlt, bei denen die uebrigen Aeste die GERINGSTE Verschachtelungstiefe besitzen. Dies dient
 * dazu, dass der IUPAC-Name moeglichst kurz und einfach wird.
 */
static uint_fast8_t                                                     // Index des Path_Data-Objektes, welches die
                                                                        // Hauptkette darstellt
Select_Suitable_Chain
(
        const struct Alkane* const restrict alkane,                     // Alkan-Objekt, welches betrachtet werden soll
        struct Path_Data_Container* const restrict path_data_container  // Path_Data_Container, aus denen die
                                                                        // Hauptkette ausgewaehlt werden
)
{
    // Die maximale Laenge des laengsten Pfades aus der Menge der moeglichen Pfade auswaehlen
    // Es ist durchaus moeglich, dass mehrere Pfade die maximale Laenge besitzen
    uint_fast8_t main_chain_length = 0;
    for (size_t current_path_data_index = 0; current_path_data_index < path_data_container->size;
            ++ current_path_data_index)
    {
        if (path_data_container->data [current_path_data_index]->result_path_length > main_chain_length)
        {
            main_chain_length = path_data_container->data [current_path_data_index]->result_path_length;
        }
    }

    // PRINTF_FFLUSH("Main chain length: %d\n", main_chain_length);


    // Ast mit der geringsten Verschachtelungstiefe
    uint_fast8_t index_smallest_nesting_depth = 0;
    // Wert mit dem ersten Index initialisieren, deren Laenge mit der Laenge der Hauptkette uebereinstimmt
    for (size_t current_path_data_index = 0; current_path_data_index < path_data_container->size;
            ++ current_path_data_index)
    {
        if (path_data_container->data [current_path_data_index]->result_path_length == main_chain_length)
        {
            index_smallest_nesting_depth = (uint_fast8_t) current_path_data_index;
            break;
        }
    }

    // Maximale Verschachtelungstiefe bei allen Pfaden ermitteln, wenn diese als Hauptkette gewaehlt werden wuerden
    for (uint_fast8_t current_path_data_index = 0; current_path_data_index < path_data_container->size;
            ++ current_path_data_index)
    {
        struct Path_Data* const current_path_data = path_data_container->data [current_path_data_index];

        if (current_path_data == NULL)
        {
            continue;
        }

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
            if (path_data_container->data [current_path_data_index]->max_possible_nesting_depth <
                    path_data_container->data [index_smallest_nesting_depth]->max_possible_nesting_depth)
            {
                index_smallest_nesting_depth = current_path_data_index;
            }

            // printf ("Max nesting depth %d (index: %d)\n", possible_nesting_depth, current_path_data_index);
        }
    }

    // Die Verbindungen der C-Atome, die nicht zur Hauptkette gehoeren, zur Hauptkette gerichtet machen, damit bei
    // spaeteren Untersuchungen einfacher mit den uebrigen C-Atomen gearbeitet werden kann
    struct Path_Data* const result_ptr = path_data_container->data [index_smallest_nesting_depth];

    if (result_ptr != NULL)
    {
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
    }

    return index_smallest_nesting_depth;
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
        const uint_fast8_t nesting_depth,           // Aktuelle Verschachtelungstiefe
        const _Bool reversed_number_order           // Erzeugung der Positionsnummern auf oberster Ebene in umgedrehter
                                                    // Position
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
                        // Sollen auf der obersten Ebene die Positionsnummern in umgekehrter Reihenfolge erzeugt werden ?
                        // Beispiel:
                        //
                        // C - C - C - C
                        //     |
                        //     C
                        //
                        // => 2-Methyl-Butan
                        //
                        // Umgekehrte Reihenfolge:
                        //
                        // C - C - C - C
                        //     |
                        //     C
                        //
                        // => 3-Methly-Butan
                        //
                        // In wenigen Faellen ist dies fuer die Bestimmung, ob ein erzeugter IUPAC-Name richtig ist,
                        // notwendig
                        if (reversed_number_order /* == true */ && nesting_depth == 1)
                        {
                            alkane->chains [alkane->next_free_chain].position =
                                    (unsigned char) (alkane->chains[0].length - current_result_data_element);
                        }
                        else
                        {
                            // 1 Indexierung bei der Positionsinformation verwenden !
                            alkane->chains [alkane->next_free_chain].position =
                                (unsigned char) (current_result_data_element + 1);
                        }
                    }
                }
                alkane->next_free_chain ++;

                // Schauen, ob der Ast weitere Verschachtelungen besitzen kann. Wenn ja, dann muessen diese ebenfalls
                // untersucht werden
                if (temp_chain->result_path_length <= (temp_alkane->number_of_c_atoms - i))
                {
                    // Wenn noch weitere Aeste existieren, dann wird mit dem temporaeren Path_Data-Objekt diese Funktion
                    // nochmal rekursiv aufgerufen
                    Chains_Go_Deeper (alkane, temp_chain, (uint_fast8_t) (nesting_depth + 1), reversed_number_order);
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

/**
 * Zwei Chain-Objekte mit allen Attributen vergleichen.
 * 0 als Rueckgabewert, falls alle Attribute gleich sind. Ansonsten 1.
 */
static inline int
Compare_Chains
(
        const struct Chain chain_a, // Erstes Chain-Objekt
        const struct Chain chain_b  // Zweites Chain-Objekt
)
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
    // Wenn es nur die Hauptkette gibt, dann macht eine Neustrukturierung der Aeste keinen Sinn
    if (alkane->next_free_chain == 1)
    {
        return;
    }

    /**
     * Temoraere Daten um eine spaetere richtige Umstrukturierung der Chain Objekte zu ermoeglichen. Diese
     * Umstrukturierung ist notwending damit die Erzeugung des IUPAC-Namen (mit allen Nomenklaturregeln) funktioniert.
     */
    struct Reorder_Data
    {
        struct Chain sub_main_chain;                // "Verschachtelungs-Hauptkette"
        struct Chain data [MAX_NUMBER_OF_C_ATOMS];  // Chain-Objekte, die als Gruppe bei der Umstrukturierung betrachtet
                                                    // werden muessen
        uint_fast8_t next_free_data;                // Naechtes freies Element im Array
    } reorder_data [MAX_NUMBER_OF_NESTING_DEPTH];

    memset (reorder_data, '\0', sizeof (reorder_data));

    // In einigen Faellen, wenn alternative Zahlenpraefixe wie "Bis" oder "Tris" verwendet werden koennten, erzeugt die
    // Umsortierung noch falsche Ergebnisse.
    // ToDO: Umsortierug von Gruppen ermoeglichen
    // Als Workaround wird die Zusammenfassung von Gruppen NICHT durchgefuehrt ! Ergebnisse ohne solche
    // Zusammenfassungen sind zwar laenger; aber dennoch inhaltlich richtig
    // Beispiel:
    // OHNE Zusammenfassung:    4-(1-Methylethyl)-4-(1-Methylethyl)heptan
    // MIT Zusammenfassung:     4,4-Bis(1-Methylethyl)heptan
    //
    // Erkennung einer falschen Umsortierung:
    // Die Anzahl an Aesten mit deren Attributen (Laenge und Verschachtelungstiefe) ! OHNE Position ! darf durch die
    // Umsortierung nicht geaendert werden
    struct Alkane temp_alkane;
    memcpy (&temp_alkane, alkane, sizeof (temp_alkane));


    // Fuer die spaetere Sortierung der Chain Objekte muessen diese temporaer umstrukturiert werden
    // Dafuer werden alle Chain Objekte durchlaufen und wie folgt in ein Array zusammengefasst:
    //
    // - Alle Chain Objekte auf der gleichen Verschachtelungsebene, wenn diese Chain Objekte nicht Teil einer weiteren
    //   Verschachtelung sind.
    //   Damit werden z.B. 1x Ethyl und 1x Methyl zusammengefasst, wenn die Bedingungen stimmen
    //   Dies ist erforderlich, damit die spaetere alphabetische Sortierung auf den richtigen Daten angewendet wird
    //
    // - Wenn die Verschachtelungstiefe steigt, dann wissen wir, dass es sich um eine Hauptkette in einer
    //   Verschachtelung handelt. Dabei wird die "Verschachtelungs-Hauptkette" als zusaetzliches Element in den
    //   reorder_data gespeichert. Alle weiteren Chain Objekte, die direkt oder indirekt mit dieser "Verschachtelungs-
    //   hauptkette" verbunden sind, werden in das Array eingetragen, da bei der spaeteren Bennenung dieses Chain
    //   Objekt gesondert behandelt werden muss.
    for (size_t i = 0; i < MAX_NUMBER_OF_NESTING_DEPTH; ++ i)
    {
        for (size_t i2 = 1; i2 < alkane->next_free_chain; ++ i2) // i2 == 1, da die Hauptkette nie umstrukturiert wird
        {
            // Nur wenn die Verschachtelungstiefe passt, k
            if (alkane->chains [i2].nesting_depth != (i + 1))
            {
                continue;
            }

            // Die fuenf Faelle fuer das Umgruppieren von Chain Objekten:
            // - Ist die Verschachtelungstiefe des aktuellen Chain Objektes mit dem naechsten identisch ?
            // - Ist das aktuelle Chain Objekt das letzte im Array ?
            // - Ist das vorherige Chain Objekt identisch mit der aktuellen "Verschachtelungs-Hauptkette" ? (Dieser
            //   Fall muss betrachtet werden, damit z.B. zwei aufeinanderfolgende Ethyl-Abzweigungen fuer die spaetere
            //   Benennung richtig gruppiert werden.)
            // - Ist das akuelle Chain Objekt komplett mit dem vorherigen Chain Objekt identisch ?
            // - Ist die Verschachtelungstiefe des naechsten Chain Objektes groesser als die des aktuellen Objektes ?
            //   Wenn ja, dann wurde eine "Verschachtelungs-Hauptkette" gefunden

            // Ist die Verschachtelungstiefe des aktuellen Chain Objektes mit dem naechsten identisch ?
            if (alkane->chains [i2 + 1].nesting_depth == (i + 1))
            {
                reorder_data [i].data [reorder_data [i].next_free_data] = alkane->chains [i2];
                reorder_data [i].next_free_data ++;
            }
            // Ist das aktuelle Chain Objekt das letzte im Array ?
            else if (alkane->chains [i2 + 1].nesting_depth == 0)
            {
                reorder_data [i].data [reorder_data [i].next_free_data] = alkane->chains [i2];
                reorder_data [i].next_free_data ++;
            }
            // Ist das vorherige Chain Objekt identisch mit der aktuellen "Verschachtelungs-Hauptkette" ? (Dieser
            // Fall muss betrachtet werden, damit z.B. zwei aufeinanderfolgende Ethyl-Abzweigungen fuer die spaetere
            // Benennung richtig gruppiert werden.)
            else if (Compare_Chains (alkane->chains [i2 - 1], reorder_data [i].sub_main_chain) == 0)
            {
                reorder_data [i].data [reorder_data [i].next_free_data] = alkane->chains [i2];
                reorder_data [i].next_free_data ++;
            }
            // Ist das akuelle Chain Objekt komplett mit dem vorherigen Chain Objekt identisch ?
            else if (Compare_Chains (alkane->chains [i2 - 1], alkane->chains [i2]) == 0)
            {
                reorder_data [i].data [reorder_data [i].next_free_data] = alkane->chains [i2];
                reorder_data [i].next_free_data ++;
            }
            else if (alkane->chains [i2 + 1].nesting_depth == (i + 2)) // + 2 !
            {
                // Sub main chain ("Verschachtelungs-Hauptkette") gefunden, da Steigerung der Verschachtelungstiefe
                reorder_data [i + 1].sub_main_chain = alkane->chains [i2];
            }
        }
    }

    // Maximale Verschachtelungstiefe ermitteln
    size_t max_nesting_depth = 0;
    for (size_t i = 0; i < MAX_NUMBER_OF_NESTING_DEPTH; ++ i)
    {
        if (reorder_data [i].next_free_data > 0)
        {
            max_nesting_depth = i;
        }
    }

    // Sortierungen durchfuehren
    for (size_t i = 0; i < MAX_NUMBER_OF_NESTING_DEPTH; ++ i)
    {
        if (reorder_data [i].next_free_data > 0)
        {
            // Der naechste Schritt ist - mehr oder weniger - optional. Die Positionen der Aeste auf erster
            // Verschachtelungstiefe werden so umstruktoriert, sodass die Summe der Positionsangaben so gering wie
            // moeglich ist.
            // Dies ist moeglich, da bei der Nummerierung von beiden Seiten der Hauptkette begonnen werden kann. Das
            // einzige wichtige dabei: Alle Positionsnummern muessen von einer Seite aus angegeben werden; sonst ist
            // keine eindeutige Darstellung des Molekuels mehr moeglich !
            // Ein Beispiel:
            //
            // Angaben der Positonsnummern von LINKS nach RECHTS.
            //
            // 3-Ethyl-6,6-DiMethylOctan (3 + 6 + 6 -> 15)
            //
            //                       C
            //                       |
            // 1 C - C - C - C - C - C - C - C 8
            //           |           |
            //           C           C
            //           |
            //           C
            //
            // Wenn man nun die Nummerierung von RECHTS nach LINKS durchfuehrt: 6-Ethyl-3,3-DiMethylOctan (6 + 3 + 3 -> 12)
            //
            // ! ALLERDINGS !
            // Es wird die Richtung gewaehlt, wo die hoechste Positionsnummer am geringsten ist. Diese Regel hat eine
            // noch hoehere Prioritaet ! Selbst dann, wenn die Summe der Positionen hoeher ist !
            // Auch hier wieder ein Beispiel:
            //
            // Angaben der Positonsnummern von LINKS nach RECHTS.
            //
            // 3,3,8-TriMethylNonan (3 + 3 + 8 -> 14)
            //
            //           C
            //           |
            // 1 C - C - C - C - C - C - C - C - C 9
            //           |                   |
            //           C                   C
            //
            // Wenn man nun die Nummerierung von RECHTS nach LINKS durchfuehrt: 2,7,7-Trimethylnonan (2 + 7 + 7 -> 16)
            // Obwohl von LINKS nach RECHTS eine geringere Summe besitzt, wird dennoch die Variante von RECHTS nach
            // LINKS gewaehlt, da dort das Maximum der Positionsnummern (7 vs. 8) geringer ist !
            // Keine Ahnung, warum die Regeln so priorisiert wurden ... Ist nunmal einfach so :o

            // Die Umstruktorierung nur auf der obersten Ebene durchfuehren
            if (i == 0 && max_nesting_depth == 0)
            {
                const uint_fast8_t main_chain_length    = alkane->chains [0].length;
                size_t current_direction_sum            = 0;
                size_t inverted_direction_sum           = 0;
                size_t maximum_current_direction        = 0;
                size_t maximum_inverted_direction       = 0;

                for (uint_fast8_t i2 = 0; i2 < reorder_data [i].next_free_data; ++ i2)
                {
                    // Da die Aeste aufsteigend anhand der Verschachtelungstiefe sortiert sind, weiss man, dass alle Elemente erster
                    // Verschachtelungstiefe betrachtet wurden, wenn die Verschachtelungstiefe nicht mehr 1 ist
                    if (reorder_data [i].data [i2].nesting_depth != 1)
                    {
                        break;
                    }

                    const uint_fast8_t current_position = reorder_data [i].data [i2].position;

                    // Summe der Positionen aus beiden Richtungen berechnen
                    current_direction_sum   += current_position;
                    inverted_direction_sum  += (size_t) (main_chain_length - current_position + 1);

                    if (current_position > maximum_current_direction)
                    {
                        maximum_current_direction = current_position;
                    }
                    if ((size_t) (main_chain_length - current_position + 1) > maximum_inverted_direction)
                    {
                        maximum_inverted_direction = (size_t) (main_chain_length - current_position + 1);
                    }
                }

                // Aenderung durchfuehren, falls notwendig
                if ((maximum_inverted_direction <= maximum_current_direction) &&
                        (inverted_direction_sum < current_direction_sum))
                {
                    for (uint_fast8_t i2 = 0; i2 < reorder_data [i].next_free_data; ++ i2)
                    {
                        if (reorder_data [i].data [i2].nesting_depth != 1)
                        {
                            break;
                        }

                        reorder_data [i].data [i2].position = (uint_fast8_t)
                                (main_chain_length - reorder_data [i].data [i2].position + 1);
                    }
                }
            }

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

    // Der Test am Ende:
    // Hat die Umsortierung "Schaden" angerichtet?
    _Bool chain_found [MAX_NUMBER_OF_C_ATOMS];
    _Bool chain_used [MAX_NUMBER_OF_C_ATOMS];   // Damit jeder Ast aus dem Temp-Objekt nur maximal einmal verwendet wird
    memset (chain_found, '\0', sizeof (chain_found));
    memset (chain_used, '\0', sizeof (chain_used));

    for (uint_fast8_t i = 0; i < alkane->next_free_chain; ++ i)
    {
        for (uint_fast8_t i2 = 0; i2 < temp_alkane.next_free_chain; ++ i2)
        {
            if (! chain_used [i2] /* == false */ &&
                    Compare_Chains(alkane->chains[i], temp_alkane.chains[i2]) == 0
                    /* alkane->chains [i].length == temp_alkane.chains [i2].length &&
                    alkane->chains [i].nesting_depth == temp_alkane.chains [i2].nesting_depth &&
                    alkane->chains [i].position == temp_alkane.chains [i2].position */)
            {
                chain_found [i] = true;
                chain_used [i2] = true;
                break;
            }
        }
    }

    // Wenn das Umsortieren "Schaden" angerichtet hat, dann wird der Zustand vor der Umsortierung wiederhergestellt
    for (uint_fast8_t i = 0; i < alkane->next_free_chain; ++ i)
    {
        if (! chain_found [i] /* == false */)
        {
            memcpy(alkane, &temp_alkane, sizeof(temp_alkane));
            break;
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Mit "Gruppen" sind kombinierte Aeste gemeint, die mehrfach vorkommen.
 * Z.B.: (1-methylethyl)
 *
 * Ein Alkan wie "4-(1-methylethyl)-4-(1-methylethyl)heptane"
 *
 *             C
 *             |
 *             C - C
 *             |
 * C - C - C - C - C - C - C
 *             |
 *             C - C
 *             |
 *             C
 *
 * Die beiden Gruppen "(1-methylehtyl)" koennen zusammengefasst werden:
 * 4,4-Bis(1-methylethyl)heptane
 *
 * "Bis" zeigt an, dass es sich um genau zwei gleiche Gruppen handelt, die zusammengefasst wurden.
 *
 * Das Zusammenfassen von Gruppen geschieht auf den Level des bereits erzeugten IUPAC-Namen. Dafuer wird der erzeugte
 * Name zerlegt und auf gleiche Gruppen durchsucht.
 * Das Zerlegen findet mithilfe des IUPAC_Chain_Lexers statt, der eigentlich nur fuer Testfunktionen entwickelt wurde.
 */
static void
Group_Compression
(
        char* const restrict iupac_name,    // Speicher fuer den IUPAC-Namen
        const size_t iupac_name_length      // Groesse des Speichers, welches fuer den IUPAC-Namen reserviert wurde
)
{
    const struct IUPAC_Chain_Lexer_Result lexer_result = Create_Chain_Tokens (iupac_name);

    uint_fast8_t groupable_chain_tokens [NUMBER_OF_GROUP_MERGE_WORDS];
    uint_fast8_t next_free_groupable_chain_tokens = 0;
    memset (groupable_chain_tokens, '\0', sizeof(groupable_chain_tokens));

    _Bool lexer_tokens_used [MAX_NUMBER_OF_C_ATOMS];
    memset (lexer_tokens_used, '\0', sizeof (lexer_tokens_used));
    _Bool name_resetted             = false;
    size_t iupac_name_space_left    = iupac_name_length;

    // Alle Kombinationen durchgehen, um moegliche Gruppen fuer die Zusammenfassung zu finden
    for (uint_fast8_t first_token_index = 0; first_token_index < lexer_result.next_free_token; ++ first_token_index)
    {
        for (uint_fast8_t second_token_index = 0; second_token_index < lexer_result.next_free_token; ++ second_token_index)
        {
            if (first_token_index == second_token_index) { continue; }

            if (strncmp (lexer_result.result_tokens [first_token_index], lexer_result.result_tokens [second_token_index],
                    sizeof (lexer_result.result_tokens [0])) == 0)
            {
                if (! lexer_tokens_used [first_token_index] /* == false */)
                {
                    groupable_chain_tokens [next_free_groupable_chain_tokens] = first_token_index;
                    lexer_tokens_used [first_token_index] = true;
                    ++ next_free_groupable_chain_tokens;
                }
                if (! lexer_tokens_used [second_token_index] /* == false */)
                {
                    groupable_chain_tokens [next_free_groupable_chain_tokens] = second_token_index;
                    lexer_tokens_used [second_token_index] = true;
                    ++ next_free_groupable_chain_tokens;
                }
            }
        }

        // Wurden Gruppen gefunden, die zusammengefasst werden koennen ?
        if (next_free_groupable_chain_tokens > 0)
        {
            // Wurde der Name bereits zurueckgesetzt ?
            // Einmal muss das gemacht werden, damit Namensfragmente nicht mehrfach im Ergebnis vorkommen
            if (! name_resetted /* == false */)
            {
                memset (iupac_name, '\0', IUPAC_NAME_LENGTH);
                name_resetted = true;
            }

            uint_fast8_t prefix_length = 0;

            // Alle Gruppen, die zusammengefuegt werden koennen, verarbeiten
            for (uint_fast8_t i2 = 0; i2 < next_free_groupable_chain_tokens; ++ i2)
            {
                // Nummer, die am Beginn der Gruppe steht, extrahieren
                char group_position_number [4] = { '\0', '\0', '\0', '\0' };
                uint_fast8_t next_free_group_position_number    = 0;
                size_t current_char_in_group                    = 0;

                while (isdigit (lexer_result.result_tokens [groupable_chain_tokens [i2]][current_char_in_group]))
                {
                    group_position_number [next_free_group_position_number] =
                            lexer_result.result_tokens [groupable_chain_tokens [i2]][current_char_in_group];
                    ++ next_free_group_position_number;
                    ++ current_char_in_group;
                    ++ prefix_length;
                }

                strncat (iupac_name, group_position_number, iupac_name_space_left);
                iupac_name_space_left -= strlen (group_position_number);
                if ((i2 + 1) < next_free_groupable_chain_tokens)
                {
                    strncat (iupac_name, ",", iupac_name_space_left);
                    iupac_name_space_left -= strlen (",");
                }
            }

            strncat (iupac_name, "-", iupac_name_space_left);
            iupac_name_space_left -= strlen ("-");
            strncat (iupac_name, GROUP_MERGE_WORDS [next_free_groupable_chain_tokens - 1], iupac_name_space_left);
            iupac_name_space_left -= strlen (GROUP_MERGE_WORDS [next_free_groupable_chain_tokens - 1]);

            // Eins der zusammengefassten Gruppen in den Namen einbauen
            strncat (iupac_name, &(lexer_result.result_tokens [groupable_chain_tokens [0]][prefix_length]),
                    iupac_name_space_left);
            iupac_name_space_left -= strlen (&(lexer_result.result_tokens [groupable_chain_tokens [0]][prefix_length]));

            next_free_groupable_chain_tokens = 0;
            memset (groupable_chain_tokens, '\0', sizeof (groupable_chain_tokens));
        }
    }

    // Die uebrigen Tokens vom Lexer hintereinander zum Ergebnis anhaengen
    if (name_resetted /* == true */)
    {
        // Anzahl an verwendeten Tokens zaehlen
        uint_fast8_t used_tokens = 0;

        for (uint_fast8_t i = 0; i < lexer_result.next_free_token; ++ i)
        {
            if (lexer_tokens_used [i] /* == true */)
            {
                ++ used_tokens;
            }
        }

        // "- 1", da das Token fuer die gerade Kette ignoriert werden muss
        if (used_tokens < (lexer_result.next_free_token - 1))
        {
            strncat (iupac_name, "-", iupac_name_space_left);
            iupac_name_space_left -= strlen ("-");
        }

        for (uint_fast8_t i = 0; i < lexer_result.next_free_token; ++ i)
        {
            // Nur die Tokens verwenden, die noch nicht verwendet wurden
            if (! lexer_tokens_used [i] /* == false */)
            {
                strncat (iupac_name, lexer_result.result_tokens [i], iupac_name_space_left);
                iupac_name_space_left -= strlen (lexer_result.result_tokens [i]);
            }
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------
