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
    if (container->size > container->allocated_size)
    {
        // Neuen groesseren Speicherbereich anfordern
        container->data = (struct Alkane**) REALLOC (container->data, (size_t) (container->allocated_size +
                sizeof (struct Alkane*) * ALKANE_CONTAINER_ALLOCATION_STEP_SIZE));
        ASSERT_ALLOC(container->data, "realloc () call for a Alkane_Container failed !",
                (size_t) (container->allocated_size + sizeof (struct Alkane*) * ALKANE_CONTAINER_ALLOCATION_STEP_SIZE));
        container->allocated_size += ALKANE_CONTAINER_ALLOCATION_STEP_SIZE;

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
