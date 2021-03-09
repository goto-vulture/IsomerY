/**
 * Alkane_Branch.h
 *
 *  Created on: 08.03.2021
 *      Author: x86 / Gyps
 */

#ifndef ALKANE_BRANCH_H
#define ALKANE_BRANCH_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "Alkane_Info_Constitutional_Isomer.h"



/**
 * Uebersicht ueber die Zustaende, die ein Alkane_Branch-Objekt annehmen kann. Diese Zustaende dienen hauptsaechlich
 * dazu, dass - ohne Analyse des eigentlichen Objektes - z.B. festgestellt werden kann, ob das Objekt wichtige Daten
 * enthaelt oder ob das Objekt initialisert werden muss.
 */
enum Alkane_Branch_State
{
    ALKANE_BRANCH_CREATED = 0,                  // Objekt wurde erzeugt
    ALKANE_BRANCH_INITIALIZED_WITH_ZERO_BYTES,  // Objekt wurde mit 0 Bytes initialisiert
    ALKANE_BRANCH_INITIALIZED_WITH_DATA,        // Objekt wurde mit Daten initialisert -> Es befinden sich also
                                                // richtige Informationen in dem Objekt
    ALKANE_BRANCH_DELETED,                      // Objekt wurde geloescht und enthaelt keine Informationen mehr
    ALKANE_BRANCH_INVALID_BRANCH,               // Objekt enthaelt ungueltige Astinformationen (Dieser Zustand sollte
                                                // in der Praxis nicht vorkommen - aber man weiss ja nie)

    ALKANE_BRANCH_UNKNOWN_STATE                 // Unbekannter Zustand. Dies ist hauptsaechlich dafuer da, wenn kein
                                                // anderer Zustand angegeben werden kann
};

/**
 * Dieses Objekt stellt ein Alkanast dar.
 *
 * Die Grundidee stammt aus dem Dokument "Strukturisomere der Alkane" Seite 15 von
 * Sascha Kurz, Universität Bayreuth. E-Mail: sascha.kurz@uni-bayreuth.de
 * und
 * Armin Moritz, Johanneum-Gymnasium in Herborn, E-Mail: arminmoritz@gmx.de
 *
 * Zusammengefasst:
 * Die Zahlen im branch-Array stellen eine "verbunden mit"-Aussage dar.
 * Z.B.:
 * (1, 1, 1, 2, 3)
 * Das 4. Element (die '2') besagt: Das 4. C-Atom ist mit dem 2. C-Atom verbunden.
 * Das 5. Element (die '3') besagt: Das 5. C-Atom ist mit dem 3. C-Atom verbunden.
 *
 * Einzige Ausnahme: Das 1. Element. Dies stellt keine "verbunden mit"-Aussage dar ! Das geht an dieser Stelle auch
 * nicht, da die "verbunden mit"-Aussage sich immer auf bereits vorhandene C-Atome bezieht; und ist natuerlich beim
 * ersten Element nicht moeglich.
 */
struct Alkane_Branch
{
    unsigned char branch [(MAX_NUMBER_OF_C_ATOMS + 1) / 2]; // Aufbau des Astes (die "verbunden-mit"-Aussagen)
                                                            // (MAX_NUMBER_OF_C_ATOMS + 1) / 2 -> Die Haelfte
                                                            // aufgerundet !
    uint_fast8_t length;                                    // Laenge des Astes - wie viele der Arrayelemente werden
                                                            // tatsaechlich verwendet ?
    enum Alkane_Branch_State state;                         // Status des Objektes
};



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_BRANCH_H */
