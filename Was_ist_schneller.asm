# Ein kleiner Geschwindigkeitsvergleich von einigen Anweisungen mit
# gleichen bzw. vergleichbaren Ergebnissen
# Sehr interessant wie die Ergebnisse sind :o

.section .data
Loop_Counter:       .int 2147483646     # INT32_MAX - 1
Time:               .int 0

bound_Loop_Counter: .int 2147483646 - 1 # INT32_MAX - 2 (bound ueberprueft die obere Grenze mit <= !)
bound_data:         .int 0, 2147483646



FString_Newline:        .string "\n\0"

FString_Loop_1:         .string "Schleife mit loop:             %d\n\0"
FString_Loop_2:         .string "Schleife mit jmp:              %d\n\0"

FString_inc:            .string "inc:                           %d\n\0"
FString_add:            .string "add:                           %d\n\0"

FString_xchg_1:         .string "xchg                           %d\n\0"
FString_xchg_2:         .string "mov, mov, mov                  %d\n\0"
 
FString_xadd_1:         .string "xadd                           %d\n\0"
FString_xadd_2:         .string "mov, mov, mov, add             %d\n\0"

FString_bound_1:        .string "bound:                         %d\n\0"
FString_bound_2:        .string "ge und le:                     %d\n\0"

FString_enter_leave_1:  .string "enter leave:                   %d\n\0"
FString_enter_leave_2:  .string "push, mov, sub - mov, pop:     %d\n\0"

FString_enter_1:        .string "enter:                         %d\n\0"
FString_enter_2:        .string "push, mov, sub:                %d\n\0"

FString_leave_1:        .string "leave:                         %d\n\0"
FString_leave_2:        .string "mov, pop:                      %d\n\0"

FString_xor:            .string "xor zero:                      %d\n\0"
FString_mov:            .string "mov zero:                      %d\n\0"

FString_shift_l:        .string "shl 1:                         %d\n\0"
FString_mul_2:          .string "mul 2:                         %d\n\0"

FString_shift_r:        .string "shr 1 (with movl $2):          %d\n\0"
FString_div_2:          .string "div 2 (with movl $2):          %d\n\0"

FString_push:           .string "push all registers:            %d\n\0"
FString_pusha:          .string "pusha:                         %d\n\0"


.section .bss
.macro fflush_call
    pushl %ebp
    movl %esp, %ebp
    pushl $0 # Alle I/O Puffer leeren
    call fflush
    leave
.endm

.macro first_clock time_param
    pushl %ebp
    movl %esp, %ebp
    call clock
    leave
    movl %eax, (\time_param)
.endm

.macro second_clock time_param
    pushl %ebp
    movl %esp, %ebp
    call clock
    leave
    subl (\time_param), %eax
    movl %eax, (\time_param)
.endm

.macro print_d format_string, parameter
    pushl %ebp
    movl %esp, %ebp
    pushl (\parameter)
    pushl $\format_string
    call printf
    leave
    
    fflush_call
.endm

.macro newline
    pushl %ebp
    movl %esp, %ebp
    pushl $FString_Newline
    call printf
    leave
    
    fflush_call
.endm   
    

.section .text
.extern printf
.extern clock
.extern fflush

