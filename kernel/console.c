#include <stdio.h>
#include <n7OS/console.h>
#include <n7OS/cpu.h>

/** First position of the screen. */
uint16_t *scr_tab;

/** Index of the current position on the screen. */
int index;

/** Initial value of index. */
const uint16_t index_init = VGA_WIDTH;

uint8_t weekday = 0;
uint8_t day_of_month = 0;
uint8_t month = 0;

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

void print_taskbar_time(uint32_t timer) {
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

/**
 * Get the abbreviation of the given weekday.
 * @param weekday the weekday
 */
char* get_weekday_abbr(uint8_t weekday) {
    char *weekday_disp = "";
    switch (weekday) {
        case 1:
            weekday_disp = "sun.";
            break;

        case 2:
            weekday_disp = "mon.";
            break;
        
        case 3:
            weekday_disp = "tue.";
            break;

        case 4:
            weekday_disp = "wed.";
            break;

        case 5:
            weekday_disp = "thu.";
            break;

        case 6:
            weekday_disp = "fri.";
            break;

        case 7:
            weekday_disp = "sat.";
            break;

        default:
            break;
    }

    return weekday_disp;
}

/**
 * Get the abbreviation of the given month.
 * @param month the month
 */
char* get_month_abbr(uint8_t month) {
    char *month_disp = "";
    switch (month) {
        case 1:
            month_disp = "Jan.";
            break;

        case 2:
            month_disp = "Feb.";
            break;
        
        case 3:
            month_disp = "Mar.";
            break;

        case 4:
            month_disp = "Apr.";
            break;

        case 5:
            month_disp = "May ";
            break;

        case 6:
            month_disp = "Jun.";
            break;

        case 7:
            month_disp = "Jul.";
            break;

        case 8:
            month_disp = "Aug.";
            break;

        case 9:
            month_disp = "Sep.";
            break;

        case 10:
            month_disp = "Oct.";
            break;

        case 11:
            month_disp = "Nov.";
            break;

        case 12:
            month_disp = "Dec.";
            break;

        default:
            break;
    }

    return month_disp;
}

void print_taskbar_date() {
    int index_date = 34;
    
    char *weekday_disp = get_weekday_abbr(weekday);
    for (int i = 0; i < 5; i++) {
        scr_tab[index_date] = TASKBAR_CHAR_COLOR<<8|(weekday_disp[i]);
        index_date++;
    }

    char day_of_month_disp[2];
    sprintf(day_of_month_disp, "%02d", day_of_month);
    for (int i = 0; i < 3; i++) {
        scr_tab[index_date] = TASKBAR_CHAR_COLOR<<8|(day_of_month_disp[i]);
        index_date++;
    }

    char *month_disp = get_month_abbr(month);
    for (int i = 0; i < 5; i++) {
        scr_tab[index_date] = TASKBAR_CHAR_COLOR<<8|(month_disp[i]);
        index_date++;
    }
}
