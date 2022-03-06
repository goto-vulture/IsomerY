/**
 * @file Path_Data.h
 *
 * @brief Mittels des Path_Data-Objektes koennen Pfade im Molekuel gesichert werden. Mittels dieser Pfade kann die
 * laengste Kette im Molekuel bestimmt werden. Dies ist sehr wichtig fuer die Generierung des IUPAC-Namen, da die
 * Bestimmung und Bennenung der Abzweigungen immer von der laengsten Kette her betrachtet werden !
 *
 * Ein Pfad kann in einem Molekuel im weitesten Sinne als ein Graphenproblem betrachtet werden. Daher wird das Molekuel
 * als Adjazenzmatrix angegeben. Dies ist nicht sehr Speichereffizient; aber das Arbeiten mit Adjazenzmatrizen ist sehr
 * einfach zu realisieren. Um die Adjazenzmatrix nicht dynamisch erzeugen zu muessen, wird die Groesse bereits zur
 * Kompilierungszeit angegeben. Da dadurch nicht bekannt ist, wie viele C-Atome tatsaechlich im Molekuel sind, muss vom
 * groessten Fall ausgegangen werden: MAX_NUMBER_OF_C_ATOMS (aktuell 40). Dies fuehrt zu einer Groesse von 40 x 40
 * Byte. Also 1600 Byte !
 *
 * Da diese Objekte nur fuer die Bestimmung der Hauptkette verwendet werden und nicht dauerhaft abgespeichert werden,
 * halte ich diesen Ansatz der Einfachheit halber fuer akzeptabel; auch wenn dadurch viel Speicher ungenutzt bleibt !
 *
 * In der Praxis koennen aktuell mit einer halbwegs akzeptablen Laufzeit nur maximal 15 C-Atome verwendet werden, auch
 * wenn die Algorithmen an sich keine Limitierungen besitzen.
 *
 * @date 12.06.2021
 * @author x86 / Gyps
 */

#ifndef PATH_DATA_H
#define PATH_DATA_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stddef.h>
#include "Alkane_Info_Constitutional_Isomer.h"
#include "../DMMC.h"



enum Path_Data_State
{
    PATH_DATA_CREATED = 0,                  ///< Objekt wurde erzeugt
    PATH_DATA_INITIALIZED_WITH_ZERO_BYTES,  ///< Objekt wurde mit 0 Bytes initialisiert

    PATH_DATA_DELETED,                      ///< Objekt wurde geloescht und enthaelt keine Informationen mehr

    /**
     * @brief Objekt enthaelt ungueltige Informationen (Dieser Zustand sollte in der Praxis nicht vorkommen - aber man
     * weiss ja nie)
     */
    PATH_DATA_INVALID_DATA,

    /**
     * @brief Unbekannter Zustand. Dies ist hauptsaechlich dafuer da, wenn kein anderer Zustand angegeben werden kann
     */
    PATH_DATA_UNKNOWN_STATE
};



/**
 * @brief Daten, die fuer die Bestimmung des Pfades sowie fuer das Abspeichern des Pfades benoetigt werden.
 *
 * Aus diesen Infos kann die passende Hauptkette bestimmt werden (indem mehrere Path_Data-Objekte miteinander
 * verglichen werden).
 */
struct Path_Data
{
    /**
     * @brief Kopie der Alkan Adjazenzmatrix
     */
    unsigned char adj_matrix
    [MAX_NUMBER_OF_C_ATOMS][MAX_NUMBER_OF_C_ATOMS];

    uint_fast8_t start_node;                            ///< Startknoten
    uint_fast8_t goal_node;                             ///< Zielknoten

    /**
     * @brief Aktueller Knoten (wird nur waehrend der Tiefensuche benoetigt)
     */
    uint_fast8_t current_node;

    unsigned char result_path [MAX_NUMBER_OF_C_ATOMS];  ///< Pfad, um vom Start- zum Zielknoten zu kommen
    uint_fast8_t result_path_length;                    ///< Laenge des Pfades

    /**
     * @brief Das naechste zu verwendene Objekt im result_path-Array. Diese Information wird nur fuer die Erzeugung des
     * Pfades benoetigt
     */
    uint_fast8_t path_index;

    /**
     * @brief Maximal moegliche Verschachtelungstiefe der Aeste im Molekuel, wenn der Pfad als Hauptkette gewaehlt wird
     */
    uint_fast8_t max_possible_nesting_depth;

    enum Path_Data_State state;                         ///< Status des Objektes
};

// Container erzeugen
CREATE_CONTAINER_H(struct Path_Data*, Path_Data)

//=====================================================================================================================

/**
 * @brief Ein neues mit Nullbytes initialisiertes Path_Data-Objekt erstellen.
 *
 * Asserts:
 *      N/A
 *
 * @return Zeiger auf das neue Path_Data-Objekt
 */
extern struct Path_Data*
Create_Path_Data
(
        void
);

/**
 * @brief Path_Data Objekt in eine Zeichenkettendarstellung ueberfuehren. Dies ist insbesondere fuers Debugging
 * hilfreich.
 *
 * Der Speicher fuer die Zeichenkette muss vom Aufrufer allokiert werden ! Wenn der Speicher fuer die Zeichenkette nicht
 * passt, dann wird am vorletzten Zeichen abgeschnitten.
 *
 * Adj_Matrix (first 10 elements): <Die ersten 10 Elemente>
 * Start_Node: <start_node>
 * Goal_Node: <goal_node>
 * Current_Node: <current_node>
 *
 * Result_Path: <result_path>
 * Result_Path_Length: <result_path_length>
 * Path_Index: <path_index>
 *
 * Max_Possible_Nesting_Depth: <max_possible_nesting_depth>
 *
 * State: <state>
 *
 * Asserts:
 *      container != NULL,
 *      string_memory != NULL,
 *      string_memory_size > 0
 *
 * @param[in] path_data Path_Data-Objekt, welches als Zeichenkette dargestellt werden soll
 * @param[out] string_memory Speicher, der fuer die Zeichenkettenerzeugung verwendet werden soll
 * @param[in] string_memory_size Groesse des Zeichenkettenspeichers
 *
 * @return Adresse der Eingabe-Zeichenkette
 */
extern const char*                                              // Adresse der Eingabe-Zeichenkette
Path_Data_To_String
(
        const struct Path_Data* restrict const path_data,
                                                                // Path_Data-Objekt, welches als Zeichenkette
                                                                // dargestellt werden soll
        char* const restrict string_memory,                     // Speicher, der fuer die Zeichenkettenerzeugung
                                                                // verwendet werden soll
        const size_t string_memory_size                         // Groesse des Zeichenkettenspeichers
                                                                // Wenn der Speicher nicht ausreicht, dann wird am
                                                                // vorletzten Zeichen die Zeichenkette abgeschnitten
);

/**
 * @brief Path_Data-Objekt auf stdout ausgeben. Hauptsaechlich fuer das Debbuging.
 *
 * Asserts:
 *      path_data != NULL
 *
 * @param[in] path_data Path_Data-Objekt, welches ausgegeben werden soll
 */
extern void
Print_Path_Data
(
        const struct Path_Data* const restrict path_data        // Path_Data-Objekt, welches ausgegeben werden soll
);

/**
 * @brief Path_Data-Objekt loeschen.
 *
 * @param[in] path_data Zeiger auf das Path_Data-Objekt, welches geloescht werden soll
 *
 * Asserts:
 *      path_data != NULL
 */
extern void
Delete_Path_Data
(
        struct Path_Data* restrict path_data                    // Path_Data-Objekt, welches geloescht werden soll
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PATH_DATA_H */
