/**
 * Alkane_Tests.h
 *
 *  Created on: 13.03.2021
 *      Author: x86 / Gyps
 */

#ifndef TESTS_ALKANE_TESTS_H
#define TESTS_ALKANE_TESTS_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "tinytest.h"



/**
 * Die Erstellung von Alkanen aus Alkanaesten erstellen.
 *
 * Wird der Nummerncode richtig gebildet ?
 */
extern void TEST_Create_Alkane (void);

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
extern void TEST_Create_Alkane_Constitutional_Isomers (void);

/**
 * Die Erstellung des IUPAC-Namen aus einem Alkan testen.
 *
 * Wird der IUPAC-Name richtig gebildet ?
 */
extern void TEST_Convert_Alkane_To_IUPAC_Name (void);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TESTS_ALKANE_TESTS_H */
