#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <stdio.h>

extern void handler_IT();
extern void handler_IT_50();

void init_irq() {
    init_irq_entry(50, (uint32_t)handler_IT_50);
}

void handler_C() {

}

void handler_50_C() {
    printf("Incredible! I've catched the interruption number 50!");
}
