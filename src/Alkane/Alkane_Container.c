/**
 * Alkane_Container.c
 *
 *  Created on: 10.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane_Container.h"
#include <stdlib.h>
#include <string.h>
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "Alkane_Info_Constitutional_Isomer.h"





//---------------------------------------------------------------------------------------------------------------------

/**
 * Ein neuen leeren Alkane_Container erstellen.
 *
 * Asserts:
 *      N/A
 */
struct Alkane_Container*
Create_Alkane_Container
(
        void
)
{
    // Container erzeugen
    struct Alkane_Container* new_container = (struct Alkane_Container*) CALLOC (1, sizeof (struct Alkane_Container));
    ASSERT_ALLOC(new_container, "Cannot allocate an Alkane_Container object !", sizeof (struct Alkane_Container));

    // Speicher innerhalb des Containers erzeugen
    new_container->data = (struct Alkane**) CALLOC (ALKANE_CONTAINER_ALLOCATION_STEP_SIZE, sizeof (struct Alkane*));
    ASSERT_ALLOC(new_container->data, "Cannot allocate memory for the Alkane_Container content !",
            ALKANE_CONTAINER_ALLOCATION_STEP_SIZE * sizeof (struct Alkane*));

    // Die restlichen Werte initialisieren
    new_container->allocated_size   = ALKANE_CONTAINER_ALLOCATION_STEP_SIZE;
    new_container->size             = 0;
    new_container->state            = ALKANE_CONTAINER_INITIALIZED_WITH_ZERO_BYTES;

    return new_container;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Ein Alkane-Objekt zu einem existierenden Alkane_Container hinzufuegen.
 *
 * Der Container kopiert das Alkane-Objekt NICHT ! Es wird nur die Speicheradresse im Container abgelegt !
 *
 * Asserts:
 *      container != NULL
 *      new_element != NULL
 */
void
Add_Alkane_To_Container
(
        struct Alkane_Container* const container,   // Container, welches das neue Objekt entgegennimmt
        const struct Alkane* const new_element      // Existierendes Alkane_Branch-Objekt, welches hinzugefuegt werden
                                                    // werden soll
)
{
    ASSERT_MSG(container != NULL, "container is NULL !");
    ASSERT_MSG(new_element != NULL, "new_element is NULL !");

    // Muss der Speicher im Container vergroessert werden ?
    if (container->size >= container->allocated_size)
    {
        container->allocated_size       += ALKANE_CONTAINER_ALLOCATION_STEP_SIZE;
        const size_t new_size_in_byte   = (size_t) (container->allocated_size * sizeof (struct Alkane*));

        // Neuen groesseren Speicherbereich anfordern
        container->data = (struct Alkane**) REALLOC (container->data, new_size_in_byte);
        ASSERT_ALLOC(container->data, "realloc () call for a Alkane_Container failed !", new_size_in_byte);

        // Die neuen Zeiger auf einen definerten Zustand bringen
        // "i = 1", da der neuste Speicherbereich gleich durch das neue Alkane-Objekt belegt wird
        for (uint_fast64_t i = 1; i < ALKANE_CONTAINER_ALLOCATION_STEP_SIZE; ++ i)
        {
            container->data [container->size + i] = NULL;
        }
    }

    // Der Cast ist hier notwendig, damit die beiden const Schluesselwoerter wiederrufen werden. Ohne den Cast entsteht
    // eine Compilerwarnung.
    container->data [container->size] = (struct Alkane*) new_element;
    container->size ++;

    // Wenn ungueltige Daten zum Container hinzugefuegt werden, dann wird dies auch im Container markiert
    if (new_element->state == ALKANE_INVALID_BRANCH)
    {
        container->state = ALKANE_CONTAINER_INVALID_DATA;
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alkane_Container Objekt in eine Zeichenkettendarstellung ueberfuehren. Dies ist insbesondere fuer debugging
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
const char*                                             // Adresse der Eingabe-Zeichenkette
Alkane_Container_To_String
(
        const struct Alkane_Container* const container, // Alkane_Branch, der als Zeichenkette dargestellt werden soll
        char* const string_memory,                      // Speicher, der fuer die Zeichenkettenerzeugung verwendet
                                                        // verwendet werden soll
                                                        // Der Speicher muss vorher vom Aufrufer allokiert werden !
        const size_t string_memory_size                 // Groesse des Zeichenkettenspeichers
                                                        // Wenn der Speicher nicht ausreicht, dann wird am vorletzten
                                                        // Zeichen die Zeichenkette abgeschnitten
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
    case ALKANE_CONTAINER_CREATED:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_CONTAINER_CREATED\n");
        break;
    case ALKANE_CONTAINER_DELETED:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_CONTAINER_DELETED\n");
        break;
    case ALKANE_CONTAINER_INITIALIZED_WITH_ZERO_BYTES:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_CONTAINER_INITIALIZED_WITH_ZERO_BYTES\n");
        break;
    case ALKANE_CONTAINER_INVALID_DATA:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_CONTAINER_INVALID_DATA\n");
        break;
    case ALKANE_CONTAINER_UNKNOWN_STATE:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_CONTAINER_UNKNOWN_STATE\n");
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
 * Ein Alkane_Container loeschen.
 *
 * Beim Loeschen werden die Loeschfunktionen aller enthaltenen Alkane-Objekte aufgerufen, sodass am Ende sowohl der
 * Container, als auch der Inhalt geloescht wurde.
 *
 * Daher gilt fuer den Container und fuer alle darin enthaltene Elemente:
 * Es muss sichergestellt werden, dass alle Container geloescht werden. Nach dem Loeschen sollte der verwendete Zeiger
 * - und die im Container enthaltenen Zeiger - nicht mehr dereferenziert werden !
 *
 * Asserts:
 *      container != NULL
 */
void
Delete_Alkane_Container
(
        struct Alkane_Container* container  // Alkane_Container-Objekt, welches geloescht werden soll
)
{
    ASSERT_MSG(container != NULL, "container is NULL !");

    // Status auf "Geloescht" setzen
    container->state = ALKANE_CONTAINER_DELETED;

    // Containerinhalt loeschen -> Also alle Alkane-Objekte
    for (uint_fast64_t i = 0; i < container->size; ++ i)
    {
        Delete_Alkane (container->data [i]);
        container->data [i] = NULL;
    }

    // Speicherbereich, wo der Containerinhalt lag, loeschen
    FREE_AND_SET_TO_NULL(container->data);

    // Container an sich loeschen
    FREE_AND_SET_TO_NULL(container);

    return;
}

//---------------------------------------------------------------------------------------------------------------------