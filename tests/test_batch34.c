#include "aeon_tensor.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>

_Alignas(64) static uint8_t memory[4096];

int main(void)
{
    aeon_runtime_memory_t arena;
    aeon_tensor_t tensor;
    size_t shape[3] = {2u, 3u, 4u};

    assert(aeon_runtime_memory_init(&arena, memory, sizeof(memory), 64u) == 0);
    assert(aeon_tensor_init(&tensor, AEON_TENSOR_F32, 3u, shape) == 0);
    assert(tensor.elements == 24u);
    assert(tensor.bytes == 96u);
    assert(tensor.stride[0] == 12u);
    assert(tensor.stride[1] == 4u);
    assert(tensor.stride[2] == 1u);
    assert(aeon_tensor_bind(&tensor, &arena) == 0);
    assert(aeon_tensor_validate(&tensor) == 0);
    assert(aeon_tensor_zero(&tensor) == 0);
    assert(((uint8_t *)tensor.data)[0] == 0u);
    assert(aeon_tensor_dtype_size(AEON_TENSOR_BF16) == 2u);
    assert(aeon_tensor_dtype_size((aeon_tensor_dtype_t)99) == 0u);

    tensor.bytes = 1u;
    assert(aeon_tensor_validate(&tensor) != 0);

    puts("AEON Batch 34 tensor buffer test: PASS");
    return 0;
}
