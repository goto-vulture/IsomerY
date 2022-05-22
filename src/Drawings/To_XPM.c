/**
 * @file To_XPM.c
 *
 * @date 16.04.2022
 * @author am1
 */

#include "To_XPM.h"
#include <stdlib.h>
#include <ctype.h>
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Print_Tools.h"
#include "../Misc.h"
#include "XPM/XPM_Fragments.h"
#include "../str2int.h"

// Alle XPM-Bilder, die fuer die Erzeugung zur Verfuegung stehen
#include "XPM/C_24_24_14_1.xpm"
#include "XPM/C_32_32_16_1.xpm"
#include "XPM/C_64_64_16_1.xpm"
#include "XPM/C_64_64_16_2.xpm"

#include "XPM/H_24_24_8_1.xpm"
#include "XPM/H_32_32_10_1.xpm"
#include "XPM/H_64_64_19_1.xpm"
#include "XPM/H_64_64_19_2.xpm"

#include "XPM/Hyphen_24_24_5_1.xpm"
#include "XPM/Hyphen_32_32_7_1.xpm"
#include "XPM/Hyphen_64_64_13_1.xpm"
#include "XPM/Hyphen_64_64_13_2.xpm"

#include "XPM/Hyphen_Angle_90_24_24_6_1.xpm"
#include "XPM/Hyphen_Angle_90_32_32_11_1.xpm"
#include "XPM/Hyphen_Angle_90_64_64_13_1.xpm"
#include "XPM/Hyphen_Angle_90_64_64_13_2.xpm"

#include "XPM/XPM_Color_List.h"



#ifndef EXPORT_FILE_BUFFER_SIZE
#define EXPORT_FILE_BUFFER_SIZE 100000
#else
#error "The marco \"EXPORT_FILE_BUFFER_SIZE\" is already defined !"
#endif /* EXPORT_FILE_BUFFER_SIZE */



struct char_char_array_tuple
{
    char c;
    const char** xpm_array_name;
};


static void
Draw_Char_In_XPM
(
        char** const output_data,
        uint_fast32_t* const restrict x_cursor,
        uint_fast32_t* const restrict y_cursor,
        const enum Export_To_XPM_Settings export_settings,
        const char input_char,
        struct char_char_array_tuple* tuple,
        const size_t number_of_tuple
);

//---------------------------------------------------------------------------------------------------------------------

