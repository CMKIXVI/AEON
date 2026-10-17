#include "aeon_runtime_memory.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

_Alignas(64) static uint8_t buffer[4096];

int main(void)
{
    aeon_runtime_memory_t m;
    void *a, *b;

    assert(aeon_runtime_memory_init(&m, buffer, sizeof(buffer), 64) == 0);
    assert(aeon_runtime_memory_validate(&m) == 0);
    assert(aeon_runtime_memory_available(&m) == sizeof(buffer));

    a = aeon_runtime_memory_alloc(&m, 100);
    assert(a != NULL);
    assert((uintptr_t)a % 64u == 0);
    b = aeon_runtime_memory_alloc(&m, 200);
    assert(b != NULL);
    assert((uintptr_t)b % 64u == 0);
    assert((uintptr_t)b >= (uintptr_t)a + 100u);
    assert(aeon_runtime_memory_available(&m) < sizeof(buffer));

    assert(aeon_runtime_memory_alloc(&m, sizeof(buffer)) == NULL);
    assert(aeon_runtime_memory_reset(&m) == 0);
    assert(aeon_runtime_memory_available(&m) == sizeof(buffer));
    assert(aeon_runtime_memory_alloc(&m, sizeof(buffer) + 1u) == NULL);

    m.alignment = 3;
    assert(aeon_runtime_memory_validate(&m) != 0);

    puts("AEON Batch 33 runtime memory test: PASS");
    return 0;
}
