/**
 * main.c
 *
 *  Created on: 07.03.2021
 *      Author: x86
 */

#include <assert.h>
#include <limits.h>
#include <time.h>
#include "Error_Handling/Dynamic_Memory.h"



// Die static_asserts funktionieren aktuell nur unter Linux
#if defined(__linux__) || defined(unix) || defined(__unix__) || defined(__unix)
// Ist auf der Zielplattform ein Byte genau 8 Bit gross ?
// Wenn nicht dann wird die Kompilierung nicht ausgefuehrt, da alle verwendeten Mechanismen davon ausgehen das dies der
// Fall ist !
static_assert (CHAR_BIT == 8,               "A char needs to be exact eight bit !");

// Ein int / unsigned int / size_t muss mindestens 4 Byte gross sein ! Bei Systemen wo dies nicht der Fall ist, ist
// i.d.R. ohne groesseren Aufwand kaum zu arbeiten.
static_assert (sizeof (int) >= 4,           "The type \"int\" needs to be at least 4 byte !");
static_assert (sizeof (unsigned int) >= 4,  "The type \"unsigned int\" needs to be at least 4 byte !");
static_assert (sizeof (size_t) >= 4,        "The type \"size_t\" needs to be at least 4 byte !");

// Weitere Variable(n), die mindestens 4 Byte gross sein sollte(n)
static_assert (sizeof (time_t) >= 4,        "The type \"time_t\" needs to be at least 4 byte !");
static_assert (sizeof (clock_t) >= 4,       "The type \"clock_t\" needs to be at least 4 byte !");

// Der Standard schreibt vor, dass SIZE_MAX mindestens 65535 betragen muss. Fuer einige Operationen (insb. calloc (),
// malloc () und Co.) ist dies hier aber zu wenig ! Daher muss sichergestellt sein, dass SIZE_MAX eine akzeptable
// Groesse besitzt.
// I.d.R. ist SIZE_MAX (2^32) - 1 oder (2^64) - 1
static_assert (SIZE_MAX >= UINT32_MAX,      "SIZE_MAX needs to be at least 2^32 - 1 !");
#endif /* defined(__linux__) || defined(unix) || defined(__unix__) || defined(__unix) */



//=====================================================================================================================

int main (const int argc, const char* const argv [])
{
    (void) argc;
    (void) argv;

    Show_Dynamic_Memory_Status ();

    return 0;
}

//=====================================================================================================================
