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

static int expect_stats(const aeon_tensor_cache_t *cache,
                        size_t hits, size_t misses, size_t inserts,
                        size_t evictions, size_t stale)
{
    aeon_tensor_cache_stats_t stats;
    if (aeon_tensor_cache_get_stats(cache, &stats) != 0) return 0;
    return stats.hits == hits && stats.misses == misses &&
           stats.inserts == inserts && stats.evictions == evictions &&
           stats.stale_entries == stale;
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
    if (!expect_stats(&cache, 0u, 0u, 0u, 0u, 0u)) return 4;

    for (i = 0u; i < 5u; ++i) {
        if (aeon_tensor_binding_init(&bindings[i], &stream, &index, names[i]) != 0 ||
            aeon_tensor_materialization_init(&mats[i], buffers[i], sizeof(buffers[i])) != 0) return 5;
    }
    for (i = 0u; i < 4u; ++i) {
        if (aeon_tensor_cache_materialize(&cache, &mats[i], &bindings[i]) != 0) return 6;
    }
    if (reads != 4u || !expect_stats(&cache, 0u, 4u, 4u, 0u, 0u)) return 7;
    if (aeon_tensor_cache_materialize(&cache, &mats[0], &bindings[0]) != 0 || reads != 4u) return 8;
    if (!expect_stats(&cache, 1u, 4u, 4u, 0u, 0u)) return 9;
    if (aeon_tensor_cache_materialize(&cache, &mats[4], &bindings[4]) != 0 || reads != 5u) return 10;
    if (!expect_stats(&cache, 1u, 5u, 5u, 1u, 0u)) return 11;
    if (aeon_tensor_cache_materialize(&cache, &mats[1], &bindings[1]) != 0 || reads != 6u) return 12;
    if (!expect_stats(&cache, 1u, 6u, 6u, 2u, 0u)) return 13;

    aeon_tensor_materialization_reset(&mats[0]);
    if (aeon_tensor_cache_materialize(&cache, &mats[0], &bindings[0]) != 0 ||
        reads != 7u || memcmp(buffers[0], model, 16u) != 0) return 14;
    if (!expect_stats(&cache, 2u, 6u, 7u, 2u, 1u)) return 15;

    puts("AEON Batch 56 cache observability regression test: PASS");
    return 0;
}
