/*
 * Path_Data_Container.c
 *
 *  Created on: 12.06.2021
 *      Author: x86 / Gyps
 */

#include "Path_Data_Container.h"
#include <string.h>
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Misc.h"





//---------------------------------------------------------------------------------------------------------------------

/**
 * Ein neuen leeren Path_Data_Container erstellen.
 *
 * Asserts:
 *      N/A
 */
struct Path_Data_Container*
Create_Path_Data_Container
(
        void
)
{
    // Um unused-Warnungen beim Programm Cppcheck zu vermeiden
    (void) Print_Path_Data_Container;

    // Container erzeugen
    struct Path_Data_Container* new_container = (struct Path_Data_Container*) CALLOC (1,
            sizeof (struct Path_Data_Container));
    ASSERT_ALLOC(new_container, "Cannot allocate an Path_Data_Container object !",
            sizeof (struct Path_Data_Container));

    // Speicher innerhalb des Containers erzeugen
    new_container->data = (struct Path_Data**) CALLOC (PATH_DATA_CONTAINER_ALLOCATION_STEP_SIZE,
            sizeof (struct Path_Data*));
    ASSERT_ALLOC(new_container->data, "Cannot allocate memory for the Path_Data_Container content !",
            PATH_DATA_CONTAINER_ALLOCATION_STEP_SIZE * sizeof (struct Path_Data*));

    // Die restlichen Werte initialisieren
    new_container->allocated_size   = PATH_DATA_CONTAINER_ALLOCATION_STEP_SIZE;
    new_container->size             = 0;
    new_container->state            = PATH_DATA_CONTAINER_INITIALIZED_WITH_ZERO_BYTES;

    return new_container;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Ein Path_Data-Objekt zu einem existierenden Path_Data_Container hinzufuegen.
 *
 * Der Container kopiert das Path_Data-Objekt NICHT ! Es wird nur die Speicheradresse im Container abgelegt !
 *
 * Asserts:
 *      container != NULL
 *      new_element != NULL
 */
void
Add_Path_Data_To_Container
(
        struct Path_Data_Container* restrict const container,   // Container, welches das neue Objekt entgegennimmt
        const struct Path_Data* restrict const new_element      // Existierendes Path_Data-Objekt, welches
                                                                // hinzugefuegt werden soll
)
{
    ASSERT_MSG(container != NULL, "container is NULL !");
    ASSERT_MSG(new_element != NULL, "new_element is NULL !");

    // Muss der Speicher im Container vergroessert werden ?
    if (container->size >= container->allocated_size)
    {
        container->allocated_size       += PATH_DATA_CONTAINER_ALLOCATION_STEP_SIZE;
        const size_t new_size_in_byte   = (size_t) (container->allocated_size * sizeof (struct Path_Data*));

        // Neuen groesseren Speicherbereich anfordern
        container->data = (struct Path_Data**) REALLOC (container->data, new_size_in_byte);
        ASSERT_ALLOC(container->data, "realloc () call for a Path_Data_Container failed !", new_size_in_byte);

        // Die neuen Zeiger auf einen definerten Zustand bringen
        // "i = 1", da der neuste Speicherbereich gleich durch das neue Path_Data-Objekt belegt wird
        for (uint_fast64_t i = 1; i < PATH_DATA_CONTAINER_ALLOCATION_STEP_SIZE; ++ i)
        {
            container->data [container->size + i] = NULL;
        }
    }

    // Der Cast ist hier notwendig, damit die beiden const Schluesselwoerter wiederrufen werden. Ohne den Cast entsteht
    // eine Compilerwarnung.
    container->data [container->size] = (struct Path_Data*) new_element;
    container->size ++;

    // Wenn ungueltige Daten zum Container hinzugefuegt werden, dann wird dies auch im Container markiert
    if (new_element->state == PATH_DATA_INVALID_DATA)
    {
        container->state = PATH_DATA_CONTAINER_INVALID_DATA;
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Path_Data_Container Objekt in eine Zeichenkettendarstellung ueberfuehren. Dies ist insbesondere fuer debugging
 * hilfreich.
 *
 * Der Speicher fuer die Zeichenkette muss vom Aufrufer allokiert werden !
 *
 * Size: <Groesse>
 * Allocated size: <allokierte Groesse>
 * State: <Status>
 *
 * Asserts:
 *      container != NULL,
 *      string_memory != NULL,
 *      string_memory_size > 0
 */
const char*                                                     // Adresse der Eingabe-Zeichenkette
Path_Data_Container_To_String
(
        const struct Path_Data_Container* restrict const container,
                                                                // Path_Data_Container, der als Zeichenkette
                                                                // dargestellt werden soll
        char* const restrict string_memory,                     // Speicher, der fuer die Zeichenkettenerzeugung
                                                                // verwendet werden soll
                                                                // Der Speicher muss vorher vom Aufrufer allokiert
                                                                // werden !
        const size_t string_memory_size                         // Groesse des Zeichenkettenspeichers
                                                                // Wenn der Speicher nicht ausreicht, dann wird am
                                                                // vorletzten Zeichen die Zeichenkette abgeschnitten
)
{
    ASSERT_MSG(container != NULL,       "container is NULL !");
    ASSERT_MSG(string_memory != NULL,   "string_memory is NULL !");
    ASSERT_MSG(string_memory_size > 0,  "string_memory_size is 0 !");

    size_t next_free_byte   = 0;                        // Naechstes freies Zeichen im Speicher
    size_t remaining_memory = string_memory_size - 1;   // Noch freie Zeichen im Speicher
    size_t used_char        = 0;                        // Anzahl an Zeichen, die im aktuellen snprintf-Aufruf in den
                                                        // Speicher geschrieben wurde

    // Das size-Attribut in die Zeichenkettendarstellung einbinden
    TO_STRING_HELPER_VA_ARGS("Size: %" PRIuFAST64 "\n", container->size);

    // Das allocated_size-Attribut in die Zeichenkettendarstellung einbinden
    TO_STRING_HELPER_VA_ARGS("Allocated Size: %" PRIuFAST64 "\n", container->allocated_size);

    // Status in die Zeichenkettendarstellung einbringen
    if (remaining_memory == 0) { goto no_remaining_memory; }
    switch (container->state)
    {
    case PATH_DATA_CONTAINER_CREATED:
        snprintf (string_memory + next_free_byte, remaining_memory, "PATH_DATA_CONTAINER_CREATED\n");
        break;
    case PATH_DATA_CONTAINER_DELETED:
        snprintf (string_memory + next_free_byte, remaining_memory, "PATH_DATA_CONTAINER_DELETED\n");
        break;
    case PATH_DATA_CONTAINER_INITIALIZED_WITH_ZERO_BYTES:
        snprintf (string_memory + next_free_byte, remaining_memory,
                "PATH_DATA_CONTAINER_INITIALIZED_WITH_ZERO_BYTES\n");
        break;
    case PATH_DATA_CONTAINER_INVALID_DATA:
        snprintf (string_memory + next_free_byte, remaining_memory, "PATH_DATA_CONTAINER_INVALID_DATA\n");
        break;
    case PATH_DATA_CONTAINER_UNKNOWN_STATE:
        snprintf (string_memory + next_free_byte, remaining_memory, "PATH_DATA_CONTAINER_UNKNOWN_STATE\n");
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
 * Path_Data_Container Objekt auf stdout ausgeben. Hauptsaechlich fuer das Debbuging. Fuer die Konvertierung wird
 * die Funktion "Path_Data_Container_To_String" verwendet.
 *
 * Asserts:
 *      container != NULL
 */
void
Print_Path_Data_Container
(
        const struct Path_Data_Container* const restrict container  // Path_Data_Container, welcher ausgegeben
                                                                    // werden soll
)
{
    ASSERT_MSG(container != NULL, "container is NULL !");

    char container_to_string [150];
    memset (container_to_string, '\0', sizeof (container_to_string));

    // Zeichenkettendarstellung auf stdout ausgeben
    printf ("%s", Path_Data_Container_To_String (container, container_to_string, sizeof (container_to_string)));
    fflush (stdout);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Ein Path_Data_Container loeschen.
 *
 * Beim Loeschen werden die Loeschfunktionen aller enthaltenen Path_Data-Objekte aufgerufen, sodass am Ende
 * sowohl der Container, als auch der Inhalt geloescht wurde.
 *
 * Nach dem Loeschvorgang sollten Zeiger auf den Container - oder deren enthaltenen Path_Data Elemente - nicht mehr
 * dereferenziert werden !
 *
 * Asserts:
 *      container != NULL
 */
void
Delete_Path_Data_Container
(
        struct Path_Data_Container* restrict container  // Path_Data_Container-Objekt, welches geloescht werden soll
)
{
    ASSERT_MSG(container != NULL, "container is NULL !");

    // Status auf "Geloescht" setzen
    container->state = PATH_DATA_CONTAINER_DELETED;

    // Containerinhalt loeschen -> Also alle Path_Data-Objekte
    for (uint_fast64_t i = 0; i < container->size; ++ i)
    {
        Delete_Path_Data (container->data [i]);
        container->data [i] = NULL;
    }

    // Speicherbereich, wo der Containerinhalt lag, loeschen
    FREE_AND_SET_TO_NULL(container->data);

    // Container an sich loeschen
    FREE_AND_SET_TO_NULL(container);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
