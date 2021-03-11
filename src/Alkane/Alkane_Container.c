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
