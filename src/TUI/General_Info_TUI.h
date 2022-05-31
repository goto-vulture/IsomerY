/*
 * General_Info_TUI.h
 *
 *  Created on: 30.05.2022
 *      Author: am1
 */

#ifndef GENERAL_INFO_TUI_H
#define GENERAL_INFO_TUI_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/**
 * @brief Funktion, die den Inhalt des linken Fensters fuellt. (Fuer den Unterpunkt "Initiale Situation")
 *
 * @param[in] input Optionaler Parameter fuer Eingabeinformationen
 */
extern void TUI_Build_General_Info_Initial_Situation_Left_Side (const void* const input);

/**
 * @brief Funktion, die den Inhalt des rechten Fensters fuellt. (Fuer den Unterpunkt "Initiale Situation")
 *
 * Aktuell hat diese Funktion keinen Inhalt; sie ist hauptsaechlich der Vollstaendigkeit halber da.
 *
 * @param[in] input Optionaler Parameter fuer Eingabeinformationen
 */
extern void TUI_Build_General_Info_Initial_Situation_Right_Side (const void* const input);



/**
 * @brief Funktion, die den Inhalt des linken Fensters fuellt. (Fuer den Unterpunkt der Problemsituation)
 *
 * @param[in] input Optionaler Parameter fuer Eingabeinformationen
 */
extern void TUI_Build_General_Info_Problem_Left_Side (const void* const input);

/**
 * @brief Funktion, die den Inhalt des rechten Fensters fuellt. (Fuer den Unterpunkt der Problemsituation)
 *
 * Aktuell hat diese Funktion keinen Inhalt; sie ist hauptsaechlich der Vollstaendigkeit halber da.
 *
 * @param[in] input Optionaler Parameter fuer Eingabeinformationen
 */
extern void TUI_Build_General_Info_Problem_Right_Side (const void* const input);



/**
 * @brief Funktion, die den Inhalt des linken Fensters fuellt. (Fuer den Unterpunkt "Berechnungs- / Loesungs-Ideen")
 *
 * @param[in] input Optionaler Parameter fuer Eingabeinformationen
 */
extern void TUI_Build_General_Info_Solution_Ideas_Left_Side (const void* const input);

/**
 * @brief Funktion, die den Inhalt des rechten Fensters fuellt. (Fuer den Unterpunkt "Berechnungs- / Loesungs-Ideen")
 *
 * Aktuell hat diese Funktion keinen Inhalt; sie ist hauptsaechlich der Vollstaendigkeit halber da.
 *
 * @param[in] input Optionaler Parameter fuer Eingabeinformationen
 */
extern void TUI_Build_General_Info_Solution_Ideas_Right_Side (const void* const input);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GENERAL_INFO_TUI_H */
