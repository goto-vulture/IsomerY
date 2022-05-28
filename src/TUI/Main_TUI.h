/**
 * @file Main_TUI.h
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



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* MAIN_TUI_H */
