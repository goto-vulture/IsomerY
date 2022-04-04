
# IsomerY

## DE

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
|          |                   |                 |


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

* Ausweitung auf weitere Stoffgruppen (Alkene, Alkiene)
* Implementierung einer GUI (vorzugsweise ressourcenarme Varianten wie FLTK)
* Vervollständigung der Doxygen Dokumentation
* Übersetzung dieser Readme-Datei nach Englisch
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

Will be inserted in the future ...
