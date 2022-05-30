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
#include "About_TUI.h"



#ifndef ALLOCATED_MENU_ENTRIES
#define ALLOCATED_MENU_ENTRIES 16
#else
#error "The marco \"ALLOCATED_MENU_ENTRIES\" is already defined !"
#endif /* ALLOCATED_MENU_ENTRIES */

#ifndef INFORMATION_LINE_OFFSET
#define INFORMATION_LINE_OFFSET 15
#else
#error "The macro \"INFORMATION_LINE_OFFSET\" is already defined !"
#endif /* INFORMATION_LINE_OFFSET */

#ifndef MENU_POSITION_OFFSET
#define MENU_POSITION_OFFSET 7
#else
#error "The macro \"MENU_POSITION_OFFSET\" is already defined !"
#endif /* MENU_POSITION_OFFSET */

#ifndef STATUS_OFFSET
#define STATUS_OFFSET 5
#else
#error "The macro \"STATUS_OFFSET\" is already defined !"
#endif /* STATUS_OFFSET */

#ifndef MENU_NUM_WINDOW_LINES
#define MENU_NUM_WINDOW_LINES 15
#else
#error "The macro \"MENU_NUM_WINDOW_LINES\" is already defined !"
#endif /* MENU_NUM_WINDOW_LINES */

#ifndef MENU_NUM_WINDOW_COLS
#define MENU_NUM_WINDOW_COLS 30
#else
#error "The macro \"MENU_NUM_WINDOW_COLS\" is already defined !"
#endif /* MENU_NUM_WINDOW_COLS */



enum Menu_Types
{
    MAIN_MENU = 0,
    CREATION_MENU,
    GENERAL_INFO_MENU,
    ABOUT_MENU
};



ITEM** items            = NULL;
MENU* menu              = NULL;
WINDOW* menu_window     = NULL;
WINDOW* info_window     = NULL;
WINDOW* pos_window      = NULL;
WINDOW* status_window   = NULL;
WINDOW* left_window     = NULL;
WINDOW* right_window    = NULL;

enum Menu_Types current_menue = MAIN_MENU;



static void Draw_Main_Window
(
        const char* menue_position_string
);

static void Refresh_All_Boxes (void);

static void
Update_Menu
(
        const enum Menu_Types chosen_menu
);

