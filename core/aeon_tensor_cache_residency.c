#include "aeon_tensor_cache.h"

int aeon_tensor_cache_validate(const aeon_tensor_cache_t *cache)
{
    size_t resident = 0u;
    unsigned i;
    unsigned j;

    if (cache == NULL) return -1;
    if (cache->policy != AEON_TENSOR_CACHE_POLICY_LRU) return -2;
    if (cache->stats.bytes_used > cache->budget_bytes) return -3;

    for (i = 0u; i < AEON_TENSOR_CACHE_MAX_ENTRIES; ++i) {
        const aeon_tensor_cache_entry_t *entry = &cache->entries[i];
        if (!entry->valid) {
            if (entry->materialization != NULL || entry->tensor != NULL ||
                entry->bytes != 0u || entry->age != 0u) return -4;
            continue;
        }
        if (entry->materialization == NULL || entry->tensor == NULL ||
            entry->bytes == 0u || entry->age == 0u) return -5;
        if (entry->materialization->tensor != entry->tensor ||
            entry->materialization->bytes != entry->bytes) return -6;
        if (aeon_tensor_materialization_validate(entry->materialization) != 0) return -7;
        if (resident > cache->budget_bytes - entry->bytes) return -8;
        resident += entry->bytes;
        for (j = i + 1u; j < AEON_TENSOR_CACHE_MAX_ENTRIES; ++j) {
            if (cache->entries[j].valid && cache->entries[j].tensor == entry->tensor) return -9;
        }
    }
    return resident == cache->stats.bytes_used ? 0 : -10;
}
