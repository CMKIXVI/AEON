#include "aeon_tensor_cache.h"

static int aeon_tensor_cache_find_index(const aeon_tensor_cache_t *cache,
                                        const aeon_model_tensor_meta_t *tensor,
                                        unsigned *index)
{
    unsigned i;

    if (cache == NULL || tensor == NULL || index == NULL) return -1;
    for (i = 0u; i < AEON_TENSOR_CACHE_MAX_ENTRIES; ++i) {
        if (cache->entries[i].valid && cache->entries[i].tensor == tensor) {
            *index = i;
            return 0;
        }
    }
    return 1;
}

static void aeon_tensor_cache_tick(aeon_tensor_cache_t *cache, unsigned index)
{
    cache->clock++;
    if (cache->clock == 0u) {
        unsigned i;
        cache->clock = 1u;
        for (i = 0u; i < AEON_TENSOR_CACHE_MAX_ENTRIES; ++i) {
            if (cache->entries[i].valid) cache->entries[i].age = 0u;
        }
    }
    cache->entries[index].age = cache->clock;
}

int aeon_tensor_cache_init(aeon_tensor_cache_t *cache)
{
    if (cache == NULL) return -1;
    *cache = (aeon_tensor_cache_t){0};
    return 0;
}

int aeon_tensor_cache_reset(aeon_tensor_cache_t *cache)
{
    unsigned i;
    if (cache == NULL) return -1;
    for (i = 0u; i < AEON_TENSOR_CACHE_MAX_ENTRIES; ++i) {
        cache->entries[i] = (aeon_tensor_cache_entry_t){0};
    }
    cache->clock = 0u;
    return 0;
}

const aeon_tensor_cache_entry_t *aeon_tensor_cache_find(
    const aeon_tensor_cache_t *cache, const aeon_model_tensor_meta_t *tensor)
{
    unsigned index;
    if (aeon_tensor_cache_find_index(cache, tensor, &index) != 0) return NULL;
    return &cache->entries[index];
}

const aeon_tensor_cache_entry_t *aeon_tensor_cache_access(
    aeon_tensor_cache_t *cache, const aeon_model_tensor_meta_t *tensor)
{
    unsigned index;
    if (aeon_tensor_cache_find_index(cache, tensor, &index) != 0) return NULL;
    aeon_tensor_cache_tick(cache, index);
    return &cache->entries[index];
}

int aeon_tensor_cache_put(aeon_tensor_cache_t *cache,
                          aeon_tensor_materialization_t *materialization)
{
    unsigned i;
    unsigned slot = 0u;
    unsigned oldest = 0xFFFFFFFFu;

    if (cache == NULL || materialization == NULL ||
        aeon_tensor_materialization_validate(materialization) != 0) return -1;
    if (aeon_tensor_cache_find(cache, materialization->tensor) != NULL) {
        (void)aeon_tensor_cache_access(cache, materialization->tensor);
        return 0;
    }

    for (i = 0u; i < AEON_TENSOR_CACHE_MAX_ENTRIES; ++i) {
        if (!cache->entries[i].valid) { slot = i; break; }
        if (cache->entries[i].age < oldest) {
            oldest = cache->entries[i].age;
            slot = i;
        }
    }
    cache->entries[slot].materialization = materialization;
    cache->entries[slot].tensor = materialization->tensor;
    cache->entries[slot].bytes = materialization->bytes;
    cache->entries[slot].valid = 1u;
    aeon_tensor_cache_tick(cache, slot);
    return 0;
}
