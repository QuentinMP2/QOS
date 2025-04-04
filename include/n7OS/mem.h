/**
 * @file mem.h
 * @brief Allocation of the physical memory pages manager.
 */

#ifndef _MEM_H
#define _MEM_H

#include <inttypes.h>

/**
 * @brief Address of the first addressable row in memory.
 */
#define FIRST_MEMORY_INDEX 0x100000

/**
 * @brief Address of the last addressable row in memory.
 * We consider here a memory of 16Mo.
 */
#define LAST_MEMORY_INDEX 0xFFFFFF 

/**
 * @brief Size of a memory page.
 * 
 * Here, 0x1000 -> 2^10 * 4 = 4096 bytes
 */
#define PAGE_SIZE 0x1000

/** 
 * @brief Size of the free_page_bitmap_table.
 */
#define BITMAP_SIZE ((LAST_MEMORY_INDEX + 1) / PAGE_SIZE / 32)

/**
 * @brief Mark the allocated page.
 * When a page is chosen, this function mark it.
 * 
 * @param addr Address of the page to be marked.
 */
void setPage(uint32_t addr);

/**
 * @brief Desallocate the page.
 * 
 * @param addr Address of the page to be free.
 */
void clearPage(uint32_t addr);

/**
 * @brief Return the first free page of the physical memory and allocate it.
 * 
 * @return Address of the selected page.
 */
uint32_t findfreePage();

/**
 * @brief Initialize physical memory manager.
 */
void init_mem();

/**
 * @brief Display the physical memory state.
 */
void print_mem();
#endif