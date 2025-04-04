#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <stdio.h>
#include <unistd.h>
#include <n7OS/paging.h>
#include <n7OS/mem.h>
#include <n7OS/kheap.h>
#include <n7OS/irq.h>

void kernel_start(void) {

    // Initialize the placement address of the memory heap 
    kinit();

    initialise_paging();

    // Enable interruptions
    sti();
    init_irq();
    
    init_console();
    
    printf("\fWelcome to QOS !\n\n");
    __asm__ ("int $50");
    printf("\n");
    print_mem();
    // alloc_page_entry(0xA000FFFC, 1, 1);
    // uint32_t *ptr = (uint32_t*)0xA000FFFC;
    // uint32_t test = *ptr;
    // test++; 

    // We mustn't quit the kernel_start function 
    while (1) {
        // This function stop the CPU
        hlt();
    }
}
