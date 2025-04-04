#include <n7OS/paging.h>
#include <n7OS/kheap.h>
#include <n7OS/mem.h>
#include <n7OS/processor_structs.h>

PageDirectory directories_table;

void initialise_paging() {
    // Initialise physical memory manager.
    init_mem();

    // Create a memory area for the table of directory pages.
    directories_table = (PageDirectory)kmalloc_a(PAGE_SIZE);

    // Address after all allocations.
    uint32_t after_pde = (uint32_t)directories_table + PAGE_SIZE;
    // Initialise tables of pages.
    for (uint32_t i = 0; i < PAGE_SIZE / sizeof(page_directory_entry_t); i++) {
        PDE pde;
        pde.page_directory.P = 1;
        pde.page_directory.W = 1;
        pde.page_directory.U = 0;
        pde.page_directory.ADDR = kmalloc_a(PAGE_SIZE) >> 12;
        directories_table[i] = pde;
        after_pde += PAGE_SIZE;
    }

    // Current address in the virtual memory.
    for (uint32_t i = 0; i < after_pde; i += PAGE_SIZE) {
        alloc_page_entry(i, 1, 1);
    }
        
    // Set CR3 with the page directory value.
    __asm__ __volatile__ ("mov %0, %%cr3" :: "r"(directories_table));

    // Enable paging
    // Set the 32th bit of CR0 to 1
    uint32_t cr0;
    __asm__ __volatile__ ("mov %%cr0, %0" : "=r"(cr0)); // Get CR0 value
    cr0 |= (0b1 << 31);  
    __asm__ __volatile__ ("mov %0, %%cr0" :: "r"(cr0)); // Set CR0 value

    setup_base((uint32_t)directories_table);
}

PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    // Get the [31..22] bits, i.e. the index of the page directory.
    uint32_t index_page_directory = address >> 22;
    index_page_directory &= 0x000003FF; // Only keep the first 10 bits.
    
    PDE pde = directories_table[index_page_directory];
    PageTable page_table = (PageTable)(pde.page_directory.ADDR << 12);

    // Get the [21..12] bits, i.e. the index of the page table.
    uint32_t index_page_table = address >> 12;
    index_page_table &= 0x000003FF; // Only keep the first 10 bits.


        
    // Get the [11..0] bits, i.e. the index of the page.
    uint32_t index_page = address;
    index_page &= 0x00000FFF; // Only keep the first 12 bits.

    // Modify the page table.
    PTE *page = &page_table[index_page_table];
    page -> page_entry.P = 1;
    page -> page_entry.W = is_writeable;
    page -> page_entry.U = ~is_kernel;
    page -> page_entry.A = 0; 
    page -> page_entry.D = 0;
    uint32_t free_p = findfreePage();
    page -> page_entry.ADDR = free_p >> 12; // Only keep the address part of the page

    return page_table;
}
