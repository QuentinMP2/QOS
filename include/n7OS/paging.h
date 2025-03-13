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
    /** Address of the page in the physical memory. */
    uint32_t ADDR: 20;
    /** Available in the volatile memory (e.g. RAM). */
    uint8_t AVAIL: 3;
    /** Reserved bits section 2. */
    uint8_t RSVD_2: 2;
    /** Dirty bit. */
    uint8_t D: 1;
    /** Accessed bit. */
    uint8_t A: 1;
    /** Reserved bits section 1. */
    uint8_t RSVD_1: 2;
    /** User/kernel page (0: kernel page). */
    uint8_t U: 1;
    /** Read/write (0: read only). */
    uint8_t W: 1;
    /** Present (0: not present). */
    uint8_t P: 1;
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
    /** Address of the page in the physical memory. */
    uint32_t ADDR: 20;
    /** Reserved memory section. */
    uint16_t RSVD: 9;
    /** User/kernel page (0: kernel page). */
    uint8_t U: 1;
    /** Read/write (0: read only). */
    uint8_t W: 1;
    /** Present (0: not present). */
    uint8_t P: 1;
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
 * @brief Cette fonction initialise le répertoire de page, alloue les pages de table du noyau
 * et active la pagination
 */
void initialise_paging();

/**
 * @brief Cette fonction alloue une page de la mémoire physique à une adresse de la mémoire virtuelle
 * 
 * @param address       Adresse de la mémoire virtuelle à mapper
 * @param is_writeable  Si is_writeable == 1, la page est accessible en écriture
 * @param is_kernel     Si is_kernel == 1, la page ne peut être accédée que par le noyau
 * @return PageTable    La table de page modifiée
 */
PageTable alloc_page_entry(uint32_t address, int is_writeable, int is_kernel);
#endif