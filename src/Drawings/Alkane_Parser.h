/**
 * @file Alkane_Parser.h
 *
 * @brief Der Alkan-Parser dient dazu, dass IUPAC-Namen von Alkanen auf ihre Gueltigkeit ueberprueft werden. Dabei
 * werden die Namen NICHT auf ihre inhaltliche Gueltigkeit ueberprueft; nur auf ihre formale Gueltigkeit.
 *
 * Beispiel: 1-(2-(3-methylethyl)propyl)Octane
 * Formal ist dieser Name gueltig. Allerdings nicht inhaltlich. "...(3-methylethyl)..." An der 3. Position befindet sich
 * eine Methylabzweigung - Und zwar bezogen auf eine uebergeordnete Ethylabzweigung.
 * Dies ist natuerlich Schwachsinn.
 *
 * Die Beurteilung, ob ein Name auch inhaltlich gueltig ist, kann nur beantwortet werden, wenn bereits die formale
 * Gueltigkeit ueberprueft wurde. Daher kann der Parser bei Bedarf auch auf die inhaltlichen Tests erweitert werden.
 *
 *
 * Beschreibung der Grammatik (kontextfrei):
 *
 * Vokabular:
 * - zahl           := Zahl
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
 * Terminalsymbole: (In den Klammern steht die Kurzform, die spaeter verwendet wird)
 * - zahl           (z)
 * - komma          (k)
 * - minus          (m)
 * - bopen          (o)
 * - bclose         (c)
 * - alkyl          (y)
 * - alkan          (a)
 * - number_word    (n)
 *
 * Startsymbol:
 * - START          (S)
 *
 * Produktionsregeln: (In den Klammern steht die Kurzform, die spaeter verwendet wird)
 * START            (S)     ->  alkan                                               (S  ->  a)
 * START            (S)     ->  BRANCH alkan                                        (S  ->  B2 a)
 * BRANCH           (B2)    ->  BRANCH_BEGIN NUMBER_WORD BRANCH_END                 (B2 ->  B1 W B3)
 * BRANCH           (B2)    ->  BRANCH_BEGIN NUMBER_WORD NESTING BRANCH_END bclose  (B2 ->  B1 W N2 B3 c)
 * BRANCH           (B2)    ->  BRANCH_BEGIN BRANCH_END                             (B2 ->  B1 B3) ! Dient dazu, wenn das Zahlenwort leer ist !
 * BRANCH           (B2)    ->  BRANCH_BEGIN NESTING BRANCH_END bclose              (B2 ->  B1 N2 B3 c) ! Dient dazu, wenn das Zahlenwort leer ist !
 * BRANCH_BEGIN     (B1)    ->  zahl minus                                          (B1 ->  z m)
 * BRANCH_BEGIN     (B1)    ->  zahl KOMMA_ZAHL minus                               (B1 ->  z K m)
 * NUMBER_WORD      (W)     ->  number_word                                         (W  ->  n)
 * NUMBER_WORD      (W)     ->  NULL                                                (W  ->  NULL) !!! Diese Regel steht der Vollstaendigkeit halber hier und hat (aktuell) keine Verwendung !!!
 * KOMMA_ZAHL       (K)     ->  komma zahl                                          (K  ->  k z)
 * KOMMA_ZAHL       (K)     ->  komma zahl KOMMA_ZAHL                               (K  ->  k z K)
 * BRANCH_END       (B3)    ->  alkyl                                               (B3 ->  y)
 * BRANCH_END       (B3)    ->  alkyl minus BRANCH                                  (B3 ->  y m B2)
 * NESTING          (N2)    ->  NESTING_BEGIN BRANCH_BEGIN NESTING NESTING_END bclose     (N2 ->  N1 B1 N2 N3 c)
 * NESTING          (N2)    ->  NESTING_BEGIN BRANCH_BEGIN NESTING_END              (N2 ->  N1 B1 N3)
 * NESTING_BEGIN    (N1)    ->  bopen                                               (N1 ->  o)
 * NESTING_END      (N3)    ->  alkyl                                               (N3 ->  y)
 *
 * "BRANCH_END" und "NESTING_END" sind als Variablen nicht erforderlich. Es koennte direkt das Terminalsymbol verwendet
 * werden. Sie werden dennoch verwendet, damit man eine analoge Bezeichnung zu "BRANCH_BEGIN" und "NESTING_BEGIN" hat.
 *
 *
 *
 * Damit das Wortproblem - also ob ein Wort von dieser Grammatik erzeugt werden kann - fuer diese Grammatik strukturiert
 * geloest werden kann, wird der sog. Cocke-Younger-Kasami-Algorithmus (CYK bzw. CKY-Algorithmus) verwendet. Dieser
 * erledigt das Problem mit einer Laufzeit von O(n^3).
 *
 * Demo als Hilfe fuer die Implementierung des CYK-Algorithmus; https://martinlaz.github.io/demos/cky.html
 *
 * Das enorme Problem bei diesem Algorithms: Die Grammatik muss in die sog. Chomsky-Normalform ueberfuehrt werden, damit
 * der Algorithmus ueberhaupt funktioniert.
 *
 * In dieser Normalform duerfen die Produktionsregeln nur eine der beiden Formen haben:
 * S -> AB      (Auf der rechten Seite GEANU ZWEI Nichtterminale)
 * S -> a       (Auf der rechten Seite GENAU EIN Terminal)
 *
 * Spoiler: Grammatiken in der Chomsky-Normalform sehen im Normalfall enorm schmandig aus. Unserer Grammatik wird es
 * wohl auch so ergehen ...
 *
 * 1. Schritt: Die Chomsky-Normalform bilden
 *      "Alkan-Grammatik" zusammengefasst:
 *      S   ->  a | B2 a
 *      B1  ->  z m | z K m
 *      B2  ->  B1 W B3 | B1 W N2 B3 c | B1 B3 | B1 W N2 B3 c
 *      B3  ->  y | y m B2
 *      N1  ->  o
 *      N2  ->  N1 B1 N3 | N1 B1 N2 N3 c
 *      N3  ->  y
 *      W   ->  n
 *      K   ->  k z | k z K
 *
 *      Neue Nichtterminale:
 *      Z   ->  z
 *      M   ->  m
 *      K2  ->  k
 *      C   ->  c
 *      A   ->  a
 *      Y   ->  y
 *
 *      Anwenden der neuen Nichtterminale:
 *      S   ->  a | B2 A
 *      B1  ->  Z M | Z K M
 *      B2  ->  B1 W B3 | B1 W N2 B3 C | B1 B3 | B1 N2 B3 C
 *      B3  ->  y | y M B2
 *      N2  ->  N1 B1 N3 | N1 B1 N2 N3 C
 *      K   ->  K2 Z | K2 Z K
 *
 *      Zu lange Regeln verkuerzen:
 *      B1  ->  Z X1
 *      X1  ->  K M
 *      B2  ->  B1 X2
 *      X2  ->  W B3
 *      B2  ->  B1 X3
 *      X3  ->  W X4
 *      X4  ->  N2 X5
 *      X5  ->  B3 C
 *      N2  ->  N1 X6
 *      X6  ->  B1 N3
 *      N2  ->  N1 X7
 *      X7  ->  B1 X8
 *      X8  ->  N2 X12
 *      X12 ->  N3 C
 *      K   ->  K2 X9
 *      X9  ->  Z K
 *      B2  ->  B1 X10
 *      X10 ->  N2 X11
 *      X11 ->  B3 C
 *
 *      => BEGINN Die komplette Chomsky-Normalform <=
        S   ->  B2 A
        S   ->  a
        B3  ->  y
        N1  ->  o
        N3  ->  y
        W   ->  n
        Z   ->  z
        M   ->  m
        K2  ->  k
        C   ->  c
        A   ->  a
        Y   ->  y
        B1  ->  Z M
        B1  ->  Z X1
        X1  ->  K M
        B2  ->  B1 X2
        X2  ->  W B3
        B2  ->  B1 X3
        X3  ->  W X4
        X4  ->  N2 X5
        X5  ->  B3 C
        B2  ->  B1 B3
        N2  ->  N1 X6
        X6  ->  B1 N3
        N2  ->  N1 X7
        X7  ->  B1 X8
        X8  ->  N2 X12
        X12 ->  N3 C
        K   ->  K2 X9
        K   ->  K2 Z
        X9  ->  Z K
        B2  ->  B1 X10
        X10 ->  N2 X11
        X11 ->  B3 C
        B3  ->  Y X13
        X13 ->  M B2
 *      => ENDE Die komplette  Chomsky-Normalform <=
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



extern _Bool Parse_Alkane (const char* const iupac_name, const size_t length);
extern struct Alkane_Lexer Create_Alkane_Tokens (const char* const iupac_name, const size_t length);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* ALKANE_PARSER_H */
