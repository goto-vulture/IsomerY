/*
 * Inline_Asm.h
 *
 * Das Carry-Flag bei der Addition der ersten beiden Makro-Argumente abfragen. Damit kann man feststellen, ob eine
 * Addition das Uebertragsbit setzt. Dieses Bit zeigt an, dass das Ergebnis beider Summanden nicht mehr in das
 * urspruengliche Speicherelement passen.
 *
 * Diese Tests sind nur selten von Bedeutung. Die Idee war eine halbwegs sinnvolle Verwendung von Inline-Assembler
 * Befehle. Irgendwie muss ich das ja lernen ... :)
 *
 *  Created on: 15.08.2021
 *      Author: x86 / Gyps
 */

#ifndef INLINE_ASM_H
#define INLINE_ASM_H

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



//---------------------------------------------------------------------------------------------------------------------

/**
 * Damit bei jedem Assemblerbefehl "\n\t" eingetippt werden muss, gibt es dieses #define.
 * Streng genommen ist ein "\n\t" am eine eines Inline-Assemblerbefehl nicht notwendig. Es hilft aber der
 * Uebersichtlichkeit im Disassembler.
 */
#ifndef NT
#define NT "\n\t"
#else
#error "The macro \"NT\" is already defined !"
#endif /* NT */

//---------------------------------------------------------------------------------------------------------------------

/**
 * Mittels einer addb (8-Bit Addition) feststellen, ob die Summe beider Summanden in 8-Bit passen.
 */
#ifndef CARRY_CHECK_8_BIT
#define CARRY_CHECK_8_BIT(add_1, add_2, result) \
    __asm__ volatile                            \
(                                               \
        "addb %1, %2"                   NT      \
        "jc carry_flag_8_bit"           NT      \
        "jmp no_carry_flag_8_bit"       NT      \
        "carry_flag_8_bit:"             NT      \
        "movb $1, %0"                   NT      \
        "jmp end_carry_flag_8_bit"      NT      \
        "no_carry_flag_8_bit:"          NT      \
        "movb $0, %0"                   NT      \
        "end_carry_flag_8_bit:"         NT      \
                                                \
        : "=r" (result)                         \
        : "r" (add_1), "r" (add_2)              \
        :                                       \
);
#else
#error "The macro \"CARRY_CHECK_8_BIT\" is already defined !"
#endif /* CARRY_CHECK_8_BIT */

//---------------------------------------------------------------------------------------------------------------------

/**
 * Mittels einer addw (16-Bit Addition) feststellen, ob die Summe beider Summanden in 16-Bit passen.
 */
#ifndef CARRY_CHECK_16_BIT
#define CARRY_CHECK_16_BIT(add_1, add_2, result)    \
__asm__ volatile                                    \
(                                                   \
        "addw %1, %2"                   NT          \
        "jc carry_flag_16_bit"          NT          \
        "jmp no_carry_flag_16_bit"      NT          \
        "carry_flag_16_bit:"            NT          \
        "movw $1, %0"                   NT          \
        "jmp end_carry_flag_16_bit"     NT          \
        "no_carry_flag_16_bit:"         NT          \
        "movw $0, %0"                   NT          \
        "end_carry_flag_16_bit:"        NT          \
                                                    \
        : "=r" (result)                             \
        : "r" (add_1), "r" (add_2)                  \
        :                                           \
);
#else
#error "The macro \"CARRY_CHECK_16_BIT\" is already defined !"
#endif /* CARRY_CHECK_16_BIT */

//---------------------------------------------------------------------------------------------------------------------

/**
 * Mittels einer addl (32-Bit Addition) feststellen, ob die Summe beider Summanden in 32-Bit passen.
 */
#ifndef CARRY_CHECK_32_BIT
#define CARRY_CHECK_32_BIT(add_1, add_2, result)    \
__asm__ volatile                                    \
(                                                   \
        "addl %1, %2"                   NT          \
        "jc carry_flag_32_bit"          NT          \
        "jmp no_carry_flag_32_bit"      NT          \
        "carry_flag_32_bit:"            NT          \
        "movl $1, %0"                   NT          \
        "jmp end_carry_flag_32_bit"     NT          \
        "no_carry_flag_32_bit:"         NT          \
        "movl $0, %0"                   NT          \
        "end_carry_flag_32_bit:"        NT          \
                                                    \
        : "=r" (result)                             \
        : "r" (add_1), "r" (add_2)                  \
        :                                           \
);
#else
#error "The macro \"CARRY_CHECK_32_BIT\" is already defined !"
#endif /* CARRY_CHECK_32_BIT */

//---------------------------------------------------------------------------------------------------------------------

/**
 * Mittels einer addq (64-Bit Addition) feststellen, ob die Summe beider Summanden in 64-Bit passen.
 */
#ifndef CARRY_CHECK_64_BIT
#define CARRY_CHECK_64_BIT(add_1, add_2, result)    \
__asm__ volatile                                    \
(                                                   \
        "addq %1, %2"                   NT          \
        "jc carry_flag_64_bit"          NT          \
        "jmp no_carry_flag_64_bit"      NT          \
        "carry_flag_64_bit:"            NT          \
        "movq $1, %0"                   NT          \
        "jmp end_carry_flag_64_bit"     NT          \
        "no_carry_flag_64_bit:"         NT          \
        "movq $0, %0"                   NT          \
        "end_carry_flag_64_bit:"        NT          \
                                                    \
        : "=r" (result)                             \
        : "r" (add_1), "r" (add_2)                  \
        :                                           \
);
#else
#error "The marco \"CARRY_CHECK_64_BIT\" is already defined !"
#endif /* CARRY_CHECK_64_BIT */

//---------------------------------------------------------------------------------------------------------------------



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INLINE_ASM_H */
