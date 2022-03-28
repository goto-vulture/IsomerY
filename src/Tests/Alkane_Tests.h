/**
 * @file Alkane_Tests.h
 *
 * @brief In dieser Uebersetzungseinheit werden viele Tests der Alkanerstellung und der Erstellung von IUPAC-Namen
 * durchgefuehrt.
 *
 * Die Tests koennen - je nach Computer - mehrere Minuten in Anspruch nehmen ! Falls die Tests innerhalb einer
 * kuerzeren Zeit durchgefuehrt werden soll, dann kann die Zeile:
 *
 * RUN(TEST_All_Possible_Tetradecan_Constitutional_Isomers);
 *
 * in der Funktion "Execute_All_Alkane_Tests" auskommentiert werden. Dieser Test verbraucht mit Abstand die meiste
 * Zeit.
 *
 * @date 13.03.2021
 * @author x86 / Gyps
 */

#ifndef TESTS_ALKANE_TESTS_H
#define TESTS_ALKANE_TESTS_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "tinytest.h"



/**
 * @brief Alle Alkan-Tests ausfuehren.
 *
 * @return 0, falls alle Tests erfolgreich waren; -1, falls mindestens ein Test fehlschlug
 */
extern int
Execute_All_Alkane_Tests
(void);

/**
 * @brief Die Erstellung von Alkanen aus Alkanaesten testen.
 *
 * Wird der Nummerncode richtig gebildet ?
 */
extern void TEST_Create_Alkane (void);

/**
 * @brief Die Erstellung der Alkane testen. Vorerst nur ueber die Anzahl an Ergebnissen.
 *
 * Die Anzahl an Ergebnissen wird mit der Anzahl an moeglichen Konstitutionsisomeren verglichen. Wenn die Werte gleich
 * sind, dann ist ein Fehler in der Berechnung relativ unwahrscheinlich.
 *
 * Die Liste an moeglichen Konstitutionsisomeren bis 40 sind in der Datei "Alkane_Info_Constitutional_Isomer.h" zu
 * finden.
 *
 * Aus Laufzeitgruenden wird nur bis inklusive 12 getestet.
 */
extern void TEST_Create_Alkane_Constitutional_Isomers (void);

/**
 * @brief Die Erstellung des IUPAC-Namen aus einem Alkan testen.
 *
 * Wird der IUPAC-Name richtig gebildet ?
 */
extern void TEST_Convert_Alkane_To_IUPAC_Name (void);

/**
 * @brief Die Erstellung des IUPAC-Namen aus einem Alkan testen (2).
 *
 * Wird der IUPAC-Name richtig gebildet ?
 */
extern void TEST_Convert_Alkane_To_IUPAC_Name_2 (void);

/**
 * @brief Die Erstellung des IUPAC-Namen aus einem Alkan testen, bei dem die Chain-Objekte von Hand erzeugt wurden.
 * Hierbei geht es in erster Linie darum den Konvertierungsprozess von den Chain-Objekten hin zu dem IUPAC-Namen zu
 * testen.
 *
 * Wird der IUPAC-Name richtig gebildet ?
 */
extern void TEST_Convert_Alkane_To_IUPAC_Name_With_Manual_Chain_Objects (void);

/**
 * @brief Die Erstellung des IUPAC-Namen aus einem Alkan mit bis zu zwei Verschachtelungstiefen testen. Das bedeutet,
 * dass an mind. einem Ast ein weiter Ast ist.
 *
 * Wird der IUPAC-Name richtig gebildet ?
 */
extern void TEST_Convert_Alkane_With_Nested_2_To_IUPAC_Name (void);

/**
 * @brief Alibi Test-Funktion, damit das Testframework eine Funktion hat, selbst wenn die Erzeugung der Isomere vom
 * Methan keinen Sinn ergibt.
 */
extern void TEST_All_Possible_Methan_Constitutional_Isomers (void);

/**
 * @brief Alibi Test-Funktion, damit das Testframework eine Funktion hat, selbst wenn die Erzeugung der Isomere vom
 * Ethan keinen Sinn ergibt.
 */
extern void TEST_All_Possible_Ethan_Constitutional_Isomers (void);

/**
 * @brief Alibi Test-Funktion, damit das Testframework eine Funktion hat, selbst wenn die Erzeugung der Isomere vom
 * Propan keinen Sinn ergibt.
 */
extern void TEST_All_Possible_Propan_Constitutional_Isomers (void);

