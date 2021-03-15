/**
 * Alkane_To_IUPAC_Name.h
 *
 *  Created on: 14.03.2021
 *      Author: x86 / Gyps
 */

#ifndef ALKANE_TO_IUPAC_NAME_H
#define ALKANE_TO_IUPAC_NAME_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "Alkane.h"



/**
 * IUPAC-Name des uebergebenen Alkans erzeugen.
 *
 * Der IUPAC-Name ist ein Name nach der Nomenklatur, die von der Institution International Union of Pure and Applied
 * Chemistry (IUPAC) entwickelt wurde. Ohne alle hier relevanten Regeln nennen zu muessen: Die Idee ist, dass aus dem
 * Namen die Struktur eindeutig gebildet werden kann. Dies ist insbesonder fuer die Stoffe interessant, fuer die es
 * keinen Trivialnamen gibt. Dies wird bei praktisch allen Konstitutionsisomeren der Fall sein.
 * ToDo: Vielleicht die wichtigsten Regeln durch ein paar Beispiele erlaeutern ...
 *
 * Weitere Infos fuer die Nomenklatur:
 *      https://de.wikipedia.org/wiki/IUPAC
 *      https://de.wikipedia.org/wiki/Nomenklatur_(Chemie)#Stammsysteme
 *
 *
 * >>> Hauptkette bestimmen <<<
 * Als Ausgangsinformation ist der Zahlencode des Alkans gegeben. Die Zahlen stellen eine "verbunden mit"-Aussage dar.
 * Bei der Nomenklatur der IUPAC spielt die laengste Kette in einem Isomer die entscheidene Rolle. Alle weiteren Atome
 * werden als Aeste in den Namen mit aufgenommen.
 *
 * Daher ist der erste Schritt das Finden der laensten Kette im Molekuel. Gluecklicherweise ist die Laenge der
 * laengsten Kette l durch die Erzeugung der Isomere bekannt, sodass diese Information nicht mehr bestimmt werden muss.
 * Wir muessen also nach einer Kette suchen, die genau l C-Atome lang ist. Zusaetzlich muss die Kette mit einem
 * aeussersten C-Atom - einer CH3-Bindung - beginnen und enden.
 * Das Problem ist, dass die laengste Kette nicht zwangslaeufig eindeutig sein muss !
 *
 * Beispiel:
 *
 *               c
 *               C
 *               |
 *               C - C d
 *               |
 *               C
 *               |
 * a C - C - C - C - C - C - C - C b
 *
 * Intuitiv wuerde man die horizontale Kette (a -> b) als laengste Kette auswaehlen; von der Laenge passt es ja. Rein
 * von der Laenge waere aber auch z.B. (b -> c) oder (b -> d) moeglich. Die Nomenklatur schreibt vor, dass in solchen
 * Faellen die Kette als Hauptkette gewaehlt wird, die dafuer sorgt, dass es moeglichst wenig Verschachtelungstiefen
 * bei den uebrigen C-Atomen gibt.
 * (a -> b) bewirkt, dass es eine Tiefe von 2 gibt. Bei (b -> c) oder (b -> d) ist die Tiefe 1. D. h. (b -> c) oder
 * (b -> d) waeren richtig. (a -> b) als Hauptkette waere FALSCH !
 *
 * Wir muessen also alle Pfade - und die daraus resultierenden Verschachtelungstiefen - betrachten, die genau l C-Atome
 * lang sind. Man kann sich das etwa wie eine Tiefensuche bei einem ungerichteten Graphen vorstellen, wobei die C-Atome
 * die Knoten und die Bindungen die ungerichteten Kanten sind. Das gesuchte Objekt der Tiefensuche ist bei uns ein
 * aeusserstes C-Atom; also ein CH3-Fragment.
 * Da, wie bereits erwaehnt, alle Pfade betrachtet werden muessen, muss die Tiefensuche komplett durchgefuehrt werden,
 * bis alle Knoten (bzw. C-Atome) gefunden wurden.
 *
 * Nach der vollstaendigen Suche werden die Pfade betrachtet, die genau l C-Atome lang sind. Um die Regeln mit der
 * minimalsten Verschachtelungstiefe zu erfuellen, muessen bei allen Pfaden die uebrigen C-Atome, die nicht zum Pfad
 * gehoeren, betrachtet werden. Der Pfad, bei denen die uebrigen C-Atome die geringste Verschachtelungstiefe erzeugen,
 * ist unsere Hauptkette.
 *
 * Wie kann man am Besten die Verschachtelungstiefe bestimmen ?
 * Man zaehlt in den Aesten die Anzahl an C-Atomen, die mit mehr als 2 C-Atomen verbunden sind.
 *
 * Wichtig: Die genauen Informationen ueber die uebrigen Aeste sind fuer die Bildung des IUPAC-Namen wichtig !
 *
 * Siehe:
 *      https://de.wikipedia.org/wiki/Tiefensuche
 *
 * Asserts:
 *      alkane != NULL
 */
extern void Convert_Alkane_To_IUPAC_Name
(
        struct Alkane* const alkane         // Alkan, von dem der IUPAC-Name gebildet werden soll
);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_TO_IUPAC_NAME_H */
