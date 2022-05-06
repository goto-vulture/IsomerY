# IsomerY

## DE (See english translation below)

### Einleitung

Dieses Programm besimmt die Konstitutionsisomere (mit deren IUPAC-Namen) von Alkanen bei gegebener Anzahl an C-Atomen.

Bei sog. Konstitutionsisomere handelt es sich um das Phaenomen, dass bei gleicher Summenformel verschiedene
Strukturformeln existieren. Ein Beispiel: Butan
<!-- Wird leider nicht innerhalb eines Absatzes richtig dargestellt :( $C_{4} H_{10}$ -->

Normalerweise wird dieses Molekuel als gerade Kette gezeichnet (ASCII-Zeichnung):

    C - C - C - C

Niemand sagt den C-Atomen, dass sie sich nur in einer geraden Kette verbinden duerfen. Es sind Abzweigungen moeglich (ebenfalls ASCII-Zeichnung):

    C - C - C
        |
        C

Diese Geschichte ist mehr als nur eine Spielerei. Das Butan mit der Verzweigung hat andere physikalische Eigenschaften
wie die Variante mit der geraden Kette. [Wikipedia beschreibt es schoen zusammengefasst:](https://de.wikipedia.org/wiki/Konstitutionsisomere#Konstitutionsisomerie_oder_Strukturisomerie)

> Die Isomere sind daher im Allgemeinen verschiedene Substanzen mit unterschiedlichen chemischen (u. a. Reaktivität) und physikalischen Eigenschaften (u. a. Schmelz- und Siedepunkt, Löslichkeit).

Daher hat dieses Thema in der organischen Chemie eine gewisse Bedeutung.

Beim Butan sehen die verschiedenen Eigenschaften so aus:<br>

|          | Schmelzpunkt (°C) | Siedepunkt (°C) |
|:--------:|:-----------------:|:---------------:|
| n-Butan  |      −138,3       |      −0,5       |
| Isobutan |      −159,42      |      −11,7      |

Beim Butan gibt es nur ein Konstitutionsisomer. Es ist aber sofort einleuchtend, dass bei zusaetzlichen C-Atomen es
immer mehr Moeglichkeiten gibt. So sind bei 10 C-Atomen (Decan) bereits 75 Konstitutionsisomere (theoretisch) moeglich.

---

### Eine Übersicht an möglichen Konstitutionsisomere bei gegebener Anzahl an C-Atomen:<br>

| Anzahl der C-Atome | Anzahl der Konstitutionsisomeren |
|:------------------:|:--------------------------------:|
|         1          |                1                 |
|         2          |                1                 |
|         3          |                1                 |
|         4          |                2                 |
|         5          |                3                 |
|         6          |                5                 |
|         7          |                9                 |
|         8          |                18                |
|         9          |                35                |
|         10         |                75                |
|         11         |                159               |
|         12         |                355               |
|         13         |                802               |
|         14         |                1.858             |
|         15         |                4.347             |
|         16         |                10.359            |
|         17         |                24.894            |
|         18         |                60.523            |
|         19         |                148.284           |
|         20         |                366.319           |
|         ...        |                ...               |

Es gibt keine einfache Formel, die die Anzahl an Konstitutionsisomere bei gegebener Anzahl an C-Atomen bestimmt. Dies ist glücklicherweise kein Problem, da die theoretische Anzahl bis 100 C-Atome bereits bekannt sind.

[Siehe Liste bis 40 C-Atomen](https://de.wikipedia.org/wiki/Alkane#Anzahl_der_Isomere_von_Alkanen)

<br>

Um den Aufbau exakt ohne einem Bild beschreiben zu koennen, existiert eine Nomenklatur, die Regeln definiert wie
Isomere genannt werden, um aus dem Namen die Struktur bilden zu koennen. Diese Namen werden haeufig "IUPAC-Namen"
genannt, da die Institution "IUPAC" (International Union of Pure and Applied Chemistry) diese Nomenklatur gebildet hat.

Hier wieder unsere beiden Beispiele:

    C - C - C - C -> "n-Butan"

Das "n-" zeigt an, dass es sich um eine gerade Kette handelt.

    C - C - C     -> "2-MethylPropan"
        |
        C

Die genaue Nomenklatur ist relativ komplex und wird hier nicht weiter erlaeutert.
[Bei Interesse:](https://de.wikipedia.org/wiki/Nomenklatur_(Chemie)#Die_IUPAC-Nomenklatur)

Tatsächlich gibt es viele andere Isomertypen, die ebenfalls eine praktische Relevanz haben. Hier soll es allerdings *vorerst* nur um die Strukturisomerie gehen.

---

### Verwendete Technologien

* C11 und deren Standardbibliothek
* Das Make-Buildsystem
* Doxygen für die Dokumentation
* Natürlich Git für die Versionsverwaltung ;)

---

### Pläne für die Zukunft

* Erzeugung der Grafiken (1. Schritt: XPM-Bilder)
* Ausweitung auf weitere Stoffgruppen (Alkene, Alkiene)
* Implementierung einer GUI (vorzugsweise ressourcenarme Varianten wie FLTK)
* Vervollständigung der Doxygen Dokumentation
* Übersetzung der Doxygen-Texte nach Englisch
* Übersetzung der wichtigsten Kommentare im Quelltext nach Englisch (auch viele, die nichts mit Doxygen zu tun haben)

---

### Das Ziel des Programms

Das Programm soll in der Lage sein bei gegebener Anzahl an C-Atomen alle Konstitutionsisomere mit deren IUPAC-Namen zu
bestimmen. Zusaetzlich sollen einfache ASCII-Bilder als auch richtige Grafiken (XPM-Bilder) erstellt werden.

Je nachdem wie gut die Umsetzung geschieht, wird das Programm auf weitere Stoffklassen sowie auf weitere Isomertypen
ausgeweitet. Wann und was? Das wird sich zeigen ...

Aktuell (25.03.2022) ist das Programm in der Prae, Prae, Prae-Phase. Die Entwicklung wird kontinuirlich fortgefuehrt,
wenn ich die Zeit dazu finde.<br>

---

### Limitierungen

Um die Programmierung auf das Wesentliche zu beschränken, besitzt das Programm einige Einschränkungen, die u.U. in Zukunft entfernt werden.

* Die Berechnung ist auf maximal 40 C-Atome beschränkt. Dies sorgt dafür, dass mit weniger dynamischen Speicher gearbeitet werden muss, da die maximale Anzahl bereits während der Kompilierung bekannt ist.
* Aktuell können nur ASCII-Zeichnungen sowie XPM-Grafiken erzeugt werden. Beide Formate lassen sich ohne externe Bibliotheken bilden.
* Das Programm verfügt über keine GUI. Diese wird in der Zukunft eingebaut; allerdings ohne hohe Priorität.

---

<br>

## EN

### Einleitung

This programm creates the structural isomer (with their IUPAC names) from alkanes by given number of C-atoms.

The structual isomer describes the effect, that by equal chemical formula different structural formula exists. One example: butane.
<!-- Wird leider nicht innerhalb eines Absatzes richtig dargestellt :( $C_{4} H_{10}$ -->

In the default case this molecule will be drawn as straight chain (ASCII drawing):

    C - C - C - C

No one forbits C-atoms, that they only be allowed to create a straight chains. Branches are possible (also a ASCII drawing):

    C - C - C
        |
        C

This is more than a gimmick. The butane has with branches different physical properties comapred to the straight chain butane.
[Wikipedia describes this in a understandable way:](https://en.wikipedia.org/wiki/Isomerism#Structural_isomers)

> Isomers do not necessarily share similar chemical or physical properties.

Therefore this topic has a  certain importance.

For the butane the different attributs look like this:<br>

|           | Melting point (°C) | Boiling point (°C) |
|:---------:|:------------------:|:------------------:|
| n-butane  |      −138,3        |        −0,5        |
| Isobutane |      −159,42       |        −11,7       |

For the butane only one structual isomer exists. It is obvious, that with more c atoms many more possibilities exists. Therefore for 10 c atoms (Decane) already 75 structual isomer (theoretical) possible.

---

### A list of possible structual isomer by given numer of c atoms:<br>

| Number of c atoms  |    Number of structual isomer    |
|:------------------:|:--------------------------------:|
|         1          |                1                 |
|         2          |                1                 |
|         3          |                1                 |
|         4          |                2                 |
|         5          |                3                 |
|         6          |                5                 |
|         7          |                9                 |
|         8          |                18                |This will be 
|         9          |                35                |
|         10         |                75                |
|         11         |                159               |
|         12         |                355               |
|         13         |                802               |
|         14         |                1.858             |
|         15         |                4.347             |
|         16         |                10.359            |
|         17         |                24.894            |
|         18         |                60.523            |
|         19         |                148.284           |
|         20         |                366.319           |
|         ...        |                ...               |

There is no simple formula, that calculates the number of structual isomer by given number of c atoms. This is happily no problem, because the theoretical number up to 100 c atoms is already known.

[See table of theoretical possible alkanes](https://en.wikipedia.org/wiki/Alkane#Table_of_alkanes)

In fact many other isomer types exsits, with relevant in practice. In this project the focus is *for now* on the structual isomer.

---

### Used technologies

* C11 with the standard library
* make buildsystem
* Doxygen for the documentation
* Of course git for versions management ;)

---

### Ideas for the future

* Picture creation (First step: XPM pictures)
* Extention to other isomer types (Alkane, Alkine)
* Implementation of a GUI (lightweight technologies like FLTK)
* Completion of the doxygen documentation
* Translation of the dozygen comments to english
* Translation of the most important comments in the souce code to english

---

### The goal of the program

The program is to be creating all structual isomers with die IUPAC name by giving the number of c atoms. In addition simple ASCII "pictures" and real picture files (XPM) are to be created.

It depends on the success in the development, if the program will be extend to other isomer types. What and why? That will be shown ...

Current (25.03.2022) this program is in the prae, prae, prae-phase. The development will be continued, if I have enough time.<br>

---

### Limitations

To limit the program to the main features, there are some restrictions, they might be removed in the future.

* The calculation is limited to 40 c atoms. The effect is, that are less dynamic memory is necessary, because the upper limit is already known while compilation.
* Current the program have only the implementation for creating ASCII and XPM pictures. The reason: both can be created without external libraries.
* The program has no GUI. This will be changed in the future. Bit without high priority.

---
