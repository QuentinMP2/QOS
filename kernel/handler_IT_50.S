.text
# The function is accessible
    .globl handler_IT_50
# Beginning of the handler
handler_IT_50:
    # Save of registers
    pushl %eax
    pushl %edx
    pushl %ecx
    # Call to the handler written in C
    call handler_50_C
    # Restauration of saved registers
    popl %ecx
    popl %edx
    popl %eax
    # Interruption return
    iret