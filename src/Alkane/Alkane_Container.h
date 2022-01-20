/**
 * @file Alkane_Container.h
 *
 * @brief Einfacher dynamischer Conainer fuer Alkane-Objekte. Bzw. genauer gesagt fuer deren Zeiger.
 *
 * Es wird fuer jedes neue Element im Container kein realloc aufgerufen, sondern es werden blockweise neue Zeiger
 * angefordert, wenn der Container voll ist. Die Groesse der neu allokierten Bloecke kann mittels der Konstante
 * ALKANE_CONTAINER_ALLOCATION_STEP_SIZE festgelegt werden. Meiner bisherigen Erfahrungen nach bietet sich eine Groesse
 * ab 256 an, um das Programm nicht durch zu viele realloc Aufrufe zu verlangsamen. (realloc kopiert den kompletten
 * Speicher um, falls notwendig !)
 *
 * Man hat hier also - wie so oft - die Abwaegung zwische Laufzeit und Speicherverbrauch.
 *
 * @date 10.03.2021
 * @author x86 / Gyps
 */

#ifndef ALKANE_CONTAINER_H
#define ALKANE_CONTAINER_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/**
 * @brief Anzahl an Elemente, die bei einem realloc-Aufruf zusaetzlich angefordert werden.
 */
#ifndef ALKANE_CONTAINER_ALLOCATION_STEP_SIZE
    #define ALKANE_CONTAINER_ALLOCATION_STEP_SIZE 512
#else
    #error "The macro \"ALKANE_CONTAINER_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* ALKANE_CONTAINER_ALLOCATION_STEP_SIZE */



#include "Alkane.h"



/**
 * @brief Uebersicht ueber die Zustaende, die ein Alkane_Container annehmen kann.
 *
 * Diese Zustaende dienen hauptsaechlich dazu, dass - ohne Analyse des eigentlichen Objektes - z.B. festgestellt werden
 * kann, ob das Objekt wichtige Daten enthaelt oder ob das Objekt initialisert werden muss.
 */
enum Alkane_Container_State
{
    ALKANE_CONTAINER_CREATED = 0,                   ///< Objekt wurde erzeugt
    ALKANE_CONTAINER_INITIALIZED_WITH_ZERO_BYTES,   ///< Objekt wurde mit 0 Bytes initialisiert
    ALKANE_CONTAINER_DELETED,                       ///< Objekt wurde geloescht und enthaelt keine Informationen mehr

    /**
     * @brief Objekt enthaelt ungueltige Daten.
     *
     * Dies ist dann der Fall, wenn z.B. ein ungueltiges Alkan Objekt zum Container hinzugefuegt wurde
     */
    ALKANE_CONTAINER_INVALID_DATA,

    /**
     * @brief Unbekannter Zustand.
     *
     * Dies ist hauptsaechlich dafuer da, wenn kein anderer Zustand angegeben werden kann
     */
    ALKANE_CONTAINER_UNKNOWN_STATE
};



/**
 * @brief Dieses Objekt stellt einen einfachen Container fuer Alkane-Objekte dar.
 */
struct Alkane_Container
{
    /**
     * @brief Da die Alkane-Objekte dynamisch erzeugt werden, werden im Container nur die Zeiger auf die Alkane
     * Objekte gespeichert.
     */
    struct Alkane** data;

    uint_fast64_t size;                 ///< Anzahl an tatsaechlich vorhandenen Objekten

    /**
     * @brief Anzahl an allokierten Speicher
     *
     * Der Ansatz mit size / allocated_size dient dazu, dass nicht bei jedem neuen Objekt ein realloc-Aufruf
     * stattfinden muss
     */
    uint_fast64_t allocated_size;
    enum Alkane_Container_State state;  ///< Aktueller Status des Objektes
};

//=====================================================================================================================

/**
 * @brief Ein neuen leeren Alkane_Container erstellen.
 *
 * Asserts:
 *      N/A
 *
 * @return Zeiger auf den erstellten Alkane_Container
 */
extern struct Alkane_Container*
Create_Alkane_Container
(
        void
);

