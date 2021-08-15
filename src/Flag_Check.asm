# Fuer GAS im x86 Modus
# Mittels dieses ASM-Programms wird das Grundgeruest fuer Inline-Assembler Anweisungen entwickelt,
# mit denen man die wichtigsten Flags auslesen und im C-Programm verfuegbar machen kann
# Die 64-Bit Variante wird vielleicht irgendwann nachgeruestet

.text
.global main
main:
    # !!! Ist nur fuer die Entwicklung mittels SASM notwendig ! Als Inline Assembler darf dieser Befehl nicht verwendet
    # werden !!!
    # movl %esp, %ebp # for correct debugging

    # Eingabeparamer
    # ebx:  1. Summand
    # ecx:  2. Summand
    # edx:  Art des Flags, welches abgefragt werden soll
    #       0: Carry-Flag 8-Bit
    #       1: Carry-Flag 16-Bit
    #       2: Carry-Flag 32-Bit
    #       3: Carry-Flag 64-Bit (!!! NICHT VERFUEGBAR !!!)
    #       4: Overflow-Flag 8-Bit
    #       5: Overflow-Flag 16-Bit
    #       6: Overflow-Flag 32-Bit
    #       7: Overflow-Flag 64-Bit (!!! NICHT VERFUEGBAR !!!)
    # esi:  Art der Operation (KEINE Division, da eine Division nie ein CF oder OF setzt !)
    #       0: Summe
    #       1: Subtraktion
    #       2: Multiplikation
    # eax:  Ergebnis
    #       0, falls nicht gesetzt
    #       1, falls gesetzt
    #       sonst, Falsche Eingabeparameter

    # Standardmaessig die Ausgabe des Flags auf 0 setzen
    xorl %eax, %eax
    xorl %ebx, %ebx
    xorl %ecx, %ecx

    movw $32000, %bx
    movw $5, %cx

    movl $5, %edx
    movl $2, %esi

    # edx ueberpruefen
    cmpl $0, %edx
    jl edx_too_small
    cmpl $7, %edx
    jg edx_too_large

    # esi ueberpruefen
    cmpl $0, %esi
    jl esi_too_small
    cmpl $2, %esi
    jg esi_too_large

    # Jetzt muessen alle gueltigen Moeglichkeiten aus edx und esi gebildet werden
    # Welche Rechenform ?
    cmpl $0, %esi
    je sum
    cmpl $1, %esi
    je subtract
    cmpl $2, %esi
    je multi

    sum:
        cmpl $0, %edx
        je sum_carry_8
        cmpl $1, %edx
        je sum_carry_16
        cmpl $2, %edx
        je sum_carry_32
        cmpl $4, %edx
        je sum_of_8
        cmpl $5, %edx
        je sum_of_16
        cmpl $6, %edx
        je sum_of_32
        jmp unsupported_type

            sum_carry_8:
            addb %bl, %cl
            jc flag_set
            jmp end
            sum_carry_16:
            addw %bx, %cx
            jc flag_set
            jmp end
            sum_carry_32:
            addl %ebx, %ecx
            jc flag_set
            jmp end
            sum_of_8:
            addb %bl, %cl
            jo flag_set
            jmp end
            sum_of_16:
            addw %bx, %cx
            jo flag_set
            jmp end
            sum_of_32:
            addl %ebx, %ecx
            jo flag_set
            jmp end

    subtract:
        cmpl $0, %edx
        je sub_carry_8
        cmpl $1, %edx
        je sub_carry_16
        cmpl $2, %edx
        je sub_carry_32
        cmpl $4, %edx
        je sub_of_8
        cmpl $5, %edx
        je sub_of_16
        cmpl $6, %edx
        je sub_of_32
        jmp unsupported_type

            sub_carry_8:
            subb %bl, %cl
            jc flag_set
            jmp end
            sub_carry_16:
            subw %bx, %cx
            jc flag_set
            jmp end
            sub_carry_32:
            subl %ebx, %ecx
            jc flag_set
            jmp end
            sub_of_8:
            subb %bl, %cl
            jo flag_set
            jmp end
            sub_of_16:
            subw %bx, %cx
            jo flag_set
            jmp end
            sub_of_32:
            subl %ebx, %ecx
            jo flag_set
            jmp end

    multi:
    # 1. Operand in eax kopieren, da mul / imul eax implizit als 1. Operanden verwenden
    movl %ebx, %eax
    cmpl $0, %edx
    je multi_carry_8
    cmpl $1, %edx
    je multi_carry_16
    cmpl $2, %edx
    je multi_carry_32
    cmpl $4, %edx
    je multi_of_8
    cmpl $5, %edx
    je multi_of_16
    cmpl $6, %edx
    je multi_of_32
    jmp unsupported_type

            # mul fuer vorzeichenlose Multiplikation
            multi_carry_8:
            mulb %cl
            jc flag_set
            jmp end
            multi_carry_16:
            mulw %cx
            jc flag_set
            jmp end
            multi_carry_32:
            mull %ecx
            jc flag_set
            jmp end
            # imul fuer vorzeichenbehaftete Multiplikation
            multi_of_8:
            imul %cl
            jo flag_set
            jmp end
            multi_of_16:
            imul %cx
            jo flag_set
            jmp end
            multi_of_32:
            imul %ecx
            jo flag_set
            jmp end

    edx_too_small:
    edx_too_large:
    esi_too_small:
    esi_too_large:
    unsupported_type:
    movl $-1, %eax
    jmp end

    flag_set:
    movl $1, %eax

    end:
    ret
