/**
 * Alkane.h
 *
 *  Created on: 10.03.2021
 *      Author: x86 / Gyps
 */

#ifndef ALKANE_H
#define ALKANE_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "Alkane_Branch.h"



/**
 * Uebersicht ueber die Zustaende, die ein Alkane-Objekt annehmen kann. Diese Zustaende dienen hauptsaechlich dazu,
 * dass - ohne Analyse des eigentlichen Objektes - z.B. festgestellt werden kann, ob das Objekt wichtige Daten
 * enthaelt oder ob das Objekt initialisert werden muss.
 */
enum Alkane_State
{
    ALKANE_CREATED = 0,                 // Objekt wurde erzeugt
    ALKANE_INITIALIZED_WITH_ZERO_BYTES, // Objekt wurde mit 0 Bytes initialisiert
    ALKANE_INITIALIZED_WITH_DATA,       // Objekt wurde mit Daten initialisert -> Es befinden sich also richtige
                                        // Informationen in dem Objekt
    ALKANE_DELETED,                     // Objekt wurde geloescht und enthaelt keine Informationen mehr
    ALKANE_INVALID_BRANCH,              // Objekt enthaelt ungueltige Informationen (Dieser Zustand sollte in der
                                        // Praxis nicht vorkommen - aber man weiss ja nie)

    ALKANE_UNKNOWN_STATE                // Unbekannter Zustand. Dies ist hauptsaechlich dafuer da, wenn kein anderer
                                        // Zustand angegeben werden kann
};



/**
 * Dieses Objekt stellt ein komplettes Alkanmolekuel dar.
 *
 * Ein Alkan besteht aus genau 4 Alkane_Branch-Objekte.
 *
 * Die Struktur wird durch einen Zahlencode - wie bei dem Alkane_Branch-Objekt - beschrieben.
 *
 * Nochmal kurz den Aufbau des Zahlencodes zusammengefasst:
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
struct Alkane
{
    struct Alkane_Branch* branches [4];                         // Die Alkane_Branch-Objekte
    uint_fast8_t next_free_branch;                              // Index des naechsten freien Alkane_Branch Zeigers
    uint_fast8_t number_of_c_atoms;                             // Anzahl an C-Atomen im kompletten Alkan

    unsigned char merged_numbercode [MAX_NUMBER_OF_C_ATOMS];    // Zusammengefuehrter Zahlencode
                                                                // Hiermit wird die genaue Struktur des Alkans bestimmt

    enum Alkane_State state;                                    // Status des Objektes
};



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_H */
