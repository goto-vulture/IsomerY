/*
 * Path_Data.h
 *
 *  Created on: 12.06.2021
 *      Author: x86 / Gyps
 */

#ifndef PATH_DATA_H
#define PATH_DATA_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stddef.h>
#include "Alkane_Info_Constitutional_Isomer.h"



enum Path_Data_State
{
    PATH_DATA_CREATED = 0,                  // Objekt wurde erzeugt
    PATH_DATA_INITIALIZED_WITH_ZERO_BYTES,  // Objekt wurde mit 0 Bytes initialisiert

    PATH_DATA_DELETED,                      // Objekt wurde geloescht und enthaelt keine Informationen mehr
    PATH_DATA_INVALID_DATA,                 // Objekt enthaelt ungueltige Informationen (Dieser Zustand sollte in der
                                            // Praxis nicht vorkommen - aber man weiss ja nie)

    PATH_DATA_UNKNOWN_STATE                 // Unbekannter Zustand. Dies ist hauptsaechlich dafuer da, wenn kein
                                            // anderer Zustand angegeben werden kann
};



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

    enum Path_Data_State state;                         // Status des Objektes
};

//=====================================================================================================================

/**
 * Ein neues mit Nullbytes initialisiertes Path_Data-Objekt erstellen.
 *
 * Asserts:
 *      N/A
 */
extern struct Path_Data*
Create_Path_Data
(
        void
);

/**
 * Path_Data Objekt in eine Zeichenkettendarstellung ueberfuehren. Dies ist insbesondere fuers debugging hilfreich.
 *
 * Der Speicher fuer die Zeichenkette muss vom Aufrufer allokiert werden !
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
 */
extern const char*                                              // Adresse der Eingabe-Zeichenkette
Path_Data_To_String
(
        const struct Path_Data* restrict const path_data,
                                                                // Path_Data-Objekt, der als Zeichenkette dargestellt
                                                                // werden soll
        char* const restrict string_memory,                     // Speicher, der fuer die Zeichenkettenerzeugung
                                                                // verwendet werden soll
                                                                // Der Speicher muss vorher vom Aufrufer allokiert
                                                                // werden !
        const size_t string_memory_size                         // Groesse des Zeichenkettenspeichers
                                                                // Wenn der Speicher nicht ausreicht, dann wird am
                                                                // vorletzten Zeichen die Zeichenkette abgeschnitten
);

/**
 * Path_Data-Objekt auf stdout ausgeben. Hauptsaechlich fuer das Debbuging.
 *
 * Asserts:
 *      path_data != NULL
 */
extern void
Print_Path_Data
(
        const struct Path_Data* const restrict path_data        // Path_Data-Objekt, welches ausgegeben werden soll
);

/**
 * Path_Data-Objekt loeschen.
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
