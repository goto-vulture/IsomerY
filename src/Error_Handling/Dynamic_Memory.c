/**
 * Dynamic_Memory.c
 *
 *  Created on: 07.03.2021
 *      Author: x86
 */

#include "Dynamic_Memory.h"
#include <stdio.h>



// Globale Variablen fuer das Zaehlen der malloc (), calloc () und free () Aufrufe
uint_fast64_t GLOBAL_malloc_calls   = 0;
uint_fast64_t GLOBAL_calloc_calls   = 0;
uint_fast64_t GLOBAL_free_calls     = 0;



//---------------------------------------------------------------------------------------------------------------------

/**
 * Aktuelle Anzahl der durch die Makros MALLOC, CALLOC und FREE getaetigten malloc (), calloc () und free () Aufrufe
 * sowie die Anzahl an fehlenden free () Aufrufe ausgeben.
 */
void Show_Dynamic_Memory_Status (void)
{
    const uint_fast64_t missing_free_calls = (GLOBAL_malloc_calls + GLOBAL_calloc_calls) - GLOBAL_free_calls;

    printf ("malloc () calls:       %"PRIuFAST64"\n"
            "calloc () calls:       %"PRIuFAST64"\n"
            "free () calls:         %"PRIuFAST64"\n"
            "Missing free () calls: %"PRIuFAST64" %s",
            GLOBAL_malloc_calls,
            GLOBAL_calloc_calls,
            GLOBAL_free_calls,
            missing_free_calls,
            (missing_free_calls == 0) ? ":D" : ":o");

    return;
}

//---------------------------------------------------------------------------------------------------------------------
