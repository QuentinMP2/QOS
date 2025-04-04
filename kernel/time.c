#include <inttypes.h>
#include <n7OS/cpu.h>
#include <n7OS/time.h>
#include <stdio.h>

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

void print_timer(uint32_t timer) {
    timer /= 1000; // Convert from ms to s
    
    printf("\r");
    if (timer % 3600 < 10) {
        printf("0%d:", timer / 3600);
    } else {
        printf("%d:", timer / 3600);
    }

    if ((timer % 3600) / 60 < 10) {
        printf("0%d:", (timer % 3600) / 60);
    } else {
        printf("%d:", (timer % 3600) / 60);
    }

    if (timer % 60 < 10) {
        printf("0%d", timer % 60);
    } else {
        printf("%d", timer % 60);
    }
}