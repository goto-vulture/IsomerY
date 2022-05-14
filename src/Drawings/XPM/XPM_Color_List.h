/**
 * @file XPM_Color_List.h
 *
 * @brief Hier sind alle Farben mit einem Zeichen aufgelistet, die in den XPM-Bildern verwendet werden.
 *
 * Bei der Konvertierung der Original-PNG-Bilder (siehe Ordner: Source_Pictures) wurden fuer gleiche Farben verschiedene
 * Zeichen verwendet. Dies fuehrte unweigerlich dazu, dass eine Farbe mittels verschiedener Zeichen dargestellt wird.
 *
 * Der einfachste Ansatz ist es bei jedem XPM-Bild alle Farben z.B. mit einer aufsteigenden Nummer zu versehen. Dadurch
 * wird dann ein Pixel mittels einer zwei Zeichen langen Zeichenkette dargestellt. An sich ist dies kein Problem.
 * Allerdings: ALLE Bilder, die vom Programm erstellt werden, sind dann doppelt so gross im Vergleich zu den
 * konvertierten Bildern !
 * Diese Problematik wird noch dadurch verschaerft, dass XPM-Bilder durch ihren simplen Aufbau grundsaetzlich immer
 * deutlich groesser sind, als andere Bildformate.
 *
 * @date 22.02.2022
 * @author x86 / Gyps
 */

#ifndef XPM_COLOR_LIST_H
#define XPM_COLOR_LIST_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */

static const char* XPM_COLOR_LIST [] =
{
    " 	g #FFFFFF",
    "A	g #CCCCCC",
    "B	g #999999",
    "C	g #666666",
    "D	g #333333",
    "E	g #222222",
    "F	g #111111",
    "G	g #000000",
    "H	g #444444",
    "I	g #555555",
    "J	g #DDDDDD",
    "K	g #BBBBBB",
    "L	g #888888",
    "M	g #AAAAAA",
    "N	g #EEEEEE",
    "O	g #777777",
    "P	g #D5D5D5",
    "Q	g #535353",
    "R	g #434343",
    "S	g #5F5F5F",
    "T	g #4F4F4F",
    "U	g #D7D7D7",
    "V	g #CACACA",
    "W	g #F3F3F3",
    "X	g #EBEBEB",
    "Y	g #EFEFEF",
    "Z	g #DBDBDB",
    "a	g #D2D2D2",
    "b	g #5D5D5D",
    "c	g #4D4D4D",
    "d	g #494949",
    "e	g #C8C8C8",
    "f	c #FDFDFD",
    "g	c #EDEDED",
    "h	c #010101",
    "i	c #DADADA",
    "j	c #FCFCFC",
    "k	c #D3D3D3",
    "l	c #D4D4D4",
    "m	c #F0F0F0",
};

// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* XPM_COLOR_LIST_H */
