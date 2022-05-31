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
#include "General_Info_TUI.h"
#include "Calculations_TUI.h"



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



/**
 * @brief Konstanten, fuer die Menues und den Untermenues
 */
enum Menu_Types
{
    MAIN_MENU = 0,
    CREATION_MENU,

    GENERAL_INFO_MENU,
    GENERAL_INFO_DEEPER_INITIAL_SITUATION_MENU,
    GENERAL_INFO_DEEPER_PROBLEM_MENU,
    GENERAL_INFO_DEEPER_SOLUTION_IDEAS_MENU,

    CALCULATIONS_SETUP_CALCULATIONS_MENU,
    ABOUT_MENU
};



ITEM** items            = NULL;             // Die einzelnen Menueeintrage
MENU* menu              = NULL;             // Das Menue an sich
WINDOW* menu_window     = NULL;             // Fenster fuer das Menue
WINDOW* info_window     = NULL;             // Fenster fuer die Info-Box
WINDOW* pos_window      = NULL;             // Fenster fuer die Angabe der aktuellen Menueposition
WINDOW* status_window   = NULL;             // Fenster fuer die Anzege des aktuellen Berechnuns-Status
WINDOW* left_window     = NULL;             // Linkes Fenster fuer unterschiedlichste Ausgaben
WINDOW* right_window    = NULL;             // Rechtes Fenster fuer unterschiedlichste Ausgaben

enum Menu_Types current_menue = MAIN_MENU;  // Aktuelle Position im Auswahlmenue



/**
 * @brief Hauptfenster zeichnen.
 *
 * @param[in] menue_position_string String, der die aktuelle Position im Menue angibt
 */
static void Draw_Main_Window
(
        const char* menue_position_string
);

/**
 * @brief Alle Boxen / ncurses-Windows auffrischen (moegliche Aenderungen auf dem Bildschirm anzeigen).
 */
static int Refresh_All_Boxes (void);

/**
 * @brief Menue-Struktur aktuallisieren und auf dem Bildschirm anzeigen.
 *
 * @param[in] chosen_menu Menue, deren hinterlegte Struktur gezeichnet werden soll
 */
static void
Update_Menu
(
        const enum Menu_Types chosen_menu
);

