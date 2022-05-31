/**
 * @file About_TUI.h
 *
 * @date 30.05.2022
 * @author: am1
 */

#ifndef ABOUT_TUI_H ///< Include-Guard
#define ABOUT_TUI_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/**
 * @brief Funktion, die den Inhalt des linken Fensters fuellt.
 *
 * @param[in] input Optionaler Parameter fuer Eingabeinformationen
 */
extern void TUI_Build_About_Left_Side (const void* const input);

/**
 * @brief Funktion, die den Inhalt des rechten Fensters fuellt.
 *
 * Aktuell hat diese Funktion keinen Inhalt; sie ist hauptsaechlich der Vollstaendigkeit halber da.
 *
 * @param[in] input Optionaler Parameter fuer Eingabeinformationen
 */
extern void TUI_Build_About_Right_Side (const void* const input);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ABOUT_TUI_H */
