/**
 * Alkane_Branch.c
 *
 *  Created on: 08.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane_Branch.h"
#include <stdlib.h>
#include <string.h>
#include "../Error_Handling/Assert_Msg.h"
#include "../Error_Handling/Dynamic_Memory.h"





//---------------------------------------------------------------------------------------------------------------------

/**
 * Alkane_Branch Objekt dynamisch erzeugen.
 *
 * Asserts:
 *      lenght > 0 && length <= (MAX_NUMBER_OF_C_ATOMS + 1) / 2, wenn branch != NULL
 */
struct Alkane_Branch*                       // Erzeugtes Alkane_Branch Objekt
Create_Alkane_Branch
(
        const unsigned char* const restrict branch,
                                            // Astinformationen, mit denen das Alkane_Branch Objekt initialisiert
                                            // werden kann
                                            // Oder NULL, falls eine Initialisierung mit Nullbyes durchgefuehrt werden
                                            // soll
        const uint_fast8_t length           // Laenge der Astinformationen
                                            // Wenn branch == NULL ist, dann wird dieser Wert ignoriert
)
{
    // Wenn Astinformationen vorhanden sind, dann muss die Laengenangabe ueberprueft werden
    if (branch != NULL)
    {
        ASSERT_MSG(length > 0, "The length for the branch values is 0 !");
        ASSERT_MSG(length <= (MAX_NUMBER_OF_C_ATOMS + 1) / 2, "The length value is too large !");
    }

    // Neues Objekt anlegen und Allokation ueberpruefen
    struct Alkane_Branch* new_branch = (struct Alkane_Branch*) CALLOC (1, sizeof (struct Alkane_Branch));
    ASSERT_ALLOC(new_branch, "Cannot allocate an Alkane_Branch object !", sizeof (struct Alkane_Branch));

    // Neues Objekt mit den uebergebenen Astinformationen initialisieren, falls vorhanden
    if (branch != NULL)
    {
        memcpy (new_branch->branch, branch, length * sizeof (unsigned char));
        new_branch->length = length;
        new_branch->state = ALKANE_BRANCH_INITIALIZED_WITH_DATA;
    }
    // Falls keine Initialisierungsinformationen vorhanden sind, dann wird das Objekt mit Nullbytes initialisiert
    else
    {
        memset (new_branch, '\0', sizeof (struct Alkane_Branch));
        new_branch->state = ALKANE_BRANCH_INITIALIZED_WITH_ZERO_BYTES;
    }

    return new_branch;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alkane_Branch Objekt in eine Zeichenkettendarstellung ueberfuehren. Dies ist insbesondere fuer debugging hilfreich.
 *
 * Asserts:
 *      branch != NULL,
 *      string_memory != NULL,
 *      string_memory_size > 0
 */
const char*                                         // Adresse der Eingabe-Zeichenkette
Alkane_Branch_To_String
(
        const struct Alkane_Branch* restrict const branch,
                                                    // Alkane_Branch, der als Zeichenkette dargestellt werden soll
        char* const restrict string_memory,         // Speicher, der fuer die Zeichenkettenerzeugung verwendet werden
                                                    // soll
                                                    // Der Speicher muss vorher vom Aufrufer allokiert werden
        const size_t string_memory_size             // Groesse des Zeichenkettenspeichers
                                                    // Wenn der Speicher nicht ausreicht, dann wird am vorletzten
                                                    // Zeichen die Zeichenkette abgeschnitten
)
{
    ASSERT_MSG(branch != NULL, "branch is NULL !");
    ASSERT_MSG(string_memory != NULL, "string_memory is NULL !");
    ASSERT_MSG(string_memory_size > 0, "string_memory_size is 0 !");

    size_t next_free_byte   = 0;                        // Naechstes freies Zeichen im Speicher
    size_t remaining_memory = string_memory_size - 1;   // Noch freie Zeichen im Speicher
    size_t used_char        = 0;                        // Anzahl an Zeichen, die im aktuellen snprintf-Aufruf in den
                                                        // Speicher geschrieben wurde

    if (remaining_memory == 0) { goto no_remaining_memory; }
    used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, "(");
    next_free_byte += used_char;
    remaining_memory -= used_char;

    // Astinhalt in die Zeichenkettendarstellung einbinden
    for (uint_fast8_t i = 0; i < branch->length; ++ i)
    {
        if (remaining_memory == 0) { goto no_remaining_memory; }
        used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, "%" PRIuFAST8, branch->branch [i]);
        next_free_byte += used_char;
        remaining_memory -= used_char;

        if ((i + 1) < branch->length)
        {
            if (remaining_memory == 0) { goto no_remaining_memory; }
            used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, ", ");
            next_free_byte += used_char;
            remaining_memory -= used_char;
        }
    }

    if (remaining_memory == 0) { goto no_remaining_memory; }
    used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, ")\n");
    next_free_byte += used_char;
    remaining_memory -= used_char;

    // Laenge des Astes in die Zeichenkettendarstellung einbinden
    if (remaining_memory == 0) { goto no_remaining_memory; }
    used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, "Length: %" PRIuFAST8 "\n", branch->length);
    next_free_byte += used_char;
    remaining_memory -= used_char;

    if (remaining_memory == 0) { goto no_remaining_memory; }
    used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, "State: ");
    next_free_byte += used_char;
    remaining_memory -= used_char;

    // Status in die Zeichenkettendarstellung einbringen
    if (remaining_memory == 0) { goto no_remaining_memory; }
    switch (branch->state)
    {
    case ALKANE_BRANCH_CREATED:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_BRANCH_CREATED\n");
        break;
    case ALKANE_BRANCH_DELETED:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_BRANCH_DELETED\n");
        break;
    case ALKANE_BRANCH_INITIALIZED_WITH_DATA:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_BRANCH_INITIALIZED_WITH_DATA\n");
        break;
    case ALKANE_BRANCH_INITIALIZED_WITH_ZERO_BYTES:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_BRANCH_INITIALIZED_WITH_ZERO_BYTES\n");
        break;
    case ALKANE_BRANCH_INVALID_BRANCH:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_BRANCH_INVALID_BRANCH\n");
        break;
    case ALKANE_BRANCH_UNKNOWN_STATE:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_BRANCH_UNKNOWN_STATE\n");
        break;

        // Der default-Pfad soll nie ausgefuehrt werden ! Wenn dies dennoch der Fall ist, dann ist dies ein Fehler !
    default:
        ASSERT_MSG(false, "default path of a switch case statement executed !");
    }

    no_remaining_memory:
    string_memory [string_memory_size - 1] = '\0';  // Nullterminierung garantieren

    return string_memory;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alkane_Branch Objekt auf stdout ausgeben. Hauptsaechlich fuer das Debbuging. Fuer die Konvertierung wird die
 * Funktion "Alkane_Branch_To_String" verwendet.
 *
 * Asserts:
 *      branch != NULL
 */
