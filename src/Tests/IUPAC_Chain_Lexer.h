/**
 * @file IUPAC_Chain_Lexer.h
 *
 * @brief Einfacher Lexer, um IUPAC-Namen an den Grenzen der Ketten (OHNE Beachtung von Verschachtelungen) zu trennen.
 *
 * In einigen Faellen sind die erzeugten IUPAC-Namen richtig; aber die Aeste sind an der falschen Stelle im Namen im
 * Vergleich zu den erwarteten Loesungen.
 *
 * Beispiel
 * Erwartet: 4-(1,1-Dimethylethyl)-2-methylheptan
 * Erzeugt:  2-Methyl-4-(1,1-DiMethylEthyl)Heptan
 *
 * Um sicher zu stellen, dass solche Ergebnisse als richtige Ergebnisse erkannt werden, werden die erzeugten Ergebnisse
 * aufgespalten.
 *
 * Aus "2-Methyl-4-(1,1-DiMethylEthyl)Heptan" wird gebildet:
 * -> 2-Methyl-
 * -> 4-(1,1-DiMethylEthyl)
 * -> Heptan
 *
 * @date 27.01.2022
 * @author am1
 */

#ifndef IUPAC_CHAIN_LEXER_H
#define IUPAC_CHAIN_LEXER_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



#include "../Alkane/Alkane.h"



/**
 * @brief Datenobjekt, welches fuer die Verwendung des Lexers verwendet wird. Im Objekt sind die Ergebnisse enthalten.
 */
struct IUPAC_Chain_Lexer_Result
{
    char result_tokens [MAX_NUMBER_OF_NESTING_DEPTH][IUPAC_NAME_LENGTH];    ///< Ergebnisse
    uint_fast8_t next_free_token;                                           ///< Nachestes freies Token
    /**
     * @brief Naechstes freies Zeichen im akteullen Token.
     */
    uint_fast8_t next_free_char_in_token;

    const char* orig_string;                                                ///< Original-String, der verarbeitet wird
    /**
     * @brief Aktuelles Zeichen im Original-String, welches verarbetet wird.
     */
    uint_fast8_t current_char;
};

/**
 * @brief Lexter mit der Originalzeichenkette starten.
 *
 * @param[in] Zeichenkette, die verarbeitet wird.
 *
 * @return Ergebnisobjekt mit den aufgeteilten Zeichenketten.
 */
struct IUPAC_Chain_Lexer_Result Create_Chain_Tokens (const char* const iupac_name);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* IUPAC_CHAIN_LEXER_H */
