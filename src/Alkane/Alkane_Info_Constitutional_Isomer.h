/**
 * Alkane_Info_Constitutional_Isomer.h
 *
 *  Created on: 07.03.2021
 *      Author: x86
 */

#ifndef ALKANE_INFO_CONSTITUTIONAL_ISOMER_H
#define ALKANE_INFO_CONSTITUTIONAL_ISOMER_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */


#ifndef MAX_NUMBER_OF_C_ATOMS
    #define MAX_NUMBER_OF_C_ATOMS 40
#else
    #error "The macro \"MAX_NUMBER_OF_C_ATOMS\" is already defined !"
#endif /* MAX_NUMBER_OF_C_ATOMS */



#include <inttypes.h>



// Anzahl der Konsitutionsisomere bei gegebener Anzahl an C-Atomen
extern const uint_fast64_t NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [MAX_NUMBER_OF_C_ATOMS];



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_INFO_CONSTITUTIONAL_ISOMER_H */
