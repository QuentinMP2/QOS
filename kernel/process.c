#include <process.h>

/** Table of processes. */
process_t process_table[NB_PROC];

/**
 * @brief Bitmap that reference free processes.
 * Each case of the table represents 32 processes.
 * 0: free processus
 */
uint32_t free_proc_bitmap_table[BITMAP_SIZE];

pid_t current_pid = 1;

pid_t get_next_pid() {
    return current_pid++;
}

void init_proc() {
    for (int i = 0; i < BITMAP_SIZE; i++) {
        free_proc_bitmap_table[i] = 0;
    }
}

void set_processus(uint32_t position) {
    uint32_t bitmap_index = position / 32;
    uint8_t bitmap_offset = position % 32;
    free_proc_bitmap_table[bitmap_index] |= (0b1 << bitmap_offset);
}

void clear_processus(uint32_t position) {
    uint32_t bitmap_index = position / 32;
    uint8_t bitmap_offset = position % 32;
    free_proc_bitmap_table[bitmap_index] &= (0b0 << bitmap_offset);
}

uint8_t is_processus(uint32_t position) {
    uint32_t bitmap_index = position / 32;
    uint8_t bitmap_offset = position % 32;
    return free_proc_bitmap_table[bitmap_index] == (0b0 << bitmap_offset);
}


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

    // Allocate the processus
    set_processus(bitmap_position);

    return bitmap_position;
}

void create_processus(char *name) {
    void *stack = malloc(sizeof(process_t));
    uint32_t new_proc_position = find_free_proc();
    process_table[new_proc_position].name = name;
    process_table[new_proc_position].pid = get_next_pid();
    process_table[new_proc_position].stack = stack;
    process_table[new_proc_position].state = PRET;
    process_table[new_proc_position].regs[1] = stack + STACK_SIZE - 1;
}

void delete_processus(pid_t pid) {
    uint32_t index = 0;
    uint32_t found = 0; 
    while (index < NB_PROC && found != 1) {
        if (is_processus(index) && process_table[index].pid == pid) {
            found = 1;
        }

        index++;
    }

    clear_processus(index - 1);
    free(process_table[index - 1].stack);
}

void block_processus() {

}

void unblock_processus() {

}