extern void Export_Text_Based_Drawing_To_XPM
(
        const struct Text_Based_Alkane_Drawing* const restrict input,
        const enum Export_To_XPM_Settings export_settings
)
{
    uint_fast8_t char_size = 0;
    if ((export_settings & EXPORT_XPM_CHAR_SIZE_24_24) != 0)
    {
        char_size = 24;
    }
    else if ((export_settings & EXPORT_XPM_CHAR_SIZE_32_32) != 0)
    {
        char_size = 32;
    }
    else if ((export_settings & EXPORT_XPM_CHAR_SIZE_64_64) != 0)
    {
        char_size = 64;
    }


    // ===== ===== ===== BEGINN Zuordnung der Konfigurationen zu den Quellbildern ===== ===== =====
    struct File_Name_And_Settings_Enum
    {
            const char** xpm_array_name;
            const enum Export_To_XPM_Settings export_settings;
    };

    const struct File_Name_And_Settings_Enum available_files_for_configs [] =
    {
        { C_24_24_14_1_xpm, EXPORT_XPM_CHAR_SIZE_24_24 | EXPORT_XPM_CHAR_PER_PIXEL_1 },
        { C_32_32_16_1_xpm, EXPORT_XPM_CHAR_SIZE_32_32 | EXPORT_XPM_CHAR_PER_PIXEL_1 },
        { C_64_64_16_1_xpm, EXPORT_XPM_CHAR_SIZE_64_64 | EXPORT_XPM_CHAR_PER_PIXEL_1 },
        { C_64_64_16_2_xpm, EXPORT_XPM_CHAR_SIZE_64_64 | EXPORT_XPM_CHAR_PER_PIXEL_2 },

        { H_24_24_8_1_xpm, EXPORT_XPM_CHAR_SIZE_24_24 | EXPORT_XPM_CHAR_PER_PIXEL_1 },
        { H_32_32_10_1_xpm, EXPORT_XPM_CHAR_SIZE_32_32 | EXPORT_XPM_CHAR_PER_PIXEL_1 },
        { H_64_64_19_1_xpm, EXPORT_XPM_CHAR_SIZE_64_64 | EXPORT_XPM_CHAR_PER_PIXEL_1 },
        { H_64_64_19_2_xpm, EXPORT_XPM_CHAR_SIZE_64_64 | EXPORT_XPM_CHAR_PER_PIXEL_2 },

        { Hyphen_24_24_5_1_xpm, EXPORT_XPM_CHAR_SIZE_24_24 | EXPORT_XPM_CHAR_PER_PIXEL_1 },
        { Hyphen_32_32_7_1_xpm, EXPORT_XPM_CHAR_SIZE_32_32 | EXPORT_XPM_CHAR_PER_PIXEL_1 },
        { Hyphen_64_64_13_1_xpm, EXPORT_XPM_CHAR_SIZE_64_64 | EXPORT_XPM_CHAR_PER_PIXEL_1 },
        { Hyphen_64_64_13_2_xpm, EXPORT_XPM_CHAR_SIZE_64_64 | EXPORT_XPM_CHAR_PER_PIXEL_2 },

        { Hyphen_Angle_90_24_24_6_1_xpm, EXPORT_XPM_CHAR_SIZE_24_24 | EXPORT_XPM_CHAR_PER_PIXEL_1 },
        { Hyphen_Angle_90_32_32_11_1_xpm, EXPORT_XPM_CHAR_SIZE_32_32 | EXPORT_XPM_CHAR_PER_PIXEL_1 },
        { Hyphen_Angle_90_64_64_13_1_xpm, EXPORT_XPM_CHAR_SIZE_64_64 | EXPORT_XPM_CHAR_PER_PIXEL_1 },
        { Hyphen_Angle_90_64_64_13_2_xpm, EXPORT_XPM_CHAR_SIZE_64_64 | EXPORT_XPM_CHAR_PER_PIXEL_2 }
    };
    const char char_order [] = { 'C', 'H', '-', '|' };
    // Offset in der Liste der Dateien, um Dateien gleicher Konfiguration (fuer andere Zeichen) erreichen zu koennen
    const uint_fast8_t offset = 4;

    // Erste Konfiguration aus der Liste ermitteln, die der Konfig aus der Eingabe entspricht
    // Auf diesen Wert wird dann der Offset angebracht
    uint_fast8_t first_entry_with_suitable_config = 0;
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(available_files_for_configs); ++ i)
    {
        if (available_files_for_configs [i].export_settings == export_settings)
        {
            first_entry_with_suitable_config = (uint_fast8_t) i;
            break;
        }
    }
    // ===== ===== ===== ENDE Zuordnung der Konfigurationen zu den Quellbildern ===== ===== =====

    // FILE char Tupel erzeugen
    struct char_char_array_tuple tuple [COUNT_ARRAY_ELEMENTS(char_order)];
    for (size_t i = 0; i < COUNT_ARRAY_ELEMENTS(tuple); ++ i)
    {
        // Mithilfe des Offsets und der ersten passenden Konfig alle weiteren passenden Elemente bestimmen
        tuple [i].c = char_order [i];
        tuple [i].xpm_array_name =
                available_files_for_configs [first_entry_with_suitable_config + (i * offset)].xpm_array_name;
    }

    const uint_fast32_t x_length = (uint_fast32_t) (input->max_dim_1_used * char_size); /* Kein + 1 notwendig (aeussere Dimension !) */
    const uint_fast32_t y_length = (uint_fast32_t) (input->max_dim_2_used * char_size + 1);

    printf("Original dimensions (width, height):                %5" PRIuFAST8 ", %5" PRIuFAST8 "\n",
            input->max_dim_2_used, input->max_dim_1_used);
    printf("Export XPM file dimensions (width, height):         %5" PRIuFAST32 ", %5" PRIuFAST32 "\n",
            y_length - 1, x_length);
    fflush(stdout);

    // Aeussere Dimension
    char** output_data = (char**) CALLOC(x_length, sizeof (char*));
    ASSERT_ALLOC(output_data, "Cannot allocate memory for exporting an text-based drawing to XPM picture !",
            x_length * sizeof (char*));

    // Innere Dimension (die eigentlichen Zeichenketten)
    for (uint_fast32_t i = 0; i < x_length; ++ i)
    {
        output_data [i] = (char*) CALLOC(y_length, sizeof (char));
        ASSERT_ALLOC(output_data [i], "Cannot allocate memory for exporting an text-based drawing to XPM picture !",
                y_length * sizeof (char));
    }

    uint_fast32_t output_data_x_cursor = 0;
    uint_fast32_t output_data_y_cursor = 0;

    for (uint_fast8_t x = 0; x < input->max_dim_1_used; ++ x)
    {
        for (uint_fast8_t y = 0; y < input->max_dim_2_used; ++ y)
        {
            if (input->drawing[x][y] == '\0')
            {
                break;
            }

            Draw_Char_In_XPM(output_data, &output_data_x_cursor, &output_data_y_cursor, export_settings,
                    input->drawing[x][y], tuple, COUNT_ARRAY_ELEMENTS(tuple));
            output_data_x_cursor -= char_size;
        }

        output_data_x_cursor += char_size;
        output_data_y_cursor = 0;
    }

    // ===== ===== ===== BEGINN Fertiges XPM-Bild vom Speicher in die Datei schreiben ===== ===== =====
    char output_file_name [IUPAC_NAME_LENGTH + strlen (".xpm")];
    char output_array_name [IUPAC_NAME_LENGTH + strlen (".xpm")];
    memset (output_file_name, '\0', sizeof(output_file_name));
    memset (output_array_name, '\0', sizeof(output_array_name));
    strncat (output_file_name, input->iupac_name, IUPAC_NAME_LENGTH);
    strncat (output_file_name, ".xpm", IUPAC_NAME_LENGTH - strlen (output_file_name));
    strncat (output_array_name, input->iupac_name, IUPAC_NAME_LENGTH);

    // Name konvertieren -> Kommata und Minuszeichen mit Unterstriche ersetzen
    for (size_t i = 0; i < strlen(output_array_name); ++ i)
    {
        if (output_array_name [i] == '-' || output_array_name [i] == ',')
        {
            output_array_name [i] = '_';
        }
    }
    // Wenn das erste Zeichen eine Ziffer ist, dann muss ein Unterstrich vorangestellt werden, damit der Name des
    // Arrays in der XPM-Datei ein gueltiger Variablenname wird
    if (isdigit(output_array_name[0]) /* == true */)
    {
        // Den kompletten Namen um ein Zeichen nach hinten verschieben, damit der Unterstrich vorangestellt werden kann
        memmove(&(output_array_name [1]), output_array_name, strlen (output_array_name));
        output_array_name [0] = '_';
    }

    printf("Used IUPAC name:  %s\n", input->iupac_name);
    printf("Export file name: %s\n", output_file_name);
    fflush(stdout);

    FILE* result_file = FOPEN (output_file_name, "w");
    ASSERT_FMSG(result_file != NULL, "Error occurred while opening / creating the file \"%s\" !", output_file_name);

    // Puffer fuer die Ausgabedatei
    char* export_buffer = (char*) CALLOC(EXPORT_FILE_BUFFER_SIZE, sizeof (char));
    ASSERT_ALLOC(export_buffer, "Cannot create the file buffer for exporting !",
            EXPORT_FILE_BUFFER_SIZE * sizeof (char));
    const int setvbuf_result = setvbuf(result_file, export_buffer, _IOFBF, EXPORT_FILE_BUFFER_SIZE * sizeof (char));
    ASSERT_FMSG(setvbuf_result == 0, "Cannot use a user defined buffer for the file export. Used buffer size: %zu bytes",
            (size_t) EXPORT_FILE_BUFFER_SIZE * sizeof (char));

    // Allgemeine Informationen sammelbar machen
    size_t bytes_written = 0;
    size_t lines_written = 1; // Nur die Nutzdaten, keine Farbinformationen etc. !
    size_t longest_written_line = 0;

    // Header der XPM Datei
    char xpm_header [100];
    memset(xpm_header, '\0', sizeof(xpm_header));
    Create_XPM_Header
    (
        output_array_name,
        y_length - 1,   // JA erst y, da es um die Laenge der einzelnen Zeichen geht
                        // - 1, da y_length die Groesse des zu allokierenden Speichers beschreibt -> Terminatorsymbol
                        // enthalten
        x_length,
        COUNT_ARRAY_ELEMENTS(XPM_COLOR_LIST),
        1,
        xpm_header,
        COUNT_ARRAY_ELEMENTS(xpm_header) - 1
    );

    size_t write_return = 0;
    write_return = fwrite(xpm_header, sizeof (char), strlen(xpm_header), result_file);
    ASSERT_FMSG(write_return != 0, "Error while writing in the result file \"%s\" !", output_file_name);
    bytes_written += write_return;

    // Farbliste erzeugen
    char xpm_color_list [1000];
    memset(xpm_color_list, '\0', sizeof(xpm_color_list));
    Create_XPM_Color_List
    (
        xpm_color_list,
        COUNT_ARRAY_ELEMENTS(xpm_color_list) - 1
    );
    write_return = fwrite(xpm_color_list, sizeof (char), strlen(xpm_color_list), result_file);
    ASSERT_FMSG(write_return != 0, "Error while writing in the result file \"%s\" !", output_file_name);
    bytes_written += write_return;

    // Zeile fuer Zeile die Datei erzeugen
    for (uint_fast32_t i = 0; i < x_length; ++ i)
    {
        write_return = fwrite ("\"", sizeof (char), strlen("\""), result_file);
        ASSERT_FMSG(write_return != 0, "Error while writing in the result file \"%s\" !", output_file_name);
        bytes_written += write_return;

        write_return = fwrite (output_data [i], sizeof (char), y_length - 1, result_file);
        // printf (">\"%s\"<\n", output_data [i]);
        ASSERT_FMSG(write_return != 0, "Error while writing in the result file \"%s\" !", output_file_name);
        bytes_written += write_return;
        if (write_return > longest_written_line)
        {
            longest_written_line = write_return;
        }

        if ((i + 1) < x_length)
        {
            write_return = fwrite ("\",\n", sizeof (char), strlen("\",\n"), result_file);
            lines_written ++;
        }
        else
        {
            write_return = fwrite ("\"", sizeof (char), strlen("\""), result_file);
        }
        ASSERT_FMSG(write_return != 0, "Error while writing in the result file \"%s\" !", output_file_name);
        bytes_written += write_return;
    }

    // Footer der XPM-Datei
    char xpm_footer [10];
    memset(xpm_footer, '\0', sizeof(xpm_footer));
    Create_XPM_Footer
    (
        xpm_footer,
        COUNT_ARRAY_ELEMENTS(xpm_footer) - 1
    );

    write_return = fwrite(xpm_footer, sizeof (char), strlen (xpm_footer), result_file);
    ASSERT_FMSG(write_return != 0, "Error while writing in the result file \"%s\" !", output_file_name);
    bytes_written += write_return;

    printf ("Lines written: %8zu\n", lines_written);
    printf ("Longest line:  %8zu\n", longest_written_line);
    printf ("Color list:    %8zu\n", COUNT_ARRAY_ELEMENTS(XPM_COLOR_LIST));
    printf ("Bytes written: %8zu\n", bytes_written);
    fflush(stdout);

    FCLOSE_AND_SET_TO_NULL (result_file);
    FREE_AND_SET_TO_NULL (export_buffer);
    // ===== ===== ===== ENDE Fertiges XPM-Bild vom Speicher in die Datei schreiben ===== ===== =====

    for (uint_fast32_t i = 0; i < x_length; ++ i)
    {
        FREE_AND_SET_TO_NULL(output_data [i]);
    }
    FREE_AND_SET_TO_NULL(output_data);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

