#include <n7OS/mem.h>
#include <debug.h>

/**
 * @brief Bitmap that reference free pages of the physical memory.
 * Each case of the table represents 32 pages.
 * 0: free page
 */
uint32_t free_page_bitmap_table[BITMAP_SIZE];

void setPage(uint32_t addr) {
    uint32_t page_num = addr / PAGE_SIZE;
    uint32_t bitmap_index = page_num / 32;
    uint8_t bitmap_offset = page_num % 32;
    free_page_bitmap_table[bitmap_index] |= (0b1 << bitmap_offset);
}

void clearPage(uint32_t addr) {
    uint32_t page_num = addr / PAGE_SIZE;
    uint32_t bitmap_index = page_num / 32;
    uint8_t bitmap_offset = page_num % 32;
    free_page_bitmap_table[bitmap_index] &= (0b0 << bitmap_offset);
}

uint32_t findfreePage() {
    uint32_t index = 0;
    uint8_t found_page = 0;
    uint8_t current_bit = 0;

    while (found_page != 1) {
        if (free_page_bitmap_table[index] < 0xffffffff) {
            // There is at least one bit equals to 0
            current_bit = 0;
            while (found_page != 1) {
                // We want 0 so (!) we obtain the bit with (& 0b1)
                if (!((free_page_bitmap_table[index] >> current_bit) & 0b1)) {
                    // The current bit is equals to 1
                    found_page = 1;                    
                }
                current_bit++;
            }
        }
        index++;
    }

    if (found_page == 0) {
        return 0; // Change this by an interruption later
    }

    // Minus 1 because of the last incrementation of the loop
    uint32_t addr = ((index - 1) * 32 + (current_bit - 1)) * PAGE_SIZE; 

    // Allocate the page
    setPage(addr);
    
    return addr;
}

void init_mem() {
    for (int i = 0; i < BITMAP_SIZE; i++) {
        free_page_bitmap_table[i] = 0;
    }
}

/**
 * @brief Print an unsigned integer in binary form.
 * 
 * @param number the 32 bits number to print
 */
void print_binary(uint32_t number) {
    for (int i = 0; i < 32; i++) {
        if (i % 4 == 0 && i != 0) {
            printf(" "); // For readability
        }
        printf("%d", (number >> i) & 0b1);
    }
    printf("\n");
}

/**
 * @brief Print an unsigned integer in binary form.
 * 
 * @param number the 32 bits number to print
 */
void print_mem_hex() {
    for (int i = 0; i < BITMAP_SIZE; i++) {
        if (i % 7 == 0) {
            printf("  ");
        }

        printf("0x%08x ", free_page_bitmap_table[i]);

        if ((i + 1) % 7 == 0) {
            printf("\n");
        }
    }
}

void print_mem() {
    // printf("\f");
    // for (int i = 0; i < BITMAP_SIZE; i++) {
    //     print_binary(free_page_bitmap_table[i]);
    // }

    print_mem_hex();
}