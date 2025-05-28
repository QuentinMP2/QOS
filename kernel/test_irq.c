#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <n7OS/time.h>
#include <n7OS/console.h>
#include <n7OS/process.h>
#include <n7OS/keyboard.h>
#include <stdio.h>

extern void handler_IT();
extern void handler_IT_32();
extern void handler_IT_33();
extern void handler_IT_50();

extern char buffer_keyboard[BUFFER_SIZE];
extern uint16_t buff_start;
extern uint16_t buff_end;

/* Current value of the timer. */
uint32_t timer = 0;

/* 1 if the keyboard is in the shift state, 0 otherwise. */
uint8_t lshift = 0;
uint8_t rshift = 0;

void init_irq() {
    init_irq_entry(32, (uint32_t)handler_IT_32);
    init_irq_entry(33, (uint32_t)handler_IT_33);
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

void handler_33_C() {
    sti();
    // console_putbytes("\nTEST\n", 6);

    char c = 0;
    uint8_t code_key = inb(KEYB_ENC_CMD_REG);
    // printf("%d\n", code_key); // Print code_key for debug

    /* Keep the information of if a shift key (left or right) is pressed. */
    if (code_key == LSHIFT_PRESSED)
        lshift = 1;

    if (code_key == RSHIFT_PRESSED)
        rshift = 1;

    if (code_key == LSHIFT_RELEASED)
        lshift = 0;
        
    if (code_key == RSHIFT_RELEASED)
        rshift = 0;

    if (((code_key >> 7) & 0b1) == 0) {
        /* The key has been pressed. */

        if (lshift || rshift) {
            c = scancode_map_shift[code_key];
        } else {
            c = scancode_map[code_key];
        }
        buffer_keyboard[buff_end] = c;
        buff_end = (buff_end + 1) % BUFFER_SIZE;
    }

    /* Acquitment of the keyboard interruption. */
    outb(0x20, 0x20);
}

void handler_50_C() {
    printf("Incredible! I've catched the interruption number 50!\n");
}
