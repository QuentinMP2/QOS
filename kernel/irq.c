#include <inttypes.h>
#include <n7OS/irq.h>

void init_irq_entry(int irq_num, uint32_t addr) {
    /* The [15..0] bits of addr. */
    uint16_t addr_low = addr & 0xFFFF;

    /* The [31..16] bits of addr. */
    uint16_t addr_high = (addr >> 16) & 0xFFFF;

    idt_entry_t idt_entry;
    idt_entry.offset_inf = addr_low;
    idt_entry.sel_segment = KERNEL_CS;
    idt_entry.zero = 0x00;
    idt_entry.type_attr = 0b10001110;
    idt_entry.offset_sup = addr_high;

    idt_entry_t *idt_ptr = (idt_entry_t*)&idt[irq_num]; 
    *idt_ptr = idt_entry;
}
