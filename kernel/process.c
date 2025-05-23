#include <n7OS/process.h>
#include <n7OS/cpu.h>
#include <malloc.h>
#include <debug.h>

extern int ctx_sw();
extern int processus1();

/** Table of processes. */
process_t process_table[NB_PROC];

/**
 * @brief Bitmap that reference free processes.
 * Each case of the table represents 32 processes.
 * 0: free process
 */
uint32_t free_proc_bitmap_table[PROC_BITMAP_SIZE];

/** Current state of the timer compared to the quantum. */
uint8_t current_quantum;

/** Last PID used. */
pid_t current_pid;

/** Current active process. */
pid_t current_active_process;

void update_quantum() {
    current_quantum++;
    current_quantum = current_quantum % QUANTUM;

    if (current_quantum == 0) {
        schedule();
    }
}

pid_t get_next_pid() {
    return current_pid++ % NB_PROC;
}


/**
 * @brief Check if the given position in the process_table is a process.
 * 
 * @param position the position of the process.
 * 
 * @return true if it is a process, false otherwise
 */
uint8_t is_process(uint32_t position) {
    uint32_t bitmap_index = position / 32;
    uint8_t bitmap_offset = position % 32;
    return (free_proc_bitmap_table[bitmap_index] & (0b1 << bitmap_offset)) == (uint32_t)(0b1 << bitmap_offset);
}

/**
 * @brief Get the current active process.
 * 
 * @return the pid of the current active process.
 */
pid_t get_active_process() {
    return current_active_process;
}

/**
 * @brief Get the next process to run.
 * 
 * @return the position of the next process in the process_table.
 */
uint32_t get_next_process() {
    uint32_t index = 0;
    uint8_t found = 0;
    while (index < NB_PROC && found != 1) {
        if (is_process(index) && process_table[index].state == READY) {
            found = 1;
        }

        index++;
    }

    return index - 1;
}

/**
 * @brief Set a process in the bitmap.
 * 
 * @param position the position of the process.
 */
void set_process(uint32_t position) {
    uint32_t bitmap_index = position / 32;
    uint8_t bitmap_offset = position % 32;
    free_proc_bitmap_table[bitmap_index] |= (0b1 << bitmap_offset);
}

/**
 * @brief Clear a process in the bitmap.
 * 
 * @param position the position of the process.
 */
void clear_process(uint32_t position) {
    uint32_t bitmap_index = position / 32;
    uint8_t bitmap_offset = position % 32;
    free_proc_bitmap_table[bitmap_index] &= (0b0 << bitmap_offset);
}

/** 
 * @brief Find a process. 
 * 
 * @param pid the pid of the process
 * 
 * @return the position of the process in the process_table
 */
uint32_t find_process(pid_t pid) {
    uint32_t index = 0;
    uint8_t found = 0;
    while (index < NB_PROC && found != 1) {
        if (is_process(index) && process_table[index].pid == pid) {
            found = 1;
        }

        index++;
    }

    return index - 1;
}

/**
 * @brief Find a free process in the bitmap and set it.
 * 
 * @return the position of the set process.
 */
uint32_t find_free_proc() {
    uint32_t index = 0;
    uint8_t found_proc = 0;
    uint8_t current_bit = 0;

    while (found_proc != 1) {
        if (free_proc_bitmap_table[index] < 0xffffffff) {
            // There is at least one bit equals to 0
            current_bit = 0;
            while (found_proc != 1) {
                // We want 0 so (!) we obtain the bit with (& 0b1)
                if (!((free_proc_bitmap_table[index] >> current_bit) & 0b1)) {
                    // The current bit is equals to 1
                    found_proc = 1;                    
                }
                current_bit++;
            }
        }
        index++;
    }

    if (found_proc == 0) {
        return 0; // Change this by an interruption later
    } 

    uint32_t bitmap_position = (index - 1) * 32 + (current_bit - 1);

    // Allocate the process
    set_process(bitmap_position);

    return bitmap_position;
}


void create_process(char *name, void* func) {
    uint32_t new_proc_position = find_free_proc();
    process_table[new_proc_position].name = name;
    process_table[new_proc_position].pid = get_next_pid();
    process_table[new_proc_position].stack = (uint32_t*)malloc(STACK_SIZE * sizeof(process_t));
    process_table[new_proc_position].stack[STACK_SIZE - 1] = (uint32_t)func;
    process_table[new_proc_position].state = READY;
    for (int i = 0; i < 5; i++) {
        process_table[new_proc_position].regs[i] = 0;
    }
    process_table[new_proc_position].regs[1] = (uint32_t)&process_table[new_proc_position].stack[STACK_SIZE - 1];
}

void delete_process(pid_t pid) {
    uint32_t proc_position = find_process(pid);

    clear_process(proc_position);
    free(process_table[proc_position].stack);
}

void block_process(pid_t pid) {
    uint32_t proc_position = find_process(pid);

    process_table[proc_position].state = BLOCKED;
}

void idle() {
    // Create the first process
    create_process("proc1", processus1);

    while(1) {
        for (int i = 0; i < 100000000; i++)
        {
            /* do nothing */
        }
        
        printf("Hello world from Idle\n");
        hlt();
    }
}

void schedule() {
    pid_t active_process_pid;
    uint32_t elected_process_position, active_process_position;
    process_t *active_process, *elected_process;

    active_process_pid = get_active_process();
    active_process_position = find_process(active_process_pid);
    elected_process_position = get_next_process();
    active_process = &process_table[active_process_position];
    elected_process = &process_table[elected_process_position];

    // Set the active process to the READY state
    active_process->state = READY;

    // Set the elected process to the ELECTED state
    elected_process->state = ELECTED;
    current_active_process = process_table[elected_process_position].pid;

    // Switch context
    ctx_sw(active_process->regs, elected_process->regs);
}

void init_proc() {
    for (int i = 0; i < PROC_BITMAP_SIZE; i++) {
        free_proc_bitmap_table[i] = 0;
    }

    current_pid = 0;
    current_quantum = 0;
    current_active_process = 0;

    // Create the idle process
    create_process("idle", idle);
    process_table[current_active_process].state = ELECTED;
    idle();
}
