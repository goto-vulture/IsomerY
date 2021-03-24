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
#include <stddef.h>



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
    unsigned char branch [MAX_NUMBER_OF_C_ATOMS];           // Aufbau des Astes (die "verbunden-mit"-Aussagen)
    uint_fast8_t length;                                    // Laenge des Astes - wie viele der Arrayelemente werden
                                                            // tatsaechlich verwendet ?
    enum Alkane_Branch_State state;                         // Status des Objektes
};

//=====================================================================================================================

/**
 * Alkane_Branch Objekt dynamisch erzeugen.
 *
 * Asserts:
 *      lenght > 0 && length <= MAX_NUMBER_OF_C_ATOMS, wenn branch != NULL
 */
extern struct Alkane_Branch*                            // Erzeugtes Alkane_Branch Objekt
Create_Alkane_Branch
(
        const unsigned char* const restrict branch,     // Astinformationen, mit denen das Alkane_Branch Objekt
                                                        // initialisiert werden kann
                                                        // Oder NULL, falls eine Initialisierung mit Nullbyes
                                                        // durchgefuehrt werden soll
        const uint_fast8_t length                       // Laenge der Astinformationen
                                                        // Wenn branch == NULL ist, dann wird dieser Wert ignoriert
);

/**
 * Alkane_Branch Objekt in eine Zeichenkettendarstellung ueberfuehren. Dies ist insbesondere fuer debugging hilfreich.
 *
 * Der Speicher fuer die Zeichenkette muss vom Aufrufer allokiert werden !
 *
 * (<Inhalt des Astes>)
 * Length: <Laenge>
 * State: <Status>
 *
 * Asserts:
 *      branch != NULL,
 *      string_memory != NULL,
 *      string_memory_size > 0
 */
extern const char*                                  // Adresse der Eingabe-Zeichenkette
Alkane_Branch_To_String
(
        const struct Alkane_Branch* const branch,   // Alkane_Branch, der als Zeichenkette dargestellt werden soll
        char* const string_memory,                  // Speicher, der fuer die Zeichenkettenerzeugung verwendet werden
                                                    // soll
                                                    // Der Speicher muss vorher vom Aufrufer allokiert werden
        const size_t string_memory_size             // Groesse des Zeichenkettenspeichers
                                                    // Wenn der Speicher nicht ausreicht, dann wird am vorletzten
                                                    // Zeichen die Zeichenkette abgeschnitten
);

/**
 * Alkane_Branch Objekt auf stdout ausgeben. Hauptsaechlich fuer das Debbuging. Fuer die Konvertierung wird die
 * Funktion "Alkane_Branch_To_String" verwendet.
 *
 * Asserts:
 *      branch != NULL
 */
extern void
Print_Alkane_Branch
(
        const struct Alkane_Branch* const branch    // Alkane_Branch, welches ausgegeben werden soll
);

/**
 * Alkane_Branch Objekt loeschen.
 * Da es sich bei dem Objekt um dynamischen Speicher handelt, muss sichergestellt werden, dass alle Objekte, die zuvor
 * erstellt wurden, geloescht werden. Nach dem Loeschen sollte der verwendete Zeiger nicht mehr dereferenziert werden !
 *
 * Asserts:
 *      branch != NULL
 */
extern void
Delete_Alkane_Branch
(
        struct Alkane_Branch* branch    // Alkane_Branch, welches geloescht werden soll
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_BRANCH_H */
