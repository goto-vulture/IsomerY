/**
 * @file XPM_Fragments.h
 *
 * @brief Hier sind alle Fragmente - ausser der Farbenliste - die fuer die Erzeugung einer XPM-Datei noch benoetigt
 * werden. Einige Fragmente werden durch Funktionen gebildet.
 *
 * Fuer genauere Infos bzgl. des Formates: https://de.wikipedia.org/wiki/X_PixMap#Formatbeschreibung
 *
 * @date 14.05.2022
 * @author x86 / Gyps
 */

#include "XPM_Fragments.h"
#include <stdio.h>
#include "../../Error_Handling/Assert_Msg.h"
#include "XPM_Color_List.h"
#include "../../Misc.h"



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Header einer XPM-Datei in den uebergebenen Speicher schreiben.
 *
 * Der Grundaufbau des Headers:
 * <Beginn_Blockkommentar> XPM <Ende_Blockkommentar>
 * static char * Dateiname[] = {
 * "<Breite des Bildes> <Hoehe des Bildes> <Anzahl an Farben im Bild> <Anzahl an Zeichen pro Pixel>",
 *
 * Ein Beispiel, um den Aufbau zu verdeutlichen:
 */ /* XPM */
 /** static char * H_32_32_10_1_xpm[] = {
 * " 32 32 10 1",
 * => Die Bilddatei "H_32_32_10_1_xpm" enthaelt eine Breite von 32 Pixel sowie auch eine Hoehe von 32 Pixeln. Die Anzahl
 * an Farben im Bild betraegt 10; Fuer ein Pixel im Bild wird genau ein Zeichen verwendet.
 *
 * Fuer genauere Infos bzgl. des Formates: https://de.wikipedia.org/wiki/X_PixMap#Formatbeschreibung
 *
 * Asserts:
 *      picture_name != NULL
 *      picture_name [0] != '\0'
 *      picture_width != 0
 *      picture_height != 0
 *      picture_colors != 0
 *      char_per_pixel != 0
 *      output_memory != NULL
 *      output_memory_size != 0
 *
 * @param[in] picture_name Name der Bilddatei, die im Header eingetragen wird
 * @param[in] picture_name Laenge des Dateinamens
 * @param[in] picture_width Laenge des Bildes
 * @param[in] picture_height Hoehe des Bildes
 * @param[in] picture_colors Anzahl an Farben im Bild
 * @param[in] char_per_pixel Anzahl an Zeichen, die ein Pixel im Bild codieren
 * @param[out] output_memory Zielspeicher fuer den erzeugten Header
 * @param[int] output_memory_size Groesse des Zielspeichers
 */
extern void
Create_XPM_Header
(
        const char* const restrict picture_name,
        const uint_fast32_t picture_width,
        const uint_fast32_t picture_height,
        const uint_fast8_t picture_colors,
        const uint_fast8_t char_per_pixel,
        char* const restrict output_memory,
        const size_t output_memory_size
)
{
    ASSERT_MSG(picture_name != NULL,        "picture_name is NULL !");
    ASSERT_MSG(picture_name [0] != '\0',    "picture_name has the length 0 !");
    ASSERT_MSG(picture_width != 0,          "picture_width is 0 !");
    ASSERT_MSG(picture_height != 0,         "picture_height is 0 !");
    ASSERT_MSG(picture_colors != 0,         "picture_colors is 0 !");
    ASSERT_MSG(char_per_pixel != 0,         "char_per_pixel is 0 !");
    ASSERT_MSG(output_memory != NULL,       "output_memory is NULL !");
    ASSERT_MSG(output_memory_size != 0,     "output_memory_size is 0 !");

    snprintf(output_memory, output_memory_size - 1,
            "/* XPM */\n"
            "static char * %s[] = {\n"
            "%" PRIuFAST32 " %" PRIuFAST32 " %" PRIuFAST8 " %" PRIuFAST8 "\n",
            picture_name, picture_width, picture_height, picture_colors, char_per_pixel);
    output_memory[output_memory_size - 1] = '\0';

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Farbenliste einer XPM-Datei erstellen.
 *
 * Eine Farbenliste ist wiefolgt aufgebaut:
 * "<Zeichen, welches im Bild fuer die folgende Farbe verwendet wird> <Darstellungsart der Farbe> <#Farbcode>",
 *
 * Ein Beispiel:
 * "G   g #000000",
 * => Das Zeichen 'G' kodiert ein schwarzes Pixel im Schwarz/Weiss Modus
 *
 * Fuer genauere Infos bzgl. des Formates: https://de.wikipedia.org/wiki/X_PixMap#Formatbeschreibung
 *
 * Asserts:
 *      output_memory != NULL
 *      output_memory_size != 0
 *
 * @param[out] output_memory Zielspeicher fuer den erzeugten Header
 * @param[int] output_memory_size Groesse des Zielspeichers
 */
extern void
Create_XPM_Color_List
(
        char* output_memory,
        size_t output_memory_size
)
{
    ASSERT_MSG(output_memory != NULL,       "output_memory is NULL !");
    ASSERT_MSG(output_memory_size != 0,     "output_memory_size is 0 !");

    // Die komplette Farbliste aus der Headerdatei "XPM_Color_List." in den Ausgabespeicher schreiben
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(XPM_COLOR_LIST); ++ i)
    {
        const int necessary_char = snprintf(output_memory, output_memory_size - 1, "%s\n", XPM_COLOR_LIST [i]);
        output_memory[output_memory_size - 1] = '\0';

        // Wenn nicht genug Speicher fuer den kompletten String ist, wird an dieser Stelle die Verarbeitung abgebrochen
        if ((uint_fast32_t) necessary_char > (output_memory_size - 1))
        {
            break;
        }
        output_memory += necessary_char;
        output_memory_size -= (size_t) necessary_char;
    }

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Footer einer XPM-Datei erstellen.
 *
 * Der Footer ist ganz simpel aufgebaut: "};"
 *
 * Es wird nur der Zielspeicher sowie die Laenge des Zielspeichers benoetigt.
 *
 * Fuer genauere Infos bzgl. des Formates: https://de.wikipedia.org/wiki/X_PixMap#Formatbeschreibung
 *
 * Asserts:
 *      output_memory != NULL
 *      output_memory_size != 0
 *
 * @param[out] output_memory Zielspeicher fuer den erzeugten Header
 * @param[int] output_memory_size Groesse des Zielspeichers
 */
extern void
Create_XPM_Footer
(
        char* const output_memory,
        const size_t output_memory_size
)
{
    ASSERT_MSG(output_memory != NULL,       "output_memory is NULL !");
    ASSERT_MSG(output_memory_size != 0,     "output_memory_size is 0 !");

    snprintf(output_memory, output_memory_size - 1, "};\n");
    output_memory[output_memory_size - 1] = '\0';

    return;
}

//---------------------------------------------------------------------------------------------------------------------
