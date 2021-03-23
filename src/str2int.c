/**
 * str2int.c
 *
 *  Created on: 07.03.2021
 *      Author: x86 / Gyps
 */

#include "str2int.h"
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>



//---------------------------------------------------------------------------------------------------------------------

/**
 * C-String zu long int konvertieren.
 * Fehlerhafte Eingaben werden ueber den Rueckgabewert angezeigt.
 *
 * Asserts:
 *      N/A
 */
enum str2int_errno                          // Error-enum (STR2INT_SUCCESS: Erfolgreich durchgefuehrt)
str2int
(
        long int* restrict out,             // Ausgabeadresse
        const char* restrict input_string,  // String, der konvertiert werden soll
        const int base                      // Zahlensystem
)
{
    char* end = NULL;

    if (input_string == NULL)
    {
        return STR2INT_INCONVERTIBLE;
    }
    if (input_string [0] == '\0' || isspace (input_string [0]))
    {
        return STR2INT_INCONVERTIBLE;
    }
    if (base <= 0)
    {
        return STR2INT_INCONVERTIBLE;
    }
    if (out == NULL)
    {
        return STR2INT_INCONVERTIBLE;
    }

    errno = 0;
    const long int l = strtol (input_string, &end, base);

    if (l > LONG_MAX || (errno == ERANGE && l == LONG_MAX))
    {
        return STR2INT_OVERFLOW;
    }
    if (l < LONG_MIN || (errno == ERANGE && l == LONG_MIN))
    {
        return STR2INT_UNDERFLOW;
    }
    if (*end != '\0')
    {
        return STR2INT_INCONVERTIBLE;
    }

    *out = l;

    return STR2INT_SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * long int zu C-String konvertieren.
 *
 * Asserts:
 *      N/A
 */
void
int2str
(
        char* const output_string,          // Ausgabestring
        const size_t output_string_size,    // Laenge des Ausgabestrings
        const long int input                // Integer, der konvertiert werden soll
)
{
    memset (output_string, '\0', output_string_size);
    snprintf (output_string, output_string_size - 1, "%ld", input);

    // Nullterminierung garantieren
    output_string [output_string_size - 1] = '\0';

    return;
}

//---------------------------------------------------------------------------------------------------------------------