/**
 * @brief Fensterinformationen aktuallisieren.
 *
 * Die Funktion aktuallisiert den Informationsstring des aktuell ausgewaehlten bzw. markierten Menuepunktes.
 *
 * @param[in] current_menu Aktuelles Menue
 * @param[in] selected_menu_entry Innerhalb des aktuellen Menues den ausgewaehlten Eintrag
 */
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
    if (menu != NULL)
    {
        unpost_menu(menu);
        free_menu(menu);
    }
    menu = NULL;

    if (items != NULL)
    {
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
    }

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

    // ===== ===== ===== BEGINN Grundlegende Einstellungen der TUI durchfuehren ===== ===== =====
    NULL_CHECK(initscr());              // Anzeige initialisieren
    ERR_CHECK(curs_set(0));             // Cursor auf den Ursprung setzen
    ERR_CHECK(clear ());                // Sicherheitshalber den kompletten Bildschirm leeren
    ERR_CHECK(noecho());                // Tastatureingaben werden auf den Bildschirm nicht angezeigt

    // Diese Funktionen haben ebenfalls keinen Error-Code als Rueckgabewert
    ERR_CHECK(start_color());           // Das Anzeigen von Farben aktivieren
    // Zwei Farbpaare erzeugen (Vordergrundfarbe, Hintergrundfarbe)
    ERR_CHECK(init_pair(1, COLOR_WHITE, COLOR_BLACK));
    ERR_CHECK(init_pair(2, COLOR_BLACK, COLOR_WHITE));
    ERR_CHECK(bkgd(COLOR_PAIR(1)));     // Ein Farbpaar fuer das Standardfenster waehlen

    ERR_CHECK(cbreak())                 // Eingabepufferung deaktivieren (Tastatureingaben sollen sofort verarbeitet
                                        // werden)
    ERR_CHECK(keypad(stdscr, TRUE));    // Die Eingabe mittels Tastatur aktivieren
    // ===== ===== ===== ENDE Grundlegende Einstellungen der TUI durchfuehren ===== ===== =====

    // ===== ===== ===== BEGINN Alle Fenster der TUI erzeugen ===== ===== =====
    // Fenster mit den allgemeinen Informationen zum ausgewaehlten Menuepunkt
    info_window     = newwin(INFORMATION_LINE_OFFSET - MENU_POSITION_OFFSET, COLS / 2 + 1, LINES - INFORMATION_LINE_OFFSET, 0);
    NULL_CHECK(info_window);
    // Fenster mit der aktuellen Menueposition
    pos_window      = newwin(MENU_POSITION_OFFSET - STATUS_OFFSET + 1, COLS, LINES - MENU_POSITION_OFFSET - 1, 0);
    NULL_CHECK(pos_window);
    // Fenster mit aktuellen Statusinformationen
    status_window   = newwin(STATUS_OFFSET + 1, COLS, LINES - STATUS_OFFSET - 1, 0);
    NULL_CHECK(status_window);
    // Linkes und rechtes Fenster fuer die "Nutzdaten" erzeugen
    left_window     = newwin(LINES - INFORMATION_LINE_OFFSET - MENU_NUM_WINDOW_LINES - 2,
            COLS / 2 - 1, 1 + MENU_NUM_WINDOW_LINES + 1, 1);
    NULL_CHECK(left_window);
    right_window    = newwin(LINES - MENU_POSITION_OFFSET - 3, COLS / 2 - 2, 2, COLS / 2 + 1);
    NULL_CHECK(right_window);
    // ===== ===== ===== ENDE Alle Fenster der TUI erzeugen ===== ===== =====

    Draw_Main_Window("Main menu");

    // ===== ===== ===== BEGINN Eingabemenue erzeugen, initialisieren und anzeigen ===== ===== =====
    items = (ITEM**) CALLOC(ALLOCATED_MENU_ENTRIES, sizeof(ITEM*));
    ASSERT_ALLOC(items, "Cannot allocate memory for the TUI main menu !", ALLOCATED_MENU_ENTRIES * sizeof (ITEM*));
    SET_POINTER_ARRAY_TO_NULL(items, ALLOCATED_MENU_ENTRIES);

    menu = new_menu (items);
    NULL_CHECK(menu);

    menu_window = newwin(MENU_NUM_WINDOW_LINES, MENU_NUM_WINDOW_COLS, 2, 2);
    NULL_CHECK(menu_window);
    ERR_CHECK(set_menu_win(menu, menu_window));
    ERR_CHECK(set_menu_sub (menu, derwin(menu_window, MENU_NUM_WINDOW_LINES / 2, MENU_NUM_WINDOW_COLS - 2, 3, 2)));
    ERR_CHECK(box(menu_window, 0, 0));
    ERR_CHECK(wbkgd(menu_window, COLOR_PAIR(1)));

    ERR_CHECK(set_menu_format(menu, ALLOCATED_MENU_ENTRIES - 1, 1));
    ERR_CHECK(set_menu_mark(menu, "> "));
    ERR_CHECK(set_menu_fore(menu, COLOR_PAIR(1) | A_REVERSE));
    ERR_CHECK(set_menu_back(menu, COLOR_PAIR(1)));
    ERR_CHECK(post_menu(menu));
    // ===== ===== ===== ENDE Eingabemenue erzeugen, initialisieren und anzeigen ===== ===== =====

    // Hauptmenue erzeugen
    Update_Menu(current_menue);
    Update_Window_Information(current_menue, 0);

    // ===== ===== ===== BEGINN "GUI"-Schleife ===== ===== =====
    while (1)
    {
        ERR_CHECK(refresh());
        Refresh_All_Boxes();

        // Direkte Reaktion auf der naechsten Tastatureingabe
        switch (getch())
        {
            // UP + DOWN fuehren zu der Aktuallisierung des Informations-Eintrages zu dem neu markierten Menuepunkt
        case KEY_DOWN:
            ERR_CHECK(menu_driver(menu, REQ_DOWN_ITEM));
            Update_Window_Information(current_menue, item_index(current_item(menu)));
            break;
        case KEY_UP:
            ERR_CHECK(menu_driver(menu, REQ_UP_ITEM));
            Update_Window_Information(current_menue, item_index(current_item(menu)));
            break;

            // Enter-Taste: Auswahl des aktuell markierten Menuepunktes und der dafuer hinterlegten Callback-Funktion
        case '\n':
            Exec_Menu_Entry(current_menue, item_index(current_item(menu)));
            Update_Window_Information(current_menue, 0);
            break;
        default:
            break;
        }
    }
    // ===== ===== ===== ENDE "GUI"-Schleife ===== ===== =====

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Hauptfenster zeichnen.
 *
 * @param[in] menue_position_string String, der die aktuelle Position im Menue angibt
 */
