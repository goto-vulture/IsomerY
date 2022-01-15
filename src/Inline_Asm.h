/**
 * @file Inline_Asm.h
 *
 * @brief Hier stehen einige Inline-Assembler Makros, die rein aus persoenlichen Interesse geschrieben wurden.
 *
 * Das Carry-Flag bei der Addition der ersten beiden Makro-Argumente abfragen. Damit kann man feststellen, ob eine
 * Addition das Uebertragsbit setzt. Dieses Bit zeigt an, dass das Ergebnis beider Summanden nicht mehr in das
 * urspruengliche Speicherelement passen.
 *
 * Diese Tests sind nur selten von Bedeutung. Die Idee war eine halbwegs sinnvolle Verwendung von Inline-Assembler
 * Befehle. Irgendwie muss ich das ja lernen ... :)
 *
 * @date 15.08.2021
 * @author x86 / Gyps
 */

#ifndef INLINE_ASM_H
#define INLINE_ASM_H ///< Include-Guard

// BEGINN C++-Kompablitaet herstellen
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */



/**
 * @brief Wenn die Inline-Assemlerfragmente mit 64-Bit Code verwendet werden sollen
 */
#ifndef USE_X86_64
#define USE_X86_64
#else
#error "The macro \"USE_X86_64\" is already defined !"
#endif /* USE_X86_64 */



//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Newline-Tabulator String fuer die Formatierung der Inline-Assembler Befehle
 *
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
 * @brief Mittels einer addb (8-Bit Addition) feststellen, ob die Summe beider Summanden in 8-Bit passen.
 *
 * https://de.wikipedia.org/wiki/Statusregister
 * "Ist dieses Flag gesetzt, dann trat bei der letzten Rechenoperation ein Übertrag auf, wenn man von vorzeichenlosen
 * Operanden ausgeht."
 */
#ifndef CARRY_CHECK_8_BIT_ADD
#define CARRY_CHECK_8_BIT_ADD(add_1, add_2, result) \
    __asm__ volatile                                \
(                                                   \
        "addb %1, %2"                   NT          \
        "jc carry_flag_8_bit_add"       NT          \
        "jmp no_carry_flag_8_bit_add"   NT          \
        "carry_flag_8_bit_add:"         NT          \
        "movb $1, %0"                   NT          \
        "jmp end_carry_flag_8_bit_add"  NT          \
        "no_carry_flag_8_bit_add:"      NT          \
        "movb $0, %0"                   NT          \
        "end_carry_flag_8_bit_add:"     NT          \
                                                    \
        : "=r" (result)                             \
        : "r" (add_1), "r" (add_2)                  \
        :                                           \
);
#else
#error "The macro \"CARRY_CHECK_8_BIT_ADD\" is already defined !"
#endif /* CARRY_CHECK_8_BIT_ADD */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Mittels einer addw (16-Bit Addition) feststellen, ob die Summe beider Summanden in 16-Bit passen.
 *
 * https://de.wikipedia.org/wiki/Statusregister
 * "Ist dieses Flag gesetzt, dann trat bei der letzten Rechenoperation ein Übertrag auf, wenn man von vorzeichenlosen
 * Operanden ausgeht."
 */
#ifndef CARRY_CHECK_16_BIT_ADD
#define CARRY_CHECK_16_BIT_ADD(add_1, add_2, result)    \
__asm__ volatile                                        \
(                                                       \
        "addw %1, %2"                       NT          \
        "jc carry_flag_16_bit_add"          NT          \
        "jmp no_carry_flag_16_bit_add"      NT          \
        "carry_flag_16_bit:"                NT          \
        "movw $1, %0"                       NT          \
        "jmp end_carry_flag_16_bit_add"     NT          \
        "no_carry_flag_16_bit_add:"         NT          \
        "movw $0, %0"                       NT          \
        "end_carry_flag_16_bit_add:"        NT          \
                                                        \
        : "=r" (result)                                 \
        : "r" (add_1), "r" (add_2)                      \
        :                                               \
);
#else
#error "The macro \"CARRY_CHECK_16_BIT_ADD\" is already defined !"
#endif /* CARRY_CHECK_16_BIT_ADD */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Mittels einer addl (32-Bit Addition) feststellen, ob die Summe beider Summanden in 32-Bit passen.
 *
 * https://de.wikipedia.org/wiki/Statusregister
 * "Ist dieses Flag gesetzt, dann trat bei der letzten Rechenoperation ein Übertrag auf, wenn man von vorzeichenlosen
 * Operanden ausgeht."
 */
