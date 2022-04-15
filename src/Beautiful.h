/**
 * @file Beautiful.h
 *
 * @brief Schoene Voegel, die gezeigt werden muessen !
 *
 * Alle liebevollen Voegel wurden hier gefunden: https://textart.sh/topic/vulture
 *
 * Letztendlich muss man fuer solch spezielle Ziele alle Moeglichkeiten nutzten, die zur Verfuegung stehen (koennten).
 * Also fuehren nur passende Befehle zum Ziel !
 *
    ### Try to execute: ###
    .data
    required_abilities: .string "Falconry license\0"
    personal_goal: .string "Friendship\0"
    Etc: .string "New knowledge possible\0"

    .text

    # Where is this address ???
    # How can i get it :o ?
    .extern vulture

    .global main
    main:
		    pushl Etc
		    pushl personal_goal
		    pushl required_abilities

    next_try:
		    call vulture

		    cmpl $0, %eax
		    jne what_can_i_do

		    # Call successful: Let it happen and love them!
		    ret

    what_can_i_do:
		    # New idea ...
            # Use all information and ignore useless interrupts: AARIC, Alfred Brehm
		    jmp next_try
 *
 * @date 04.09.2021
 * @author am1 / Gyps
 */

#ifndef BEAUTIFUL_H
#define BEAUTIFUL_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/**
 * @brief Show the first beautiful bird.
 *
 * Ein wunderschoener Gaensegeier ! Hoffentlich kann ich bald einen besuchen !
 */
extern void Beautiful_1 (void);

/**
 * @brief Show the second beautiful bird.
 *
 * Ein faszinierender Ohrengeier ! Wie sehr wuerde ich mich freuen, wenn er mich als Freund oder zumindest als Bekannter
 * sehen wuerde. Und gleichzeitig mich in seiner Naehe lassen wuerde.
 */
extern void Beautiful_2 (void);

/**
 * @brief Show the third beautiful bird.
 *
 * Klein aber suess. Ein liebenswerter Schmutzgeier. Wirf die Eier mit Steinen kaputt; ich werde dir dabei helfen ! :)
 */
extern void Beautiful_3 (void);

/**
 * @brief Show the fourth beautiful vulture.
 *
 * Ich benoetige deine Hilfe ! DU hast mir einige meiner Mnemonics gezeigt. Bitte zeige mir mehr !
 */
extern void Beautiful_4 (void);



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* BEAUTIFUL_H */
