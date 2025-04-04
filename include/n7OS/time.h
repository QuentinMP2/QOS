#ifndef __TIME_H__
#define __TIME_H__

#define F_OSC 1190000 /* in Hz */
#define CLOCK 1000    /* in Hz */
#define FREQUENCY (F_OSC / CLOCK)

/**
 * @brief Initialise the timer with 1kHz (1ms).
 */
void init_timer();

void print_timer(uint32_t timer);

#endif