/**
 * @file About_TUI.c
 *
 * @date 30.05.2022
 * @author: am1
 */

#include "About_TUI.h"
#include <string.h>
#include "Main_TUI.h"



//---------------------------------------------------------------------------------------------------------------------

extern void TUI_Build_About_Left_Side (const void* const input)
{
    (void) input;

    // Anzahl der verwendbaren Zeilen ermitteln
    int usable_lines = 0;
    int usable_cols = 0;
    (void) usable_cols;
    ERR_CHECK(getmaxyx(left_window, usable_lines, usable_cols));

    int effective_line = 1;

    ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "Created by: goto-vulture", (int) strlen("Created by: goto-vulture")));
    effective_line += 2;
    if (usable_lines >= 10)
    {
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "For questions: Contact me on github.",
                (int) strlen("For questions: Contact me on github.")));
        effective_line += 2;
    }

    // Das wichtigste soll wenn es irgendwie geht, immer angezeigt werden
    if (usable_lines >= 8)
    {
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1,  "          .-..-.__",        (int) strlen("          .-..-.__")));
        ++ effective_line;
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1,  "        /`  |\\_( \"\\__",  (int) strlen("        /`  |\\_( \"\\__")));
        ++ effective_line;
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1,  "        |   / __/`'--)",    (int) strlen("        |   / __/`'--)")));
        ++ effective_line;
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1,  "        |  /  | |",         (int) strlen("        |  /  | |")));
        ++ effective_line;
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1,  "        /_/ _/_/",          (int) strlen("        /_/ _/_/")));
        ++ effective_line;
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, " jgs  `|/))))",             (int) strlen(" jgs  `|/))))")));
        effective_line += 2;
    }


    if (usable_lines >= 13)
    {
        ERR_CHECK(wattrset(left_window, A_BOLD));
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "For me, a vulture is more desirable than money.",
                (int) strlen("For me, a vulture is more desirable than money.")));
        ERR_CHECK(wattrset(left_window, A_NORMAL));
        effective_line += 2;
    }

    if (usable_lines >= 15)
    {
        ERR_CHECK(mvwaddnstr(left_window, 14, 1, "If you have practical experience with these brids, please let me know.",
                (int) strlen("If you have practical experience with these brids, please let me know.")));
        ++ effective_line;
        ERR_CHECK(mvwaddnstr(left_window, 15, 1, "My goal is to get in touch with them.",
                (int) strlen("My goal is to get in touch with them.")));
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void TUI_Build_About_Right_Side (const void* const input)
{
    (void) input;
    return;
}

//---------------------------------------------------------------------------------------------------------------------
