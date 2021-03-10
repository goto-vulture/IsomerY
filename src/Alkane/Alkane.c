/**
 * Alkane.c
 *
 *  Created on: 10.03.2021
 *      Author: x86 / Gyps
 */

#include "Alkane.h"
#include <stdlib.h>
#include <string.h>
#include "../Error_Handling/Dynamic_Memory.h"
#include "../Error_Handling/Assert_Msg.h"





//---------------------------------------------------------------------------------------------------------------------

/**
 * Alkane Objekt dynamisch erzeugen.
 *
 * Alle Zeiger duerfen NULL sein, in diesem Fall wird davon ausgegangen, dass dieser Ast nur aus einem H-Atom besteht.
 * Damit die Adressen, die ungleich NULL sind, aufsteigend im Alkane-Objekt gespeichert werden, (dies vereinfacht die
 * weiteren Operationen) werden die uebergebenen Adressen im Objekt neu angeordnet, wenn die Alkane_Branch-Objekte
 * nicht "von links nach rechts" der Funktion uebergeben werden.
 *
 * Asserts:
 *      N/A
 */
struct Alkane*                          // Erzeugtes Alkane-Objekt
Create_Alkane
(
        struct Alkane_Branch* branch_1, // 1. Ast des neuen Alkane-Objektes
        struct Alkane_Branch* branch_2, // 2. Ast des neuen Alkane-Objektes
        struct Alkane_Branch* branch_3, // 3. Ast des neuen Alkane-Objektes
        struct Alkane_Branch* branch_4  // 4. Ast des neuen Alkane-Objektes
)
{
    // Neues Objekt erzeugen
    struct Alkane* new_alkane = (struct Alkane*) CALLOC (1, sizeof (struct Alkane));
    ASSERT_ALLOC(new_alkane, "Cannot allocate an Alkane object !", sizeof (struct Alkane));

    // Die uebergebenen Adressen linksbuendig in das Alkane-Objekt eintragen
    uint_fast8_t next_free_branch = 0;
    if (branch_1 != NULL) { new_alkane->branches [next_free_branch] = branch_1; next_free_branch ++; }
    if (branch_2 != NULL) { new_alkane->branches [next_free_branch] = branch_2; next_free_branch ++; }
    if (branch_3 != NULL) { new_alkane->branches [next_free_branch] = branch_3; next_free_branch ++; }
    if (branch_4 != NULL) { new_alkane->branches [next_free_branch] = branch_4; next_free_branch ++; }

    new_alkane->next_free_branch = next_free_branch;

    // Anzahl an C-Atomen im gesamten Alkan bestimmen
    unsigned int number_of_c_atoms = 0;
    for (size_t i = 0; i < (sizeof (new_alkane->branches) / sizeof (new_alkane->branches [0])); ++ i)
    {
        number_of_c_atoms += new_alkane->branches [i]->length;
    }
    new_alkane->number_of_c_atoms = (uint_fast8_t) number_of_c_atoms;


    /**
     * Jetzt kommt der spanendste Teil dieser Funktion:
     * Das Zusammenfuehren der Zahlencodes zu einen Zahlencode
     *
     * Ein Beispiel fuer das Zusammenfuehren der Zahlencodes:
     * Ast 1: (1)
     * Ast 2: (1, 1, 1)
     * Ast 3: (1, 1, 1, 1)
     * Ast 4: ()
     *
     * Ergebnis: (1, 1, 2, 2, 1, 5, 5, 5)
     */
    unsigned int c_atoms_wrote = 0;
    for (uint_fast8_t i = 0; i < new_alkane->next_free_branch; ++ i)
    {
        // Der Zahlencode des ersten Alkane_Branch kann ohne Aenderungen uebernommen werden
        if (i == 0)
        {
            memcpy (new_alkane->merged_numbercode, new_alkane->branches [0], new_alkane->branches [0]->length);
            c_atoms_wrote += new_alkane->branches [0]->length;
        }
        // Ansonsten muss der Zahlencode der Aeste angepasst werden, damit ein Zahlencode fuer das gesamte Molekuel
        // gebildet wird
        else
        {
            // Der Offset wird aus den aufsummierten Laengen, der bereits vollstaendig betrachteten Alkane_Branch
            // Objekte, gebildet
            unsigned int numbercode_offset = 0;
            for (uint_fast8_t i2 = 0; i2 < i; ++ i2)
            { numbercode_offset += new_alkane->branches [i2]->length; }

            for (uint_fast8_t i2 = 0; i2 < new_alkane->branches [i]->length; ++ i2)
            {
                // Jedes 1. Objekt eines Zahlencodes MUSS unveraendert uebernommen werden
                if (i2 == 0)
                {
                    new_alkane->merged_numbercode [c_atoms_wrote] = new_alkane->branches [i]->branch [0];
                }
                else
                {
                    new_alkane->merged_numbercode [c_atoms_wrote] =
                            (unsigned char) (new_alkane->branches [i]->branch [i2] + numbercode_offset);
                }

                ++ c_atoms_wrote;
            }
        }
    }


    // Den Zustand anpassen => Wurde das Objekt mit bereits vorhandenen Daten oder mit Nullbytes initialisiert ?
    new_alkane->state = (next_free_branch == 0) ? ALKANE_INITIALIZED_WITH_ZERO_BYTES : ALKANE_INITIALIZED_WITH_DATA;

    return new_alkane;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alkane Objekt in eine Zeichenkettendarstellung ueberfuehren. Dies ist insbesondere fuer debugging hilfreich.
 *
 * Der Speicher fuer die Zeichenkette muss vom Aufrufer allokiert werden !
 *
 * Branch 1:
 * <To_String Darstellung des 1. Astes>
 * Branch 2:
 * <To_String Darstellung des 2. Astes>
 * Branch 3:
 * <To_String Darstellung des 3. Astes>
 * Branch 4:
 * <To_String Darstellung des 4. Astes>
 *
 * (<Aufbau des Alkans>)
 *
 * Length: <Laenge>
 * State: <Status>
 *
 * Asserts:
 *      alkane != NULL,
 *      string_memory != NULL,
 *      string_memory_size > 0
 */
const char*                                 // Adresse der Eingabe-Zeichenkette
Alkane_To_String
(
        const struct Alkane* const alkane,  // Alkane der als Zeichenkette dargestellt werden soll
        char* const string_memory,          // Speicher, der fuer die Zeichenkettenerzeugung verwendet werden soll
                                            // Der Speicher muss vorher vom Aufrufer allokiert werden
        const size_t string_memory_size     // Groesse des Zeichenkettenspeichers
                                            // Wenn der Speicher nicht ausreicht, dann wird am vorletzten Zeichen die
                                            // Zeichenkette abgeschnitten
)
{
    ASSERT_MSG(alkane != NULL,          "alkane is NULL !");
    ASSERT_MSG(string_memory != NULL,   "string_memory is NULL !");
    ASSERT_MSG(string_memory_size > 0,  "string_memory_size is 0 !");

    size_t next_free_byte   = 0;                        // Naechstes freies Zeichen im Speicher
    size_t remaining_memory = string_memory_size - 1;   // Noch freie Zeichen im Speicher
    size_t used_char        = 0;                        // Anzahl an Zeichen, die im aktuellen snprintf-Aufruf in den
                                                        // Speicher geschrieben wurde

    // Die Aeste des Alkans in die Zeichenkettendarstellung einbinden
    for (size_t i = 0; i < (sizeof (alkane->branches) / sizeof (alkane->branches [0])); ++ i)
    {
        if (remaining_memory == 0) { goto no_remaining_memory; }
        used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, "Branch %zu:\n", i);
        next_free_byte += used_char;
        remaining_memory -= used_char;

        if (alkane->branches [i] != NULL)
        {
            char temp_string [150];
            memset (temp_string, '\0', sizeof (temp_string));

            // Das aktuelle Alkane_Branch-Objekt in eine Zeichenkettendarstellung ueberfuehren
            Alkane_Branch_To_String(alkane->branches [i], temp_string, sizeof (temp_string));

            const size_t temp_string_length = strlen (temp_string);
            strncpy (string_memory + next_free_byte, temp_string, remaining_memory);
            next_free_byte += temp_string_length;
            remaining_memory -= temp_string_length;
        }
        else
        {
            if (remaining_memory == 0) { goto no_remaining_memory; }
            used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, "NULL");
            next_free_byte += used_char;
            remaining_memory -= used_char;
        }

        if (remaining_memory == 0) { goto no_remaining_memory; }
        used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, "\n");
        next_free_byte += used_char;
        remaining_memory -= used_char;
    }

    if (remaining_memory == 0) { goto no_remaining_memory; }
    used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, "\n(");
    next_free_byte += used_char;
    remaining_memory -= used_char;

    // Aufbau des Alkans in eine Zeichenkettendarstellung ueberfuehren
    for (uint_fast8_t i = 0; i < alkane->number_of_c_atoms; ++ i)
    {
        if (remaining_memory == 0) { goto no_remaining_memory; }
        used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, "%" PRIuFAST8,
                alkane->merged_numbercode [i]);
        next_free_byte += used_char;
        remaining_memory -= used_char;

        if ((i + 1) < alkane->number_of_c_atoms)
        {
            if (remaining_memory == 0) { goto no_remaining_memory; }
            used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, ", ");
            next_free_byte += used_char;
            remaining_memory -= used_char;
        }
    }

    if (remaining_memory == 0) { goto no_remaining_memory; }
    used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, ")\n\n");
    next_free_byte += used_char;
    remaining_memory -= used_char;

    // Laengeninformation einfuegen
    if (remaining_memory == 0) { goto no_remaining_memory; }
    used_char = (size_t) snprintf (string_memory + next_free_byte, remaining_memory, "%" PRIuFAST8 "\n",
            alkane->number_of_c_atoms);
    next_free_byte += used_char;
    remaining_memory -= used_char;

    // Status in die Zeichenkettendarstellung einbringen
    if (remaining_memory == 0) { goto no_remaining_memory; }
    switch (alkane->state)
    {
    case ALKANE_CREATED:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_CREATED\n");
        break;
    case ALKANE_DELETED:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_DELETED\n");
        break;
    case ALKANE_INITIALIZED_WITH_DATA:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_INITIALIZED_WITH_DATA\n");
        break;
    case ALKANE_INITIALIZED_WITH_ZERO_BYTES:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_INITIALIZED_WITH_ZERO_BYTES\n");
        break;
    case ALKANE_INVALID_BRANCH:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_INVALID_BRANCH\n");
        break;
    case ALKANE_UNKNOWN_STATE:
        snprintf (string_memory + next_free_byte, remaining_memory, "ALKANE_UNKNOWN_STATE\n");
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
 * AlkaneObjekt auf stdout ausgeben. Hauptsaechlich fuer das Debbuging. Fuer die Konvertierung wird die Funktion
 * "Alkane_To_String" verwendet.
 *
 * Asserts:
 *      alkane != NULL
 */
void
Print_Alkane
(
        const struct Alkane* const alkane   // Alkane, welches ausgegeben werden soll
)
{
    ASSERT_MSG(alkane != NULL, "branch is NULL !");

    char alkane_to_string [1024];
    memset (alkane_to_string, '\0', sizeof (alkane_to_string));

    // Zeichenkettendarstellung auf stdout ausgeben
    printf ("%s", Alkane_To_String (alkane, alkane_to_string, sizeof (alkane_to_string)));
    fflush (stdout);

    return;
}

//---------------------------------------------------------------------------------------------------------------------

/**
 * Alkane Objekt loeschen.
 *
 * Die enthaltenen Alkane_Branch-Objekte werden NICHT geloescht, wenn das Alkane-Objekt geloescht wird. Es werden
 * lediglich die Zeiger des Objektes auf NULL gesetzt !
 *
 * Da es sich bei dem Objekt um dynamischen Speicher handelt, muss sichergestellt werden, dass alle Objekte, die zuvor
 * erstellt wurden, geloescht werden. Nach dem Loeschen sollte der verwendete Zeiger nicht mehr dereferenziert werden !
 *
 * Asserts:
 *      alkane != NULL
 */
void
Delete_Alkane
(
        struct Alkane* alkane   // Alkane, welches geloescht werden soll
)
{
    ASSERT_MSG(alkane != NULL, "alkane is NULL !");

    // Alle Zeiger sicherheitshalber auf NULL setzen, auch wenn ein Zugriff drauf nach dieser Funktion recht
    // unwahrscheinlich ist
    for (size_t i = 0; i < (sizeof (alkane->branches) / sizeof (alkane->branches [0])); ++ i)
    {
        alkane->branches [i] = NULL;
    }

    alkane->state = ALKANE_DELETED;

    // Alkane-Objekt loeschen
    FREE_AND_SET_TO_NULL(alkane);

    return;
}

//---------------------------------------------------------------------------------------------------------------------
