#include <n7OS/keyboard.h>
#include <n7OS/cpu.h>

/* Store chars before writing it. */
char buffer_keyboard[BUFFER_SIZE];
/* Implementation of a circulate buffer so
 * we need the index of the start and the 
 * end of the buffer.
 */
uint16_t buff_start;
uint16_t buff_end;

void init_keyboard() {
    /* Init the keyboard interruption */
    outb(inb(0x21) & (~0x2), 0x21);

    /* Reset the buffer. */
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer_keyboard[i] = 0;
    }

    buff_start = 0;
    buff_end = 0;
}

char kgetch() {
    char c = 0;

    if (buff_start != buff_end) {
        /* The buffer isn't empty. */
        c = buffer_keyboard[buff_start];
        buff_start = (buff_start + 1) % BUFFER_SIZE;
    }

    return c;
}