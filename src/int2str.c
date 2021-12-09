/**
 * @file int2str.c
 *
 *  Created on: 09.12.2021
 *      Author: am1
 */

#include "int2str.h"
#include <stdlib.h>
#include <limits.h>
#include <errno.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>



//---------------------------------------------------------------------------------------------------------------------

/**
 * long int zu C-String konvertieren.
 */
enum int2str_errno
int2str
(
        char* const output_string,          // Ausgabestring
        const size_t output_string_size,    // Laenge des Ausgabestrings
        const long int input                // Integer, der konvertiert werden soll
)
{
    if (output_string == NULL)
    {
        return INT2STR_INCONVERTIBLE;
    }
    if (output_string_size < 2)
    {
        return INT2STR_INCONVERTIBLE;
    }

    memset (output_string, '\0', output_string_size);
    snprintf (output_string, output_string_size - 1, "%ld", input);

    // Gab es eine Ausgabe von snprintf ?
    if (output_string [0] == '\0' || isspace (output_string [0]))
    {
        return INT2STR_INCONVERTIBLE;
    }

    // Nullterminierung garantieren
    output_string [output_string_size - 1] = '\0';

    return INT2STR_SUCCESS;
}

//---------------------------------------------------------------------------------------------------------------------
