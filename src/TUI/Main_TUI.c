/**
 * @file Main_TUI.c
 *
 * @date 28.05.2022
 * @author: am1
 */

#include "Main_TUI.h"
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <menu.h>
#include "../Misc.h"
#include "../Error_Handling/Dynamic_Memory.h"



#ifndef ALLOCATED_MENU_ENTRIES
#define ALLOCATED_MENU_ENTRIES 16
#else
#error "The marco \"ALLOCATED_MENU_ENTRIES\" is already defined !"
#endif /* ALLOCATED_MENU_ENTRIES */



enum Menu_Types
{
    MAIN_MENU = 0,
    CREATION_MENU,
    GENERAL_INFO_MENU,
    ABOUT_MENU
};



ITEM** items        = NULL;
MENU* menu          = NULL;
WINDOW* menu_window = NULL;

enum Menu_Types current_menue = MAIN_MENU;



static void Draw_Main_Window
(
        const char* menue_position_string
);

static void
Update_Menu
(
        const enum Menu_Types chosen_menu
);

static void
Exec_Menu_Entry
(
        const enum Menu_Types current_menu,
        const int selected_menu_entry
);

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Funktion fuer die Lib-Funktion atexit().
 *
 * Diese Funktion automatisch am Ende der Programmausfuehrung aufgerufen.
 */
