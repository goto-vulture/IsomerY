/**
 * main.c
 *
 *  Created on: 07.03.2021
 *      Author: x86
 */

#include <assert.h>
#include <limits.h>
#include "Error_Handling/Dynamic_Memory.h"



// Die static_asserts funktionieren aktuell nur unter Linux
#if defined(__linux__) || defined(unix) || defined(__unix__) || defined(__unix)
static_assert (sizeof (int) == 4, "sizeof (int) needs to be exact 4 byte !");
static_assert (CHAR_BIT == 8, "A byte needs to be exact 8 bit !");
#endif /* defined(__linux__) || defined(unix) || defined(__unix__) || defined(__unix) */



int main (const int argc, const char* const argv [])
{
    (void) argc;
    (void) argv;

    Show_Dynamic_Memory_Status ();

    return 0;
}
