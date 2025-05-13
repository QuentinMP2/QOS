#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <inttypes.h>

#define NB_PROC 256 // Maximum number of processes
#define BITMAP_SIZE (256/32)
#define STACK_SIZE 1024

typedef uint32_t pid_t;

/** Differents states of a process. */
typedef enum {
    ELU,
    PRET,
    BLOQUE
} PROCESS_STATE;

/** Definition of a process. */
typedef struct {
    char *name;
    pid_t pid;
    void *stack;
    PROCESS_STATE state;
    uint32_t regs[5];
} process_t;

#endif