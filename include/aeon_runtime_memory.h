#ifndef AEON_RUNTIME_MEMORY_H
#define AEON_RUNTIME_MEMORY_H
#include <stddef.h>
#include <stdint.h>

#define AEON_RUNTIME_MEMORY_MAGIC 0x41454D31u
#define AEON_RUNTIME_MEMORY_VERSION 1u

typedef enum {
    AEON_MEMORY_FREE = 0,
    AEON_MEMORY_RESERVED = 1,
    AEON_MEMORY_ACTIVE = 2
} aeon_memory_state_t;

typedef struct {
    uintptr_t base;
    size_t size;
    size_t used;
    aeon_memory_state_t state;
} aeon_memory_region_t;

typedef struct {
    uint32_t magic;
    uint16_t version;
    uint16_t flags;
    size_t alignment;
    aeon_memory_region_t region;
} aeon_runtime_memory_t;

int aeon_runtime_memory_init(aeon_runtime_memory_t *m, void *buffer,
                             size_t size, size_t alignment);
void *aeon_runtime_memory_alloc(aeon_runtime_memory_t *m, size_t size);
int aeon_runtime_memory_reset(aeon_runtime_memory_t *m);
int aeon_runtime_memory_validate(const aeon_runtime_memory_t *m);
size_t aeon_runtime_memory_available(const aeon_runtime_memory_t *m);

#endif
