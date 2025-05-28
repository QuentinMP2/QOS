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

/**
 * @brief Check if the given position in the process_table is a process.
 * 
 * @param position the position of the process.
 * 
 * @return true if it is a process, false otherwise
 */
uint8_t is_process(uint32_t position);

/**
 * @brief Get a processus from it's position in the process table.
 * 
 * @param position the position of the process
 * 
 * @return the associated process
 */
process_t* get_process(uint32_t position);

/** 
 * @brief Find a process. 
 * 
 * @param pid the pid of the process
 * 
 * @return the position of the process in the process_table
 */
uint32_t find_process(pid_t pid);

/**
 * @brief Block a process.
 * 
 * @param pid the pid of the process
 */
void block_process(pid_t pid);

/**
 * @brief Unblock a process.
 * 
 * @param pid the pid of the process
 */
void unblock_process(pid_t pid);

/**
 * @brief Kill a process.
 * 
 * @param pid the pid of the process
 */
void kill_process(pid_t pid);

#endif