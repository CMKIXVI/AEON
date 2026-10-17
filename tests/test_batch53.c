#include "aeon_model_index.h"
#include "aeon_model_stream.h"
#include "aeon_tensor_binding.h"
#include "aeon_tensor_cache_materialization.h"

#include <stdint.h>
#include <stdio.h>
#include <string.h>

static unsigned reads;

static int counted_read(void *context, size_t offset, void *buffer, size_t length)
{
    const uint8_t *data = (const uint8_t *)context;
    ++reads;
    memcpy(buffer, data + offset, length);
    return 0;
}

int main(void)
{
    uint8_t model[32];
    uint8_t first[16] = {0};
    uint8_t second[16] = {0};
    uint64_t shape[2] = {2u, 2u};
    aeon_model_index_t index;
    aeon_model_stream_t stream;
    aeon_tensor_binding_t binding;
    aeon_tensor_materialization_t first_materialization;
    aeon_tensor_materialization_t second_materialization;
    aeon_tensor_cache_t cache;
    size_t i;

    for (i = 0u; i < sizeof(model); ++i) model[i] = (uint8_t)(0x60u + i);
    if (aeon_model_index_init(&index) != 0 ||
        aeon_model_index_add(&index, "weight", 8u, 16u,
                             AEON_MODEL_DTYPE_F32, 2u, shape) != 0 ||
        aeon_model_index_validate(&index, sizeof(model)) != 0 ||
        aeon_model_stream_init(&stream, sizeof(model)) != 0 ||
        aeon_model_stream_bind(&stream, counted_read, model) != 0 ||
        aeon_tensor_binding_init(&binding, &stream, &index, "weight") != 0 ||
        aeon_tensor_materialization_init(&first_materialization, first, sizeof(first)) != 0 ||
        aeon_tensor_materialization_init(&second_materialization, second, sizeof(second)) != 0 ||
        aeon_tensor_cache_init(&cache) != 0) {
        return 1;
    }

    if (aeon_tensor_cache_materialize(&cache, &first_materialization, &binding) != 0 ||
        reads != 1u || memcmp(first, model + 8u, 16u) != 0) return 2;

    if (aeon_tensor_materialization_reset(&first_materialization) != 0 ||
        aeon_tensor_cache_materialize(&cache, &second_materialization, &binding) != 0 ||
        reads != 2u || memcmp(second, model + 8u, 16u) != 0) return 3;

    if (aeon_tensor_cache_find(&cache, binding.tensor) == NULL) return 4;

    puts("AEON Batch 53 cache integrity guard test: PASS");
    return 0;
}
