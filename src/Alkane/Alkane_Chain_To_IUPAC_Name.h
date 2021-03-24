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



/**
 * Diese Funktion konvertiert die zuvor erzeugten Chain-Objekte in den IUPAC-Namen.
 * Intern arbeitet dieser Prozess mit zustandsbasierten statischen Funktionen.
 */
void
Chain_To_IUPAC
(
        char* const restrict iupac_name,        // Speicher fuer den IUPAC-Namen
        const size_t iupac_name_length,         // Groesse des Speichers, welches fuer den IUPAC-Namen reserviert wurde
        struct Alkane* const restrict alkane    // Alkan-Objekt (enthaelt die Chain-Objekte)
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_CHAIN_TO_IUPAC_NAME_H */
