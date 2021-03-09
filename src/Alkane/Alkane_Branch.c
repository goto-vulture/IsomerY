/**
 * Alkane_Branch.c
 *
 *  Created on: 08.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane_Branch.h"
#include <stdlib.h>
#include <string.h>
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"





//---------------------------------------------------------------------------------------------------------------------

/**
 * Alkane_Branch Objekt dynamisch erzeugen.
 *
 * Asserts:
 *      lenght > 0 && length < (MAX_NUMBER_OF_C_ATOMS + 1) / 2, wenn branch != NULL
 */
struct Alkane_Branch*                       // Erzeugtes Alkane_Branch Objekt
Create_Alkane_Branch
(
        const unsigned char* const restrict branch,
                                            // Astinformationen, mit denen das Alkane_Branch Objekt initialisiert
                                            // werden kann
                                            // Oder NULL, falls eine Initialisierung mit Nullbyes durchgefuehrt werden
                                            // soll
        const uint_fast8_t length           // Laenge der Astinformationen
                                            // Wenn branch == NULL ist, dann wird dieser Wert ignoriert
)
{
    // Wenn Astinformationen vorhanden sind, dann muss die Laengenangabe ueberprueft werden
    if (branch != NULL)
    {
        ASSERT_MSG(length > 0, "The length for the branch values is 0 !")
        ASSERT_MSG(length <= (MAX_NUMBER_OF_C_ATOMS + 1) / 2, "The length value is too large !")
    }

    // Neues Objekt anlegen und Allokation ueberpruefen
    struct Alkane_Branch* new_branch = (struct Alkane_Branch*) calloc (1, sizeof (struct Alkane_Branch));
    ASSERT_ALLOC(new_branch, "Cannot allocate an Alkane_Branch object !", sizeof (struct Alkane_Branch))

    // Neues Objekt mit den uebergebenen Astinformationen initialisieren, falls vorhanden
    if (branch != NULL)
    {
        memcpy (new_branch->branch, branch, length * sizeof (unsigned char));
        new_branch->length = length;
        new_branch->state = ALKANE_BRANCH_INITIALIZED_WITH_DATA;
    }
    // Falls keine Initialisierungsinformationen vorhanden sind, dann wird das Objekt mit Nullbytes initialisiert
    else
    {
        memset (new_branch, '\0', sizeof (struct Alkane_Branch));
        new_branch->state = ALKANE_BRANCH_INITIALIZED_WITH_ZERO_BYTES;
    }

    return new_branch;
}

//---------------------------------------------------------------------------------------------------------------------
