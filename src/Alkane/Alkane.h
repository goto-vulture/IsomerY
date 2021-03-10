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

    unsigned char merged_numbercode [MAX_NUMBER_OF_C_ATOMS];    // Zusammengefuehrter Zahlencode
                                                                // Hiermit wird die genaue Struktur des Alkans bestimmt
    uint_fast8_t number_of_c_atoms;                             // Anzahl an C-Atomen im kompletten Alkan

    enum Alkane_State state;                                    // Status des Objektes
};

//=====================================================================================================================

/**
 * Alkane Objekt dynamisch erzeugen.
 *
 * Alle Zeiger duerfen NULL sein, in diesem Fall wird davon ausgegangen, dass dieser Ast nur aus einem H-Atom besteht.
 * Damit die Adressen, die ungleich NULL sind, aufsteigend im Alkane-Objekt gespeichert werden, (dies vereinfacht die
 * weiteren Operationen) werden die uebergebenen Adressen im Objekt neu angeordnet, wenn die Alkane_Branch-Objekte
 * nicht "von links nach rechts" der Funktion uebergeben werden.
 *
 * Asserts:
 *      N/A
 */
extern struct Alkane*                   // Erzeugtes Alkane-Objekt
Create_Alkane
(
        struct Alkane_Branch* branch_1, // 1. Ast des neuen Alkane-Objektes
        struct Alkane_Branch* branch_2, // 2. Ast des neuen Alkane-Objektes
        struct Alkane_Branch* branch_3, // 3. Ast des neuen Alkane-Objektes
        struct Alkane_Branch* branch_4  // 4. Ast des neuen Alkane-Objektes
);

/**
 * Alkane Objekt in eine Zeichenkettendarstellung ueberfuehren. Dies ist insbesondere fuer debugging hilfreich.
 *
 * Der Speicher fuer die Zeichenkette muss vom Aufrufer allokiert werden !
 *
 * Branch 1:
 * <To_String Darstellung des 1. Astes>
 * Branch 2:
 * <To_String Darstellung des 2. Astes>
 * Branch 3:
 * <To_String Darstellung des 3. Astes>
 * Branch 4:
 * <To_String Darstellung des 4. Astes>
 *
 * (<Aufbau des Alkans>)
 *
 * Length: <Laenge>
 * State: <Status>
 *
 * Asserts:
 *      alkane != NULL,
 *      string_memory != NULL,
 *      string_memory_size > 0
 */
extern const char*                          // Adresse der Eingabe-Zeichenkette
Alkane_To_String
(
        const struct Alkane* const branch,  // Alkane der als Zeichenkette dargestellt werden soll
        char* const string_memory,          // Speicher, der fuer die Zeichenkettenerzeugung verwendet werden soll
                                            // Der Speicher muss vorher vom Aufrufer allokiert werden
        const size_t string_memory_size     // Groesse des Zeichenkettenspeichers
                                            // Wenn der Speicher nicht ausreicht, dann wird am vorletzten Zeichen die
                                            // Zeichenkette abgeschnitten
);

/**
 * AlkaneObjekt auf stdout ausgeben. Hauptsaechlich fuer das Debbuging. Fuer die Konvertierung wird die Funktion
 * "Alkane_To_String" verwendet.
 *
 * Asserts:
 *      alkane != NULL
 */
extern void
Print_Alkane
(
        const struct Alkane* const alkane   // Alkane, welches ausgegeben werden soll
);

/**
 * Alkane Objekt loeschen.
 *
 * Die enthaltenen Alkane_Branch-Objekte werden NICHT geloescht, wenn das Alkane-Objekt geloescht wird. Es werden
 * lediglich die Zeiger des Objektes auf NULL gesetzt !
 *
 * Da es sich bei dem Objekt um dynamischen Speicher handelt, muss sichergestellt werden, dass alle Objekte, die zuvor
 * erstellt wurden, geloescht werden. Nach dem Loeschen sollte der verwendete Zeiger nicht mehr dereferenziert werden !
 *
 * Asserts:
 *      alkane != NULL
 */
extern void
Delete_Alkane
(
        struct Alkane* alkane   // Alkane, welches geloescht werden soll
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_H */
