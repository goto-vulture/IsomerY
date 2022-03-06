/**
 * @file Terminal_Alkane_Drawing.h
 *
 * @brief Aus einem IUPAC-Alkannamen eine einfache textbasierte Monofont-Zeichnung erstellen.
 *
 * Diese Zeichnungen sind hauptsaechlich fuer die Verwendung des Programms im Terminal gedacht.
 *
 * @date 28.02.2022
 * @author: N450
 */

#ifndef TEXT_BASED_ALKANE_DRAWING
#define TEXT_BASED_ALKANE_DRAWING ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "../Alkane/Alkane.h"



/**
 * @brief Groesse der Textzeichnung 1. Dimension.
 */
#ifndef TEXT_BASED_ALKANE_DRAWING_DIM_1
#define TEXT_BASED_ALKANE_DRAWING_DIM_1 50
#else
#error "The macro \"TEXT_BASED_ALKANE_DRAWING_DIM_1\" is already defined !"
#endif /* TEXT_BASED_ALKANE_DRAWING_DIM_1 */

/**
 * @brief Groesse der Textzeichnung 2. Dimension.
 */
#ifndef TEXT_BASED_ALKANE_DRAWING_DIM_2
#define TEXT_BASED_ALKANE_DRAWING_DIM_2 50
#else
#error "The macro \"TEXT_BASED_ALKANE_DRAWING_DIM_2\" is already defined !"
#endif /* TEXT_BASED_ALKANE_DRAWING_DIM_2 */



/**
 * @brief Uebersicht ueber die Zustaende, die ein Text_Based_Alkane_Drawing-Objekt annehmen kann.
 *
 * Diese Zustaende dienen hauptsaechlich dazu, dass - ohne Analyse des eigentlichen Objektes - z.B. festgestellt werden
 * kann, ob das Objekt wichtige Daten enthaelt oder ob das Objekt initialisert werden muss.
 *
 * Insbesondere, wenn viele der Objekte ueberprueft werden muessen, ist diese Information eine gute Hilfestellung. :)
 */
enum Text_Based_Alkane_Drawing_State
{
    TEXT_BASED_ALKANE_DRAWING_CREATED = 0, ///< Erzeugung war erfolgreich

    /**
     * @brief Objekt wurde mit Nullbytes erzeugt
     *
     * Bei diesem Objekt wird dieser Zustand nie auftreten. -> Ist also nur der Vollstaendigkeit halber da
     */
    TEXT_BASED_ALKANE_DRAWING_INITIALIZED_WITH_ZERO_BYTES,

    /**
     * @brief Der Standardzustand. Das Objekt wurde erfolgreich mit passenden Daten erzeugt
     */
    TEXT_BASED_ALKANE_DRAWING_INITIALIZED_WITH_DATA,
    TEXT_BASED_ALKANE_DRAWING_DELETED, ///< Objekt wurde geloescht

    /**
     * @brief Zustand nachdem ein ungueltiger Name uebergeben wurde
     *
     * Eine Ueberpruefung entspricht in diesem Zusammenhang nur eine FORMALE Ueberpruefung (Adressencheck, etc.). Der
     * IUPAC-Name wird inhaltlich NICHT ueberprueft !
     */
    TEXT_BASED_ALKANE_DRAWING_INVALID_IUPAC_NAME,

    /**
     * @brief Unbekannter Zustand. Dies ist hauptsaechlich dafuer da, wenn kein anderer Zustand angegeben werden kann
     */
    TEXT_BASED_ALKANE_DRAWING_UNKNOWN_STATE ///<
};



/**
 * @brief Dieses Objekt bietet eine text-basierte Zeichnung eins Alkans aus dem IUPAC-Namen.
 *
 * Es ist die einfachste Methode ein Alkan halbwegs brauchbar zu visualisieren. Bei steigender Verschachtelungstiefe
 * werden die Zeichnungen immer unansehlicher, da die Bindungen immer eindeutig dargestellt werden muessen.
 *
 * Ein Beispiel:
 *
 *                   *
 * ... - C - C - C - C - C - C - C ...
 *
 * Wenn an der mit einem Stern makierten Stelle eine mehrfach verschachtelte Kette angebracht werden muss, dann wird
 * es mit der Zeichnung schwierig. (Z.B. bei "1-(1,1-DiMethylEthyl)Propyl")
 *
 *                   C
 *                   |
 *                   C   C
 *                   |   |
 *                   C - C - C
 *                   |   C?
 * ... - C - C - C - C - C - C - C ...
 *
 * Die erste Methyl-Abzweigung an der verschachtelten Ethylabzweigung laesst sich noch problemlos zeichnen. Bei der
 * zweiten geht es nicht mehr. Es ist nicht mehr eindeutig erkennbar, mit welchen anderen C-Atomen das markierte C-Atom
 * verbunden ist.
 * Um dieses Problem zu loesen muss die gezeichnete Laenge der Bindungen mit der Tiefe der Verschachtelung abnehmen.
 *
 * Bei unserem Beispiel sieht das dann so aus:
 *
 *                       C
 *                       |
 *                       |
 *                       C
 *                       |
 *                       |   C
 *                       C - C - C
 *                       |   C
 *                       |
 * ... -- C -- C -- C -- C -- C -- C -- C ...
 *
 * Hierdurch wird der Aufbau wieder eindeutig. Aber das Ergebnis sieht 1.: Ziemlich schmandig aus und 2. Nimmt unnoetig
 * viel Speicher in Anspruch.
 *
 * Die text-basierten Zeichnungen sind rein zweckdienlich. D.h. sie werden mit dieser Herangehenweise bei der Zeichnung
 * von Verschachtelungen dennoch verwendet. Wenn mit bessere Ansaetze einfallen, dann werden sie auf jeden Fall
 * verwendet!
 */
