/**
 * Alkane_Container.h
 *
 *  Created on: 10.03.2021
 *      Author: x86 / Gyps
 */

#ifndef ALKANE_CONTAINER_H
#define ALKANE_CONTAINER_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#ifndef ALKANE_CONTAINER_ALLOCATION_STEP_SIZE
    #define ALKANE_CONTAINER_ALLOCATION_STEP_SIZE 512
#else
    #error "The macro \"ALKANE_CONTAINER_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* ALKANE_CONTAINER_ALLOCATION_STEP_SIZE */



#include "Alkane.h"



/**
 * Uebersicht ueber die Zustaende, die ein Alkane_Container annehmen kann. Diese Zustaende dienen hauptsaechlich
 * dazu, dass - ohne Analyse des eigentlichen Objektes - z.B. festgestellt werden kann, ob das Objekt wichtige Daten
 * enthaelt oder ob das Objekt initialisert werden muss.
 */
enum Alkane_Container_State
{
    ALKANE_CONTAINER_CREATED = 0,                   // Objekt wurde erzeugt
    ALKANE_CONTAINER_INITIALIZED_WITH_ZERO_BYTES,   // Objekt wurde mit 0 Bytes initialisiert
    ALKANE_CONTAINER_DELETED,                       // Objekt wurde geloescht und enthaelt keine Informationen mehr
    ALKANE_CONTAINER_INVALID_DATA,                  // Objekt enthaelt ungueltige Daten. Dies ist dann der Fall, wenn
                                                    // z.B. ein ungueltiges Alkan Objekt zum Container hinzugefuegt
                                                    // wurde

    ALKANE_CONTAINER_UNKNOWN_STATE                  // Unbekannter Zustand. Dies ist hauptsaechlich dafuer da, wenn
                                                    // kein anderer Zustand angegeben werden kann
};



/**
 * Dieses Objekt stellt einen einfachen Container fuer Alkane-Objekte dar.
 */
struct Alkane_Container
{
    struct Alkane** data;               // Da die Alkane-Objekte dynamisch erzeugt werden, werden im Container nur die
                                        // Zeiger auf die Alkane-Objekte gespeichert
    uint_fast64_t size;                 // Anzahl an tatsaechlich vorhandenen Objekten
    uint_fast64_t allocated_size;       // Anzahl an allokierten Speicher
                                        // Der Ansatz mit size / allocated_size dient dazu, dass nicht bei jedem neuen
                                        // Objekt ein realloc ()-Aufruf stattfinden muss
    enum Alkane_Container_State state;  // Aktueller Status des Objektes
};








// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_CONTAINER_H */
