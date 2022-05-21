/**
 * @file Dynamic_Memory.c
 *
 * @brief Anzahl an malloc (), calloc (), realloc () und free () Aufrufe ausgeben.
 *
 * Zusaetzlich wird die Anzahl an fopen () und fclose () Aufrufe gezaehlt.
 *
 * @date 07.03.2021
 * @author x86 / Gyps
 */

#include "Dynamic_Memory.h"
#include <stdio.h>



// Globale Variablen fuer das Zaehlen der malloc (), calloc (), realloc () und free () Aufrufe
uint_fast64_t GLOBAL_malloc_calls   = 0;
uint_fast64_t GLOBAL_calloc_calls   = 0;
uint_fast64_t GLOBAL_realloc_calls  = 0;
uint_fast64_t GLOBAL_free_calls     = 0;

// Globale Variablen fur das Zaehlen von fopen () und fclose () Aufrufe
uint_fast64_t GLOBAL_fopen_calls	= 0;
uint_fast64_t GLOBAL_fclose_calls	= 0;



//---------------------------------------------------------------------------------------------------------------------

/*
 * Aktuelle Anzahl der durch die Makros MALLOC, CALLOC und FREE getaetigten malloc (), calloc () und free () Aufrufe
 * sowie die Anzahl an fehlenden free () Aufrufe ausgeben.
 *
 * Zusaetzlich wird die Anzahl an fopen () und fclose () Aufrufe gezaehlt.
 */
void Show_Dynamic_Memory_Status (void)
{
    const uint_fast64_t missing_free_calls = (GLOBAL_malloc_calls + GLOBAL_calloc_calls) - GLOBAL_free_calls;
    const uint_fast64_t missing_fclose_calls = GLOBAL_fopen_calls - GLOBAL_fclose_calls;

    printf ("malloc () calls:           %10" PRIuFAST64 "\n"
            "calloc () calls:           %10" PRIuFAST64 "\n"
            "realloc () calls:          %10" PRIuFAST64 "\n"
            "free () calls:             %10" PRIuFAST64 "\n"
            "fopen () calls:            %10" PRIuFAST64 "\n"
            "fclose () calls:           %10" PRIuFAST64 "\n"
            "Missing free () calls:     %10" PRIuFAST64 " %s\n"
            "Missing fclose () calls:   %10" PRIuFAST64 " %s\n",
            GLOBAL_malloc_calls,
            GLOBAL_calloc_calls,
            GLOBAL_realloc_calls,
            GLOBAL_free_calls,
            GLOBAL_fopen_calls,
            GLOBAL_fclose_calls,
            missing_free_calls,
            (missing_free_calls == 0) ? ":D" : ":o",
        missing_fclose_calls,
            (missing_fclose_calls == 0) ? ":D" : ":(");
    fflush (stdout);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
