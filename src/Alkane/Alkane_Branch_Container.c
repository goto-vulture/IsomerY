/**
 * @file Alkane_Branch_Container.c
 *
 * @brief Wurde durch einen Container, der mittels DMMC.h erstellt wurde, ersetzt !
 */

unsigned char variable_to_prevent_an_warning_because_of_an_empty_translation_unit = 0;

#if 0
/**
 * Alkane_Branch_Container.c
 *
 *  Created on: 09.03.2021
 *      Author: x86
 */

#include "Alkane_Branch_Container.h"
#include <stdlib.h>
#include <string.h>
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "Alkane_Info_Constitutional_Isomer.h"
#include "../Misc.h"





//---------------------------------------------------------------------------------------------------------------------

/**
 * Ein neuen leeren Alkane_Branch_Container erstellen.
 *
 * Asserts:
 *      N/A
 */
struct Alkane_Branch_Container*
Create_Alkane_Branch_Container
(
        void
)
{
    // Um unused-Warnungen beim Programm Cppcheck zu vermeiden
    (void) Print_Alkane_Branch_Container;

    // Container erzeugen
    struct Alkane_Branch_Container* new_container = (struct Alkane_Branch_Container*) CALLOC (1,
            sizeof (struct Alkane_Branch_Container));
    ASSERT_ALLOC(new_container, "Cannot allocate an Alkane_Branch_Container object !",
            sizeof (struct Alkane_Branch_Container));

    // Speicher innerhalb des Containers erzeugen
    new_container->data = (struct Alkane_Branch**) CALLOC (ALKANE_BRANCH_CONTAINER_ALLOCATION_STEP_SIZE,
            sizeof (struct Alkane_Branch*));
    ASSERT_ALLOC(new_container->data, "Cannot allocate memory for the Alkane_Branch_Container content !",
            ALKANE_BRANCH_CONTAINER_ALLOCATION_STEP_SIZE * sizeof (struct Alkane_Branch*));

    // Die restlichen Werte initialisieren
    new_container->allocated_size   = ALKANE_BRANCH_CONTAINER_ALLOCATION_STEP_SIZE;
    new_container->size             = 0;
    new_container->state            = ALKANE_BRANCH_CONTAINER_INITIALIZED_WITH_ZERO_BYTES;

    return new_container;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Ein Alkane_Branch-Objekt zu einem existierenden Alkane_Branch_Container hinzufuegen.
 *
 * Der Container kopiert das Alkane_Branch-Objekt NICHT ! Es wird nur die Speicheradresse im Container abgelegt !
 *
 * Asserts:
 *      container != NULL
 *      new_element != NULL
 */
void
Add_Alkane_Branch_To_Container
(
        struct Alkane_Branch_Container* restrict const container,   // Container, welches das neue Objekt entgegennimmt
        const struct Alkane_Branch* restrict const new_element      // Existierendes Alkane_Branch-Objekt, welches
                                                                    // hinzugefuegt werden soll
)
{
    ASSERT_MSG(container != NULL, "container is NULL !");
    ASSERT_MSG(new_element != NULL, "new_element is NULL !");

    // Muss der Speicher im Container vergroessert werden ?
    if (container->size >= container->allocated_size)
    {
        container->allocated_size       += ALKANE_BRANCH_CONTAINER_ALLOCATION_STEP_SIZE;
        const size_t new_size_in_byte   = (size_t) (container->allocated_size * sizeof (struct Alkane_Branch*));

        // Neuen groesseren Speicherbereich anfordern
        struct Alkane_Branch** temp_ptr = (struct Alkane_Branch**) REALLOC (container->data, new_size_in_byte);

        // Diesmal diese Variante der Nullpointer-Ueberpruefung, da - wenn der realloc () Aufruf fehlschlaegt - der
        // urspruengliche Speicherbereich nicht mehr erreichbar ist !
        if (temp_ptr == NULL)
        {
            FREE_AND_SET_TO_NULL(container->data)
            ASSERT_ALLOC(container->data, "realloc () call for a Alkane_Branch_Container failed !", new_size_in_byte);
        }
        else
        {
            container->data = temp_ptr;
        }

        // Die neuen Zeiger auf einen definerten Zustand bringen
        // "i = 1", da der neuste Speicherbereich gleich durch das neue Alkane_Branch-Objekt belegt wird
        for (uint_fast64_t i = 1; i < ALKANE_BRANCH_CONTAINER_ALLOCATION_STEP_SIZE; ++ i)
        {
            container->data [container->size + i] = NULL;
        }
    }

    // Der Cast ist hier notwendig, damit die beiden const Schluesselwoerter wiederrufen werden. Ohne den Cast entsteht
    // eine Compilerwarnung.
    container->data [container->size] = (struct Alkane_Branch*) new_element;
    container->size ++;

    // Wenn ungueltige Daten zum Container hinzugefuegt werden, dann wird dies auch im Container markiert
    if (new_element->state == ALKANE_BRANCH_INVALID_BRANCH)
    {
        container->state = ALKANE_BRANCH_CONTAINER_INVALID_DATA;
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alkane_Branch_Container Objekt in eine Zeichenkettendarstellung ueberfuehren. Dies ist insbesondere fuer debugging
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
Alkane_Branch_Container_To_String
(
        const struct Alkane_Branch_Container* restrict const container,
                                                                // Alkane_Branch, der als Zeichenkette dargestellt
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
    if (remaining_memory > 0)
    {
        switch (container->state)
        {
        case ALKANE_BRANCH_CONTAINER_CREATED:
            snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_BRANCH_CONTAINER_CREATED\n");
            break;
        case ALKANE_BRANCH_CONTAINER_DELETED:
            snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_BRANCH_CONTAINER_DELETED\n");
            break;
        case ALKANE_BRANCH_CONTAINER_INITIALIZED_WITH_ZERO_BYTES:
            snprintf (string_memory + next_free_byte, remaining_memory,
                    "ALKANE_BRANCH_CONTAINER_INITIALIZED_WITH_ZERO_BYTES\n");
            break;
        case ALKANE_BRANCH_CONTAINER_INVALID_DATA:
            snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_BRANCH_CONTAINER_INVALID_DATA\n");
            break;
        case ALKANE_BRANCH_CONTAINER_UNKNOWN_STATE:
            snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_BRANCH_CONTAINER_UNKNOWN_STATE\n");
            break;

            // Der default-Pfad soll nie ausgefuehrt werden ! Wenn dies dennoch der Fall ist, dann ist dies ein Fehler !
        default:
            ASSERT_MSG(false, "default path of a switch case statement executed !");
        }
    }

    // Dieses Label ist fuer das Makro TO_STRING_HELPER_VA_ARGS erforderlich !
    no_remaining_memory:
    string_memory [string_memory_size - 1] = '\0';  // Nullterminierung garantieren

    return string_memory;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alkane_Branch_Container Objekt auf stdout ausgeben. Hauptsaechlich fuer das Debbuging. Fuer die Konvertierung wird
 * die Funktion "Alkane_Branch_To_String" verwendet.
 *
 * Asserts:
 *      container != NULL
 */
void
Print_Alkane_Branch_Container
(
        const struct Alkane_Branch_Container* const restrict container  // Alkane_Branch_Container, welcher ausgegeben
                                                                        // werden soll
)
{
    ASSERT_MSG(container != NULL, "container is NULL !");

    char container_to_string [150];
    memset (container_to_string, '\0', sizeof (container_to_string));

    // Zeichenkettendarstellung auf stdout ausgeben
    printf ("%s", Alkane_Branch_Container_To_String (container, container_to_string, sizeof (container_to_string)));
    fflush (stdout);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Ein Alkane_Branch_Container loeschen.
 *
 * Beim Loeschen werden die Loeschfunktionen aller enthaltenen Alkane_Branch-Objekte aufgerufen, sodass am Ende
 * sowohl der Container, als auch der Inhalt geloescht wurde.
 *
 * Daher gilt fuer den Container und fuer alle darin enthaltene Elemente:
 * Es muss sichergestellt werden, dass alle Container geloescht werden. Nach dem Loeschen sollte der verwendete Zeiger
 * - und die im Container enthaltenen Zeiger - nicht mehr dereferenziert werden !
 *
 * Asserts:
 *      container != NULL
 */
void
Delete_Alkane_Branch_Container
(
        struct Alkane_Branch_Container* restrict container  // Alkane_Branch_Container-Objekt, welches geloescht werden
                                                            // soll
)
{
    ASSERT_MSG(container != NULL, "container is NULL !");

    // Status auf "Geloescht" setzen
    container->state = ALKANE_BRANCH_CONTAINER_DELETED;

    // Containerinhalt loeschen -> Also alle Alkane_Branch-Objekte
    for (uint_fast64_t i = 0; i < container->size; ++ i)
    {
        Delete_Alkane_Branch (container->data [i]);
        container->data [i] = NULL;
    }

    // Speicherbereich, wo der Containerinhalt lag, loeschen
    FREE_AND_SET_TO_NULL(container->data);

    // Container an sich loeschen
    FREE_AND_SET_TO_NULL(container);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

#endif /* 0 */
