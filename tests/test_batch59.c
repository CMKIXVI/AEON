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
    uint8_t model[80] = {0};
    uint8_t buffers[5][16] = {{0}};
    uint64_t shape[2] = {2u, 2u};
    const char *names[5] = {"w0", "w1", "w2", "w3", "w4"};
    aeon_model_index_t index;
    aeon_model_stream_t stream;
    aeon_tensor_cache_t cache;
    aeon_tensor_binding_t bindings[5];
    aeon_tensor_materialization_t mats[5];
    aeon_tensor_cache_stats_t stats;
    size_t i;

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
    if (aeon_tensor_cache_get_stats(&cache, &stats) != 0) return 6;
    if (stats.bytes_used != 64u || stats.peak_bytes_used != 64u || stats.inserts != 4u) return 7;

    if (aeon_tensor_cache_materialize(&cache, &mats[0], &bindings[0]) != 0) return 8;
    if (aeon_tensor_cache_materialize(&cache, &mats[4], &bindings[4]) != 0) return 9;
    if (aeon_tensor_cache_get_stats(&cache, &stats) != 0) return 10;
    if (stats.bytes_used != 64u || stats.peak_bytes_used != 80u ||
        stats.evictions != 1u || stats.inserts != 5u) return 11;

    if (aeon_tensor_cache_detach(&cache, &mats[4]) != 0) return 12;
    if (aeon_tensor_cache_get_stats(&cache, &stats) != 0) return 13;
    if (stats.bytes_used != 48u || stats.peak_bytes_used != 80u) return 14;

    puts("AEON Batch 59 cache memory accounting regression test: PASS");
    return 0;
}
