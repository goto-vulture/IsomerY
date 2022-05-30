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

    mvwaddnstr(left_window, 1, 1, "Created by: goto-vulture", (int) strlen("Created by: goto-vulture"));
    mvwaddnstr(left_window, 3, 1, "For questions: Contact me on github.",
            (int) strlen("For questions: Contact me on github."));

    mvwaddnstr(left_window, 5, 1,  "          .-..-.__",        (int) strlen("          .-..-.__"));
    mvwaddnstr(left_window, 6, 1,  "        /`  |\\_( \"\\__",  (int) strlen("        /`  |\\_( \"\\__"));
    mvwaddnstr(left_window, 7, 1,  "        |   / __/`'--)",    (int) strlen("        |   / __/`'--)"));
    mvwaddnstr(left_window, 8, 1,  "        |  /  | |",         (int) strlen("        |  /  | |"));
    mvwaddnstr(left_window, 9, 1,  "        /_/ _/_/",          (int) strlen("        /_/ _/_/"));
    mvwaddnstr(left_window, 10, 1, " jgs  `|/))))",             (int) strlen(" jgs  `|/))))"));

    wattrset(left_window, A_BOLD);
    mvwaddnstr(left_window, 12, 1, "For me, a vulture is more desirable than money.",
            (int) strlen("For me, a vulture is more desirable than money."));
    wattrset(left_window, A_NORMAL);

    mvwaddnstr(left_window, 14, 1, "If you have practical experience with these brids, please let me know.",
            (int) strlen("If you have practical experience with these brids, please let me know."));
    mvwaddnstr(left_window, 15, 1, "My goal is to get in touch with them.",
            (int) strlen("My goal is to get in touch with them."));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void TUI_Build_About_Right_Side (const void* const input)
{
    (void) input;
    return;
}

//---------------------------------------------------------------------------------------------------------------------