struct Text_Based_Alkane_Drawing
{
    /**
     * @brief Die Zeichenflache, wo die Textzeichnung erstellt wird
     */
    char drawing [TEXT_BASED_ALKANE_DRAWING_DIM_1][TEXT_BASED_ALKANE_DRAWING_DIM_2];

    /**
     * @brief Tiefste verwendete 1. Dimension
     *
     * Diese Information ist besonders hilfreich, wenn das Objekt gut formatiert ausgegeben werden soll.
     */
    uint_fast8_t max_dim_1_used;
    /**
     * @brief Tiefste verwendete 2. Dimension
     *
     * Diese Information ist besonders hilfreich, wenn das Objekt gut formatiert ausgegeben werden soll.
     */
    uint_fast8_t max_dim_2_used;

    /**
     * @brief IUPAC-Name des Alkans, welches in der Zeichenflaeche gezeichnet wurde.
     */
    char iupac_name [IUPAC_NAME_LENGTH];

    enum Text_Based_Alkane_Drawing_State state; ///< Status des Objektes
};

//=====================================================================================================================

/**
 * @brief Eine text-basierte Zeichnung eines Alkans erstellen.
 *
 * Grundsaetziches Vorgehen:
 * - Aufspaltung des Namens an bestimmten Minuszeichnen mithilfe des Chain-Lexers, der urspruenglich fuer die
 *   Testfunktionen geschrieben wurde
 *
 * - Ermittlung der tiefsten Verschachtelung
 *
 * - Zeichnung der Hauptkette in der mittleren Zeile (Anhand der tiefsten Verschachtelung wird entschieden wie gross
 *   der Abstand zwischen den C-Atomen sein wird)
 *
 * - Zeichnung aller Aeste, die sich direkt an der Hauptkette befinden und keine Verschachtelung besitzen
 *
 * - Zeichnung aller Aeste, die eine Verschachtelung besitzen. Dabei wird "von Oben nach Unten" die Zeichnung erzeugt.
 *   Stufenweise wird in die Tiefe der Verschachtelung gegangen.
 *
 * Asserts:
 *      iupac_name != NULL
 *      name_length > 0
 *      name_length <= IUPAC_NAME_LENGTH
 *
 * @param[in] iupac_name IUPAC-Name eines Alkans
 * @param[in] name_length Laenge des IUPAC-Namen
 */
extern struct Text_Based_Alkane_Drawing*        // Das Ergebnis (die textbasierte-Zeichnung)
Create_Text_Based_Alkane_Drawing
(
        const char* const restrict iupac_name,  // IUPAC-Name, der gezeichnet werden soll
        const size_t name_length                // Laenge des IUPAC-Namen
);

/**
 * @brief Eine text-basierte Zeichnung komplett loeschen.
 *
 * Asserts:
 *         text_based_drawing != NULL
 *
 * @param[in] text_based_drawing Text-Zeichnung, welches geloescht werden soll
 */
extern void
Delete_Text_Based_Alkane_Drawing
(
        struct Text_Based_Alkane_Drawing* restrict text_based_drawing   // Objekt, welches geloescht werden soll
);

/**
 * @brief Die textbasierte Zeichnung auf stdout ausgegeben.
 *
 * Dabei wird wirklich nur die Zeichenflache (ein zweidimensionales Array) sowie der IUPAC-Name ausgegeben.
 *
 * Asserts:
 *          text_based_drawing != NULL
 *
 * @param[in] Text_Based_Alkane_Drawing, wo die Zeichenflache ausgegeben wird
 */
extern void
Show_Text_Based_Alkane_Drawing
(
        const struct Text_Based_Alkane_Drawing* const restrict text_based_drawing
);


// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TEXT_BASED_ALKANE_DRAWING */
