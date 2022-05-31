/**
 * @file About_TUI.c
 *
 * @date 30.05.2022
 * @author: am1
 */

#include "About_TUI.h"
#include <string.h>
#include "Main_TUI.h"
#include "../Misc.h"



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Funktion, die den Inhalt des linken Fensters fuellt.
 *
 * @param[in] input Optionaler Parameter fuer Eingabeinformationen
 */
extern void TUI_Build_About_Left_Side (const void* const input)
{
    (void) input;

    // Anzahl der verwendbaren Zeilen ermitteln
    int usable_lines = 0;
    int usable_cols = 0;
    (void) usable_cols;
    ERR_CHECK(getmaxyx(left_window, usable_lines, usable_cols));

    int effective_line = 1;

    // Zeichenkette mit den wichtigsten Informationen erstellen
    char main_infos [50];
    memset(main_infos, '\0', sizeof(main_infos));
    size_t char_left = COUNT_ARRAY_ELEMENTS(main_infos) - 1;

    strncat (main_infos, "IsomerY v", char_left);
    char_left -= strlen("IsomerY v");
#ifdef VERSION
    strncat(main_infos, VERSION, char_left);
    char_left -= strlen(VERSION);
#else
    strncat(main_infos, "N/A", char_left);
    char_left -= strlen("N/A");
#endif /* VERSION */

    strncat(main_infos, " Build type: ", char_left);
    char_left -= strlen(" Build type: ");

#ifdef RELEASE_BUILD
    strncat(main_infos, "Release", char_left);
    char_left -= strlen("Release");
#elif defined(DEBUG_BUILD)
    strncat(main_infos, "Debug", char_left);
    char_left -= strlen("Debug");
#else
    strncat(main_infos, "N/A", char_left);
    char_left -= strlen("N/A");
#endif /* RELEASE_BUILD */
    main_infos [COUNT_ARRAY_ELEMENTS(main_infos) - 1] = '\0';

    ERR_CHECK(wattrset(left_window, A_BOLD));
    ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, main_infos, (int) (COUNT_ARRAY_ELEMENTS(main_infos) - char_left)));
    ERR_CHECK(wattrset(left_window, A_NORMAL));
    effective_line += 2;
    ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "Created by: goto-vulture", (int) strlen("Created by: goto-vulture")));
    effective_line += 2;

    // Das wichtigste soll wenn es irgendwie geht, immer angezeigt werden
    if (usable_lines >= 10)
    {
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "          .-..-.__",          (int) strlen("          .-..-.__")));
        ++ effective_line;
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "        /`  |\\_( \"\\__",    (int) strlen("        /`  |\\_( \"\\__")));
        ++ effective_line;
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "        |   / __/`'--)",      (int) strlen("        |   / __/`'--)")));
        ++ effective_line;
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "        |  /  | |",           (int) strlen("        |  /  | |")));
        ++ effective_line;
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "        /_/ _/_/",            (int) strlen("        /_/ _/_/")));
        ++ effective_line;
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, " jgs  `|/))))",               (int) strlen(" jgs  `|/))))")));
        effective_line += 2;
    }

    if (usable_lines >= 12)
    {
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "For questions: Contact me on github.",
                (int) strlen("For questions: Contact me on github.")));
        effective_line += 2;
    }

    if (usable_lines >= 15)
    {
        ERR_CHECK(wattrset(left_window, A_BOLD));
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "For me, a vulture is more desirable than money.",
                (int) strlen("For me, a vulture is more desirable than money.")));
        ERR_CHECK(wattrset(left_window, A_NORMAL));
        effective_line += 2;
    }

    if (usable_lines >= 17)
    {
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "If you have practical experience with these brids, please let me know.",
                (int) strlen("If you have practical experience with these brids, please let me know.")));
        ++ effective_line;
        ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "My goal is to get in touch with them.",
                (int) strlen("My goal is to get in touch with them.")));
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Funktion, die den Inhalt des rechten Fensters fuellt.
 *
 * Aktuell hat diese Funktion keinen Inhalt; sie ist hauptsaechlich der Vollstaendigkeit halber da.
 *
 * @param[in] input Optionaler Parameter fuer Eingabeinformationen
 */
extern void TUI_Build_About_Right_Side (const void* const input)
{
    (void) input;
    return;
}

//---------------------------------------------------------------------------------------------------------------------
