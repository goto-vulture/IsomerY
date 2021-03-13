/**
 * Alkane_Tests.c
 *
 *  Created on: 13.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane_Tests.h"
#include "../Alkane/Alkane_Create_Constitutional_Isomer.h"
#include "../Alkane/Alkane_Info_Constitutional_Isomer.h"





//---------------------------------------------------------------------------------------------------------------------

/**
 * Die Erstellung der Alkane testen. Vorerst nur ueber die Anzahl an Ergebnissen.
 * Die Anzahl an Ergebnissen wird mit der Anzahl an moeglichen Konstitutionsisomeren verglichen. Wenn die Werte gleich
 * sind, dann ist ein Fehler in der Berechnung relativ unwahrscheinlich.
 *
 * Die Liste an moeglichen Konstitutionsisomeren bis 40 sind in der Datei "Alkane_Info_Constitutional_Isomer.h" zu
 * finden.
 *
 * Aus Laufzeitgruenden wird nur bis inklusive 12 getestet.
 */
void TEST_Create_Alkane_Constitutional_Isomers (void)
{
    uint_fast64_t number_of_created_isomers = 0;

    for (uint_fast8_t i = 3; i <= 12; ++ i)
    {
        Create_Alkane_Constitutional_Isomers (i, &number_of_created_isomers);

        ASSERT_EQUALS(NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [i - 1], number_of_created_isomers);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------
