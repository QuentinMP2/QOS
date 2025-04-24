#include <stdio.h>
#include <n7OS/console.h>
#include <n7OS/cpu.h>

/** First position of the screen. */
uint16_t *scr_tab;

/** Index of the current position on the screen. */
int index;

/** Initial value of index. */
const uint16_t index_init = VGA_WIDTH;

/** Initialise the taskbar. */
void init_taskbar() {
    // Fill the lign with blanks.
    for (int i = 0; i < VGA_WIDTH; i++)
        scr_tab[i] = TASKBAR_CHAR_COLOR<<8|32;

    // Display the name of the OS at the left.
    char *msg = "QOS";
    int i = 0;
    while (msg[i] != '\0') {
        scr_tab[i+1] = TASKBAR_CHAR_COLOR<<8|(msg[i]);
        i++;
    }
}

void init_console() {
    scr_tab = (uint16_t*) SCREEN_ADDR;
    index = index_init;
    init_taskbar();
}

/** 
 * Get the current column on the screen.
 * @return the number of the current column on the screen
 */
int get_column() {
    return index % VGA_WIDTH ;
}

/** 
 * Get the current line on the screen.
 * @return the number of the current line on the screen
 */
int get_line() {
    return index / VGA_WIDTH ;
}

/**
 * Go to the next position on the screen.
 */
void next_position() {
    index++;
}

/**
 * Go to the previous position on the screen.
 */
void previous_position() {
    index--;
}

/**
 * Go to the next line of the screen.
 */
void next_line() {
    index += VGA_WIDTH - get_column();
}

/**
 * Update the cursor position.
 */
void update_cursor() {
    uint16_t position = (uint16_t) index;
    
    /* Low weight */
    outb(CMD_LOW, PORT_CMD);
    outb((uint8_t) position, PORT_DATA);

    /* High weight */
    outb(CMD_HIGH, PORT_CMD);
    outb((uint8_t) (position >> 8), PORT_DATA);
}

/**
 * Print space at the given index.
 * @param index index where print a space
 */
void print_space(int index) {
    scr_tab[index]= CHAR_COLOR<<8|32;
}

/** 
 * Print a character on the screen.
 * @param c the character to print
 */
void console_putchar(const char c) {
    if (c >= 32 && c < 127) {
        scr_tab[index] = CHAR_COLOR<<8|c;
        next_position();
    } else if (c == '\b') {
        previous_position();
        print_space(index);
    } else if (c == '\t') {
        for (int i = 0; i < 8; i++) {
            print_space(index);
            next_position();
        }
    } else if (c == '\n') {
        next_line();
    } else if (c == '\f') {
        for (int i = index_init; i < VGA_HEIGHT * VGA_WIDTH; i++) {
            print_space(i);
        }
        index = index_init;
    } else if (c == '\r') {
        index -= get_column();
    }
}

void console_putbytes(const char *s, int len) {
    for (int i = 0; i < len; i++) {
        console_putchar(s[i]);
    }

    update_cursor();
}

void print_taskbar_hour(uint32_t timer) {
    timer /= 1000; // Convert from ms to s
    uint32_t seconds = timer % 60;
    uint32_t minutes = (timer % 3600) / 60;
    uint32_t hours = timer / 3600;
    char hour_buffer[8];
    sprintf(hour_buffer, "%02d:%02d:%02d", hours, minutes, seconds);

    int index_hour = VGA_WIDTH - 9;
    for (int i = 0; i < 8; i++)
        scr_tab[index_hour + i] = TASKBAR_CHAR_COLOR<<8|(hour_buffer[i]);
}
