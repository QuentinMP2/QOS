#include <inttypes.h>
#include <n7OS/cpu.h>
#include <n7OS/time.h>
#include <n7OS/console.h>

#include <stdio.h>

extern uint32_t timer;
extern uint8_t weekday;
extern uint8_t day_of_month;
extern uint8_t month;

void init_timer() {
    /* Get CMOS seconds. */
    outb(0x00, CMOS_WR_PORT);
    uint8_t seconds = inb(CMOS_RD_PORT);

    /* Get CMOS minutes. */
    outb(0x02, CMOS_WR_PORT);
    uint8_t minutes = inb(CMOS_RD_PORT);

    /* Get CMOS hours. */
    outb(0x04, CMOS_WR_PORT);
    uint8_t hours = inb(CMOS_RD_PORT);

    /* RTC format is BCD mode so need to convert into Binary mode. 
     * See: https://wiki.osdev.org/CMOS#Format_of_Bytes 
     */
    seconds = ((seconds & 0xF0) >> 1) + ((seconds & 0xF0) >> 3) + (seconds & 0xF);
    minutes = ((minutes & 0xF0) >> 1) + ((minutes & 0xF0) >> 3) + (minutes & 0xF);
    hours = ((hours & 0xF0) >> 1) + ((hours & 0xF0) >> 3) + (hours & 0xF);

    /* Set the local timer at the CMOS time. */
    timer = 1000 * (seconds + (minutes * 60) + (hours * 3600));

    /* Set up the timer. */
    outb(0x34, 0x43);

    /* Set up frequency: low bits. */
    outb(FREQUENCY & 0xFF, 0x40);

    /* Set up frequency: high bits. */
    outb(FREQUENCY >> 8, 0x40);

    /* Start timer. */
    outb(inb(0x21) & 0xFE, 0x21);

    /* Get CMOS weekday. */
    outb(0x06, CMOS_WR_PORT);
    weekday = inb(CMOS_RD_PORT);

    /* Get CMOS day of month. */
    outb(0x07, CMOS_WR_PORT);
    day_of_month = inb(CMOS_RD_PORT);

    /* Get CMOS month. */
    outb(0x08, CMOS_WR_PORT);
    month = inb(CMOS_RD_PORT);

    weekday = ((weekday & 0xF0) >> 1) + ((weekday & 0xF0) >> 3) + (weekday & 0xF);
    day_of_month = ((day_of_month & 0xF0) >> 1) + ((day_of_month & 0xF0) >> 3) + (day_of_month & 0xF);
    month = ((month & 0xF0) >> 1) + ((month & 0xF0) >> 3) + (month & 0xF);

    print_taskbar_date();
}

