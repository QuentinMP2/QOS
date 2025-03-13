#include <n7OS/paging.h>
#include <stddef.h> // mandatory for NULL value
#include <n7OS/kheap.h>
#include <mem.h>

void initialise_paging() {
    // Initialise physical memory manager.
    init_mem();

    // Create a memory area for the table of directory pages.
    PageDirectory directories_table = (PageDirectory)kmalloc_a(sizeof(PAGE_SIZE));

    // Initialise tables of pages.
    for (int i = 0; i < PAGE_SIZE / sizeof(page_directory_entry_t); i++) {
        uint32_t current_address = (uint32_t)directories_table;
        
        PDE pde;
        pde.page_directory.P = 1;
        pde.page_directory.W = 0;
        pde.page_directory.U = 0;
        pde.page_directory.ADDR = alloc_page_entry(current_address, 1, 1);
        directories_table[i] = pde; 
        
        current_address += PAGE_SIZE;
    }
    
    // Set CR3 with the page directory value.
    __asm__ __volatile__ ("mov %0, %%cr3"::"r"(&directories_table));

    // Enable paging
    // Set the 31th bit of CR0 to 1
    uint32_t cr0;
    __asm__ __volatile__ ("mov %%cr0, %0" : "=r"(cr0)); // Get CR0 value
    cr0 |= (0b1 << 30);
    __asm__ __volatile__ ("mov %0, %%cr0" :: "r"(cr0)); // Set CR0 value
}

PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel) {
    PageTable pgtab = (PageTable)kmalloc_a(sizeof(PAGE_SIZE));

    for (int i = 0; i < PAGE_SIZE / sizeof(page_table_entry_t); i++) {
        PTE pte;
        pte.page_entry.P = 1;
        pte.page_entry.W = 1;
        pte.page_entry.U = 0;
        pte.page_entry.A = 0;
        pte.page_entry.D = 0;
        pte.page_entry.ADDR = findfreePage();
    }
    
    return pgtab;
}
