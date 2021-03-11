/*
 * Alkane_Create_Constitutional_Isomer.c
 *
 *  Created on: 08.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane_Create_Constitutional_Isomer.h"
#include "Alkane_Info_Constitutional_Isomer.h"
#include "../Error_Handling/Assert_Msg.h"





//---------------------------------------------------------------------------------------------------------------------

/**
 * Die Alkan Konstitutionsisomere bei gegebener Anzahl an C-Atomen bestimmen.
 *
 * Die Grundidee stammt aus dem Dokument "Strukturisomere der Alkane" von
 * Sascha Kurz, Universität Bayreuth. E-Mail: sascha.kurz@uni-bayreuth.de
 * und
 * Armin Moritz, Johanneum-Gymnasium in Herborn, E-Mail: arminmoritz@gmx.de
 *
 *
 * >>> Zusammengefasst <<<
 * Der Prozess der Erzeugung laesst sich in drei Schritte unterteilen:
 * 1. Teil: Liste der Alkanaeste erzeugen
 *          Siehe Dokument "Strukturisomere der Alkane" Seite 2 - 8
 * 2. Teil: Alle moeglichen Alkane aus den Alkanastlisten bilden
 *          Siehe Dokument "Strukturisomere der Alkane" Seite 8 - 13
 * 3. Teil: Die Alkane, die genau die gesuchte Laenge haben, auswaehlen
 *          => Dies sind die gesuchten Konstitutionsisomere
 *          Siehe Dokument "Strukturisomere der Alkane" Seite 13 - 14
 *
 * Asserts:
 *      number_of_c_atoms < MAX_NUMBER_OF_C_ATOMS
 */
void Create_Alkane_Constitutional_Isomers
(
       const uint_fast8_t number_of_c_atoms
)
{
    ASSERT_FMSG(number_of_c_atoms <= MAX_NUMBER_OF_C_ATOMS, "Number of C atoms is too large ! Got: %" PRIuFAST8
            "; Max. valid: %" PRIuFAST8,  number_of_c_atoms, MAX_NUMBER_OF_C_ATOMS);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