.global main
main:
    movl %esp, %ebp # for correct debugging
    
    xorl %eax, %eax
    xorl %ebx, %ebx
    xorl %ecx, %ecx
    xorl %edx, %edx
    xorl %esi, %esi
    xorl %edi, %edi
    xorl %ebp, %ebp
    
    # ------------------------------------------------------------------------------
    
    first_clock Time    
    movl (Loop_Counter), %ecx
    l:
        loop l
    second_clock Time
    print_d FString_Loop_1 Time
    
    first_clock Time
    movl (Loop_Counter), %ecx
    l_2:
        sub $1, %ecx
        jnz l_2
    second_clock Time
    print_d FString_Loop_2 Time
    
    # ------------------------------------------------------------------------------
    
    newline
    first_clock Time
    xorl %ecx, %ecx
    movl (Loop_Counter), %eax
    l_3:
        incl %ecx
        cmpl %eax, %ecx
        jle l_3
    second_clock Time
    print_d FString_inc Time
    
    first_clock Time
    xorl %ecx, %ecx
    movl (Loop_Counter), %eax
    l_4:
        addl $1, %ecx
        cmpl %eax, %ecx
        jle l_4
    second_clock Time
    print_d FString_add Time
    
    # ------------------------------------------------------------------------------
    
    newline
    first_clock Time
    movl $11111, %ebx
    movl $22222, %edx
    xorl %ecx, %ecx
    movl (Loop_Counter), %eax
    l_21:
        xchgl %ebx, %edx
        incl %ecx
        cmpl %eax, %ecx
        jle l_21
    second_clock Time
    print_d FString_xchg_1 Time
    
    first_clock Time
    movl $11111, %ebx
    movl $22222, %edx
    xorl %ecx, %ecx
    movl (Loop_Counter), %eax
    l_22:
        movl %ebx, %esi
        movl %edx, %ebx
        movl %esi, %edx
        incl %ecx
        cmpl %eax, %ecx
        jle l_22
    second_clock Time
    print_d FString_xchg_2 Time
    
    #-------------------------------------------------------------------------------
    
    newline
    first_clock Time
    movl $11111, %ebx
    movl $22222, %edx
    xorl %ecx, %ecx
    movl (Loop_Counter), %eax
    l_23:
        xaddl %ebx, %edx
        incl %ecx
        cmpl %eax, %ecx
        jle l_23
    second_clock Time
    print_d FString_xadd_1 Time
    
    first_clock Time
    movl $11111, %ebx
    movl $22222, %edx
    xorl %ecx, %ecx
    movl (Loop_Counter), %eax
    l_24:
        movl %ebx, %esi
        movl %edx, %ebx
        movl %esi, %edx
        addl %ebx, %edx
        incl %ecx
        cmpl %eax, %ecx
        jle l_24
    second_clock Time
    print_d FString_xadd_2 Time
        
    # ------------------------------------------------------------------------------
    
    newline
    first_clock Time
    xorl %ecx, %ecx
    movl (bound_Loop_Counter), %eax
    l_13:
        incl %ecx
        bound %ecx, (bound_data)
        cmpl %eax, %ecx
        jle l_13
    second_clock Time
    print_d FString_bound_1 Time
    
    first_clock Time
    xorl %ecx, %ecx
    movl $0, %edx
    movl (Loop_Counter), %eax
    l_14:
        incl %ecx
        cmpl %eax, %ecx
        jle l_14
        cmpl %eax, %ecx # Zweimal den Test nach der oberen Schranke durchfuehren, um den Schleifen-Overhead
        jle l_14        # in der bound-Testschleife ebenfalls in dieser Schleife zu haben
        cmpl %eax, %edx
        jge l_14
    second_clock Time
    print_d FString_bound_2 Time
    
    # ------------------------------------------------------------------------------
    
    newline
    first_clock Time
    xorl %ecx, %ecx
    mov (Loop_Counter), %eax
    l_15:
        enter $0, $0
        leave
        incl %ecx
        cmpl %eax, %ecx
        jle l_15
    second_clock Time
    print_d FString_enter_leave_1 Time
    
    first_clock Time
    xorl %ecx, %ecx
    mov (Loop_Counter), %eax
    l_16:
        pushl %ebp
        movl %esp, %ebp
        #subl $0, %esp
        # ...
        movl %ebp, %esp
        popl %ebp
        incl %ecx
        cmpl %eax, %ecx
        jle l_16
    second_clock Time
    print_d FString_enter_leave_2 Time
    
    # ------------------------------------------------------------------------------
    
    newline
    first_clock Time
    xorl %ecx, %ecx
    mov (Loop_Counter), %eax
    l_17:
        enter $0, $0
        leave
        incl %ecx
        cmpl %eax, %ecx
        jle l_17
    second_clock Time
    print_d FString_enter_1 Time
    
    first_clock Time
    xorl %ecx, %ecx
    mov (Loop_Counter), %eax
    l_18:
        pushl %ebp
        movl %esp, %ebp
        #subl $0, %esp
        # ...
        leave
        incl %ecx
        cmpl %eax, %ecx
        jle l_18
    second_clock Time
    print_d FString_enter_2 Time
    
    # ------------------------------------------------------------------------------
    
    newline
    first_clock Time
    xorl %ecx, %ecx
    mov (Loop_Counter), %eax
    l_19:
        pushl %ebp
        movl %esp, %ebp
        #subl $0, %esp
        # ...
        leave
        incl %ecx
        cmpl %eax, %ecx
        jle l_19
    second_clock Time
    print_d FString_leave_1 Time
    
    first_clock Time
    xorl %ecx, %ecx
    mov (Loop_Counter), %eax
    l_20:
        pushl %ebp
        movl %esp, %ebp
        #subl $0, %esp
        # ...
        movl %ebp, %esp
        popl %ebp
        incl %ecx
        cmpl %eax, %ecx
        jle l_20
    second_clock Time
    print_d FString_leave_2 Time
    
    # ------------------------------------------------------------------------------
    
    newline
    first_clock Time
    xorl %ecx, %ecx
    movl (Loop_Counter), %eax
    l_5:
        xorl %ebx, %ebx
        incl %ecx
        cmpl %eax, %ecx
        jle l_5
    second_clock Time
    print_d FString_xor Time
    
    first_clock Time
    xorl %ecx, %ecx
    movl (Loop_Counter), %eax
    l_6:
        movl $0, %ebx
        incl %ecx
        cmpl %eax, %ecx
        jle l_6
    second_clock Time
    print_d FString_mov Time
    
    # ------------------------------------------------------------------------------
    
    newline
    first_clock Time
    movl $1, %eax
    xorl %ecx, %ecx
    movl (Loop_Counter), %ebx
    l_7:
        shll $1, %eax
        incl %ecx
        cmpl %ebx, %ecx
        jle l_7
    second_clock Time
    print_d FString_shift_l Time
    
    first_clock Time
    movl $1, %eax
    movl $2, %esi
    xorl %ecx, %ecx
    movl (Loop_Counter), %ebx
    l_8:
        mull %esi # mul verwendet edx:eax als implizite Ergebnisregister, sodass man keine 
                  # der beiden fuer den Schleifenzaehler oder fuer die Schleifengrenze
                  # verwenden kann !
        incl %ecx
        cmpl %ebx, %ecx
        jle l_8
    second_clock Time
    print_d FString_mul_2 Time
    
    # ------------------------------------------------------------------------------
    
    newline
    first_clock Time
    movl $2, %eax
    xorl %ecx, %ecx
    xorl %edx, %edx
    movl (Loop_Counter), %ebx
    l_9:
        shrl $1, %eax
        movl $2, %eax   # Beim Divisionstest ist dies erforderlich, damit keine Nulldivision
                        # ausgefuehrt wird. Hiermit wird der zusaetzliche Aufwand auch in
                        # dieser Schleife durchgefuehrt, um die beiden Testwerte miteinander
                        # vergleichen zu koennen
        incl %ecx
        cmpl %ebx, %ecx
        jle l_9
    second_clock Time
    print_d FString_shift_r Time
    
    first_clock Time
    movl $2, %eax
    movl $2, %esi
    xorl %ecx, %ecx
    xorl %edx, %edx
    movl (Loop_Counter), %ebx
    l_10:
        divl %esi # div verwendet edx:eax als implizite Ergebnisregister, sodass man keine 
                  # der beiden fuer den Schleifenzaehler oder fuer die Schleifengrenze
                  # verwenden kann !
        movl $2, %eax   # Damit es keine Ausnahme wegen einer Nulldivison gibt, muss immer
                        # wieder %eax auf 2 gesetzt werden
        incl %ecx
        cmpl %ebx, %ecx
        jle l_10
    second_clock Time
    print_d FString_div_2 Time
    
    # ------------------------------------------------------------------------------
    
    newline
    first_clock Time
    xorl %ecx, %ecx
    movl (Loop_Counter), %eax
    l_11:
        pushl %eax
        pushl %ebx
        pushl %ecx
        pushl %edx
        pushl %esp
        pushl %ebp
        pushl %esi
        pushl %edi
        
        addl $32, %esp # Stack Pointer wieder zuruecksetzen
        incl %ecx
        cmpl %eax, %ecx
        jne l_11
    second_clock Time
    print_d FString_push Time
    
    first_clock Time
    xorl %ecx, %ecx
    movl (Loop_Counter), %eax
    l_12:
        pusha
        addl $32, %esp # Stack Pointer wieder zuruecksetzen
        incl %ecx
        cmpl %eax, %ecx
        jne l_12
    second_clock Time
    print_d FString_pusha Time
    
    # ------------------------------------------------------------------------------
    
    xorl %eax, %eax
    ret
    