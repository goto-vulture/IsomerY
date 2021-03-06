/**
 * @file Alkane.h
 *
 * @brief Ein Alkan-Objekt wird durch vier Alkane_Branch-Objekte dargestellt und ist das zentrale Objekt in diesem
 * Programm.
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
 * Dies Idee der Darstellung stammt nicht von mir sondern von
 * Sascha Kurz, Universität Bayreuth. E-Mail: sascha.kurz@uni-bayreuth.de
 * und
 * Armin Moritz, Johanneum-Gymnasium in Herborn, E-Mail: arminmoritz@gmx.de
 *
 * Generell haben die Beiden durch das Dokument "Strukturisomere der Alkane" den Grundstein fuer dieses Projekt gelegt.
 * An sich hat mich diese Problemstellung bereits seit der Oberstufenzeit interessiert. Aber ohne diesen fantastischen
 * Ansatz von den Beiden haette ich wahrscheinlich dieses Problem nie in ein Programm ueberfuehrt !
 *
 * Vielen Dank fuer eure Hilfestellung ! :D
 *
 * @date 10.03.2021
 * @author x86 / Gyps
 */

#ifndef ALKANE_H
#define ALKANE_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "Alkane_Branch.h"



/**
 * @brief Groesse des char-Arrays fuer den IUPAC-Namen.
 */
#ifndef IUPAC_NAME_LENGTH
    #define IUPAC_NAME_LENGTH 150u
#else
    #error "The macro \"IUPAC_NAME_LENGTH\" is already defined !"
#endif /* IUPAC_NAME_LENGTH */



/**
 * @brief Uebersicht ueber die Zustaende, die ein Alkane-Objekt annehmen kann.
 *
 * Diese Zustaende dienen hauptsaechlich dazu, dass - ohne Analyse des eigentlichen Objektes - z.B. festgestellt werden
 * kann, ob das Objekt wichtige Daten enthaelt oder ob das Objekt initialisert werden muss.
 */
enum Alkane_State
{
    ALKANE_CREATED = 0,                 ///< Objekt wurde erzeugt
    ALKANE_INITIALIZED_WITH_ZERO_BYTES, ///< Objekt wurde mit 0 Bytes initialisiert

    /**
     * @brief Objekt wurde mit Daten initialisert -> Es befinden sich also richtige Informationen in dem Objekt
     */
    ALKANE_INITIALIZED_WITH_DATA,

    ALKANE_DELETED,                     ///< Objekt wurde geloescht und enthaelt keine Informationen mehr

    /**
     * @brief Objekt enthaelt ungueltige Informationen (Dieser Zustand sollte in der Praxis nicht vorkommen - aber man
     * weiss ja nie)
     */
    ALKANE_INVALID_BRANCH,

    /**
     * @brief Unbekannter Zustand. Dies ist hauptsaechlich dafuer da, wenn kein anderer Zustand angegeben werden kann
     */
    ALKANE_UNKNOWN_STATE
};



/**
 * @brief Dieses Objekt stellt ein komplettes Alkanmolekuel dar.
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
    struct Alkane_Branch* branches [4];                         ///< Die Alkane_Branch-Objekte
    uint_fast8_t next_free_branch;                              ///< Index des naechsten freien Alkane_Branch Zeigers

    /**
     * @brief Zusammengefuehrter Zahlencode
     *
     * Hiermit wird die genaue Struktur des Alkans bestimmt
     */
    unsigned char merged_numbercode [MAX_NUMBER_OF_C_ATOMS];

    uint_fast8_t number_of_c_atoms;                             ///< Anzahl an C-Atomen im kompletten Alkan

    /**
     * @brief Aufbau des Alkans ueber eine Adjazenzmatrix
     *
     * Da die Bindungen in beide Richtungen existieren, ist die Adjazenzmatrix symmetrisch zur Diagonalen
     */
    unsigned char structure [MAX_NUMBER_OF_C_ATOMS][MAX_NUMBER_OF_C_ATOMS];

    /**
     * @brief Ketteninformationen, die fuer die Zeichnung und fuer die Erstellung des IUPAC-Namen benoetigt werden.
     */
    struct Chain
    {
        uint_fast8_t length;                                    ///< Laenge der Kette

        /**
         * @brief Position der Kette bezogen auf die Kette, an dem sich diese Kette befindet
         */
        uint_fast8_t position;

        /**
         * @brief Verschachtelungstiefe
         *
         * Tiefe 0 steht fuer die Hauptkette
         */
        uint_fast8_t nesting_depth;
    } chains [MAX_NUMBER_OF_C_ATOMS];                           ///< Ketteninformationen des Isomers
                                                                // ToDo: Arraygroesse verringern falls moeglich
    uint_fast8_t next_free_chain;                               ///< Index der naechsten freien Kette

    /**
     * @brief Der IUPAC-Name des Alkans
     *
     * Die Laenge entspricht/ eingener Erfahrungen und wird hoffentlich reichen.
     */
    char iupac_name [IUPAC_NAME_LENGTH];

    enum Alkane_State state;                                    ///< Status des Objektes
};

