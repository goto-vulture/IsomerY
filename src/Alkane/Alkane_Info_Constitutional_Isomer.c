/**
 * @file Alkane_Info_Constitutional_Isomer.c
 *
 * @brief Hier stehen einige allgemeine Infos ueber die Konstitutionsisomere der Alkane. Z.B. die Anzahl der
 * Konstitutionsisomere bei gegebener Anzahl an C-Atomen.
 *
 * @date 07.03.2021
 * @author x86 / Gyps
 */

#include "Alkane_Info_Constitutional_Isomer.h"



/**
 * @brief Anzahl an Konstitutionsisomere (Strukturisomere) bei gegebener Anzahl an C-Atomen.
 *
 * @see http://www.mathe2.uni-bayreuth.de/sascha/oeis/alkane.html
 */
const uint_fast64_t NUMBER_OF_ALKANE_CONSTITUTIONAL_ISOMER [MAX_NUMBER_OF_C_ATOMS] =
{
        1,              1,              1,              2,              3,
        5,              9,              18,             35,             75,

        159,            355,            802,            1858,           4347,
        10359,          24894,          60523,          148284,         366319,

        910726,         2278658,        5731580,        14490245,       36797588,
        93839412,       240215803,      617105614,      1590507121,     4111846763,

        10660307791,    27711253769,    72214088660,    188626236139,   493782952902,
        1295297588128,  3404490780161,  8964747474595,  23647478933969, 62481801147341
};

/**
 * @brief Die Zahlenwoerter (Zahlenpraefixe), die im Programm hinterlegt sind.
 *
 * @see https://de.wikipedia.org/wiki/Nomenklatur_(Chemie)#Zahlenpr%C3%A4fixe_in_chemischen_Namen
 */
const char* NUMBER_WORDS [NUMBER_OF_NUMBER_WORDS] =
{
        "Mono", "Di",    "Tri",   "Tetra", "Penta",
        "Hexa", "Hepta", "Ocata", "Nona",  "Deca"
};

/**
 * @brief Die Alkanwoerter, die im Programm hinterlegt sind. (Deutsch)
 *
 * @see https://de.wikipedia.org/wiki/Nomenklatur_(Chemie)#Lineare_Ketten
 * @see https://de.wikipedia.org/wiki/H%C3%B6here_Alkane
 */
const char* ALKAN_WORDS_DE [NUMBER_OF_ALKAN_WORDS] =
{
        "Methan",         "Ethan",           "Propan",         "Butan",           "Pentan" ,
        "Hexan",          "Heptan",          "Octan",          "Nonan",           "Decan",

        "Undecan",        "Dodecan",         "Tridecan",       "Tetradecan",      "Pentadecan",
        "Hexadecan",      "Heptadecan",      "Octadecan",      "Nonadecan",       "Eicosan",

        "Heneicosan",     "Docosan",         "Tricosan",       "Tetracosan",      "Pentacosan",
        "Hexacosan",      "Heptacosan",      "Octacosan",      "Nonacosan",       "Triacontan",

        "Hentriacontan",  "Dotriacontan",    "Dotriacontan",   "Tetratriacontan", "Pentatriacontan",
        "Hexatriacontan", "Heptatriacontan", "Octatriacontan", "Nonatriacontan",  "Tetracontan"
};

/**
 * @brief Die Alkanwoerter, die im Programm hinterlegt sind. Diesmal auf Englisch.
 *
 * Hauptunterschied:    An den deutschen Namen wird ein "e" angehaengt.
 *                      Aus "Methan" wird "Methane"; aus "Ethan" wird "Ethane" u.s.w
 */
const char* ALKAN_WORDS_EN [NUMBER_OF_ALKAN_WORDS] =
{
        "Methane",          "Ethane",           "Propane",          "Butane",           "Pentane",
        "Hexane",           "Heptane",          "Octane",           "Nonane",           "Decane",

        "Undecane",         "Dodecane",         "Tridecane",        "Tetradecane",      "Pentadecane",
        "Hexadecane",       "Heptadecane",      "Octadecane",       "Nonadecane",       "Eicosane",

        "Heneicosane",      "Docosane",         "Tricosane",        "Tetracosane",      "Pentacosane",
        "Hexacosane",       "Heptacosane",      "Octacosane",       "Nonacosane",       "Triacontane",

        "Hentriacontane",   "Dotriacontane",    "Dotriacontane",    "Tetratriacontane", "Pentatriacontane",
        "Hexatriacontane",  "Heptatriacontane", "Octatriacontane",  "Nonatriacontane",  "Tetracontane"
};

/**
 * @brief Die Substituenten, die im Programm hinterlegt sind.
 *
 * Die Substituenten wurden erzeugt indem die letzten beiden Zeichen der Alkan-Woerter durch "yl" ersetzt werden !
 */
const char* ALKYL_WORDS [NUMBER_OF_ALKYL_WORDS] =
{
        "Methyl",         "Ethyl",           "Propyl",         "Butyl",           "Pentyl" ,
        "Hexyl",          "Heptyl",          "Octyl",          "Nonyl",           "Decyl",

        "Undecyl",        "Dodecyl",         "Tridecyl",       "Tetradecyl",      "Pentadecyl",
        "Hexadecyl",      "Heptadecyl",      "Octadecyl",      "Nonadecyl",       "Eicosyl",

        "Heneicosyl",     "Docosyl",         "Tricosyl",       "Tetracosyl",      "Pentacosyl",
        "Hexacosyl",      "Heptacosyl",      "Octacosyl",      "Nonacosyl",       "Triacontyl",

        "Hentriacontyl",  "Dotriacontyl",    "Dotriacontyl",   "Tetratriacontyl", "Pentatriacontyl",
        "Hexatriacontyl", "Heptatriacontyl", "Octatriacontyl", "Nonatriacontyl",  "Tetracontyl"
};
