/**
 * Alkane_Branch_Container.c
 *
 *  Created on: 09.03.2021
 *      Author: x86
 */

#include "Alkane_Branch_Container.h"
#include <stdlib.h>
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"





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
