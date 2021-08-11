/**
 * Alkane_Chain_To_IUPAC_Name.h
 *
 * Konstitutionsisomere in Form von Alkan-Objekten sind schoen und gut. Aber fuer die Verwendung im Bereich der
 * organischen Chemie sind sie unbrauchbar !
 *
 * Es gibt eine Nomenklatur, die von der International Union of Pure and Applied Chemistry (IUPAC) erstellt wurde.
 * (Siehe: https://de.wikipedia.org/wiki/IUPAC)
 *
 * Namen von Chemikalien, die diese Nomenklatur verwenden, werden haeufig IUPAC-Namen genannt. Ich werde in diesem
 * Programm diese Bezeichnung ebenfalls verwenden, da fuer den Chemiker klar ist was genau damit gemeint ist.
 *
 * Ziel dieser Nomenklatur ist, dass die Struktur einer Chemikalie im Namen codiert ist. Es soll also moeglich sein nur
 * anhand des Namens die Struktur des Molekuels zu rekonstruieren.
 *
 * Ein paar Beispiele werden hier vielleicht noch eingefuegt ...
 *
 * Jedenfalls macht diese Funktion die Konvertierung der Alkan-Objekte zu den zugehoerigen IUPAC-Namen.
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
