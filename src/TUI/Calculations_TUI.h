/*
 * Calculations_TUI.h
 *
 *  Created on: 31.05.2022
 *      Author: am1
 */

#ifndef CALCULATIONS_TUI_H
#define CALCULATIONS_TUI_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <inttypes.h>



extern uint_fast8_t GLOBAL_selected_type;
extern uint_fast8_t GLOBAL_selected_number_of_c_atoms;



/**
 * @brief Funktion, die den Inhalt des linken Fensters fuellt.
 *
 * @param[in] input Optionaler Parameter fuer Eingabeinformationen
 */
extern void TUI_Build_Calculations_Left_Side (const void* const input);

/**
 * @brief Funktion, die den Inhalt des rechten Fensters fuellt.
 *
 * Aktuell hat diese Funktion keinen Inhalt; sie ist hauptsaechlich der Vollstaendigkeit halber da.
 *
 * @param[in] input Optionaler Parameter fuer Eingabeinformationen
 */
extern void TUI_Build_Calculations_Right_Side (const void* const input);

extern void TUI_Choose_Type (const void* const input);

extern void TUI_Choose_Number_Of_C_Atoms (const void* const input);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* TUI_CALCULATIONS_H */
