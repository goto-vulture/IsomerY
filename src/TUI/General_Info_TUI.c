/*
 * General_Info_TUI.c
 *
 *  Created on: 30.05.2022
 *      Author: am1
 */

#include "General_Info_TUI.h"
#include <string.h>
#include "Main_TUI.h"



//---------------------------------------------------------------------------------------------------------------------

extern void TUI_Build_General_Info_Initial_Situation_Left_Side (const void* const input)
{
    (void) input;

    // Anzahl der verwendbaren Zeilen ermitteln
    int usable_lines = 0;
    int usable_cols = 0;
    (void) usable_cols;
    ERR_CHECK(getmaxyx(left_window, usable_lines, usable_cols));
    ASSERT_FMSG(usable_lines > 0, "Got invalid number of usable lines (%d) ! Expect at least one usable line !",
            usable_lines);

    int effective_line = 1;

    ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "Placeholder for initial situation.",
            (int) strlen("Placeholder for initial situation.")));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void TUI_Build_General_Info_Initial_Situation_Right_Side (const void* const input)
{
    (void) input;

    return;
}

//=====================================================================================================================

extern void TUI_Build_General_Info_Problem_Left_Side (const void* const input)
{
    (void) input;

    // Anzahl der verwendbaren Zeilen ermitteln
    int usable_lines = 0;
    int usable_cols = 0;
    (void) usable_cols;
    ERR_CHECK(getmaxyx(left_window, usable_lines, usable_cols));
    ASSERT_FMSG(usable_lines > 0, "Got invalid number of usable lines (%d) ! Expect at least one usable line !",
            usable_lines);

    int effective_line = 1;

    ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "Placeholder for the problems.",
            (int) strlen("Placeholder for the problems.")));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void TUI_Build_General_Info_Problem_Right_Side (const void* const input)
{
    (void) input;

    return;
}

//=====================================================================================================================

extern void TUI_Build_General_Info_Solution_Ideas_Left_Side (const void* const input)
{
    (void) input;

    // Anzahl der verwendbaren Zeilen ermitteln
    int usable_lines = 0;
    int usable_cols = 0;
    (void) usable_cols;
    ERR_CHECK(getmaxyx(left_window, usable_lines, usable_cols));
    ASSERT_FMSG(usable_lines > 0, "Got invalid number of usable lines (%d) ! Expect at least one usable line !",
            usable_lines);

    int effective_line = 1;

    ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "Placeholder for solution ideas.",
            (int) strlen("Placeholder for solution ideas.")));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void TUI_Build_General_Info_Solution_Ideas_Right_Side (const void* const input)
{
    (void) input;

    return;
}

//---------------------------------------------------------------------------------------------------------------------
