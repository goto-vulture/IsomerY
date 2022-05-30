/**
 * @file Main_TUI.h
 *
 * @brief Dokumentation von ncurses: https://pubs.opengroup.org/onlinepubs/7908799/xcurses/curses.h.html
 *
 * @date 28.05.2022
 * @author: am1
 */

#ifndef MAIN_TUI_H
#define MAIN_TUI_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <ncurses.h>
#include "../Error_Handling/Assert_Msg.h"



extern WINDOW* left_window;
extern WINDOW* right_window;



/**
 * @brief Funktion fuer die Lib-Funktion atexit().
 *
 * Diese Funktion automatisch am Ende der Programmausfuehrung aufgerufen.
 */
extern void At_Exit_Function (void);

/**
 * @brief Hauptfenster zeichnen.
 *
 * @param[in] argc argc-Parameter
 * @param[in] argv Anzahl an argc-Parametern
 */
extern void
TUI_Build_Main_Window
(
        const int argc,
        const char* argv []
);

//---------------------------------------------------------------------------------------------------------------------

#ifndef ERR_CHECK
#define ERR_CHECK(test_value)                                                                                         \
    if ((test_value) == ERR)                                                                                          \
    {                                                                                                                 \
        At_Exit_Function();                                                                                           \
        ASSERT_MSG(false, "ERR as return value in ncurses function call !");                                          \
    }
#else
#error "The macro \"ERR_CHECK\" is already defined !"
#endif /* ERR_CHECK */

//---------------------------------------------------------------------------------------------------------------------

#ifndef NULL_CHECK
#define NULL_CHECK(test_value)                                                                                        \
    if ((test_value) == NULL)                                                                                         \
    {                                                                                                                 \
        At_Exit_Function();                                                                                           \
        ASSERT_MSG(false, "NULL as return value in ncurses function call !");                                         \
    }
#else
#error "The macro \"NULL_CHECK\" is already defined !"
#endif /* NULL_CHECK */

//---------------------------------------------------------------------------------------------------------------------



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MAIN_TUI_H */
