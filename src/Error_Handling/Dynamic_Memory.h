/**
 * Dynamic_Memory.h
 *
 *  Created on: 07.03.2021
 *      Author: x86 / Gyps
 */

#ifndef DYNAMIC_MEMORY_H
#define DYNAMIC_MEMORY_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <inttypes.h>



// Globale Variablen fuer das Zaehlen der malloc (), calloc () und free () Aufrufe
extern uint_fast64_t GLOBAL_malloc_calls;
extern uint_fast64_t GLOBAL_calloc_calls;
extern uint_fast64_t GLOBAL_free_calls;



/**
 * Aktuelle Anzahl der durch die Makros MALLOC, CALLOC und FREE getaetigten malloc (), calloc () und free () Aufrufe
 * sowie die Anzahl an fehlenden free () Aufrufe ausgeben.
 */
extern void Show_Dynamic_Memory_Status (void);



// Malloc-Aufrufe mitzaehlen
#ifndef MALLOC
    #define MALLOC(memory_size)                                                                                         \
        malloc (memory_size);                                                                                           \
        ++ GLOBAL_malloc_calls;
#else
    #error "The macro \"MALLOC\" is already defined !"
#endif /* MALLOC */

// Calloc-Aufrufe mitzaehlen
#ifndef CALLOC
    #define CALLOC(number_of_elements, element_size)                                                                    \
        calloc (number_of_elements, element_size);                                                                      \
        ++ GLOBAL_calloc_calls;
#else
    #error "The macro \"CALLOC\" is already defined !"
#endif /* CALLOC */

// Das uebergebene Objekt wird geloescht und auf nullptr gesetzt
// Eine Abfrage auf nullptr, vor dem Loeschen, ist NICHT erforderlich, da dies die free-Funktion bzw. dies sowieso vor
// dem eigentlichen Loeschprozess machen ! Falls der Zeiger wirklich ein Nullzeiger sein sollte, dann wird von diesen
// beiden Funktion einfach nichts gemacht !
// Siehe: https://stackoverflow.com/questions/4190703/is-it-safe-to-delete-a-null-pointer
#ifndef FREE_AND_SET_TO_NULL
    #define FREE_AND_SET_TO_NULL(pointer)                                                                               \
        free (pointer);                                                                                                 \
        pointer = NULL;                                                                                                 \
        ++ GLOBAL_free_calls;
#else
    #error "The macro \"FREE_AND_SET_TO_NULL\" is already defined !"
#endif /* FREE_AND_SET_TO_NULL */



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* DYNAMIC_MEMORY_H */
