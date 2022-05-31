/*
 * Calculations_TUI.c
 *
 *  Created on: 31.05.2022
 *      Author: am1
 */

#include "Calculations_TUI.h"
#include <string.h>
#include "Main_TUI.h"


//---------------------------------------------------------------------------------------------------------------------

extern void TUI_Build_Calculations_Left_Side (const void* const input)
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

    ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "Placeholder for Calculations.",
            (int) strlen("Placeholder for Calculations.")));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void TUI_Build_Calculations_Right_Side (const void* const input)
{
    (void) input;

    return;
}

//---------------------------------------------------------------------------------------------------------------------
