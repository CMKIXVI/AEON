#include "aeon_runtime_memory.h"
#include <stdint.h>

static int power_of_two(size_t x)
{
    return x != 0 && (x & (x - 1u)) == 0;
}

static uintptr_t align_up(uintptr_t value, size_t alignment)
{
    uintptr_t mask = (uintptr_t)alignment - 1u;
    return (value + mask) & ~mask;
}

int aeon_runtime_memory_validate(const aeon_runtime_memory_t *m)
{
    uintptr_t end;
    if (!m || m->magic != AEON_RUNTIME_MEMORY_MAGIC ||
        m->version != AEON_RUNTIME_MEMORY_VERSION ||
        !power_of_two(m->alignment) || m->region.size == 0 ||
        m->region.used > m->region.size) return -1;
    if (m->region.base > UINTPTR_MAX - m->region.size) return -2;
    end = m->region.base + m->region.size;
    (void)end;
    if (m->region.state != AEON_MEMORY_ACTIVE) return -3;
    return 0;
}

int aeon_runtime_memory_init(aeon_runtime_memory_t *m, void *buffer,
                             size_t size, size_t alignment)
{
    if (!m || !buffer || size == 0 || !power_of_two(alignment)) return -1;
    if ((uintptr_t)buffer % alignment != 0) return -2;
    m->magic = AEON_RUNTIME_MEMORY_MAGIC;
    m->version = AEON_RUNTIME_MEMORY_VERSION;
    m->flags = 0;
    m->alignment = alignment;
    m->region.base = (uintptr_t)buffer;
    m->region.size = size;
    m->region.used = 0;
    m->region.state = AEON_MEMORY_ACTIVE;
    return 0;
}

void *aeon_runtime_memory_alloc(aeon_runtime_memory_t *m, size_t size)
{
    uintptr_t cursor, aligned, end;
    size_t offset;
    if (!m || size == 0 || aeon_runtime_memory_validate(m) != 0) return NULL;
    cursor = m->region.base + m->region.used;
    aligned = align_up(cursor, m->alignment);
    if (aligned < cursor) return NULL;
    offset = (size_t)(aligned - m->region.base);
    if (offset > m->region.size || size > m->region.size - offset) return NULL;
    end = aligned + size;
    if (end < aligned) return NULL;
    m->region.used = (size_t)(end - m->region.base);
    return (void *)aligned;
}

int aeon_runtime_memory_reset(aeon_runtime_memory_t *m)
{
    if (aeon_runtime_memory_validate(m) != 0) return -1;
    m->region.used = 0;
    return 0;
}

size_t aeon_runtime_memory_available(const aeon_runtime_memory_t *m)
{
    if (aeon_runtime_memory_validate(m) != 0) return 0;
    return m->region.size - m->region.used;
}