#ifndef CARRY_CHECK_32_BIT_ADD
#define CARRY_CHECK_32_BIT_ADD(add_1, add_2, result)    \
__asm__ volatile                                        \
(                                                       \
        "addl %1, %2"                       NT          \
        "jc carry_flag_32_bit_add"          NT          \
        "jmp no_carry_flag_32_bit_add"      NT          \
        "carry_flag_32_bit_add:"            NT          \
        "movl $1, %0"                       NT          \
        "jmp end_carry_flag_32_bit_add"     NT          \
        "no_carry_flag_32_bit_add:"         NT          \
        "movl $0, %0"                       NT          \
        "end_carry_flag_32_bit_add:"        NT          \
                                                        \
        : "=r" (result)                                 \
        : "r" (add_1), "r" (add_2)                      \
        :                                               \
);
#else
#error "The macro \"CARRY_CHECK_32_BIT_ADD\" is already defined !"
#endif /* CARRY_CHECK_32_BIT_ADD */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Mittels einer addq (64-Bit Addition) feststellen, ob die Summe beider Summanden in 64-Bit passen.
 *
 * https://de.wikipedia.org/wiki/Statusregister
 * "Ist dieses Flag gesetzt, dann trat bei der letzten Rechenoperation ein Übertrag auf, wenn man von vorzeichenlosen
 * Operanden ausgeht."
 */
#ifdef USE_X86_64
#ifndef CARRY_CHECK_64_BIT_ADD
#define CARRY_CHECK_64_BIT_ADD(add_1, add_2, result)    \
__asm__ volatile                                        \
(                                                       \
        "addq %1, %2"                       NT          \
        "jc carry_flag_64_bit_add"          NT          \
        "jmp no_carry_flag_64_bit_add"      NT          \
        "carry_flag_64_bit_add:"            NT          \
        "movq $1, %0"                       NT          \
        "jmp end_carry_flag_64_bit_add"     NT          \
        "no_carry_flag_64_bit_add:"         NT          \
        "movq $0, %0"                       NT          \
        "end_carry_flag_64_bit_add:"        NT          \
                                                        \
        : "=r" (result)                                 \
        : "r" (add_1), "r" (add_2)                      \
        :                                               \
);
#else
#error "The marco \"CARRY_CHECK_64_BIT_ADD\" is already defined !"
#endif /* CARRY_CHECK_64_BIT_ADD */
#endif /* USE_X86_64 */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Mittels einer addb (8-Bit Addition) feststellen, ob die Summe zweier vorzeichenbehafteter Zahlen in der Zweier-
 * komplementdarstellung in 8-Bit passen.
 *
 * https://de.wikipedia.org/wiki/Statusregister
 * "Ist das Overflow-Flag gesetzt, dann trat bei der letzten Rechenoperation ein Überlauf auf, wenn man von
 * vorzeichenbehafteten Operanden ausgeht."
 */
#ifndef OVERFLOW_CHECK_8_BIT_ADD
#define OVERFLOW_CHECK_8_BIT_ADD(add_1, add_2, result)  \
__asm__ volatile                                        \
(                                                       \
    "addb %1, %2"                       NT              \
    "jo overflow_flag_8_bit_add"        NT              \
    "jmp no_overflow_flag_8_bit_add"    NT              \
    "overflow_flag_8_bit_add:"          NT              \
    "movb $1, %0"                       NT              \
    "jmp end_overflow_flag_8_bit_add"   NT              \
    "no_overflow_flag_8_bit_add:"       NT              \
    "movb $0, %0"                       NT              \
    "end_overflow_flag_8_bit_add:"      NT              \
                                                        \
    : "=r" (result)                                     \
    : "r" (add_1), "r" (add_2)                          \
    :                                                   \
);
#else
#error "The macro \"OVERFLOW_CHECK_8_BIT_ADD\" is already defined !"
#endif /* OVERFLOW_CHECK_8_BIT_ADD */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Mittels einer addw (16-Bit Addition) feststellen, ob die Summe zweier vorzeichenbehafteter Zahlen in der Zweier-
 * komplementdarstellung in 16-Bit passen.
 *
 * https://de.wikipedia.org/wiki/Statusregister
 * "Ist das Overflow-Flag gesetzt, dann trat bei der letzten Rechenoperation ein Überlauf auf, wenn man von
 * vorzeichenbehafteten Operanden ausgeht."
 */
