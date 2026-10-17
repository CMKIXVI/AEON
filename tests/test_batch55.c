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
    uint8_t model[80];
    uint8_t buffers[5][16] = {{0}};
    uint64_t shape[2] = {2u, 2u};
    const char *names[5] = {"w0", "w1", "w2", "w3", "w4"};
    aeon_model_index_t index;
    aeon_model_stream_t stream;
    aeon_tensor_cache_t cache;
    aeon_tensor_binding_t bindings[5];
    aeon_tensor_materialization_t mats[5];
    size_t i;

    for (i = 0u; i < sizeof(model); ++i) model[i] = (uint8_t)(0x20u + i);

    if (aeon_model_index_init(&index) != 0) return 1;
    for (i = 0u; i < 5u; ++i) {
        if (aeon_model_index_add(&index, names[i], i * 16u, 16u,
                                 AEON_MODEL_DTYPE_F32, 2u, shape) != 0) return 2;
    }
    if (aeon_model_index_validate(&index, sizeof(model)) != 0 ||
        aeon_model_stream_init(&stream, sizeof(model)) != 0 ||
        aeon_model_stream_bind(&stream, counted_read, model) != 0 ||
        aeon_tensor_cache_init(&cache) != 0) return 3;

    for (i = 0u; i < 5u; ++i) {
        if (aeon_tensor_binding_init(&bindings[i], &stream, &index, names[i]) != 0 ||
            aeon_tensor_materialization_init(&mats[i], buffers[i], sizeof(buffers[i])) != 0) return 4;
    }

    for (i = 0u; i < 4u; ++i) {
        if (aeon_tensor_cache_materialize(&cache, &mats[i], &bindings[i]) != 0) return 5;
    }
    if (reads != 4u) return 6;

    if (aeon_tensor_cache_materialize(&cache, &mats[0], &bindings[0]) != 0 || reads != 4u) return 7;

    if (aeon_tensor_cache_materialize(&cache, &mats[4], &bindings[4]) != 0 || reads != 5u) return 8;

    if (aeon_tensor_cache_find(&cache, bindings[1].tensor) != NULL) return 9;
    if (aeon_tensor_cache_find(&cache, bindings[0].tensor) == NULL ||
        aeon_tensor_cache_find(&cache, bindings[4].tensor) == NULL) return 10;

    if (aeon_tensor_cache_materialize(&cache, &mats[1], &bindings[1]) != 0 ||
        reads != 6u || memcmp(buffers[1], model + 16u, 16u) != 0) return 11;

    puts("AEON Batch 55 cache eviction regression test: PASS");
    return 0;
}
