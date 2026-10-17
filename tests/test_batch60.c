#include "aeon_model_index.h"
#include "aeon_model_stream.h"
#include "aeon_tensor_binding.h"
#include "aeon_tensor_cache_materialization.h"

#include <stdint.h>
#include <stdio.h>

static int counted_read(void *context, size_t offset, void *buffer, size_t length)
{
    const uint8_t *data = (const uint8_t *)context;
    for (size_t i = 0u; i < length; ++i) ((uint8_t *)buffer)[i] = data[offset + i];
    return 0;
}

int main(void)
{
    uint8_t model[48] = {0};
    uint8_t buffers[3][16] = {{0}};
    uint64_t shape[2] = {2u, 2u};
    const char *names[3] = {"w0", "w1", "w2"};
    aeon_model_index_t index;
    aeon_model_stream_t stream;
    aeon_tensor_cache_t cache;
    aeon_tensor_binding_t bindings[3];
    aeon_tensor_materialization_t mats[3];
    aeon_tensor_cache_stats_t stats;

    if (aeon_model_index_init(&index) != 0) return 1;
    if (aeon_model_index_add(&index, names[0], 0u, 16u, AEON_MODEL_DTYPE_F32, 2u, shape) != 0 ||
        aeon_model_index_add(&index, names[1], 16u, 16u, AEON_MODEL_DTYPE_F32, 2u, shape) != 0 ||
        aeon_model_index_add(&index, names[2], 32u, 16u, AEON_MODEL_DTYPE_F32, 2u, shape) != 0) return 2;
    if (aeon_model_index_validate(&index, sizeof(model)) != 0 ||
        aeon_model_stream_init(&stream, sizeof(model)) != 0 ||
        aeon_model_stream_bind(&stream, counted_read, model) != 0 ||
        aeon_tensor_cache_init(&cache) != 0) return 3;
    if (aeon_tensor_cache_get_budget(&cache) != AEON_TENSOR_CACHE_UNLIMITED_BUDGET) return 4;
    if (aeon_tensor_cache_set_budget(&cache, 32u) != 0 ||
        aeon_tensor_cache_get_budget(&cache) != 32u) return 5;

    for (size_t i = 0u; i < 3u; ++i) {
        if (aeon_tensor_binding_init(&bindings[i], &stream, &index, names[i]) != 0 ||
            aeon_tensor_materialization_init(&mats[i], buffers[i], sizeof(buffers[i])) != 0) return 6;
    }

    if (aeon_tensor_cache_materialize(&cache, &mats[0], &bindings[0]) != 0 ||
        aeon_tensor_cache_materialize(&cache, &mats[1], &bindings[1]) != 0) return 7;
    if (aeon_tensor_cache_get_stats(&cache, &stats) != 0 ||
        stats.bytes_used != 32u || stats.peak_bytes_used != 32u || stats.inserts != 2u) return 8;

    if (aeon_tensor_cache_materialize(&cache, &mats[0], &bindings[0]) != 0 ||
        aeon_tensor_cache_materialize(&cache, &mats[2], &bindings[2]) != 0) return 9;
    if (aeon_tensor_cache_find(&cache, bindings[1].tensor) != NULL ||
        aeon_tensor_cache_find(&cache, bindings[0].tensor) == NULL ||
        aeon_tensor_cache_find(&cache, bindings[2].tensor) == NULL) return 10;
    if (aeon_tensor_cache_get_stats(&cache, &stats) != 0 ||
        stats.bytes_used != 32u || stats.peak_bytes_used != 32u ||
        stats.evictions != 1u || stats.inserts != 3u) return 11;

    if (aeon_tensor_cache_set_budget(&cache, 15u) == 0) return 12;
    if (aeon_tensor_cache_get_budget(&cache) != 32u) return 13;

    if (aeon_tensor_cache_put(&cache, &mats[1]) == 0) return 14;
    if (aeon_tensor_cache_get_stats(&cache, &stats) != 0 ||
        stats.bytes_used != 32u || stats.evictions != 1u || stats.inserts != 3u) return 15;

    puts("AEON Batch 60 tensor cache budget guard regression test: PASS");
    return 0;
}