extern void At_Exit_Function (void)
{
    unpost_menu(menu);
    free_menu(menu);
    menu = NULL;

    for (int i = 0; i < ALLOCATED_MENU_ENTRIES; i ++)
    {
        if (items [i] != NULL)
        {
            free_item(items [i]);
            items [i] = NULL;
        }
    }
    FREE_AND_SET_TO_NULL(items);

    endwin();

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Hauptfenster zeichnen.
 *
 * @param[in] argc argc-Parameter
 * @param[in] argv Anzahl an argc-Parametern
 */
extern void
TUI_Build_Main_Window
(
        const int argc,
        const char* argv []
)
{
    (void) argc;
    (void) argv;

    atexit(At_Exit_Function);

    initscr();
    curs_set(0);
    clear ();
    noecho();

    start_color();
    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_BLACK, COLOR_WHITE);
    bkgd(COLOR_PAIR(1));

    cbreak();
    keypad(stdscr, TRUE);

    Draw_Main_Window("Main menu");

    items = (ITEM**) CALLOC(ALLOCATED_MENU_ENTRIES, sizeof(ITEM*));
    ASSERT_ALLOC(items, "Cannot allocate memory for the TUI main menu !", ALLOCATED_MENU_ENTRIES * sizeof (ITEM*));
    SET_POINTER_ARRAY_TO_NULL(items, ALLOCATED_MENU_ENTRIES);

    menu = new_menu (items);

    menu_window = newwin(15, 30, 2, 2);
    set_menu_win(menu, menu_window);
    set_menu_sub (menu, derwin(menu_window, 6, 28, 3, 2));
    box(menu_window, 0, 0);
    wbkgd(menu_window, COLOR_PAIR(1));

    set_menu_format(menu, ALLOCATED_MENU_ENTRIES - 1, 1);
    set_menu_mark(menu, "> ");
    set_menu_fore(menu, COLOR_PAIR(1)|A_REVERSE);
    set_menu_back(menu, COLOR_PAIR(1));
    post_menu(menu);

    // Hauptmenue erzeugen
    Update_Menu(current_menue);

    while (1)
    {
        switch (getch())
        {
        case KEY_DOWN:
            menu_driver(menu, REQ_DOWN_ITEM);
            break;
        case KEY_UP:
            menu_driver(menu, REQ_UP_ITEM);
            break;
        case '\n':
            Exec_Menu_Entry(current_menue, item_index(current_item(menu)));
            break;
        default:
            break;
        }
        refresh();
        wrefresh(menu_window);
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Draw_Main_Window
(
        const char* menue_position_string
)
{
    clear();

    // Komplette Umrandung des Hauptfensters
    box(stdscr, 0, 0);

    char window_title [25];
    memset(window_title, '\0', sizeof (window_title));

    #ifdef VERSION
        const char* version = VERSION;
    #else
        const char* version = "N/A";
    #endif /* VERSION */

    #ifdef RELEASE_BUILD
        const char* type = "Release";
    #elif defined(DEBUG_BUILD)
        const char* type = "Debug";
    #else
        const char* type = "N/A";
    #endif /* RELEASE_BUILD */

    // Fenstertitel erzeugen und zeichnen
    const int title_length = snprintf(window_title, COUNT_ARRAY_ELEMENTS(window_title), " IsomerY v.%s %s ",
            version, type);
    mvprintw(0, (COLS / 2) - (title_length / 2), "%s", window_title);

    // Vertikale Linie durch's Fenster
    for (int i = 2; i < LINES - 6; ++ i)
    {
        move(i, COLS / 2);
        addch(ACS_VLINE);
    }

    // Flaeche mit optionalen Informationen zum aktuell markierten Menuepunkt zeichnen
    move(LINES - 15, 0);
    addch(ACS_LTEE);
    for (int i = 1; i < COLS / 2; ++ i)
    {
        addch(ACS_HLINE);
    }
    addch(ACS_RTEE);
    move(LINES - 14, 1);
    attrset(A_BOLD);
    addstr("Information:");
    attrset(A_NORMAL);

    // Die beiden Flaechen am unteren Ende des Fensters zeichnen
    move(LINES - 7, 0);
    addch(ACS_LTEE);
    for (int i = 1; i < COLS - 1; ++ i)
    {
        addch((i == (COLS / 2)) ? ACS_BTEE : ACS_HLINE);
    }
    addch(ACS_RTEE);
    move(LINES - 6, 1);
    attrset(A_BOLD);
    addstr("Menu position:");
    attrset(A_NORMAL);
    addch(' ');
    addstr(menue_position_string);

    move(LINES - 5, 0);
    addch(ACS_LTEE);
    for (int i = 1; i < COLS - 1; ++ i)
    {
        addch(ACS_HLINE);
    }
    addch(ACS_RTEE);

    move(LINES - 4, 1);
    attrset(A_BOLD);
    addstr("Status:");
    attrset(A_NORMAL);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void
Update_Menu
(
        const enum Menu_Types chosen_menu
)
{
    struct Menu_Content
    {
        const enum Menu_Types menu;
        const char* menu_name;
        const char* menu_position;
        const char* item_first_string [ALLOCATED_MENU_ENTRIES];
        const char* item_second_string [ALLOCATED_MENU_ENTRIES];
    };

    // "Datenbank" mit allen Infos, die fuer die Menuerstellung benoetigt werden
    const struct Menu_Content menu_content [] =
    {
            {
                    .menu               = MAIN_MENU,
                    .menu_name          = "MAIN MENU",
                    .menu_position      = "Main menu",
                    .item_first_string  =
                    {
                            "1.",
                            "2.",
                            "3.",
                            "4.",
                            NULL
                    },
                    .item_second_string =
                    {
                            "Calculations",
                            "General info",
                            "About",
                            "Exit",
                            NULL
                    }
            },
            {
                    .menu               = CREATION_MENU,
                    .menu_name          = "CREATION MENU",
                    .menu_position      = "Main menu > Creation menu",
                    .item_first_string  =
                    {
                            "1.",
                            "2.",
                            NULL
                    },
                    .item_second_string =
                    {
                            "Setup calculation",
                            "Back",
                            NULL
                    }
            },
            {
                    .menu               = GENERAL_INFO_MENU,
                    .menu_name          = "GENERAL INFOS",
                    .menu_position      = "Main menu > General infos",
                    .item_first_string  =
                    {
                            "1.",
                            "2.",
                            NULL
                    },
                    .item_second_string =
                    {
                            "...",
                            "Back",
                            NULL
                    }
            },
            {
                    .menu               = ABOUT_MENU,
                    .menu_name          = "ABOUT",
                    .menu_position      = "Main menu > About",
                    .item_first_string  =
                    {
                            "1.",
                            "2.",
                            NULL
                    },
                    .item_second_string =
                    {
                            "Show something",
                            "Back (Esc)",
                            NULL
                    }
            }
    };

    // Globale Menue-Information anpassen
    current_menue = chosen_menu;

    // Komplette Menuestruktur loeschen
    for (int i = 0; i < ALLOCATED_MENU_ENTRIES; i ++)
    {
        if (items [i] != NULL)
        {
            free_item(items [i]);
            items [i] = NULL;
        }
    }

    int menu_window_lines = 0;
    int menu_window_cols = 0;
    int menu_window_cursor_x = 0;
    int menu_window_cursor_y = 0;
    getmaxyx(menu_window, menu_window_lines, menu_window_cols);
    getyx (menu_window, menu_window_cursor_x, menu_window_cursor_y);

    // Menunamenzeile komplett leeren
    for (int i = 0; i < menu_window_cols - 4; ++ i)
    {
        mvwaddch(menu_window, 1, 2 + i, ' ');
    }

    // Menueinhalt anpassen
    const char* menu_position_string = NULL;
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(menu_content); ++ i)
    {
        if (menu_content [i].menu == chosen_menu)
        {
            int counter = 0;
            while (menu_content [i].item_first_string [counter] != NULL &&
                    menu_content [i].item_second_string [counter] != NULL)
            {
                items [counter] = new_item (menu_content [i].item_first_string [counter],
                        menu_content [i].item_second_string [counter]);
                ++ counter;
            }
            items [counter] = NULL;

            mvwaddnstr(menu_window, 1, 2, menu_content [i].menu_name, menu_window_cols - 4);
            menu_position_string = menu_content [i].menu_position;

            break;
        }
    }

    // Anzeigen aktuallisieren (Hauptfenster, Menue-Fenster, Umrandung des Menue-Fensters)
    Draw_Main_Window(menu_position_string);
    unpost_menu(menu);
    set_menu_items(menu, items);

    post_menu(menu);
    box(menu_window, 0, 0);
    refresh();
    wrefresh(menu_window);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Exit_Wrapper (const void* input);
static void Exit_Wrapper (const void* input)
{
    exit(*(int*) input);
    return;
}
static void Update_Menu_Wrapper (const void* input);
static void Update_Menu_Wrapper (const void* input)
{
    Update_Menu(*(enum Menu_Types*) input);
    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void
Exec_Menu_Entry
(
        const enum Menu_Types current_menu,
        const int selected_menu_entry
)
{
    struct Menu_Functions
    {
        const enum Menu_Types menu;
        void (*function [ALLOCATED_MENU_ENTRIES]) (const void*);
        const void* function_input [ALLOCATED_MENU_ENTRIES];
    };

    static const enum Menu_Types main_menu          = MAIN_MENU;
    static const enum Menu_Types about_menu         = ABOUT_MENU;
    static const enum Menu_Types general_info_menu  = GENERAL_INFO_MENU;
    static const enum Menu_Types creation_menu      = CREATION_MENU;
    static const int exit_input                     = 0;

    const struct Menu_Functions menu_functions [] =
    {
            {
                    .menu           = MAIN_MENU,
                    .function       =
                    {
                            &Update_Menu_Wrapper,
                            &Update_Menu_Wrapper,
                            &Update_Menu_Wrapper,
                            &Exit_Wrapper,
                            NULL
                    },
                    .function_input =
                    {
                            &creation_menu,
                            &general_info_menu,
                            &about_menu,
                            &exit_input,
                            NULL
                    }
            },
            {
                    .menu           = GENERAL_INFO_MENU,
                    .function       =
                    {
                            &Exit_Wrapper,
                            &Update_Menu_Wrapper,
                            NULL
                    },
                    .function_input =
                    {
                            &exit_input,
                            &main_menu,
                            NULL
                    }
            },
            {
                    .menu           = CREATION_MENU,
                    .function       =
                    {
                            &Exit_Wrapper,
                            &Update_Menu_Wrapper,
                            NULL
                    },
                    .function_input =
                    {
                            &exit_input,
                            &main_menu,
                            NULL
                    }
            },
            {
                    .menu           = ABOUT_MENU,
                    .function       =
                    {
                            &Exit_Wrapper,
                            &Update_Menu_Wrapper,
                            NULL
                    },
                    .function_input =
                    {
                            &exit_input,
                            &main_menu,
                            NULL
                    }
            }
    };

    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(menu_functions); ++ i)
    {
        if (menu_functions [i].menu == current_menu)
        {
            // Passende Funktion mit den hinterlegten Parameter aufrufen
            menu_functions [i].function [selected_menu_entry] (menu_functions [i].function_input [selected_menu_entry]);
            break;
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

#ifdef ALLOCATED_MENU_ENTRIES
#undef ALLOCATED_MENU_ENTRIES
#endif /* ALLOCATED_MENU_ENTRIES */
