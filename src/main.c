/**
 * main.c
 *
 *  Created on: 07.03.2021
 *      Author: x86
 */

#include <assert.h>
#include <limits.h>
#include "Error_Handling/Dynamic_Memory.h"



static_assert (sizeof (int) == 4, "sizeof (int) needs to be exact 4 byte !");
static_assert (CHAR_BIT == 8, "A byte needs to be exact 8 bit !");



int main (const int argc, const char* const argv [])
{
    (void) argc;
    (void) argv;

    return 0;
}
