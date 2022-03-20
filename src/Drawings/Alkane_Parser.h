/**
 * @file Alkane_Parser.h
 *
 * Beschreibung der Grammatik (kontextfrei):
 *
 * Vokabular:
 * - ZAHL           := Zahl
 * - ziffer         := Ziffer
 * - komma          := Komma
 * - minus          := Minuszeichen
 *
 * - bopen          := Geoeffnete runde Klammer
 * - bclose         := Geschlossene runde Klammer
 * - KOMMA_ZAHL     := Komma gefolgt von einer Zahl
 * - komma_zahl     := Komma gefolgt von einer Zahl
 *
 * - alkyl          := Alkyl-Woerter (Siehe: Alkane_Info_Constitutional_Isomer.h / *.c)
 * - alkan          := Alkan-Woerter (Siehe: Alkane_Info_Constitutional_Isomer.h / *.c)
 * - NUMBER_WORD    := Zahlenwort (Siehe: Alkane_Info_Constitutional_Isomer.h / *.c)
 * - number_word    := Zahlenwort (Siehe: Alkane_Info_Constitutional_Isomer.h / *.c)
 * - START          := '\0'
 * - NULL           := '\0'
 * - BRANCH         := Verzweigung
 * - NESTING        := Verschachtelung
 *
 * - BRANCH_BEGIN   := Beginn einer Verzweigung
 * - BRANCH_END     := Ende einer Verzweigung
 * - NESTING_BEGIN  := Beginn einr Verschachtelung
 * - NESTING_END    := Ende einer Verschachtelung
 *
 * Terminalsymbole:
 * - ziffer
 * - komma
 * - minus
 * - bopen
 * - bclose
 * - alkyl
 * - alkan
 * - number_word
 *
 * Startsymbol:
 * - START
 *
 * Produktionsregeln:
 * START            ->  alkan
 * START            ->  BRANCH alkan
 * BRANCH           ->  BRANCH_BEGIN NUMBER_WORD BRANCH_END
 * BRANCH           ->  BRANCH_BEGIN NUMBER_WORD NESTING BRANCH_END bclose
 * BRANCH_BEGIN     ->  ZAHL minus
 * BRANCH_BEGIN     ->  ZAHL KOMMA_ZAHL minus
 * NUMBER_WORD      ->  number_word
 * NUMBER_WORD      ->  NULL
 * ZAHL             ->  ziffer
 * ZAHL             ->  ZAHL ziffer
 * KOMMA_ZAHL       ->  komma ZAHL
 * KOMMA_ZAHL       ->  komma ZAHL KOMMA_ZAHL
 * BRANCH_END       ->  alkyl
 * NESTING          ->  NESTING_BEGIN BRANCH_BEGIN NESTING NESTING_END
 * NESTING          ->  NESTING_BEGIN BRANCH_BEGIN NESTING_END
 * NESTING_BEGIN    ->  bopen
 * NESTING_END      ->  alkyl
 *
 * "BRANCH_END" und "NESTING_END" sind als Variablen nicht erforderlich. Es koennte direkt das Terminalsymbol verwendet
 * werden. Sie werden dennoch verwendet, damit man eine analoge Bezeichnung zu "BRANCH_BEGIN" und "NESTING_BEGIN" hat.
 *
 * @date 10.03.2022
 * @author am1
 */

#ifndef ALKANE_PARSER_H
#define ALKANE_PARSER_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include <stddef.h>
#include "../Alkane/Alkane.h"



/**
 * Was fuer ein Typ ist der erzeugte Token ?
 */
enum Token_Type
{
    TOKEN_TYPE_N_A = 0,

    TOKEN_TYPE_NUMBER,
    TOKEN_TYPE_ALKYL_WORD,
    TOKEN_TYPE_ALKANE_WORD,
    TOKEN_TYPE_NUMBER_WORD,
    TOKEN_TYPE_COMMA_CHAR,
    TOKEN_TYPE_SUB_CHAR,
    TOKEN_TYPE_OPEN_BRACKET,
    TOKEN_TYPE_CLOSE_BRACKET
};

/**
 * Einfacher Lexer, um den IUPAC-Namen in passende Tokens zu zerlegen.
 */
struct Alkane_Lexer
{
    // Die Groesse der 2. Dimension ist vorerst nur ein Platzhalterwert !
    char result_tokens [MAX_NUMBER_OF_C_ATOMS][30];                 ///< Ergebnisse
    uint_fast8_t next_free_token;                                   ///< Nachestes freies Token
    /**
     * @brief Naechstes freies Zeichen im akteullen Token.
     */
    uint_fast8_t next_free_char_in_token;

    enum Token_Type token_type [MAX_NUMBER_OF_C_ATOMS];             ///< Art der Tokens

    const char* alkane_name;                                        ///< Alkanname
    size_t name_length;                                             ///< Laenge des Alkannamens
    /**
     * @brief Aktuelles Zeichen im Alkannamen, welches verarbetet wird.
     */
    uint_fast8_t current_char;
    /**
     * @brief Letztes Zeichen, welches bereits verarbeitet wurde.
     */
    uint_fast8_t last_char_used;
};



extern void Parse_Alkane (const char* const iupac_name, const size_t length);
extern struct Alkane_Lexer Create_Alkane_Tokens (const char* const iupac_name, const size_t length);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_PARSER_H */