static void Draw_Main_Window
(
        const char* menue_position_string
)
{
    ERR_CHECK(clear());
    ERR_CHECK(wclear(info_window));
    ERR_CHECK(wclear(status_window));
    ERR_CHECK(wclear(pos_window));
    ERR_CHECK(wclear(left_window));
    ERR_CHECK(wclear(right_window));
    // Muss auskommentiert werden, damit der Name des aktuellen Menus nicht ausgeblendet wird
    // wclear(menu_window);

    // Komplette Umrandung des Hauptfensters
    ERR_CHECK(box(stdscr, 0, 0));

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
    ERR_CHECK(mvprintw(0, (COLS / 2) - (title_length / 2), "%s", window_title));

    // Vertikale Linie durch's Fenster
    for (int i = 2; i < LINES - 6; ++ i)
    {
        move(i, COLS / 2);
        addch(ACS_VLINE);
    }

    // Flaeche mit optionalen Informationen zum aktuell markierten Menuepunkt zeichnen
    ERR_CHECK(wattrset(info_window, A_BOLD));
    ERR_CHECK(mvwaddnstr(info_window, 1, 1, "Information:", strlen ("Information:")));
    ERR_CHECK(wattrset(info_window, A_NORMAL));
    ERR_CHECK(mvwaddnstr(info_window, 2, 1, "N/A", strlen ("N/A")));
    ERR_CHECK(box(info_window, 0, 0));

    // Zeile mit dem Inhalt der Menueposition komplett leeren
    ERR_CHECK(wmove(pos_window, 1, 1));
    for (int i = 0; i < COLS - 2; ++ i)
    {
        ERR_CHECK(waddch(pos_window, ' '));
    }

    ERR_CHECK(wattrset(pos_window, A_BOLD));
    ERR_CHECK(mvwaddnstr(pos_window, 1, 1, "Menu position:", strlen("Menu position:")));
    ERR_CHECK(wattrset(pos_window, A_NORMAL));
    ERR_CHECK(mvwaddnstr(pos_window, 1, strlen("Menu position:") + 2, menue_position_string,
            (int) strlen (menue_position_string)));
    ERR_CHECK(box(pos_window, 0, 0));

    ERR_CHECK(wattrset(status_window, A_BOLD));
    ERR_CHECK(mvwaddnstr(status_window, 1, 1, "Status:", strlen("Status:")));
    ERR_CHECK(wattrset(status_window, A_NORMAL));
    ERR_CHECK(mvwaddnstr(status_window, 2, 1, "N/A", strlen ("N/A")));
    ERR_CHECK(box(status_window, 0, 0));

    // "Merge" Zeichen an den Ecken der Fenster einbauen
    // Dies dient dazu, dass die Eckelemente der Boxen von den einzelnen Fenstern besser aussehen
    int status_window_lines = 0;
    int status_window_cols = 0;
    (void) status_window_lines;
    ERR_CHECK(getmaxyx(status_window, status_window_lines, status_window_cols));
    ERR_CHECK(wmove(status_window, 0, 0));
    ERR_CHECK(waddch(status_window, ACS_LTEE));
    ERR_CHECK(wmove(status_window, 0, status_window_cols - 1));
    ERR_CHECK(waddch(status_window, ACS_RTEE));

    int info_window_lines = 0;
    int info_window_cols = 0;
    (void) info_window_lines;
    ERR_CHECK(getmaxyx(info_window, info_window_lines, info_window_cols));
    ERR_CHECK(wmove(info_window, 0, 0));
    ERR_CHECK(waddch(info_window, ACS_LTEE));
    ERR_CHECK(wmove(info_window, 0, info_window_cols - 1));
    ERR_CHECK(waddch(info_window, ACS_RTEE));

    int pos_window_lines = 0;
    int pos_window_cols = 0;
    (void) pos_window_lines;
    ERR_CHECK(getmaxyx(pos_window, pos_window_lines, pos_window_cols));
    ERR_CHECK(wmove(pos_window, 0, 0));
    ERR_CHECK(waddch(pos_window, ACS_LTEE));
    ERR_CHECK(wmove(pos_window, 0, pos_window_cols - 1));
    ERR_CHECK(waddch(pos_window, ACS_RTEE));
    ERR_CHECK(wmove(pos_window, 0, pos_window_cols / 2));
    ERR_CHECK(waddch(pos_window, ACS_BTEE));

    ERR_CHECK(refresh());

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Alle Boxen / ncurses-Windows auffrischen (moegliche Aenderungen auf dem Bildschirm anzeigen).
 */
static int Refresh_All_Boxes (void)
{
    int error_value = OK;
    error_value = wrefresh(menu_window);
    if (error_value == ERR) { return error_value; }
    error_value = wrefresh(info_window);
    if (error_value == ERR) { return error_value; }
    error_value = wrefresh(pos_window);
    if (error_value == ERR) { return error_value; }
    error_value = wrefresh(status_window);
    if (error_value == ERR) { return error_value; }
    error_value = wrefresh(left_window);
    if (error_value == ERR) { return error_value; }
    error_value = wrefresh(right_window);
    if (error_value == ERR) { return error_value; }

    return error_value;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Menue-Struktur aktuallisieren und auf dem Bildschirm anzeigen.
 *
 * @param[in] chosen_menu Menue, deren hinterlegte Struktur gezeichnet werden soll
 */
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
                            "General infos",
                            "About",
                            "Exit",
                            NULL
                    }
            },
            {
                    .menu               = CREATION_MENU,
                    .menu_name          = "CREATION MENU",
                    .menu_position      = "Main menu > Calculations",
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
                    .menu               = CALCULATIONS_SETUP_CALCULATIONS_MENU,
                    .menu_name          = "SETUP CALCULATIONS",
                    .menu_position      = "Main menu > Creation menu > Setup Calculations",
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
            },
            {
                    .menu               = GENERAL_INFO_MENU,
                    .menu_name          = "GENERAL INFOS",
                    .menu_position      = "Main menu > General infos",
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
                            "Initial situation",
                            "The problem",
                            "Solution ideas",
                            "Back",
                            NULL
                    }
            },
            {
                    .menu               = GENERAL_INFO_DEEPER_INITIAL_SITUATION_MENU,
                    .menu_name          = "INITIAL SITUATION",
                    .menu_position      = "Main menu > General infos > Initial situation",
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
            },
            {
                    .menu               = GENERAL_INFO_DEEPER_PROBLEM_MENU,
                    .menu_name          = "THE PROBLEM",
                    .menu_position      = "Main menu > General infos > The problem",
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
            },
            {
                    .menu               = GENERAL_INFO_DEEPER_SOLUTION_IDEAS_MENU,
                    .menu_name          = "SOLUTION IDEAS",
                    .menu_position      = "Main menu > General infos > Solution ideas",
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
            ERR_CHECK(free_item(items [i]));
            items [i] = NULL;
        }
    }

    int menu_window_lines = 0;
    int menu_window_cols = 0;
    (void) menu_window_lines;
    ERR_CHECK(getmaxyx(menu_window, menu_window_lines, menu_window_cols));

    // Menuenamenzeile komplett leeren
    for (int i = 0; i < menu_window_cols - 4; ++ i)
    {
        ERR_CHECK(mvwaddch(menu_window, 1, 2 + i, ' '));
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
                NULL_CHECK(items [counter]);
                ++ counter;
            }
            items [counter] = NULL;

            ERR_CHECK(mvwaddnstr(menu_window, 1, 2, menu_content [i].menu_name, menu_window_cols - 4));
            menu_position_string = menu_content [i].menu_position;

            break;
        }
    }

    // Anzeigen aktuallisieren (Hauptfenster, Menue-Fenster, Umrandung des Menue-Fensters)
    Draw_Main_Window(menu_position_string);
    ERR_CHECK(unpost_menu(menu));
    ERR_CHECK(set_menu_items(menu, items));

    ERR_CHECK(post_menu(menu));
    ERR_CHECK(box(menu_window, 0, 0));

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Fensterinformationen aktuallisieren.
 *
 * Die Funktion aktuallisiert den Informationsstring des aktuell ausgewaehlten bzw. markierten Menuepunktes.
 *
 * @param[in] current_menu Aktuelles Menue
 * @param[in] selected_menu_entry Innerhalb des aktuellen Menues den ausgewaehlten Eintrag
 */
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
                    .menu           = CALCULATIONS_SETUP_CALCULATIONS_MENU,
                    .description    =
                    {
                            "Back to upper menu",
                            NULL
                    }
            },
            {
                    .menu           = GENERAL_INFO_MENU,
                    .description    =
                    {
                            "Infos about the initial situation",
                            "Description of the problem",
                            "The solution ideas",
                            "Back to upper menu",
                            NULL
                    }
            },
            {
                    .menu           = GENERAL_INFO_DEEPER_INITIAL_SITUATION_MENU,
                    .description    =
                    {
                            "Back to upper menu",
                            NULL
                    }
            },
            {
                    .menu           = GENERAL_INFO_DEEPER_PROBLEM_MENU,
                    .description    =
                    {
                            "Back to upper menu",
                            NULL
                    }
            },
            {
                    .menu           = GENERAL_INFO_DEEPER_SOLUTION_IDEAS_MENU,
                    .description    =
                    {
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

    // Passenden Eintrag in der "Datenbank" fuer die Information-Box suchen
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(window_information); ++ i)
    {
        if (window_information [i].menu == current_menu)
        {
            ERR_CHECK(wclear(info_window));

            ERR_CHECK(wattrset(info_window, A_BOLD));
            ERR_CHECK(mvwaddnstr(info_window, 1, 1, "Information:", strlen ("Information:")));
            ERR_CHECK(wattrset(info_window, A_NORMAL));
            ERR_CHECK(box(info_window, 0, 0));

            // Nach dem Loeschen der Box muessen einige Eckelemente angepasst werden, damit die Zeichnung auf der TUI
            // besser aussieht
            int info_window_lines = 0;
            int info_window_cols = 0;
            (void) info_window_lines;
            ERR_CHECK(getmaxyx(info_window, info_window_lines, info_window_cols));
            ERR_CHECK(wmove(info_window, 0, 0));
            ERR_CHECK(waddch(info_window, ACS_LTEE));
            ERR_CHECK(wmove(info_window, 0, info_window_cols - 1));
            ERR_CHECK(waddch(info_window, ACS_RTEE));

            int pos_window_lines = 0;
            int pos_window_cols = 0;
            (void) pos_window_lines;
            ERR_CHECK(getmaxyx(pos_window, pos_window_lines, pos_window_cols));
            ERR_CHECK(wmove(pos_window, 0, 0));
            ERR_CHECK(waddch(pos_window, ACS_LTEE));
            ERR_CHECK(wmove(pos_window, 0, pos_window_cols - 1));
            ERR_CHECK(waddch(pos_window, ACS_RTEE));
            ERR_CHECK(wmove(pos_window, 0, pos_window_cols / 2));
            ERR_CHECK(waddch(pos_window, ACS_BTEE));

            // Hinterlegten String fuer die Informationen zu dem aktuellen Menue-Eintrag anzeigen
            ERR_CHECK(wmove(info_window, 2, 1));
            ERR_CHECK(mvwaddnstr(info_window, 2, 1, window_information [i].description [selected_menu_entry],
                    (int) strlen (window_information [i].description [selected_menu_entry])));

            ERR_CHECK(wrefresh(info_window));

            break;
        }
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Wrapper-Funktion fuer die exit-Funktion.
 *
 * Diese Wrapper-Funktion ist notwendig, damit die Eingabeparameter void* Zeiger sind. Vor dem Aufruf von exit werden
 * die Eingabedaten passend gecastet.
 *
 * Damit innerhalb der Funktion "Exec_Menu_Entry" eine "Datenbank" mit den Callback-Funktionen angelegt werden kann,
 * muessen alle Callback-Funkionen die gleiche Signatur besitzen. Daher dieser Umweg.
 *
 * @param[in] input Eingabedateien fuer die exit-Funktion
 */
static void Exit_Wrapper (const void* const input);
static void Exit_Wrapper (const void* const input)
{
    exit(*(const int* const) input);
    return;
}

/**
 * @brief Wrapper-Funktion fuer die Update_Menu-Funktion.
 *
 * Diese Wrapper-Funktion ist notwendig, damit die Eingabeparameter void* Zeiger sind. Vor dem Aufruf von Update_Menu
 * werden die Eingabedaten passend gecastet.
 *
 * Damit innerhalb der Funktion "Exec_Menu_Entry" eine "Datenbank" mit den Callback-Funktionen angelegt werden kann,
 * muessen alle Callback-Funkionen die gleiche Signatur besitzen. Daher dieser Umweg.
 *
 * @param[in] input Eingabedateien fuer die Update_Menu-Funktion
 */
static void Update_Menu_Wrapper (const void* const input);
static void Update_Menu_Wrapper (const void* const input)
{
    Update_Menu(*(const enum Menu_Types* const) input);
    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Funktion, die die Callback-Funkionen fuer alle Menuepunkte enthaelt.
 *
 * Die Callback-Funktionen werden ausgefuehrt, wenn der Menueeintrag mittels Enter ausgewaelt wird.
 *
 * @param[in] current_menu Aktuelles Menue
 * @param[in] selected_menu_entry Innerhalb des aktuellen Menues den ausgewaehlten Eintrag
 */
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

    static const enum Menu_Types main_menu                                  = MAIN_MENU;
    static const enum Menu_Types about_menu                                 = ABOUT_MENU;
    static const enum Menu_Types general_info_menu                          = GENERAL_INFO_MENU;
    static const enum Menu_Types general_info_deeper_initial_situation_menu = GENERAL_INFO_DEEPER_INITIAL_SITUATION_MENU;
    static const enum Menu_Types general_info_deeper_problem_menu           = GENERAL_INFO_DEEPER_PROBLEM_MENU;
    static const enum Menu_Types general_info_deeper_solution_ideas_menu    = GENERAL_INFO_DEEPER_SOLUTION_IDEAS_MENU;
    static const enum Menu_Types creation_menu                              = CREATION_MENU;
    static const enum Menu_Types calculations_setup_calculations_menu       = CALCULATIONS_SETUP_CALCULATIONS_MENU;
    static const int exit_input                                             = 0;

    // ===== ===== ===== BEGINN "Datenbank" mit den Callback Funktionen und deren Parametern ===== ===== =====
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
                    .menu           = CALCULATIONS_SETUP_CALCULATIONS_MENU,
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
                            &creation_menu,
                            NULL
                    }
            },
            {
                    .menu           = GENERAL_INFO_MENU,
                    .function       =
                    {
                            &Update_Menu_Wrapper,
                            &Update_Menu_Wrapper,
                            &Update_Menu_Wrapper,
                            &Update_Menu_Wrapper,
                            NULL
                    },
                    .function_2     =
                    {
                            &TUI_Build_General_Info_Initial_Situation_Left_Side,
                            &TUI_Build_General_Info_Problem_Left_Side,
                            &TUI_Build_General_Info_Solution_Ideas_Left_Side,
                            NULL,
                            NULL
                    },
                    .function_input =
                    {
                            &general_info_deeper_initial_situation_menu,
                            &general_info_deeper_problem_menu,
                            &general_info_deeper_solution_ideas_menu,
                            &main_menu,
                            NULL
                    }
            },
            {
                    .menu           = GENERAL_INFO_DEEPER_INITIAL_SITUATION_MENU,
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
                            &general_info_menu,
                            NULL
                    }
            },
            {
                    .menu           = GENERAL_INFO_DEEPER_PROBLEM_MENU,
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
                            &general_info_menu,
                            NULL
                    }
            },
            {
                    .menu           = GENERAL_INFO_DEEPER_SOLUTION_IDEAS_MENU,
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
                            &general_info_menu,
                            NULL
                    }
            },
            {
                    .menu           = CREATION_MENU,
                    .function       =
                    {
                            &Update_Menu_Wrapper,
                            &Update_Menu_Wrapper,
                            NULL
                    },
                    .function_2     =
                    {
                            &TUI_Build_Calculations_Left_Side,
                            NULL,
                            NULL
                    },
                    .function_input =
                    {
                            &calculations_setup_calculations_menu,
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
    // ===== ===== ===== ENDE "Datenbank" mit den Callback Funktionen und deren Parametern ===== ===== =====

    // Passenden Eintrag in der "Datenbank" suchen
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(menu_functions); ++ i)
    {
        if (menu_functions [i].menu == current_menu)
        {
            // Passende Funktion mit den hinterlegten Parameter aufrufen
            menu_functions [i].function [selected_menu_entry] (menu_functions [i].function_input [selected_menu_entry]);
            // Die zweite Funktion ist optional -> NULL Check erforderlich !
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
