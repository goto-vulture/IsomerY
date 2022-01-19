/**
 * @file Path_Data_Container.h
 *
 * @brief Wurde durch einen Container, der mittels DMMC.h erstellt wurde, ersetzt !
 */

#if 0
/**
 * @file Path_Data_Container.h
 *
 * @brief Einfacher dynamischer Conainer fuer Path_Data-Objekte. Bzw. genauer gesagt fuer deren Zeiger.
 *
 * Es wird fuer jedes neue Element im Container kein realloc aufgerufen, sondern es werden blockweise neue Zeiger
 * angefordert, wenn der Container voll ist. Die Groesse der neu allokierten Bloecke kann mittels der Konstante
 * PATH_DATA_CONTAINER_ALLOCATION_STEP_SIZE festgelegt werden. Meiner bisherigen Erfahrungen nach bietet sich eine
 * Groesse ab 128 bei diesem Typ an (Bei den anderen Containern: Alkane_Branch_Container und Alkane_Container sind es
 * 256), um das Programm nicht durch zu viele realloc Aufrufe zu verlangsamen. (realloc kopiert den kompletten Speicher
 * um, falls notwendig !)
 *
 * Man hat hier also - wie so oft - die Abwaegung zwische Laufzeit und Speicherverbrauch.
 *
 * @date 12.06.2021
 * @author x86 / Gyps
 */

#ifndef PATH_DATA_CONTAINER_H
#define PATH_DATA_CONTAINER_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#ifndef PATH_DATA_CONTAINER_ALLOCATION_STEP_SIZE
    #define PATH_DATA_CONTAINER_ALLOCATION_STEP_SIZE 128 ///< Groesse der Allokationsschritte
#else
    #error "The macro \"PATH_DATA_CONTAINER_ALLOCATION_STEP_SIZE\" is already defined !"
#endif /* PATH_DATA_CONTAINER_ALLOCATION_STEP_SIZE */



#include "Path_Data.h"



/**
 * @brief Uebersicht ueber die Zustaende, die ein Path_Data_Container annehmen kann.
 *
 * Diese Zustaende dienen hauptsaechlich dazu, dass - ohne Analyse des eigentlichen Objektes - z.B. festgestellt werden
 * kann, ob das Objekt wichtige Daten enthaelt oder ob das Objekt initialisert werden muss.
 */
enum Path_Data_Container_State
{
    PATH_DATA_CONTAINER_CREATED = 0,                    //< Objekt wurde erzeugt
    PATH_DATA_CONTAINER_INITIALIZED_WITH_ZERO_BYTES,    //< Objekt wurde mit 0 Bytes initialisiert
    PATH_DATA_CONTAINER_DELETED,                        //< Objekt wurde geloescht und enthaelt keine Informationen mehr

    /**
     * @brief Objekt enthaelt ungueltige Daten.
     *
     * Dies ist dann der Fall, wenn z.B. ein ungueltiges Alkan_Branch Objekt zum Container hinzugefuegt wurde.
     */
    PATH_DATA_CONTAINER_INVALID_DATA,

    /**
     * @brief Unbekannter Zustand. Dies ist hauptsaechlich dafuer da, wenn kein anderer Zustand angegeben werden kann.
     */
    PATH_DATA_CONTAINER_UNKNOWN_STATE
};



/**
 * @brief Dieses Objekt stellt einen einfachen Container fuer Path_Data-Objekte dar.
 */
struct Path_Data_Container
{
    /**
     * @brief Daten des Containers.
     *
     * Da die Path_Data-Objekte dynamisch erzeugt werden, werden im Container nur die Zeiger auf die Path_Data-Objekte
     * gespeichert.
     */
    struct Path_Data** data;

    uint_fast64_t size;                     //< Anzahl an tatsaechlich vorhandenen Objekten

