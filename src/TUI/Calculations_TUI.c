/*
 * Calculations_TUI.c
 *
 *  Created on: 31.05.2022
 *      Author: am1
 */

#include "Calculations_TUI.h"
#include <string.h>
#include <menu.h>
#include "Main_TUI.h"
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Misc.h"


//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Funktion, die den Inhalt des linken Fensters fuellt. (Fuer den Unterpunkt "Initiale Situation")
 *
 * @param[in] input Optionaler Parameter fuer Eingabeinformationen
 */
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

    int effective_line = (usable_lines <= 1) ? 0 : 1;

    ERR_CHECK(mvwaddnstr(left_window, effective_line, 1, "Placeholder for Calculations.",
            (int) strlen("Placeholder for Calculations.")));

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
extern void TUI_Build_Calculations_Right_Side (const void* const input)
{
    (void) input;

    return;
}

//---------------------------------------------------------------------------------------------------------------------

extern void TUI_Choose_Type (const void* const input)
{
    (void) input;

    const uint_fast8_t NUMBER_OF_TYPES = 5;
    ITEM** items = (ITEM**) CALLOC(NUMBER_OF_TYPES, sizeof(ITEM*));
    ASSERT_ALLOC(items, "Cannot allocate memory for the TUI select type menu !", NUMBER_OF_TYPES * sizeof (ITEM*));
    SET_POINTER_ARRAY_TO_NULL(items, NUMBER_OF_TYPES);

    items [0] = new_item("1.", "Alkane");
    items [1] = new_item("2.", "Alkene");
    items [2] = new_item("3.", "Alkine");
    items [3] = new_item("4.", "Back");

    MENU* menu = new_menu (items);
    NULL_CHECK(menu);

    WINDOW* menu_window = newwin(LINES / 3, COLS / 3, LINES / 3, COLS / 3);
    NULL_CHECK(menu_window);
    ERR_CHECK(set_menu_win(menu, menu_window));
    ERR_CHECK(set_menu_sub(menu, derwin(menu_window, 15, 15, 3, 2)));
    ERR_CHECK(box(menu_window, 0, 0));
    ERR_CHECK(wbkgd(menu_window, COLOR_PAIR(1)));

    ERR_CHECK(set_menu_format(menu, NUMBER_OF_TYPES - 1, 1));
    ERR_CHECK(set_menu_mark(menu, "> "));
    ERR_CHECK(set_menu_fore(menu, COLOR_PAIR(1) | A_REVERSE));
    ERR_CHECK(set_menu_back(menu, COLOR_PAIR(1)));
    ERR_CHECK(post_menu(menu));

    const char* header_string = "> SELECT TYPE <";
    const size_t header_length = strlen(header_string);
    const int col_pos = (int) (((COLS / 3) - (int) header_length) / 2);
    ERR_CHECK(mvwaddnstr(menu_window, 1, col_pos, header_string, (int) header_length));

    _Bool got_newline = false;
    while (1)
    {
        ERR_CHECK(wrefresh(menu_window));

        // Direkte Reaktion auf der naechsten Tastatureingabe
        switch (getch())
        {
            // UP + DOWN fuehren zu der Aktuallisierung des Informations-Eintrages zu dem neu markierten Menuepunkt
        case KEY_DOWN:
            ERR_CHECK(menu_driver(menu, REQ_DOWN_ITEM));
            break;
        case KEY_UP:
            ERR_CHECK(menu_driver(menu, REQ_UP_ITEM));
            break;
            // Enter-Taste: Auswahl des aktuell markierten Menuepunktes und der dafuer hinterlegten Callback-Funktion
        case '\n':
            //*((int*) input) = item_index(current_item(menu));
            got_newline = true;
            break;
        default:
            break;
        }

        if (got_newline /* == true */)
        {
            break;
        }
    }

    if (menu != NULL)
    {
        unpost_menu(menu);
        free_menu(menu);
    }
    menu = NULL;

    for (uint_fast8_t i = 0; i < NUMBER_OF_TYPES; i ++)
    {
        if (items [i] != NULL)
        {
            free_item(items [i]);
            items [i] = NULL;
        }
    }
    FREE_AND_SET_TO_NULL(items);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
