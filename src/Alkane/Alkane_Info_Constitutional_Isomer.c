/**
 * Alkane_Constitutional_Isomer.c
 *
 *  Created on: 07.03.2021
 *      Author: x86
 */

#include "Alkane_Info_Constitutional_Isomer.h"



/**
 * Anzahl an Konstitutionsisomere (Strukturisomere) bei gegebener Anzahl an C-Atomen.
 *
 * Quelle: http://www.mathe2.uni-bayreuth.de/sascha/oeis/alkane.html
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
