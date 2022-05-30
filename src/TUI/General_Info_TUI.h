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



extern void TUI_Build_General_Info_Initial_Situation_Left_Side (const void* const input);

extern void TUI_Build_General_Info_Initial_Situation_Right_Side (const void* const input);



extern void TUI_Build_General_Info_Problem_Left_Side (const void* const input);

extern void TUI_Build_General_Info_Problem_Right_Side (const void* const input);



extern void TUI_Build_General_Info_Solution_Ideas_Left_Side (const void* const input);

extern void TUI_Build_General_Info_Solution_Ideas_Right_Side (const void* const input);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GENERAL_INFO_TUI_H */
