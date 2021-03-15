/**
 * Alkane_To_IUPAC_Name.c
 *
 *  Created on: 14.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane_To_IUPAC_Name.h"
#include "../Error_Handling/Assert_Msg.h"





//---------------------------------------------------------------------------------------------------------------------

/**
 * IUPAC-Name des uebergebenen Alkans erzeugen.
 *
 * ...
 *
 * Asserts:
 *      alkane != NULL
 */
void Convert_Alkane_To_IUPAC_Name
(
        struct Alkane* const alkane         // Alkan, von dem der IUPAC-Name gebildet werden soll
)
{
    ASSERT_MSG(alkane != NULL, "alkane is NULL !");

    return;
}

//---------------------------------------------------------------------------------------------------------------------
