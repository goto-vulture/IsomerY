/**
 * Alkane_Chain_To_IUPAC_Name.h
 *
 *  Created on: 23.03.2021
 *      Author: x86 / Gyps
 */

#ifndef ALKANE_CHAIN_TO_IUPAC_NAME_H
#define ALKANE_CHAIN_TO_IUPAC_NAME_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "Alkane_To_IUPAC_Name.h"



void
Chain_To_IUPAC
(
        char* const restrict iupac_name,
        const size_t iupac_name_length,
        struct Alkane* const restrict alkane
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_CHAIN_TO_IUPAC_NAME_H */