/**
 * @brief Ein Alkane-Objekt zu einem existierenden Alkane_Container hinzufuegen.
 *
 * Der Container kopiert das Alkane-Objekt NICHT ! Es wird nur die Speicheradresse im Container abgelegt !
 *
 * Asserts:
 *      container != NULL
 *      new_element != NULL
 *
 * @param[in] container Alkane_Container, der das Objekt sichert
 * @param[in] new_element Alkane Objekt (bzw. ein Zeiger darauf), der im Container gespeichert werden soll
 */
extern void
Add_Alkane_To_Container
(
        struct Alkane_Container* const restrict container,  // Container, welches das neue Objekt entgegennimmt
        const struct Alkane* const restrict new_element     // Existierendes Alkane Objekt, welches hinzugefuegt
                                                            // werden soll
);

/**
 * @brief Ein Alkane-Objekt zu einem existierenden Alkane_Container hinzufuegen.
 *
 * Bei dieser Funktion kopiert der Container das uebergebene Alkane-Objekt komplett !
 *
 * Asserts:
 *      container != NULL
 *      new_element != NULL
 *
 * @param[in] container Alkane_Container, welcher das neue Objekt entgegennimmt
 * @param[in] new_element Alkane Objekt, welches TIEF KOPIERT im Container gesichert wird
 */
extern void
Add_Full_Alkane_To_Container
(
        struct Alkane_Container* const restrict container,  // Container, welches das neue Objekt entgegennimmt
        const struct Alkane* const restrict new_element     // Existierendes Alkane_Branch-Objekt, welches hinzugefuegt
                                                            // werden soll
);

/**
 * @brief Alkane_Container Objekt in eine Zeichenkettendarstellung ueberfuehren. Dies ist insbesondere fuer debugging
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
 *
 * @param[in] container Alkane_Container, dessen Stringdarstellung gebildet wird
 * @param[out] string_memory C-String Speicher fuer die Zeichenkette
 * @param[in] string_memory_size Groesse des Speichers, welches fuer die Zeichenkettendarstellung zur Verfuegung steht
 */
extern const char*                                      // Adresse der Eingabe-Zeichenkette
Alkane_Container_To_String
(
        const struct Alkane_Container* const restrict container,
                                                        // Alkane_Branch, der als Zeichenkette dargestellt werden soll
        char* const restrict string_memory,             // Speicher, der fuer die Zeichenkettenerzeugung verwendet
                                                        // verwendet werden soll
                                                        // Der Speicher muss vorher vom Aufrufer allokiert werden !
        const size_t string_memory_size                 // Groesse des Zeichenkettenspeichers
                                                        // Wenn der Speicher nicht ausreicht, dann wird am vorletzten
                                                        // Zeichen die Zeichenkette abgeschnitten
);

/**
 * @brief Alkane_Container Objekt auf stdout ausgeben. Hauptsaechlich fuer das Debbuging.
 *
 * Fuer die Konvertierung wird die Funktion "Alkane_Container_To_String" verwendet.
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Alkane_Objekt, welches ausgegeben werden soll
 */
extern void
Print_Alkane_Container
(
        const struct Alkane_Container* const restrict container     // Alkane_Container, welcher ausgegeben werden soll
);

/**
 * @brief Ein Alkane_Container loeschen.
 *
 * Beim Loeschen werden die Loeschfunktionen aller enthaltenen Alkane-Objekte aufgerufen, sodass am Ende sowohl der
 * Container, als auch der Inhalt geloescht wurde.
 *
 * Daher gilt fuer den Container und fuer alle darin enthaltene Elemente:
 * Es muss sichergestellt werden, dass alle Container geloescht werden. Nach dem Loeschen sollte der verwendete Zeiger
 * - und die im Container enthaltenen Zeiger - nicht mehr dereferenziert werden !
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Alkane_Container Objekt, welches - mit allen enthaltenen Alkane Objekten - geloescht wird
 */
extern void
Delete_Alkane_Container
(
        struct Alkane_Container* restrict container     // Alkane_Container-Objekt, welches geloescht werden soll
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_CONTAINER_H */
