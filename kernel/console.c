#include <n7OS/console.h>
#include <n7OS/cpu.h>

/** First position of the screen. */
uint16_t* scr_tab;

/** Index of the current position on the screen. */
int index;

/** Initialize the console. */
void init_console() {
    scr_tab = (uint16_t*) SCREEN_ADDR;
    index = 0;
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

// void update_cursor() {
//     uint8_t command_port = 0x3D4;
//     uint8_t value_port = 0x3D5;
//     uint16_t position = (uint16_t) index;
    
//     /* Low weight */
//     outb(0x0F, command_port);
//     outb((uint8_t) (position & 0xFF), value_port);

//     /* High weight */
//     outb(0x0E, command_port);
//     outb((uint8_t) ((position >> 8) & 0xFF), value_port);
// }

void update_cursor(int x, int y)
{
	uint16_t pos = y * VGA_WIDTH + x;

	outb(0x0F, 0x3D4);
	outb((uint8_t) (pos & 0xFF), 0x3D5);
	outb(0x0E, 0x3D4);
	outb((uint8_t) ((pos >> 8) & 0xFF), 0x3D5);
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
        scr_tab[index]= CHAR_COLOR<<8|c;
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
        for (int i = 0; i < VGA_HEIGHT * VGA_WIDTH; i++) {
            print_space(i);
        }
        index = 0;
    } else if (c == '\r') {
        index -= get_column();
    }
}

void console_putbytes(const char* s, int len) {
    for (int i = 0; i < len; i++) {
        console_putchar(s[i]);
    }

    update_cursor(get_line(), get_column());
}