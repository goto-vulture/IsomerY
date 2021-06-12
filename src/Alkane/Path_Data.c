/*
 * Path_Data.c
 *
 *  Created on: 12.06.2021
 *      Author: x86 / Gyps
 */

#include "Path_Data.h"
#include <string.h>
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Error_Handling/Assert_Msg.h"
#include "../Misc.h"





//---------------------------------------------------------------------------------------------------------------------

/**
 * Ein neues Path_Data-Objekt erstellen.
 *
 * Asserts:
 *      N/A
 */
struct Path_Data*
Create_Path_Data
(
        void
)
{
    // Neues Objekt erzeugen
    struct Path_Data* new_path_data = (struct Path_Data*) CALLOC (1, sizeof (struct Path_Data));
    ASSERT_ALLOC(new_path_data, "Cannot allocate an Path_Data object !", sizeof (struct Path_Data));

    new_path_data->state = PATH_DATA_INITIALIZED_WITH_ZERO_BYTES;

    return new_path_data;
}

//---------------------------------------------------------------------------------------------------------------------

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
)
{
    ASSERT_MSG(path_data != NULL,       "path_data is NULL !");
    ASSERT_MSG(string_memory != NULL,   "string_memory is NULL !");
    ASSERT_MSG(string_memory_size > 0,  "string_memory_size is 0 !");

    size_t next_free_byte   = 0;                        // Naechstes freies Zeichen im Speicher
    size_t remaining_memory = string_memory_size - 1;   // Noch freie Zeichen im Speicher
    size_t used_char        = 0;                        // Anzahl an Zeichen, die im aktuellen snprintf-Aufruf in den
                                                        // Speicher geschrieben wurde

    TO_STRING_HELPER("Adj Matrix (first 10 elements):\n");

    // Die ersten 10 Elemente der Adjazenzmatrix in die Zeichenkettendarstellung einbauen
    for (uint_fast8_t i = 0; i < 10; i ++)
    {
        for (uint_fast8_t i2 = 0; i2 < 10; i2 ++)
        {
            TO_STRING_HELPER_VA_ARGS(PRIuFAST8, path_data->adj_matrix [i][i2]);
        }

        TO_STRING_HELPER("\n");
    }

    TO_STRING_HELPER_VA_ARGS("Start node: %" PRIuFAST8 "\n", path_data->start_node);
    TO_STRING_HELPER_VA_ARGS("Goal node: %" PRIuFAST8 "\n", path_data->goal_node);
    TO_STRING_HELPER_VA_ARGS("Current node: %" PRIuFAST8 "\n\n", path_data->current_node);

    TO_STRING_HELPER("Result path: { ");
    for (uint_fast8_t i = 0; i < path_data->result_path_length; i ++)
    {
        TO_STRING_HELPER_VA_ARGS("%2" PRIuFAST8, path_data->result_path [i]);

        if ((i + 1) < path_data->result_path_length)
        {
            TO_STRING_HELPER(", ");
        }
    }
    TO_STRING_HELPER(" }\n");
    TO_STRING_HELPER_VA_ARGS("Result path length: %" PRIuFAST8 "\n", path_data->result_path_length);
    TO_STRING_HELPER_VA_ARGS("Path index: %" PRIuFAST8 "\n\n", path_data->path_index);

    TO_STRING_HELPER_VA_ARGS("Max possible nesting depth: %" PRIuFAST8 "\n\n", path_data->max_possible_nesting_depth);

    TO_STRING_HELPER("State: ");

    // Status in die Zeichenkettendarstellung einbringen
    if (remaining_memory == 0) { goto no_remaining_memory; }
    switch (path_data->state)
    {
    case PATH_DATA_CREATED:
        snprintf (string_memory + next_free_byte, remaining_memory, "PATH_DATA_CREATED\n");
        break;
    case PATH_DATA_DELETED:
        snprintf (string_memory + next_free_byte, remaining_memory, "PATH_DATA_DELETED\n");
        break;
    case PATH_DATA_INITIALIZED_WITH_ZERO_BYTES:
        snprintf (string_memory + next_free_byte, remaining_memory, "PATH_DATA_INITIALIZED_WITH_ZERO_BYTES\n");
        break;
    case PATH_DATA_INVALID_DATA:
        snprintf (string_memory + next_free_byte, remaining_memory, "PATH_DATA_INVALID_DATA\n");
        break;
    case PATH_DATA_UNKNOWN_STATE:
        snprintf (string_memory + next_free_byte, remaining_memory, "PATH_DATA_UNKNOWN_STATE\n");
        break;

        // Der default-Pfad soll nie ausgefuehrt werden ! Wenn dies dennoch der Fall ist, dann ist dies ein Fehler !
    default:
        ASSERT_MSG(false, "default path of a switch case statement executed !");
    }

    no_remaining_memory:
    string_memory [string_memory_size - 1] = '\0';  // Nullterminierung garantieren

    return string_memory;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Path_Data-Objekt auf stdout ausgeben. Hauptsaechlich fuer das Debbuging.
 *
 * Asserts:
 *      path_data != NULL
 */
extern void
Print_Path_Data
(
        const struct Path_Data* const restrict path_data    // Path_Data-Objekt, welches ausgegeben werden soll
)
{
    ASSERT_MSG(path_data != NULL, "path_data is NULL !");

    char path_data_to_string [1024];
    memset (path_data_to_string, '\0', sizeof (path_data_to_string));

    // Zeichenkettendarstellung auf stdout ausgeben
    printf ("%s", Path_Data_To_String (path_data, path_data_to_string, sizeof (path_data_to_string)));
    fflush (stdout);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Path_Data-Objekt loeschen.
 *
 * Asserts:
 *      path_data != NULL
 */
extern void
Delete_Path_Data
(
        struct Path_Data* restrict path_data
)
{
    ASSERT_MSG(path_data != NULL, "path_data is NULL !");

    path_data->state = PATH_DATA_DELETED;

    FREE_AND_SET_TO_NULL(path_data);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
