/*
 * AEON x86-64 kernel integration boundary.
 *
 * This preserves the supplied IDT/APIC/paging direction without pretending
 * that a bare UEFI application is already a kernel. The code is intentionally
 * isolated until ISR stubs, LAPIC programming, page-table construction and
 * scheduler ownership are implemented together.
 */
#include <stdint.h>
#include <stddef.h>

#if defined(__x86_64__)
typedef struct {
    uint16_t off_low;
    uint16_t seg_sel;
    uint8_t ist;
    uint8_t flags;
    uint16_t off_mid;
    uint32_t off_high;
    uint32_t reserved;
} __attribute__((packed)) aeon_idt_entry_t;

typedef struct {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed)) aeon_idtr_t;

typedef struct {
    uint64_t entries[512];
} aeon_pml4_t __attribute__((aligned(4096)));

static aeon_idt_entry_t g_idt[256] __attribute__((aligned(16)));
static aeon_idtr_t g_idtr;
static aeon_pml4_t g_pml4;

int aeon_x86_idt_set_gate(unsigned vector, uintptr_t handler,
                          uint16_t selector, uint8_t flags)
{
    aeon_idt_entry_t *e;
    if (vector >= 256u || handler == 0u) return -1;
    e = &g_idt[vector];
    e->off_low = (uint16_t)(handler & 0xffffu);
    e->seg_sel = selector;
    e->ist = 0u;
    e->flags = flags;
    e->off_mid = (uint16_t)((handler >> 16) & 0xffffu);
    e->off_high = (uint32_t)((handler >> 32) & 0xffffffffu);
    e->reserved = 0u;
    return 0;
}

int aeon_x86_idt_load(void)
{
    g_idtr.limit = (uint16_t)(sizeof(g_idt) - 1u);
    g_idtr.base = (uint64_t)(uintptr_t)g_idt;
    __asm__ volatile ("lidt %0" :: "m"(g_idtr));
    return 0;
}

void *aeon_x86_pml4_storage(void)
{
    return &g_pml4;
}

#else
int aeon_x86_idt_set_gate(unsigned vector, uintptr_t handler,
                          uint16_t selector, uint8_t flags)
{
    (void)vector; (void)handler; (void)selector; (void)flags;
    return -2;
}
int aeon_x86_idt_load(void) { return -2; }
void *aeon_x86_pml4_storage(void) { return NULL; }
#endif