extern void
Print_Alkane_Branch
(
        const struct Alkane_Branch* const branch    // Alkane_Branch, welches ausgegeben werden soll
)
{
    ASSERT_MSG(branch != NULL, "branch is NULL !");

    char branch_to_string [150];
    memset (branch_to_string, '\0', sizeof (branch_to_string));

    // Zeichenkettendarstellung auf stdout ausgeben
    printf ("%s", Alkane_Branch_To_String (branch, branch_to_string, sizeof (branch_to_string)));
    fflush (stdout);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alkane_Branch Objekt loeschen.
 * Da es sich bei dem Objekt um dynamischen Speicher handelt, muss sichergestellt werden, dass alle Objekte, die zuvor
 * erstellt wurden, geloescht werden. Nach dem Loeschen sollte der verwendete Zeiger nicht mehr dereferenziert werden !
 *
 * Asserts:
 *      branch != NULL
 */
void
Delete_Alkane_Branch
(
        struct Alkane_Branch* restrict branch   // Alkane_Branch, welches geloescht werden soll
)
{
    ASSERT_MSG(branch != NULL, "branch is NULL !");

    // Status auf "Geloescht" setzen
    branch->state = ALKANE_BRANCH_DELETED;
    FREE_AND_SET_TO_NULL(branch);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
