#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <n7OS/time.h>
#include <n7OS/console.h>
#include <n7OS/process.h>
#include <stdio.h>

extern void handler_IT();
extern void handler_IT_32();
extern void handler_IT_50();

uint32_t timer = 0;

void init_irq() {
    init_irq_entry(32, (uint32_t)handler_IT_32);
    init_irq_entry(50, (uint32_t)handler_IT_50);
}

void handler_C() {

}

void handler_32_C() {
    /* Acquitment of the timer interruption. */
    outb(0x20, 0x20);

    timer++;
    print_taskbar_time(timer);

    sti();
    if (timer % 100 == 0) {
        update_quantum();
    }
}

void handler_50_C() {
    printf("Incredible! I've catched the interruption number 50!\n");
}
