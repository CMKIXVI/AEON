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

static int aeon_tensor_cache_select_lru(const aeon_tensor_cache_t *cache,
                                        unsigned *slot,
                                        unsigned *evicting)
{
    unsigned i;
    unsigned oldest = 0xFFFFFFFFu;

    if (cache == NULL || slot == NULL || evicting == NULL) return -1;
    *slot = 0u;
    *evicting = 0u;
    for (i = 0u; i < AEON_TENSOR_CACHE_MAX_ENTRIES; ++i) {
        if (!cache->entries[i].valid) {
            *slot = i;
            return 0;
        }
        if (cache->entries[i].age < oldest) {
            oldest = cache->entries[i].age;
            *slot = i;
            *evicting = 1u;
        }
    }
    return 0;
}

static int aeon_tensor_cache_select_slot(const aeon_tensor_cache_t *cache,
                                         unsigned *slot,
                                         unsigned *evicting)
{
    if (cache == NULL || slot == NULL || evicting == NULL) return -1;
    switch (cache->policy) {
    case AEON_TENSOR_CACHE_POLICY_LRU:
        return aeon_tensor_cache_select_lru(cache, slot, evicting);
    default:
        return -2;
    }
}

static int aeon_tensor_cache_fits(const aeon_tensor_cache_t *cache, size_t bytes)
{
    if (cache == NULL) return 0;
    if (bytes > cache->budget_bytes) return 0;
    return cache->stats.bytes_used <= cache->budget_bytes - bytes;
}

int aeon_tensor_cache_init(aeon_tensor_cache_t *cache)
{
    if (cache == NULL) return -1;
    *cache = (aeon_tensor_cache_t){0};
    cache->policy = AEON_TENSOR_CACHE_POLICY_LRU;
    cache->budget_bytes = AEON_TENSOR_CACHE_UNLIMITED_BUDGET;
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
    cache->stats = (aeon_tensor_cache_stats_t){0};
    cache->policy = AEON_TENSOR_CACHE_POLICY_LRU;
    cache->budget_bytes = AEON_TENSOR_CACHE_UNLIMITED_BUDGET;
    return 0;
}

int aeon_tensor_cache_set_policy(aeon_tensor_cache_t *cache,
                                 aeon_tensor_cache_policy_t policy)
{
    if (cache == NULL) return -1;
    if (policy != AEON_TENSOR_CACHE_POLICY_LRU) return -2;
    cache->policy = policy;
    return 0;
}

aeon_tensor_cache_policy_t aeon_tensor_cache_get_policy(const aeon_tensor_cache_t *cache)
{
    if (cache == NULL) return AEON_TENSOR_CACHE_POLICY_LRU;
    return cache->policy;
}

int aeon_tensor_cache_set_budget(aeon_tensor_cache_t *cache, size_t budget_bytes)
{
    if (cache == NULL) return -1;
    if (budget_bytes < cache->stats.bytes_used) return -2;
    cache->budget_bytes = budget_bytes;
    return 0;
}

size_t aeon_tensor_cache_get_budget(const aeon_tensor_cache_t *cache)
{
    if (cache == NULL) return 0u;
    return cache->budget_bytes;
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
    if (aeon_tensor_cache_find_index(cache, tensor, &index) != 0) {
        if (cache != NULL) cache->stats.misses++;
        return NULL;
    }
    cache->stats.hits++;
    aeon_tensor_cache_tick(cache, index);
    return &cache->entries[index];
}

int aeon_tensor_cache_put(aeon_tensor_cache_t *cache,
                          aeon_tensor_materialization_t *materialization)
{
    unsigned slot;
    unsigned evicting;

    if (cache == NULL || materialization == NULL ||
        aeon_tensor_materialization_validate(materialization) != 0) return -1;
    if (aeon_tensor_cache_find(cache, materialization->tensor) != NULL) {
        (void)aeon_tensor_cache_access(cache, materialization->tensor);
        return 0;
    }
    if (materialization->bytes > cache->budget_bytes) return -3;

    while (!aeon_tensor_cache_fits(cache, materialization->bytes)) {
        if (aeon_tensor_cache_select_slot(cache, &slot, &evicting) != 0 || evicting == 0u) {
            return -4;
        }
        if (cache->stats.bytes_used >= cache->entries[slot].bytes) {
            cache->stats.bytes_used -= cache->entries[slot].bytes;
        } else {
            cache->stats.bytes_used = 0u;
        }
        cache->stats.evictions++;
        cache->entries[slot] = (aeon_tensor_cache_entry_t){0};
    }

    if (aeon_tensor_cache_select_slot(cache, &slot, &evicting) != 0 || evicting != 0u) {
        return -5;
    }
    cache->entries[slot].materialization = materialization;
    cache->entries[slot].tensor = materialization->tensor;
    cache->entries[slot].bytes = materialization->bytes;
    cache->entries[slot].valid = 1u;
    cache->stats.inserts++;
    cache->stats.bytes_used += materialization->bytes;
    if (cache->stats.bytes_used > cache->stats.peak_bytes_used) {
        cache->stats.peak_bytes_used = cache->stats.bytes_used;
    }
    aeon_tensor_cache_tick(cache, slot);
    return 0;
}

int aeon_tensor_cache_detach(aeon_tensor_cache_t *cache,
                             aeon_tensor_materialization_t *materialization)
{
    unsigned i;

    if (cache == NULL || materialization == NULL) return -1;
    for (i = 0u; i < AEON_TENSOR_CACHE_MAX_ENTRIES; ++i) {
        if (cache->entries[i].valid &&
            cache->entries[i].materialization == materialization) {
            size_t bytes = cache->entries[i].bytes;
            if (cache->stats.bytes_used >= bytes) {
                cache->stats.bytes_used -= bytes;
            } else {
                cache->stats.bytes_used = 0u;
            }
            cache->entries[i] = (aeon_tensor_cache_entry_t){0};
            return 0;
        }
    }
    return 1;
}

int aeon_tensor_cache_get_stats(const aeon_tensor_cache_t *cache,
                                aeon_tensor_cache_stats_t *stats)
{
    if (cache == NULL || stats == NULL) return -1;
    *stats = cache->stats;
    return 0;
}

int aeon_tensor_cache_record_stale(aeon_tensor_cache_t *cache)
{
    if (cache == NULL) return -1;
    cache->stats.stale_entries++;
    return 0;
}