static void
Draw_Char_In_XPM
(
        char** const output_data,
        uint_fast32_t* const restrict x_cursor,
        uint_fast32_t* const restrict y_cursor,
        const enum Export_To_XPM_Settings export_settings,
        const char input_char,
        struct char_char_array_tuple* tuple,
        const size_t number_of_tuple
)
{
    uint_fast8_t char_size = 0;
    if ((export_settings & EXPORT_XPM_CHAR_SIZE_24_24) != 0)
    {
        char_size = 24;
    }
    else if ((export_settings & EXPORT_XPM_CHAR_SIZE_32_32) != 0)
    {
        char_size = 32;
    }
    else if ((export_settings & EXPORT_XPM_CHAR_SIZE_64_64) != 0)
    {
        char_size = 64;
    }

    // -1 bedeutet, dass es keinen Eintrag gibt, da es sich um ein Leerzeichen in der textbasierten Zeichnung handelt
    int_fast8_t tuple_index = -1;
    switch (input_char)
    {
    case ' ':
        tuple_index = -1;
        break;
    case 'C':
        tuple_index = 0;
        break;
    case 'H':
        tuple_index = 1;
        break;
    case '-':
        tuple_index = 2;
        break;
    case '|':
        tuple_index = 3;
        break;

        // Der default-Pfad soll nie ausgefuehrt werden ! Wenn dies dennoch der Fall ist, dann ist dies immer ein
        // Fehler !
    default:
        ASSERT_MSG(false, "default path of a switch case statement executed !");
    }

    if (tuple_index == -1)
    {
        for (uint_fast8_t i = 0; i < char_size; ++ i)
        {
            memset(&(output_data[*x_cursor][*y_cursor]), ' ', char_size * sizeof (char));
            *x_cursor += 1;
        }
    }
    else
    {
        // Ueberpruefung des Index
        ASSERT_FMSG((size_t) tuple_index < number_of_tuple, "Invalid index ! Got: %zu; max. valid: %zu !",
                (size_t) tuple_index, number_of_tuple);

        // ===== ===== ===== BEGINN Informationen ermitteln ===== ===== =====
        // Notwendige Informationen aus dem XPM Bild ermitteln (Laenge einer Zeile, Anzahl an Zeilen, Anzahl an Farben)
        char line_length_c_str [10]         = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
        char number_of_lines_c_str [10]     = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
        char number_of_colors_c_str [10]    = { '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0' };
        uint_fast8_t temp_index = 0;
        uint_fast8_t temp_index_conversion_string = 0;
        while (isdigit(tuple [tuple_index].xpm_array_name [0][temp_index]) &&
                temp_index < COUNT_ARRAY_ELEMENTS(line_length_c_str) - 1)
        {
            line_length_c_str [temp_index_conversion_string] =
                    tuple [tuple_index].xpm_array_name [0][temp_index];
            temp_index_conversion_string ++;
            temp_index ++;
        }
        temp_index_conversion_string = 0;
        temp_index ++;
        while (isdigit(tuple [tuple_index].xpm_array_name [0][temp_index]) &&
                temp_index < COUNT_ARRAY_ELEMENTS(number_of_lines_c_str) - 1)
        {
            number_of_lines_c_str [temp_index_conversion_string] =
                    tuple [tuple_index].xpm_array_name [0][temp_index];
            temp_index_conversion_string ++;
            temp_index ++;
        }
        temp_index_conversion_string = 0;
        temp_index ++;
        while (isdigit(tuple [tuple_index].xpm_array_name [0][temp_index]) &&
                temp_index < COUNT_ARRAY_ELEMENTS(number_of_colors_c_str) - 1)
        {
            number_of_colors_c_str [temp_index_conversion_string] =
                    tuple [tuple_index].xpm_array_name [0][temp_index];
            temp_index_conversion_string ++;
            temp_index ++;
        }

        long int line_length        = 0;
        long int number_of_lines    = 0;
        long int number_of_colors   = 0;
        enum str2int_errno conversion_result = str2int(&line_length, line_length_c_str, 10);
        ASSERT_FMSG(conversion_result == STR2INT_SUCCESS, "Cannot convert \"%s\" to int ! (line length)",
                line_length_c_str);
        conversion_result = str2int(&number_of_lines, number_of_lines_c_str, 10);
        ASSERT_FMSG(conversion_result == STR2INT_SUCCESS, "Cannot convert \"%s\" to int ! (number of lines)",
                number_of_lines_c_str);
        conversion_result = str2int(&number_of_colors, number_of_colors_c_str, 10);
        ASSERT_FMSG(conversion_result == STR2INT_SUCCESS, "Cannot convert \"%s\" to int ! (number of colors)",
                number_of_colors_c_str);

        ASSERT_FMSG(line_length > 0, "line_length is a negative number (%ld) !", line_length);
        ASSERT_FMSG(number_of_lines > 0, "number_of_lines is a negative number (%ld) !", number_of_lines);
        ASSERT_FMSG(number_of_colors > 0, "number_of_colors is a negative number (%ld) !", number_of_colors);
        // ===== ===== ===== ENDE Informationen ermitteln ===== ===== =====

        // Alle Nutzdaten aus dem XPM-Array kopieren
        for (long int i = number_of_colors + 1; i < (number_of_colors + number_of_lines + 1); ++ i)
        {
            // PRINTF_FFLUSH("\"%s\"\n", tuple [tuple_index].xpm_array_name [i]);
            memcpy(&(output_data[*x_cursor][*y_cursor]), tuple [tuple_index].xpm_array_name [i],
                    (size_t) line_length);
            // PRINTF_FFLUSH("\"%s\"\n", output_data[*x_cursor]);
            *x_cursor += 1;
        }
    }

    *y_cursor += char_size;

    return;
}

//---------------------------------------------------------------------------------------------------------------------
