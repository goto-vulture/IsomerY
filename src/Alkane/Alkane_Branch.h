/**
 * @file Alkane_Branch.h
 *
 * @brief Ein Alkan_Branch-Objekt stellt einen Ast eines Alkans dar. Wegen der Vierbindigkeit (im Normalfall) eines
 * C-Atoms, besteht ein Alkan-Objekt aus maximal vier Alkan_Branch-Objekte.
 *
 * In solch einem Alkanast wird der Aufbau des Astes abgespeichert. Und zwar in Form von "verbunden mit"-Aussagen. Eine
 * genaue Beschreibung was damit gemeint ist (mit einen Beispiel) findet sich ueber der Deklaration der Alkane_Branch
 * Struktur.
 *
 * Hier steht diese Beschreibung ...
 * struct Alkane_Branch
 * {
 *     ...
 * }
 *
 * @date 08.03.2021
 * @author x86 / Gyps
 */

#ifndef ALKANE_BRANCH_H
#define ALKANE_BRANCH_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "Alkane_Info_Constitutional_Isomer.h"
#include <stddef.h>
#include "../DMMC.h"



/**
 * @brief Uebersicht ueber die Zustaende, die ein Alkane_Branch-Objekt annehmen kann.
 *
 * Diese Zustaende dienen hauptsaechlich dazu, dass - ohne Analyse des eigentlichen Objektes - z.B. festgestellt werden
 * kann, ob das Objekt wichtige Daten enthaelt oder ob das Objekt initialisert werden muss.
 */
enum Alkane_Branch_State
{
    ALKANE_BRANCH_CREATED = 0,                  ///< Objekt wurde erzeugt
    ALKANE_BRANCH_INITIALIZED_WITH_ZERO_BYTES,  ///< Objekt wurde mit 0 Bytes initialisiert

    /**
     * @brief Objekt wurde mit Daten initialisert -> Es befinden sich also richtige Informationen in dem Objekt.
     */
    ALKANE_BRANCH_INITIALIZED_WITH_DATA,

    ALKANE_BRANCH_DELETED,                      ///< Objekt wurde geloescht und enthaelt keine Informationen mehr

    /**
     * @brief Objekt enthaelt ungueltige Astinformationen.
     *
     * (Dieser Zustand sollte in der Praxis nicht vorkommen - aber man weiss ja nie)
     */
    ALKANE_BRANCH_INVALID_BRANCH,

    /**
     * @brief  Unbekannter Zustand.
     *
     * Dies ist hauptsaechlich dafuer da, wenn kein anderer Zustand angegeben werden kann.
     */
    ALKANE_BRANCH_UNKNOWN_STATE
};

/**
 * @brief Dieses Objekt stellt ein Alkanast dar.
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
    unsigned char branch [MAX_NUMBER_OF_C_ATOMS];           ///< Aufbau des Astes (die "verbunden-mit"-Aussagen)

    /**
     * @brief Laenge des Astes - wie viele der Arrayelemente werden tatsaechlich verwendet ?
     */
    uint_fast8_t length;

    enum Alkane_Branch_State state;                         ///< Status des Objektes
};

CREATE_CONTAINER_H(struct Alkane_Branch*, Alkane_Branch)

//=====================================================================================================================

/**
 * @brief Alkane_Branch Objekt dynamisch erzeugen.
 *
 * Asserts:
 *      lenght > 0 && length <= MAX_NUMBER_OF_C_ATOMS, wenn branch != NULL
 *
 * @param[in] container Container, welches das neue Objekt entgegennimmt
 * @param[in] new_element Existierendes Alkane_Branch Objekt (bzw. deren Zeiger), welches hinzugefuegt werden soll
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
 * @brief Alkane_Branch Objekt in eine Zeichenkettendarstellung ueberfuehren. Dies ist insbesondere fuer debugging
 * hilfreich.
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
 *
 * @param[in] container Alkane_Branch Objekt, welches als Zeichenkette dargestellt werden soll
 * @param[out] string_memory Speicher, der fuer die Zeichenkettenerzeugung verwendet werden soll
 * @param[in] string_memory_size Groesse des Zeichenkettenspeichers
 *
 * @return Adresse der Eingabe-Zeichenkette
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
 * @brief Alkane_Branch Objekt auf stdout ausgeben. Hauptsaechlich fuer das Debbuging. Fuer die Konvertierung wird
 * die Funktion "Path_Data_Container_To_String" verwendet.
 *
 * Asserts:
 *      container != NULL
 *
 * @param[in] container Alkane_Branch Objekt, welches ausgegeben werden soll
 */
extern void
Print_Alkane_Branch
(
        const struct Alkane_Branch* const branch    // Alkane_Branch, welches ausgegeben werden soll
);

/**
 * @brief Alkane_Branch Objekt loeschen.
 *
 * Da es sich bei dem Objekt um dynamischen Speicher handelt, muss sichergestellt werden, dass alle Objekte, die zuvor
 * erstellt wurden, geloescht werden. Nach dem Loeschen sollte der verwendete Zeiger nicht mehr dereferenziert werden !
 *
 * Asserts:
 *      branch != NULL
 *
 * @param[in] container Alkane_Branch Objekt, welches geloescht werden soll
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
