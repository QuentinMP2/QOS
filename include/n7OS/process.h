#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <inttypes.h>

#define NB_PROC 256 // Maximum number of processes
#define PROC_BITMAP_SIZE (256/32)
#define STACK_SIZE 1024
#define QUANTUM 1

typedef uint32_t pid_t;

/** Differents states of a process. */
typedef enum {
    ELECTED,
    READY,
    BLOCKED
} PROCESS_STATE;

/** Definition of a process. */
typedef struct {
    char *name;
    pid_t pid;
    uint32_t *stack;
    PROCESS_STATE state;
    uint32_t regs[5];
} process_t;

/**
 * @brief Update the current state of the quantum.
 */
void update_quantum();

/**
 * @brief Initialise processes.
 */
void init_proc();

/**
 * @brief Schedule processes.
 */
void schedule();

#endif