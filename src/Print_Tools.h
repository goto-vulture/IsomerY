/**
 * Print_Tools.h
 *
 *  Created on: 13.03.2021
 *      Author: x86
 */

#ifndef PRINT_TOOLS_H
#define PRINT_TOOLS_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stdio.h>



/**
 * Ausgabe mittels printf, wobei automatisch stdout geleert wird.
 * Dies dient dazu, dass eine Ausgabe direkt auf dem Bildschirm erscheint.
 */
#ifndef PRINTF_FFLUSH
    #define PRINTF_FFLUSH(format_string, ...)                   \
    printf (format_string, __VA_ARGS__); fflush (stdout);
#else
    #error "The macro \"PRINTF_FFLUSH\" is already defined !"
#endif /* PRINTF_FFLUSH */

/**
 * Ausgabe mittels printf, wobei automatisch stdout geleert wird.
 * Dies dient dazu, dass eine Ausgabe direkt auf dem Bildschirm erscheint.
 *
 * Der Hauptunterschied zwischen diesem Makro und der Verwendung eines Makros mit puts: Es wird hier nicht zwingend ein
 * Zeilenumbruch geschrieben !
 */
#ifndef PRINTF_NO_VA_ARGS_FFLUSH
    #define PRINTF_NO_VA_ARGS_FFLUSH(format_string)             \
    printf (format_string); fflush (stdout);
#else
    #error "The macro \"PRINTF_NO_VA_ARGS_FFLUSH\" is already defined !"
#endif /* PRINTF_NO_VA_ARGS_FFLUSH */

/**
 * Ausgabe mittels puts, wobei automatisch stdout geleert wird.
 * Dies dient dazu, dass eine Ausgabe direkt auf dem Bildschirm erscheint.
 */
#ifndef PUTS_FFLUSH
    #define PUTS_FFLUSH(string)                                 \
    puts (string); fflush (stdout);
#else
    #error "The macro \"PUTS_FFLUSH\" is already defined !"
#endif /* PUTS_FFLUSH */



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* PRINT_TOOLS_H */