static void
Update_Window_Information
(
        const enum Menu_Types current_menu,
        const int selected_menu_entry
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

    // Fenster mit den allgemeinen Informationen zum ausgewaehlten Menuepunkt
    info_window     = newwin(INFORMATION_LINE_OFFSET - MENU_POSITION_OFFSET, COLS / 2 + 1, LINES - INFORMATION_LINE_OFFSET, 0);
    // Fenster mit der aktuellen Menueposition
    pos_window      = newwin(MENU_POSITION_OFFSET - STATUS_OFFSET + 1, COLS, LINES - MENU_POSITION_OFFSET - 1, 0);
    // Fenster mit aktuellen Statusinformationen
    status_window   = newwin(STATUS_OFFSET + 1, COLS, LINES - STATUS_OFFSET - 1, 0);
    // Linkes und rechtes Fenster fuer die "Nutzdaten" erzeugen
    left_window     = newwin(LINES - INFORMATION_LINE_OFFSET - MENU_NUM_WINDOW_LINES - 2,
            COLS / 2 - 1, 1 + MENU_NUM_WINDOW_LINES + 1, 1);
    right_window    = newwin(LINES - MENU_POSITION_OFFSET - 3, COLS / 2 - 2, 2, COLS / 2 + 1);
    // box(left_window, 0, 0);
    // box(right_window, 0, 0);


    Draw_Main_Window("Main menu");

    items = (ITEM**) CALLOC(ALLOCATED_MENU_ENTRIES, sizeof(ITEM*));
    ASSERT_ALLOC(items, "Cannot allocate memory for the TUI main menu !", ALLOCATED_MENU_ENTRIES * sizeof (ITEM*));
    SET_POINTER_ARRAY_TO_NULL(items, ALLOCATED_MENU_ENTRIES);

    menu = new_menu (items);

    menu_window = newwin(MENU_NUM_WINDOW_LINES, MENU_NUM_WINDOW_COLS, 2, 2);
    set_menu_win(menu, menu_window);
    set_menu_sub (menu, derwin(menu_window, MENU_NUM_WINDOW_LINES / 2, MENU_NUM_WINDOW_COLS - 2, 3, 2));
    box(menu_window, 0, 0);
    wbkgd(menu_window, COLOR_PAIR(1));

    set_menu_format(menu, ALLOCATED_MENU_ENTRIES - 1, 1);
    set_menu_mark(menu, "> ");
    set_menu_fore(menu, COLOR_PAIR(1)|A_REVERSE);
    set_menu_back(menu, COLOR_PAIR(1));
    post_menu(menu);

    // Hauptmenue erzeugen
    Update_Menu(current_menue);
    Update_Window_Information(current_menue, 0);

    refresh();
    Refresh_All_Boxes();

    while (1)
    {
        switch (getch())
        {
        case KEY_DOWN:
            menu_driver(menu, REQ_DOWN_ITEM);
            Update_Window_Information(current_menue, item_index(current_item(menu)));
            break;
        case KEY_UP:
            menu_driver(menu, REQ_UP_ITEM);
            Update_Window_Information(current_menue, item_index(current_item(menu)));
            break;
        case '\n':
            Exec_Menu_Entry(current_menue, item_index(current_item(menu)));
            Update_Window_Information(current_menue, 0);
            break;
        default:
            break;
        }
        refresh();
        Refresh_All_Boxes();
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
    wclear(info_window);
    wclear(status_window);
    wclear(pos_window);
    wclear(left_window);
    wclear(right_window);
    // Muss auskommentiert werden, damit der Name des aktuellen Menus nicht ausgeblendet wird
    // wclear(menu_window);

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
    wattrset(info_window, A_BOLD);
    mvwaddnstr(info_window, 1, 1, "Information:", strlen ("Information:"));
    wattrset(info_window, A_NORMAL);
    mvwaddnstr(info_window, 2, 1, "N/A", strlen ("N/A"));
    box(info_window, 0, 0);

    // Zeile mit dem Inhalt der Menueposition komplett leeren
    wmove(pos_window, 1, 1);
    for (int i = 0; i < COLS - 2; ++ i)
    {
        waddch(pos_window, ' ');
    }

    wattrset(pos_window, A_BOLD);
    mvwaddnstr(pos_window, 1, 1, "Menu position:", strlen("Menu position:"));
    wattrset(pos_window, A_NORMAL);
    mvwaddnstr(pos_window, 1, strlen("Menu position:") + 2, menue_position_string, (int) strlen (menue_position_string));
    box(pos_window, 0, 0);

    wattrset(status_window, A_BOLD);
    mvwaddnstr(status_window, 1, 1, "Status:", strlen("Status:"));
    wattrset(status_window, A_NORMAL);
    mvwaddnstr(status_window, 2, 1, "N/A", strlen ("N/A"));
    box(status_window, 0, 0);

    // "Merge" Zeichen an den Ecken der Fenster einbauen
    int status_window_lines = 0;
    int status_window_cols = 0;
    (void) status_window_lines;
    getmaxyx(status_window, status_window_lines, status_window_cols);
    wmove(status_window, 0, 0);
    waddch(status_window, ACS_LTEE);
    wmove(status_window, 0, status_window_cols - 1);
    waddch(status_window, ACS_RTEE);

    int info_window_lines = 0;
    int info_window_cols = 0;
    (void) info_window_lines;
    getmaxyx(info_window, info_window_lines, info_window_cols);
    wmove(info_window, 0, 0);
    waddch(info_window, ACS_LTEE);
    wmove(info_window, 0, info_window_cols - 1);
    waddch(info_window, ACS_RTEE);

    int pos_window_lines = 0;
    int pos_window_cols = 0;
    (void) pos_window_lines;
    getmaxyx(pos_window, pos_window_lines, pos_window_cols);
    wmove(pos_window, 0, 0);
    waddch(pos_window, ACS_LTEE);
    wmove(pos_window, 0, pos_window_cols - 1);
    waddch(pos_window, ACS_RTEE);
    wmove(pos_window, 0, pos_window_cols / 2);
    waddch(pos_window, ACS_BTEE);

    refresh();

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Refresh_All_Boxes (void)
{
    wrefresh(menu_window);
    wrefresh(info_window);
    wrefresh(pos_window);
    wrefresh(status_window);
    wrefresh(left_window);
    wrefresh(right_window);

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
        const char* const menu_name;
        const char* const menu_position;
        const char* const item_first_string [ALLOCATED_MENU_ENTRIES];
        const char* const item_second_string [ALLOCATED_MENU_ENTRIES];
    };

    // ===== ===== ===== BEGINN "Datenbank" mit allen Infos, die fuer die Menuerstellung benoetigt werden ===== ===== =====
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
                            NULL
                    },
                    .item_second_string =
                    {
                            "Back",
                            NULL
                    }
            }
    };
    // ===== ===== ===== ENDE "Datenbank" mit allen Infos, die fuer die Menuerstellung benoetigt werden ===== ===== =====

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
    (void) menu_window_lines;
    getmaxyx(menu_window, menu_window_lines, menu_window_cols);

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

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void
Update_Window_Information
(
        const enum Menu_Types current_menu,
        const int selected_menu_entry
)
{
    struct Window_Information
    {
        const enum Menu_Types menu;
        const char* const description [ALLOCATED_MENU_ENTRIES];
    };
    // ===== ===== ===== BEGINN Daten fuer die Information-Box ===== ===== =====
    const struct Window_Information window_information [] =
    {
            {
                    .menu           = MAIN_MENU,
                    .description    =
                    {
                            "Start calculations",
                            "General information about this program and the whole project",
                            "About",
                            "Exit program",
                            NULL
                    }
            },
            {
                    .menu           = CREATION_MENU,
                    .description    =
                    {
                            "Setup calculations for further operations",
                            "Back to upper menu",
                            NULL
                    }
            },
            {
                    .menu           = GENERAL_INFO_MENU,
                    .description    =
                    {
                            "...",
                            "Back to upper menu",
                            NULL
                    }
            },
            {
                    .menu           = ABOUT_MENU,
                    .description    =
                    {
                            "Back to upper menu",
                            NULL
                    }
            }
    };
    // ===== ===== ===== ENDE Daten fuer die Information-Box ===== ===== =====

    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(window_information); ++ i)
    {
        if (window_information [i].menu == current_menu)
        {
            wclear(info_window);

            wattrset(info_window, A_BOLD);
            mvwaddnstr(info_window, 1, 1, "Information:", strlen ("Information:"));
            wattrset(info_window, A_NORMAL);
            box(info_window, 0, 0);

            int info_window_lines = 0;
            int info_window_cols = 0;
            (void) info_window_lines;
            getmaxyx(info_window, info_window_lines, info_window_cols);
            wmove(info_window, 0, 0);
            waddch(info_window, ACS_LTEE);
            wmove(info_window, 0, info_window_cols - 1);
            waddch(info_window, ACS_RTEE);

            int pos_window_lines = 0;
            int pos_window_cols = 0;
            (void) pos_window_lines;
            getmaxyx(pos_window, pos_window_lines, pos_window_cols);
            wmove(pos_window, 0, 0);
            waddch(pos_window, ACS_LTEE);
            wmove(pos_window, 0, pos_window_cols - 1);
            waddch(pos_window, ACS_RTEE);
            wmove(pos_window, 0, pos_window_cols / 2);
            waddch(pos_window, ACS_BTEE);

            wmove(info_window, 2, 1);
            mvwaddnstr(info_window, 2, 1, window_information [i].description [selected_menu_entry],
                    (int) strlen (window_information [i].description [selected_menu_entry]));

            wrefresh(info_window);

            break;
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void Exit_Wrapper (const void* const input);
static void Exit_Wrapper (const void* const input)
{
    exit(*(const int* const) input);
    return;
}
static void Update_Menu_Wrapper (const void* const input);
static void Update_Menu_Wrapper (const void* const input)
{
    Update_Menu(*(const enum Menu_Types* const) input);
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
        void (*function [ALLOCATED_MENU_ENTRIES]) (const void* const);
        void (*function_2 [ALLOCATED_MENU_ENTRIES]) (const void* const);
        const void* const function_input [ALLOCATED_MENU_ENTRIES];
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
                    .function_2     =
                    {
                            NULL,
                            NULL,
                            &TUI_Build_About_Left_Side,
                            NULL,
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
                    .function_2     =
                    {
                            NULL,
                            NULL,
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
                    .function_2     =
                    {
                            NULL,
                            NULL,
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
                            &Update_Menu_Wrapper,
                            NULL
                    },
                    .function_2     =
                    {
                            NULL,
                            NULL
                    },
                    .function_input =
                    {
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
            if (menu_functions [i].function_2 [selected_menu_entry] != NULL)
            {
                menu_functions [i].function_2 [selected_menu_entry] (NULL);
            }
            break;
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

#ifdef ALLOCATED_MENU_ENTRIES
#undef ALLOCATED_MENU_ENTRIES
#endif /* ALLOCATED_MENU_ENTRIES */

#ifdef INFORMATION_LINE_OFFSET
#undef INFORMATION_LINE_OFFSET
#endif /* INFORMATION_LINE_OFFSET */

#ifdef MENU_POSITION_OFFSET
#undef MENU_POSITION_OFFSET
#endif /* MENU_POSITION_OFFSET */

#ifdef STATUS_OFFSET
#undef STATUS_OFFSET
#endif /* STATUS_OFFSET */

#ifdef MENU_NUM_WINDOW_LINES
#undef MENU_NUM_WINDOW_LINES
#endif /* MENU_NUM_WINDOW_LINES */

#ifdef MENU_NUM_WINDOW_COLS
#undef MENU_NUM_WINDOW_COLS
#endif /* MENU_NUM_WINDOW_COLS */
