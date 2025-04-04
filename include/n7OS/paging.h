/**
 * @file paging.h
 * @brief Paging management in the kernel
 */
#ifndef _PAGING_H
#define _PAGING_H

#include <inttypes.h>

/**
 * @brief Format of an entry in the table of pages.
 */
typedef struct {
    /** Present (0: not present). */
    uint32_t P: 1;
    /** Read/write (0: read only). */
    uint32_t W: 1;
    /** User/kernel page (0: kernel page). */
    uint32_t U: 1;
    /** Reserved bits section 1. */
    uint32_t RSVD_1: 2;
    /** Accessed bit. */
    uint32_t A: 1;
    /** Dirty bit. */
    uint32_t D: 1;
    /** Reserved bits section 2. */
    uint32_t RSVD_2: 2;
    /** Available in the volatile memory (e.g. RAM). */
    uint32_t AVAIL: 3;
    /** Address of the page in the physical memory. */
    uint32_t ADDR: 20;
} page_table_entry_t;

/**
 * @brief An entry in the table of pages can be manipulated
 * using the page_table_entry_t struct or directly the value.
 */
typedef union {
    page_table_entry_t page_entry;
    uint32_t value;
} PTE; // PTE = Page Table Entry 

/**
 * @brief A table of pages (PageTable) is an array of page descriptor.
 */
typedef PTE* PageTable;

/**
 * @brief Format of an entry in the directory of pages.
 */
typedef struct {
    /** Present (0: not present). */
    uint32_t P: 1;
    /** Read/write (0: read only). */
    uint32_t W: 1;
    /** User/kernel page (0: kernel page). */
    uint32_t U: 1;
    /** Reserved memory section. */
    uint32_t RSVD: 9;
    /** Address of the page in the physical memory. */
    uint32_t ADDR: 20;
} page_directory_entry_t;

/**
 * @brief An entry in the table of directories can be manipulated
 * using the page_directory_entry_t struct or directly the value.
 */
typedef union {
    page_directory_entry_t page_directory;
    uint32_t value;
} PDE; // PDE = Page Directory Entry 

/**
 * @brief A table of directories (PageDirectory) is an array of directory pages.
 */
typedef PDE* PageDirectory;

/**
 * @brief This function initialise the repertory of page, allocate the pages of kernel tables
 * and enable the paging.
 */
void initialise_paging();

/**
 * @brief This function allocate a page of the physical memory at a virtual memory address.
 * 
 * @param address       Address of the virtual memory to map
 * @param is_writeable  If is_writeable == 1, the page is accessible in writing
 * @param is_kernel     Si is_kernel == 1, la page ne peut être accédée que par le noyau
 * @return PageTable    La table de page modifiée
 */
PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel);
#endif