/**
 * @brief Alle moeglichen Konstitutionsisomere des Butan mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Butan_Constitutional_Isomers (void);

/**
 * @brief Alle moeglichen Konstitutionsisomere des Pentan mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Pentan_Constitutional_Isomers (void);

/**
 * @brief Alle moeglichen Konstitutionsisomere des Hexans mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Hexan_Constitutional_Isomers (void);

/**
 * @brief Alle moeglichen Konstitutionsisomere des Heptans mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Heptan_Constitutional_Isomers (void);

/**
 * @brief Alle moeglichen Konstitutionsisomere des Octan mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Octan_Constitutional_Isomers (void);

/**
 * @brief Alle moeglichen Konstitutionsisomere des Nonans mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Nonan_Constitutional_Isomers (void);

/**
 * @brief Alle moeglichen Konstitutionsisomere des Decans mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Decan_Constitutional_Isomers (void);

/**
 * @brief Alle moeglichen Konstitutionsisomere des Undecans mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Undecan_Constitutional_Isomers (void);

/**
 * @brief Alle moeglichen Konstitutionsisomere des Dodecans mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Dodecan_Constitutional_Isomers (void);

/**
 * @brief Alle moeglichen Konstitutionsisomere des Tridecans mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Tridecan_Constitutional_Isomers (void);

/**
 * @brief Alle moeglichen Konstitutionsisomere des Tetradecan mit IUPAC-Namen erzeugen.
 *
 * Dabei werden die IUPAC-Namen kontrolliert.
 */
extern void TEST_All_Possible_Tetradecan_Constitutional_Isomers (void);

/**
 * @brief Das Zusammenfassen von Gruppen im IUPAC-Namen testen.
 *
 * Was ist damit genau gemeint?
 *
 * Mit "Gruppen" sind kombinierte Aeste gemeint, die mehrfach vorkommen.
 * Z.B.: (1-methylethyl)
 *
 * Ein Alkan wie "4-(1-methylethyl)-4-(1-methylethyl)heptane"
 *
 *             C
 *             |
 *             C - C
 *             |
 * C - C - C - C - C - C - C
 *             |
 *             C - C
 *             |
 *             C
 *
 * Die beiden Gruppen "(1-methylehtyl)" koennen zusammengefasst werden:
 * 4,4-Bis(1-methylethyl)heptane
 *
 * "Bis" zeigt an, dass es sich um genau zwei gleiche Gruppen handelt, die zusammengefasst wurden.
 *
 */
extern void TEST_Group_Compression (void);

/**
 * @brief Testen, ob der Alkan-Lexer die gewuenschen Ergebnisse erzeugt.
 *
 * So soll z.B. "4-(1-methylethyl)heptane" in folgende Tokens zerlegt werden:
 * - "4"
 * - "-"
 * - "("
 * - "1"
 * - "-"
 * - "methyl"
 * - "ethyl"
 * - ")"
 * - "heptane"
 */
extern void TEST_Alkane_Lexer (void);

/**
 * @brief Testen, ob der Alkan-Parser seine Aufgaben richtig erledigt.
 *
 * Im Hintergrund wird der Alkan-Lexer verwendet. Dieser erzeugt die Tokens, die dann vom Parser ueberprueft werden.
 * Die Tests in dieser Testfunktion beziehen sich alleine auf die des Parsers. Wenn Fehler beim Lexer auftauchen, dann
 * koennen diese von den daraus resultierenden Fehlern des Parsers nicht unterschieden werden. Daher gibt es auch eine
 * eigene Testfunktion fuer den Lexer (TEST_Alkane_Lexer).
 */
extern void TEST_Alkane_Parser (void);

/**
 * @brief Das textbasierte Zeichnen eines Alkans testen. Erster Zeichnungstest.
 *
 * Es soll "4-(1-methylethyl)heptane" gezeichnet werden.
 *
 * Das Ergebnis soll etwa so aussehen:
 *
 * C - C - C - C - C - C - C
 *             |
 *             C - C
 *             |
 *             C
 */
extern void TEST_Text_Based_Alkane_Drawing_1 (void);

/**
 * @brief Alle Testfunktionen ausfuehren.
 *
 * Die Funktionalitaet liegt nicht direkt im Koerper dieser Funktion. Vielmehr dient diese Funktion als eine Art
 * "Platzhalterfunktion", da in der Funktion "Execute_All_Alkane_Tests" alle Testfunktionen mittels Fuktionszeiger
 * verfuegbar gemacht werden, indem sie in die Liste der Testfunktionen uebernommen wird.
 */
extern void TEST_Use_All_Testfunctions (void);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TESTS_ALKANE_TESTS_H */