#ifndef OVERFLOW_CHECK_16_BIT_ADD
#define OVERFLOW_CHECK_16_BIT_ADD(add_1, add_2, result) \
__asm__ volatile                                        \
(                                                       \
    "addw %1, %2"                       NT              \
    "jo overflow_flag_16_bit_add"       NT              \
    "jmp no_overflow_flag_16_bit_add"   NT              \
    "overflow_flag_16_bit_add:"         NT              \
    "movw $1, %0"                       NT              \
    "jmp end_overflow_flag_16_bit_add"  NT              \
    "no_overflow_flag_16_bit_add:"      NT              \
    "movw $0, %0"                       NT              \
    "end_overflow_flag_16_bit_add:"     NT              \
                                                        \
    : "=r" (result)                                     \
    : "r" (add_1), "r" (add_2)                          \
    :                                                   \
);
#else
#error "The macro \"OVERFLOW_CHECK_16_BIT_ADD\" is already defined !"
#endif /* OVERFLOW_CHECK_16_BIT_ADD */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Mittels einer addl (32-Bit Addition) feststellen, ob die Summe zweier vorzeichenbehafteter Zahlen in der Zweier-
 * komplementdarstellung in 32-Bit passen.
 *
 * https://de.wikipedia.org/wiki/Statusregister
 * "Ist das Overflow-Flag gesetzt, dann trat bei der letzten Rechenoperation ein Überlauf auf, wenn man von
 * vorzeichenbehafteten Operanden ausgeht."
 */
#ifndef OVERFLOW_CHECK_32_BIT_ADD
#define OVERFLOW_CHECK_32_BIT_ADD(add_1, add_2, result) \
__asm__ volatile                                        \
(                                                       \
    "addl %1, %2"                       NT              \
    "jo overflow_flag_32_bit_add"       NT              \
    "jmp no_overflow_flag_32_bit_add"   NT              \
    "overflow_flag_32_bit_add:"         NT              \
    "movl $1, %0"                       NT              \
    "jmp end_overflow_flag_32_bit_add"  NT              \
    "no_overflow_flag_32_bit_add:"      NT              \
    "movl $0, %0"                       NT              \
    "end_overflow_flag_32_bit_add:"     NT              \
                                                        \
    : "=r" (result)                                     \
    : "r" (add_1), "r" (add_2)                          \
    :                                                   \
);
#else
#error "The macro \"OVERFLOW_CHECK_32_BIT_ADD\" is already defined !"
#endif /* OVERFLOW_CHECK_32_BIT_ADD */

//---------------------------------------------------------------------------------------------------------------------

/**
 * @brief Mittels einer addq (64-Bit Addition) feststellen, ob die Summe zweier vorzeichenbehafteter Zahlen in der Zweier-
 * komplementdarstellung in 64-Bit passen.
 *
 * https://de.wikipedia.org/wiki/Statusregister
 * "Ist das Overflow-Flag gesetzt, dann trat bei der letzten Rechenoperation ein Überlauf auf, wenn man von
 * vorzeichenbehafteten Operanden ausgeht."
 */
#ifdef USE_X86_64
#ifndef OVERFLOW_CHECK_64_BIT_ADD
#define OVERFLOW_CHECK_64_BIT_ADD(add_1, add_2, result) \
__asm__ volatile                                        \
(                                                       \
    "addq %1, %2"                       NT              \
    "jo overflow_flag_64_bit_add"       NT              \
    "jmp no_overflow_flag_64_bit_add"   NT              \
    "overflow_flag_64_bit_add:"         NT              \
    "movq $1, %0"                       NT              \
    "jmp end_overflow_flag_64_bit_add"  NT              \
    "no_overflow_flag_64_bit_add:"      NT              \
    "movq $0, %0"                       NT              \
    "end_overflow_flag_64_bit_add:"     NT              \
                                                        \
    : "=r" (result)                                     \
    : "r" (add_1), "r" (add_2)                          \
    :                                                   \
);
#else
#error "The macro \"OVERFLOW_CHECK_64_BIT_ADD\" is already defined !"
#endif /* OVERFLOW_CHECK_64_BIT_ADD */
#endif /* USE_X86_64 */

//---------------------------------------------------------------------------------------------------------------------



// ENDE C++-Kompablitaet herstellen
#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* INLINE_ASM_H */