    /**
     * @brief Anzahl an allokierten Speicher
     *
     * Der Ansatz mit size / allocated_size dient dazu, dass nicht bei jedem neuen Objekt ein realloc ()-Aufruf
     * stattfinden muss.
     */
    uint_fast64_t allocated_size;

    enum Path_Data_Container_State state;   //< Aktueller Status des Objektes
};

//=====================================================================================================================

/**
 * @brief Ein neuen leeren Path_Data_Container erstellen.
 *
 * Asserts:
 *      N/A
 *
 * @return Zeiger auf das neue Path_Data_Container Objekt.
 */
extern struct Path_Data_Container*
Create_Path_Data_Container
(
        void
);

/**
 * @brief Ein Path_Data-Objekt zu einem existierenden Path_Data_Container hinzufuegen.
 *
 * Der Container kopiert das Path_Data-Objekt NICHT ! Es wird nur die Speicheradresse im Container abgelegt !
 *
 * Asserts:
 *      container != NULL
 *      new_element != NULL
 *
 * @param[in] container Container, welches das neue Objekt entgegennimmt
 * @param[in] new_element Existierendes Path_Data-Objekt (bzw. deren Zeiger), welches hinzugefuegt werden soll
 */
extern void
Add_Path_Data_To_Container
(
        struct Path_Data_Container* restrict const container,   // Container, welches das neue Objekt entgegennimmt
        const struct Path_Data* restrict const new_element      // Existierendes Path_Data-Objekt, welches
                                                                // hinzugefuegt werden soll
);

/**
 * @brief Path_Data_Container Objekt in eine Zeichenkettendarstellung ueberfuehren. Dies ist insbesondere fuer debugging
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
 * @param[in] container Path_Data_Container Objekt, welches als Zeichenkette dargestellt werden soll
 * @param[out] string_memory Speicher, der fuer die Zeichenkettenerzeugung verwendet werden soll
 * @param[in] string_memory_size Groesse des Zeichenkettenspeichers
 *
 * @return Adresse der Eingabe-Zeichenkette
 */
extern const char*                                              // Adresse der Eingabe-Zeichenkette
Path_Data_Container_To_String
(
        const struct Path_Data_Container* restrict const container,
                                                                // Path_Data_Container, der als Zeichenkette
                                                                // dargestellt werden soll
        char* const restrict string_memory,                     // Speicher, der fuer die Zeichenkettenerzeugung
                                                                // verwendet werden soll
                                                                // Der Speicher muss vorher vom Aufrufer allokiert
                                                                // werden !
        const size_t string_memory_size                         // Groesse des Zeichenkettenspeichers
                                                                // Wenn der Speicher nicht ausreicht, dann wird am
                                                                // vorletzten Zeichen die Zeichenkette abgeschnitten
);

/**
 * @brief Path_Data_Container Objekt auf stdout ausgeben. Hauptsaechlich fuer das Debbuging. Fuer die Konvertierung wird
 * die Funktion "Path_Data_Container_To_String" verwendet.
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Path_Data_Container Objekt, welches ausgegeben werden soll
 */
extern void
Print_Path_Data_Container
(
        const struct Path_Data_Container* const restrict container  // Path_Data_Container, welcher ausgegeben
                                                                    // werden soll
);

/**
 * @brief Ein Path_Data_Container loeschen.
 *
 * Beim Loeschen werden die Loeschfunktionen aller enthaltenen Path_Data-Objekte aufgerufen, sodass am Ende
 * sowohl der Container, als auch der Inhalt geloescht wurde.
 *
 * Nach dem Loeschvorgang sollten Zeiger auf den Container - oder deren enthaltenen Path_Data Elemente - nicht mehr
 * dereferenziert werden !
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Path_Data_Container Objekt, welches geloescht werden soll
 */
extern void
Delete_Path_Data_Container
(
        struct Path_Data_Container* restrict container  // Path_Data_Container-Objekt, welches geloescht werden soll
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PATH_DATA_CONTAINER_H */

#endif /* 0 */
