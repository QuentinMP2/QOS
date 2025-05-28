.text
# The function is accessible
    .globl handler_IT_33
# Beginning of the handler
handler_IT_33:
    # Save of registers
    pushl %eax
    pushl %edx
    pushl %ecx
    # Call to the handler written in C
    call handler_33_C
    # Restauration of saved registers
    popl %ecx
    popl %edx
    popl %eax
    # Interruption return
    iret