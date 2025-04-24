#include <stdio.h>
#include <inttypes.h>
#include <n7OS/cpu.h>
#include <n7OS/time.h>
#include <n7OS/console.h>

void init_timer() {
    /* Set up the timer. */
    outb(0x34, 0x43);

    /* Set up frequency: low bits. */
    outb(FREQUENCY & 0xFF, 0x40);

    /* Set up frequency: high bits. */
    outb(FREQUENCY >> 8, 0x40);

    /* Start timer. */
    outb(inb(0x21) & 0xFE, 0x21);
}