//=====================================================================================================================

/**
 * @brief Alkane Objekt dynamisch erzeugen.
 *
 * Alle Zeiger duerfen NULL sein, in diesem Fall wird davon ausgegangen, dass dieser Ast nur aus einem H-Atom besteht.
 * Damit die Adressen, die ungleich NULL sind, aufsteigend im Alkane-Objekt gespeichert werden, (dies vereinfacht die
 * weiteren Operationen) werden die uebergebenen Adressen im Objekt neu angeordnet, wenn die Alkane_Branch-Objekte
 * nicht "von links nach rechts" der Funktion uebergeben werden.
 *
 * Asserts:
 *      N/A
 *
 * @param[in] branch_1 Erster Ast des neuen Alkans
 * @param[in] branch_2 Zweiter Ast des neuen Alkans
 * @param[in] branch_3 Dritter Ast des neuen Alkans
 * @param[in] branch_4 Vierter Ast des neuen Alkans
 *
 * @return Zeiger auf das erzeugte Alkan-Objekt
 */
extern struct Alkane*                                     // Erzeugtes Alkane-Objekt
Create_Alkane
(
        // Hier darf KEIN restrict verwendet werden, da einige Alkane aus gleichen Alkan_Branch-Objekte gebildet
        // werden ! Manche Compiler erkennen das und erzeugen eine [-Wrestrict] Warnung.
        struct Alkane_Branch* /* restrict */ branch_1,    // 1. Ast des neuen Alkane-Objektes
        struct Alkane_Branch* /* restrict */ branch_2,    // 2. Ast des neuen Alkane-Objektes
        struct Alkane_Branch* /* restrict */ branch_3,    // 3. Ast des neuen Alkane-Objektes
        struct Alkane_Branch* /* restrict */ branch_4     // 4. Ast des neuen Alkane-Objektes
);

/**
 * @brief Alkane Objekt in eine Zeichenkettendarstellung ueberfuehren. Dies ist insbesondere fuer debugging hilfreich.
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
 *
 * @param[in] alkane Alkan-Objekt, welches als Zeichenkette dargestellt werden soll
 * @param[out] string_memory Speicher, der fuer die Zeichenkettenerzeugung verwendet werden soll
 * @param[in] string_memory_size Groesse des Zeichenkettenspeichers
 *
 * @return Adresse der Eingabe-Zeichenkette
 */
extern const char*                                      // Adresse der Eingabe-Zeichenkette
Alkane_To_String
(
        const struct Alkane* const restrict alkane,     // Alkane der als Zeichenkette dargestellt werden soll
        char* const restrict string_memory,             // Speicher, der fuer die Zeichenkettenerzeugung verwendet
                                                        // werden soll
                                                        // Der Speicher muss vorher vom Aufrufer allokiert werden
        const size_t string_memory_size                 // Groesse des Zeichenkettenspeichers
                                                        // Wenn der Speicher nicht ausreicht, dann wird am vorletzten
                                                        // Zeichen die Zeichenkette abgeschnitten
);

/**
 * @brief Alkane-Objekt auf stdout ausgeben. Hauptsaechlich fuer das Debbuging. Fuer die Konvertierung wird die
 * Funktion Alkane_To_String() verwendet.
 *
 * Asserts:
 *      alkane != NULL
 *
 * @param[in] alkane Alkane Objekt, welches ausgegeben werden soll
 */
extern void
Print_Alkane
(
        const struct Alkane* const restrict alkane  // Alkane, welches ausgegeben werden soll
);

/**
 * @brief Alkane Objekt loeschen.
 *
 * Die enthaltenen Alkane_Branch-Objekte werden NICHT geloescht, wenn das Alkane-Objekt geloescht wird. Es werden
 * lediglich die Zeiger des Objektes auf NULL gesetzt !
 *
 * Da es sich bei dem Objekt um dynamischen Speicher handelt, muss sichergestellt werden, dass alle Objekte, die zuvor
 * erstellt wurden, geloescht werden. Nach dem Loeschen sollte der verwendete Zeiger nicht mehr dereferenziert werden !
 *
 * Asserts:
 *      alkane != NULL
 *
 * @param[in] alkane Alkane, welches geloescht werden soll
 */
extern void
Delete_Alkane
(
        struct Alkane* restrict alkane  // Alkane, welches geloescht werden soll
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_H */
