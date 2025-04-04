.text
# The function is accessible
    .globl handler_IT_32
# Beginning of the handler
handler_IT_32:
    # Save of registers
    pushl %eax
    pushl %edx
    pushl %ecx
    # Call to the handler written in C
    call handler_32_C
    # Restauration of saved registers
    popl %ecx
    popl %edx
    popl %eax
    # Interruption return
    iret