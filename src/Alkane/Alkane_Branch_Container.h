/**
 * Alkane_Branch_Container.h
 *
 *  Created on: 09.03.2021
 *      Author: x86 / Gyps
 */

#ifndef ALKANE_BRANCH_CONTAINER_H
#define ALKANE_BRANCH_CONTAINER_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#ifndef ALKANE_BRANCH_CONTAINER_ALLOCATION_STEP_SIZE
    #define ALKANE_BRANCH_CONTAINER_ALLOCATION_STEP_SIZE 512
#else
    #error "The macro \"ALKANE_BRANCH_CONTAINER_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* ALKANE_BRANCH_CONTAINER_ALLOCATION_STEP_SIZE */



#include "Alkane_Branch.h"



/**
 * Uebersicht ueber die Zustaende, die ein Alkane_Branch_Container annehmen kann. Diese Zustaende dienen hauptsaechlich
 * dazu, dass - ohne Analyse des eigentlichen Objektes - z.B. festgestellt werden kann, ob das Objekt wichtige Daten
 * enthaelt oder ob das Objekt initialisert werden muss.
 */
enum Alkane_Branch_Container_State
{
    ALKANE_BRANCH_CONTAINER_CREATED = 0,                    // Objekt wurde erzeugt
    ALKANE_BRANCH_CONTAINER_INITIALIZED_WITH_ZERO_BYTES,    // Objekt wurde mit 0 Bytes initialisiert
    ALKANE_BRANCH_CONTAINER_DELETED,                        // Objekt wurde geloescht und enthaelt keine Informationen
                                                            // mehr
    ALKANE_BRANCH_CONTAINER_INVALID_DATA,                   // Objekt enthaelt ungueltige Daten. Dies ist dann der
                                                            // Fall, wenn z.B. ein ungueltiges Alkan_Branch Objekt zum
                                                            // Container hinzugefuegt wurde

    ALKANE_BRANCH_CONTAINER_UNKNOWN_STATE                   // Unbekannter Zustand. Dies ist hauptsaechlich dafuer da,
                                                            // wenn kein anderer Zustand angegeben werden kann
};



/**
 * Dieses Objekt stellt einen einfachen Container fuer Alkane_Branch-Objekte dar.
 */
struct Alkane_Branch_Container
{
    struct Alkane_Branch** data;                // Da die Alkane_Branch-Objekte dynamisch erzeugt werden, werden im
                                                // Container nur die Zeiger auf die Alkane_Branch-Objekte gespeichert
    uint_fast64_t size;                         // Anzahl an tatsaechlich vorhandenen Objekten
    uint_fast64_t allocated_size;               // Anzahl an allokierten Speicher
                                                // Der Ansatz mit size / allocated_size dient dazu, dass nicht bei
                                                // jedem neuen Objekt ein realloc ()-Aufruf stattfinden muss
    enum Alkane_Branch_Container_State state;   // Aktueller Status des Objektes
};

//=====================================================================================================================

/**
 * Ein neuen leeren Alkane_Branch_Container erstellen.
 *
 * Asserts:
 *      N/A
 */
extern struct Alkane_Branch_Container*
Create_Alkane_Branch_Container
(
        void
);

/**
 * Ein Alkane_Branch-Objekt zu einem existierenden Alkane_Branch_Container hinzufuegen.
 *
 * Der Container kopiert das Alkane_Branch-Objekt NICHT ! Es wird nur die Speicheradresse im Container abgelegt !
 *
 * Asserts:
 *      container != NULL
 *      new_element != NULL
 */
extern void
Add_Alkane_Branch_To_Container
(
        struct Alkane_Branch_Container* restrict const container,   // Container, welches das neue Objekt entgegennimmt
        const struct Alkane_Branch* restrict const new_element      // Existierendes Alkane_Branch-Objekt, welches
                                                                    // hinzugefuegt werden soll
);

/**
 * Alkane_Branch_Container Objekt in eine Zeichenkettendarstellung ueberfuehren. Dies ist insbesondere fuer debugging
 * hilfreich.
 *
 * Der Speicher fuer die Zeichenkette muss vom Aufrufer allokiert werden !
 *
 * Size: <Groesse>
 * Allocated size: <allokierte Groesse>
 * State: <Status>
 *
 * Asserts:
 *      container != NULL,
 *      string_memory != NULL,
 *      string_memory_size > 0
 */
extern const char*                                                     // Adresse der Eingabe-Zeichenkette
Alkane_Branch_Container_To_String
(
        const struct Alkane_Branch_Container* restrict const container,
                                                                // Alkane_Branch, der als Zeichenkette dargestellt
                                                                // werden soll
        char* const restrict string_memory,                     // Speicher, der fuer die Zeichenkettenerzeugung
                                                                // verwendet werden soll
                                                                // Der Speicher muss vorher vom Aufrufer allokiert
                                                                // werden !
        const size_t string_memory_size                         // Groesse des Zeichenkettenspeichers
                                                                // Wenn der Speicher nicht ausreicht, dann wird am
                                                                // vorletzten Zeichen die Zeichenkette abgeschnitten
);

/**
 * Alkane_Branch_Container Objekt auf stdout ausgeben. Hauptsaechlich fuer das Debbuging. Fuer die Konvertierung wird
 * die Funktion "Alkane_Branch_Container_To_String" verwendet.
 *
 * Asserts:
 *      container != NULL
 */
extern void
Print_Alkane_Branch_Container
(
        const struct Alkane_Branch_Container* const restrict container  // Alkane_Branch_Container, welcher ausgegeben
                                                                        // werden soll
);

/**
 * Ein Alkane_Branch_Container loeschen.
 *
 * Beim Loeschen werden die Loeschfunktionen aller enthaltenen Alkane_Branch-Objekte aufgerufen, sodass am Ende
 * sowohl der Container, als auch der Inhalt geloescht wurde.
 *
 * Daher gilt fuer den Container und fuer alle darin enthaltene Elemente:
 * Es muss sichergestellt werden, dass alle Container geloescht werden. Nach dem Loeschen sollte der verwendete Zeiger
 * - und die im Container enthaltenen Zeiger - nicht mehr dereferenziert werden !
 *
 * Asserts:
 *      container != NULL
 */
extern void
Delete_Alkane_Branch_Container
(
        struct Alkane_Branch_Container* restrict container  // Alkane_Branch_Container-Objekt, welches geloescht werden soll
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_ALKANE_BRANCH_CONTAINER_H */
