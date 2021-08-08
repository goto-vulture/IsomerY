/**
 * Alkane_Create_Constitutional_Isomer.h
 *
 * Hier wird der Algorithmus, der die Konstitutionsisomere erzeugt, definiert.
 *
 * Fuer eine Beschreibung des Algorithmus: Siehe Funktionskommentar von Create_Alkane_Constitutional_Isomers
 *
 *  Created on: 08.03.2021
 *      Author: x86 / Gyps
 */

#ifndef ALKANE_CREATE_CONSTITUTIONAL_ISOMER_H
#define ALKANE_CREATE_CONSTITUTIONAL_ISOMER_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <inttypes.h>



/**
 * Die Alkan Konstitutionsisomere bei gegebener Anzahl an C-Atomen bestimmen.
 *
 * Die Grundidee stammt aus dem Dokument "Strukturisomere der Alkane" von
 * Sascha Kurz, Universität Bayreuth. E-Mail: sascha.kurz@uni-bayreuth.de
 * und
 * Armin Moritz, Johanneum-Gymnasium in Herborn, E-Mail: arminmoritz@gmx.de
 *
 * Die weiteren Erlaeuterungen wurden aus der Implementierung von Sascha Kurz in leicht abgwandelter Form uebernommen.
 *
 * +-----------------+
 * | Der Algorithmus |
 * +-----------------+
 * Der Algorithmus basiert auf der Grundidee, die im "Strukturisomere der Alkane" angegeben ist. Dabei werden rekursiv
 * Wurzelbaeume erzeugt, aus denen die Alkane zusammengesetzt werden.
 *
 *
 * >>> Wurzelbaeume <<<
 * Ein Wurzelbaum ist in der Graphentheorie ein Baum, der einen ausgezeichneten Knoten enthaelt. Dieser Knoten wird
 * "Wurzel" genannt. Siehe: https://de.wikipedia.org/wiki/Gewurzelter_Baum
 * Die fuer diesen Algorithmus verwendeten Wurzelbaeume sind zusaetzlich ungerichtet, besitzen keine Kantengewichte und haben
 * genau eine Wurzel. Siehe: https://de.wikipedia.org/wiki/Wurzel_(Graphentheorie)
 *
 * Beispiel fuer einen hier verwendeten Wurzelbaum:
 * Ebene 3        o o o o o
 *                 \|  \|/
 * Ebene 2          o o o
 *                   \|/
 * Ebene 1            o <- Wurzel
 *
 * Jeder Knoten, ausser der Wurzel, hat genau eine Kante zu einem Knoten auf einer um 1 niedrigeren Ebene.
 *
 * Gespeichert werden die Wurzelbaeume in Form einfacher unsigned char-Arrays. Dafuer werden die Verbindungen zwischen den
 * Knoten in Zahlen codiert. Eine Zahl stellt eine "verbunden mit"-Aussage dar. Durch den besonderen Aufbau der hier verwendeten
 * Wurzelbaeume, koennen mit diesen Informationen der komplette Aufbau des Graphens eindeutig abgespeichert werden.
 *
 * Beispiel:
 * Jedem Knoten wird eine Nummer gegeben. Als Konvention wird festgelegt, dass von der Wurzel aus durchnummeriert wird. Die
 * Wurzel ist demnach immer 0 bzw. 1. (Je nachdem ob man bei 0 oder bei 1 mit der Nummerierung beginnt)
 * Die Anzahl der Ebenen wird als die Hoehe h des Baums bezeichnet
 *
 * Ebene 3        4 5 6 7 8
 *                 \|  \|/
 * Ebene 2          1 2 3
 *                   \|/
 * Ebene 1            0 <- Wurzel
 *
 * Das Array wuerde fuer diesen Graphen also so aussehen: { 0, 0, 0, 0, 1, 1, 3, 3, 3 }
 * Im weiteren Verlauf wird diese Darstellung der Graphen "Zahlencodes" genannt.
 * Wichtig hierbei (!): Das 1. Element im Array ist die Wurzel. Da die Wurzel immer der erste Knoten ist, stellt die Wurzel
 *                      keine "verbunden mit"-Aussage dar !
 *
 *
 * >>> Zusammensetzen von Wurzelbaeumen <<<
 * Kleinere Wurzelbaeume koennen zu groesseren Wurzelbaeumen zusammengesetzt werden. Dies ist der Grundansatz bei der Bestimmung
 * der Alkanaseste. Siehe weiter unten: "1. Teil"
 *
 *       4 5 6 7 8   2 3 4              5 6  7 8 9 12 13 14
 *        \|  \|/     \|/                \|   \|/    \|/
 *         1 2 3       1     1   --->     2 3 4      11 16
 *          \|/        |     |             \|/        | |
 *           0         0     0              1        10 15
 *                                           \        |/
 *                                            --------0
 *
 * Damit die "verbunden mit"-Aussagen korrekt und eindeutig bleiben, muessen die Zahlencodes der Aeste angepasst werden. Der
 * dargestellte zusammengesetzte Graph hat den Zahlencode: { 0, 0, 1, 1, 1, 2, 2, 4, 4, 4, 0, 0, 10, 11, 11, 11, 15 }
 *
 *
 * >>> Erzeugen von Wurzelbaeumen <<<
 * Da der Prozess der Wurzelbaumerstellung rekursiv ablaeuft, muessen Startelemente vorhanden sein. Dafuer werden die
 * Wurzelbaeume mit der Hoehe 0 und der Hoehe 1 verwendet. Diese Wurzelbaeume sind im Quellcode hart codiert.
 *
 * Im weiteren Verlauf werden die Wurzelbaeume mit der Hoehe <= h aus den Wurzelbaeumen mit der Hoehe <= h - 1 erzeugt.
 *
 *
 * >>> Zusammensetzen von Wurzelbaeumen zu Alkanen <<<
 * Um aus den zuvor generierten Wurzelbaeumen Alkane erzeugen zu koennen, muss die laengste gerade Kette des Alkans betrachtet
 * werden. Der Pfad muss nicht eindeutig sein ! U.U. gibt es also mmehrere Ketten mit der Maximallaenge k im Molekuel.
 *
 * Wenn k gerade ist, dann kann ein Alkan mit genau zwei Wurzelbaeumen dargestellt werden, indem die Wurzeln mittels einer
 * Kante verbunden werden:
 *
 * W_1---W_2
 *
 * Ist dagegen k ungerade, so besteht das Alkanmolekuel aus 4 Wurzelbaeumen, wovon mindestens 2 die Hoehe (k - 1) / 2 haben
 * muessen und die restlichen maximal diese Hoehe besitzen. Deren Wurzeln sind mit einem zusaetzlichen Knoten durch je eine
 * Kante verbunden:
 *
 *          W_2
 *           |
 *       W_1-o-W_3
 *           |
 *          W_4
 *
 * Ein Alkan kann also durch 2 oder 4 Wurzelbaeume und der Info, ob die laengste gerade Kette gerade ist, dargestellt werden.
 *
 *
 *
 * >>> Zusammengefasst <<<
 * Der Prozess der Erzeugung laesst sich in drei Schritte unterteilen:
 * 1. Teil: Liste der Alkanaeste erzeugen
 *          Siehe Dokument "Strukturisomere der Alkane" Seite 2 - 8
 * 2. Teil: Alle moeglichen Alkane aus den Alkanastlisten bilden
 *          Siehe Dokument "Strukturisomere der Alkane" Seite 8 - 13
 * 3. Teil: Die Alkane, die genau die gesuchte Laenge haben, auswaehlen
 *          => Dies sind die gesuchten Konstitutionsisomere
 *          Siehe Dokument "Strukturisomere der Alkane" Seite 13 - 14
 *
 * Asserts:
 *      number_of_c_atoms < MAX_NUMBER_OF_C_ATOMS
 */
extern struct Alkane_Container*
Create_Alkane_Constitutional_Isomers
(
       const uint_fast8_t number_of_c_atoms
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_CREATE_CONSTITUTIONAL_ISOMER_H */
